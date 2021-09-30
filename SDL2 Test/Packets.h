#pragma once
#include <msgpack.hpp>

/// <summary>
/// (c) ur mom
/// </summary>

enum PacketType
{
    CPacketAuthorization = 1,
    CPacketHeartbeat = 2,
    CPacketHello = 3,
    CPacketUpdateEntity = 4,
    CPacketRequestGameState = 5,
    CPacketShoot = 6,
    SPacketHello = -1,
    SPacketStatus = -2,
    SPacketUpdateGameState = -3,
    SPacketInvalidPacket = -4,
    SPacketShootResponse = -5
};

class Packet
{
    public:
        int PacketType;
        long Order;
        MSGPACK_DEFINE_MAP(PacketType, Order);
};


class CPacketAuthorization_t : public Packet
{
    public:
        std::string secret;

        std::string username;

        MSGPACK_DEFINE_MAP(secret, username);
};

class CPacketHeartbeat_t : public Packet
{
    public:
        long time;

        MSGPACK_DEFINE_MAP(time);
};


class CPacketHello_t : public Packet
{
   
};

class CPacketRequestGameState_t : public Packet
{

};

class vector2 {
    public:
        float x;
        float y;

    MSGPACK_DEFINE_MAP(x,y);
};

class CPacketShoot_t : public Packet
{
    public:
        vector2 MousePosition;

        vector2 Position;

        vector2 VelocityDelta;
        MSGPACK_DEFINE_MAP(MousePosition, Position,VelocityDelta);
};

class CPacketUpdateEntity_t : public Packet
{
    public:
        unsigned int entityid;

        vector2 Position;
        MSGPACK_DEFINE_MAP(entityid,Position);
};

class SPacketStatus_t : public Packet
{
    public:
        int code;

        std::string status;

        MSGPACK_DEFINE_MAP(code,status);
};

enum EntityType
{
    Player_e = 0,
    Bullet_e = 1,
    Misc_e = -1,
    Wall_e = -2
};

MSGPACK_ADD_ENUM(EntityType);


class Entity
{
    public:
        int id;
        vector2 position;
        int EntityType;
        std::string username;

        vector2 direction;

        int ammo;
        float health;
        
        MSGPACK_DEFINE_MAP(id, position, EntityType, username, direction, health, ammo);
};


class SPacketHello_t : public Packet
{
    public:
        std::string hello;

        std::vector<Entity> Entities;

        unsigned int localPlayerID;

        MSGPACK_DEFINE_MAP(hello, Entities, localPlayerID);
};

class SPacketInvalidPacket_t : public Packet
{

};


class SPacketUpdateGameState_t : public Packet
{
    public:
        std::vector<Entity> Entities;

        MSGPACK_DEFINE_MAP(Entities);
};

class SPacketShootResponse_t : public Packet
{
    public:
        int id;

        std::vector<Entity> Entities;

        vector2 MousePosition;

        MSGPACK_DEFINE_MAP(id, Entities, MousePosition);
};