#ifndef __FAKESERVER_H__
#define __FAKESERVER_H__

#include"ClientPacket.h"
#include"ServerPacket.h"

bool FakeServer(ClientPacket &cp);
// test
void WorldListPacket();
void CharacterSelectPacket();
void CharacterListPacket();
void CharacterListPacket_Test();

enum SERVERPACKET {
	// Login Server
	SP_VERSION = 0x01,
	SP_CRASH = 0x02,
	SP_LOGIN_FAILED = 0x03,
	SP_CHARACTER_SELECT = 0x04,
	SP_CHARACTER_LIST = 0x05,
	SP_DELETE_CHARACTER_MSG = 0x07,
	SP_WORLD_SELECT = 0x08,
	SP_WORLD_LIST = 0x09,
	SP_CS_INVITED = 0x0A,
	SP_CS_KOREAN_MSG = 0x0B,
	// Game Server
	SP_GET_GAME_SERVER = 0x0C,
	SP_GET_LOGIN_SERVER = 0x0D,
	SP_CONNECTED = 0x0E,
	SP_MAP_RESET = 0x0F,
	SP_MAP_CHANGE = 0x10,
	SP_CHARACTER_SPAWN = 0x11,
	SP_ACCOUNT_DATA = 0x3D,
	SP_PLAYER_LEVEL_UP = 0x42,
	SP_PLAYER_STAT_EXP = 0x43,
	SP_PLAYER_STAT_SP = 0x45,
	SP_PLAYER_STAT_AP = 0x46,
	SP_FRIEND_REQUEST = 0x87,
	SP_GUILD_RANK_DOWN = 0x92,
	SP_GM_MSG = 0xD0,
};

enum CLIENTPACKET {
	// Login
	CP_VERSION = 0x01,
	CP_CONNECT_RESPONSE = 0x03, // 0x03 Recv -> 0x03 Send
	CP_GAME_START = 0x04,
	CP_CREATE_CHARACTER = 0x05,
	CP_DELETE_CHARACTER = 0x06,
	CP_INVITED = 0x07,
	CP_BACK_TO_LOGIN_SERVER = 0x0A,
	CP_LOGOUT = 0x09,
	// Game
	CP_DROP_COIN = 0x11,
	CP_USE_PORTAL = 0x4E,
	CP_PLAYER_CHAT = 0x55,
	CP_FRIEND_REQUEST = 0x74,
};

#endif