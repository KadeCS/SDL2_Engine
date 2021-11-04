#include "Multiplayer.h"
#include "Game.h"
#include "Bullet.h"
#include "Gameplay.h"
#include "Particles.h"

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

client c;
websocketpp::connection_hdl connectionHdl;

long Multiplayer::localId;

bool Multiplayer::awaitingLogin = false;

std::vector<Entity> Multiplayer::entites = {};

bool Multiplayer::loggedIn = false;

std::string username;

void Multiplayer::SendPacket(std::string data, PacketType packet) {

    websocketpp::lib::error_code ec;

    std::string dataStr = data;

    char* sendData = (char*)malloc(dataStr.length() + 8);

    char* writeTo = (char*)(((__int64)sendData) + 8);

    __int32* dataPtrs = (__int32*)sendData;
    dataPtrs[0] = packet;
    dataPtrs[1] = (__int32)dataStr.length();

    memcpy(writeTo, dataStr.c_str(), dataStr.length());

    c.send(connectionHdl, std::string(sendData, dataStr.length() + 8), websocketpp::frame::opcode::BINARY, ec);
}

DWORD WINAPI NewThread(LPVOID param) {
    for (;;) {
        Sleep(1000);

        CPacketHeartbeat_t heartbeat;
        heartbeat.PacketType = CPacketHeartbeat;
        heartbeat.Order = 0;
        heartbeat.time = time(NULL);

        Multiplayer::sendMessage<CPacketHeartbeat_t>(heartbeat);

    }
}

std::string gen_random(const int len) {

    std::string tmp_s;
    static const char alphanum[] =
        "player"
        "PLAYER"
        "player";

    srand((unsigned)time(NULL) * 900);

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


    return tmp_s;

}


void on_message(client* c, websocketpp::connection_hdl hdl, client::message_ptr msg) {

    if (msg->get_opcode() != websocketpp::frame::opcode::BINARY)
        return;

    std::string string = msg->get_raw_payload();

    char* ptr = (char*)malloc(string.length());

    memcpy(ptr, string.c_str(), string.length());

    int32_t* ints = (int32_t*)ptr;

    int32_t packetType = ints[0];
    int32_t length = ints[1];

    size_t size = (size_t)length;

    char* data = (char*)((__int64)(ptr)+8);

    PacketType type = (PacketType)packetType;

    msgpack::unpacked result;

    CPacketHello_t helloPacket;
    
    SPacketHello_t helloServer;
    SPacketStatus_t status;
    SPacketUpdateGameState_t gameState;
    SPacketShootResponse_t shoot;
    msgpack::object obj;

    switch (type)
    {
        case SPacketStatus:

            unpack(result, data, length);

            obj = msgpack::object(result.get());

            free(ptr);

            obj.convert(status);

            std::cout << "Status: " << status.status.c_str() << " Code: " << status.code << std::endl;

            switch (status.code)
            {
                case 403: // pls login

                    Multiplayer::awaitingLogin = true;

                    CreateThread(NULL, NULL, NewThread, NULL, NULL, NULL);

                break;
                case 202: // oh hi

                    std::cout << "sending henlo" << std::endl;
                    helloPacket.Order = 0;
                    helloPacket.PacketType = CPacketHello;
                    Multiplayer::sendMessage<CPacketHello_t>(helloPacket);
                break;

                case 203: // oh hi

                    std::cout << "sending henlo" << std::endl;
                    helloPacket.Order = 0;
                    helloPacket.PacketType = CPacketHello;
                    Multiplayer::sendMessage<CPacketHello_t>(helloPacket);
                    break;
            }
            break;
        case SPacketHello:

            Multiplayer::loggedIn = true;

            unpack(result, data, length);

            obj = msgpack::object(result.get());

            free(ptr);

            obj.convert(helloServer);

            std::cout << "our id is smth like " << helloServer.localPlayerID << std::endl;

            Multiplayer::localId = helloServer.localPlayerID;

            Multiplayer::setEntites(helloServer.Entities);

            Gameplay::onLoggedIn();
            break;
        case SPacketUpdateGameState:
            unpack(result, data, length);

            obj = msgpack::object(result.get());

            free(ptr);

            obj.convert(gameState);

            Multiplayer::setEntites(gameState.Entities);

            Gameplay::onUpdateGameState();

            break;
        case SPacketShootResponse:
			std::cout << "recieved " << data << std::endl;
            unpack(result, data, length);

            obj = msgpack::object(result.get());

            free(ptr);

            obj.convert(shoot);

            Multiplayer::setEntites(shoot.Entities);

            Entity en = Gameplay::findEntityById(shoot.id);

            Bullet* b = new Bullet(en.position.x,en.position.y);
			b->isLocal = true;
            b->mpEntity = en;
            Bullet::addBullet(b);
            b->create();
            b->direction = en.direction;


			std::cout << "bullet " << en.direction.x << " " << en.direction.y << std::endl;

			Particles* p = new Particles(b->x, b->y, en.direction.x, en.direction.y, 4, 450);
			p->create();

            b->bulletX = en.position.x;
            b->bulletY = en.position.y;

            std::cout << "created bullet from server " << shoot.id << std::endl;

            Gameplay::getLocalPlayer()->onShot(shoot);

            break;
    }


}



bool verify_certificate(const char* hostname, bool preverified, boost::asio::ssl::verify_context& ctx) {
    return true;
}


context_ptr on_tls_init(const char* hostname, websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2 |
            boost::asio::ssl::context::no_sslv3 |
            boost::asio::ssl::context::single_dh_use);


        ctx->set_verify_mode(boost::asio::ssl::verify_peer);
        ctx->set_verify_callback(bind(&verify_certificate, hostname, ::_1, ::_2));

    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return ctx;
}




DWORD WINAPI Multiplayer::connect(LPVOID agh)
{
    awaitingLogin = false;
    // don't look at this url (not my choice)
    std::string url = "wss://titnoas.xyz/ballsandsex/";

    std::cout << "Creating things" << std::endl;

    try {

        while (true)
        {

            c.set_access_channels(websocketpp::log::alevel::none);
            c.clear_access_channels(websocketpp::log::alevel::all);
            c.set_error_channels(websocketpp::log::elevel::none);

            // Initialize ASIO
            c.init_asio();

            // Register our message handler
            std::cout << "Setting handlers" << std::endl;
            c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));

            c.set_tls_init_handler(bind(&on_tls_init, "titnoas.xyz", ::_1));

            std::cout << "handlers set" << std::endl;
            websocketpp::lib::error_code ec;
            client::connection_ptr con = c.get_connection(url, ec);
            if (ec) {
                std::cout << "could not create connection because: " << ec.message() << std::endl;
                return 0;
            }
            std::cout << "got connection" << std::endl;

            // Note that connect here only requests a connection. No network messages are
            // exchanged until the event loop starts running in the next line.
            connectionHdl = c.connect(con);
            std::cout << "connected" << std::endl;
            // Start the ASIO io_service run loop
            // this will cause a single connection to be made to the server. c.run()
            // will exit when this connection is closed.



            std::cout << "Calling run" << std::endl;
            c.run();
            std::cout << "done run" << std::endl;

            if (Multiplayer::loggedIn)
                Multiplayer::login(Gameplay::getLocalPlayer()->mpEntity.username);
        }
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
}


void Multiplayer::setEntites(std::vector<Entity> ent)
{
    entites.clear();
    for (int i = 0; i < ent.size(); i++) {
        entites.push_back(ent[i]);
    };
}

std::vector<Entity> Multiplayer::getEntites()
{
    return entites;
}

void Multiplayer::login(std::string username)
{
    CPacketAuthorization_t authPacket;

    authPacket.Order = 0;
    authPacket.PacketType = CPacketAuthorization;
    if (Multiplayer::loggedIn)
        authPacket.secret = std::to_string(Multiplayer::localId);
    else
        authPacket.secret = "";
    authPacket.username = username;

    std::cout << "logging in as " << username << std::endl;

    Multiplayer::sendMessage<CPacketAuthorization_t>(authPacket);
}

void Multiplayer::inQuotesGracefullyDisconnect()
{
    c.close(connectionHdl,1000,"no you");
}
