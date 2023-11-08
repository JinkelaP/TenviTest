#ifndef __FAKESERVER_H__
#define __FAKESERVER_H__

#include"ClientPacket.h"
#include"ServerPacket.h"

#define MAPID_ITEM_SHOP 65535
#define MAPID_PARK 65534
#define MAPID_EVENT 62501

bool FakeServer(ClientPacket &cp);
// test
void WorldListPacket();
void CharacterSelectPacket();
void CharacterListPacket();
void CharacterListPacket_Test();

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
	CP_PLAYER_MOVE = 0x19,
	CP_USE_AP = 0x1A,
	CP_EMOTION = 0x1C,
	CP_UPDATE_PROFILE = 0x20,
	CP_TUTORIAL_LEAVE = 0x21,
	CP_ITEM_SHOP = 0x38,
	CP_USE_SP = 0x45,
	CP_USE_PORTAL = 0x4E,
	CP_CHANGE_CHANNEL = 0x50,
	CP_PLAYER_CHAT = 0x55,
	CP_FRIEND_REQUEST = 0x74,
	CP_ITEM_SHOP_OPERATION = 0xAE,
	CP_PARK = 0xC0,
	CP_PARK_BATTLE_FIELD = 0xC3,
	CP_EVENT = 0xC6,
	CP_TIME_GET_TIME = 0xC8,
	CP_PING = 0xF1,
};

#endif