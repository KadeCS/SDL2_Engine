#pragma once
#include <openssl/opensslconf.h>

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <msgpack.hpp>

#include "Packets.h"

#include <iostream>

class Multiplayer
{
	public:
		static DWORD WINAPI connect(LPVOID agh);

		static void SendPacket(std::string data, PacketType packet);

		template<typename T>
		static void sendMessage(T p)
		{
			std::stringstream stream;

			msgpack::pack(stream, p);

			websocketpp::lib::error_code ec;

			std::string dataStr = stream.str();

			SendPacket(dataStr, (PacketType)p.PacketType);
		}

		static void setEntites(std::vector<Entity> ent);
		static std::vector<Entity> getEntites();

		static long localId;

		static std::string username;

		static void inQuotesGracefullyDisconnect();

		static std::vector<Entity> entites;

		static bool loggedIn;

		static void login(std::string username);
};

