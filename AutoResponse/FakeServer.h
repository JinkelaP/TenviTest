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
	// Login Server - Character Select
	SP_VERSION = 0x01,
	SP_CRASH = 0x02,
	SP_CHARACTER_SELECT = 0x04,
	SP_CHARACTER_LIST = 0x05,
	SP_WORLD_SELECT = 0x08,
	SP_WORLD_LIST = 0x09,
	// Game Server
	SP_GET_GAME_SERVER = 0x0C,
	SP_GET_LOGIN_SERVER = 0x0D,
	SP_CONNECTED = 0x0E,
	SP_MAP_CHANGE = 0x10,
	SP_CHARACTER_SPAWN = 0x11,
	SP_ACCOUNT_DATA = 0x3D,
};

enum CLIENTPACKET {
	// Login
	CP_GAME_START = 0x04,
	CP_CREATE_CHARACTER = 0x05,
	CP_BACK_TO_LOGIN_SERVER = 0x0A,
	CP_LOGOUT = 0x09,
	// Game
	CP_CHAT = 0x55,
};

#endif