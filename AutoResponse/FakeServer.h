#ifndef __FAKESERVER_H__
#define __FAKESERVER_H__

#include"ClientPacket.h"
#include"ServerPacket.h"

bool FakeServer(ClientPacket &cp);

enum SERVERPACKET {
	// Login Server
	// Login Server - Character Select
	SP_CHARACTER_LIST = 0x05,
	SP_GOTO_LOGIN = 0x08,
	// Game Server
	SP_LOGOUT = 0x09,
	SP_GOTO_LOGIN_FROM_GAME =  0x0D,
	SP_CONNECTED = 0x0E,
	SP_CHARACTER_SPAWN = 0x11,
};

enum CLIENTPACKET {
	// Login
	CP_CREATE_CHARACTER = 0x05,
	CP_GOTO_LOGIN = 0x0A,
	CP_LOGOUT = 0x09,
	// Game
	CP_CHAT = 0x55,
};

#endif