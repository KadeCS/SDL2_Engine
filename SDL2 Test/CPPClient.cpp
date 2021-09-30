#include <openssl/opensslconf.h>

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <msgpack.hpp>


#include <iostream>


typedef enum PacketType
{
    CPacketAuthorization = 1,
    CPacketHeartbeat = 2,
    CPacketHello = 3,
    CPacketUpdateEntity = 4,
    CPacketRequestGameState = 5,
    SPacketHello = -1,
    SPacketStatus = -2,
    SPacketUpdateGameState = -3,
    SPacketInvalidPacket = -4,
};

class SPacketStatus_t {
public:
    int PacketType;
    int Order;
    int code;
    std::string status;
    MSGPACK_DEFINE_MAP(PacketType, Order, code, status);
};

class CPacketAuthorization_t {
public:
    int PacketType;
    int Order;
    std::string secret;
    MSGPACK_DEFINE_MAP(PacketType, Order, secret);
};

class CPacketHeartbeat_t {
public:
    int PacketType;
    int Order;
    long time;
    MSGPACK_DEFINE_MAP(PacketType, Order, time);
};




typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

client c;
websocketpp::connection_hdl connectionHdl;

void SendPacket(std::string data, PacketType packet) {

    websocketpp::lib::error_code ec;

    std::string dataStr = data;

    char* sendData = (char*)malloc(dataStr.length() + 8);

    char* writeTo = (char*)(((__int64)sendData) + 8);

    __int32* dataPtrs = (__int32*)sendData;
    dataPtrs[0] = packet;
    dataPtrs[1] = (__int32)dataStr.length();

    memcpy(writeTo, dataStr.c_str(), dataStr.length());

    std::cout << "sending " << dataStr.length() << " bytes" << std::endl;

    c.send(connectionHdl, std::string(sendData, dataStr.length() + 8), websocketpp::frame::opcode::BINARY, ec);
}

DWORD WINAPI NewThread(LPVOID param) {
    for (;;) {
        Sleep(1000);

        CPacketHeartbeat_t heartbeat;
        heartbeat.PacketType = CPacketHeartbeat;
        heartbeat.Order = 0;
        heartbeat.time = time(NULL);

        std::stringstream stream;

        msgpack::pack(stream, heartbeat);

        SendPacket(stream.str(), CPacketHeartbeat);

    }
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


    if (type == SPacketStatus) {

        msgpack::unpacked result;

        unpack(result, data, length);

        msgpack::object obj(result.get());

        SPacketStatus_t status;

        free(ptr);

        obj.convert(status);

        std::cout << "Status: " << status.status.c_str() << " Code: " << status.code << std::endl;
        if (status.code == 403) {
            CPacketAuthorization_t authPacket;

            authPacket.Order = 0;
            authPacket.PacketType = CPacketAuthorization;
            authPacket.secret = "admin lol";

            std::stringstream stream;

            msgpack::pack(stream, authPacket);

            websocketpp::lib::error_code ec;

            std::string dataStr = stream.str();

            char* sendData = (char*)malloc(dataStr.length() + 8);

            char* writeTo = (char*)(((__int64)sendData) + 8);

            __int32* dataPtrs = (__int32*)sendData;
            dataPtrs[0] = CPacketAuthorization;
            dataPtrs[1] = (__int32)dataStr.length();

            memcpy(writeTo, dataStr.c_str(), dataStr.length());

            std::cout << "sending " << dataStr.length() << " bytes" << std::endl;

            c->send(hdl, std::string(sendData, dataStr.length() + 8), websocketpp::frame::opcode::BINARY, ec);
            free(sendData);
            CreateThread(NULL, NULL, NewThread, NULL, NULL, NULL);
        }
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




int main()
{
	std::string url = "wss://titnoas.xyz/ballsandsex/";

    std::cout << "Creating things" << std::endl;

    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);
        c.set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        std::cout << "Setting handlers" << std::endl;
        c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));

        c.set_tls_init_handler(bind(&on_tls_init, "titnoas.xyz", ::_1));

        std::cout << "handlers set" << std::endl;
        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(url , ec);
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
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }


}
