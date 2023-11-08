#ifndef __CLIENTPACKET_H__
#define __CLIENTPACKET_H__

// Client to Server (Send, Out)

#include<Windows.h>
#include<string>
#include<vector>

enum CLIENT_PACKET {
	CP_BEGIN,
	CP_VERSION,
	CP_CONNECT_RESPONSE,
	CP_GAME_START,
	CP_CREATE_CHARACTER,
	CP_DELETE_CHARACTER,
	CP_INVITED,
	CP_BACK_TO_LOGIN_SERVER,
	CP_LOGOUT,
	CP_DROP_COIN,
	CP_PLAYER_MOVE,
	CP_USE_AP,
	CP_EMOTION,
	CP_UPDATE_PROFILE,
	CP_TUTORIAL_LEAVE,
	CP_ITEM_SHOP,
	CP_USE_SP,
	CP_USE_PORTAL,
	CP_CHANGE_CHANNEL,
	CP_PLAYER_CHAT,
	CP_FRIEND_REQUEST,
	CP_ITEM_SHOP_OPERATION,
	CP_PARK,
	CP_PARK_BATTLE_FIELD,
	CP_EVENT,
	CP_TIME_GET_TIME,
	CP_PING,
	CP_UNKNOWN,
	CP_END,
};

class ClientPacket {
private:
	static BYTE opcode[];

public:
	static BYTE* GetOpcode();

private:
	std::vector<BYTE> packet;
	DWORD decoded;

public:
	ClientPacket(BYTE *spacket, DWORD size);
	CLIENT_PACKET DecodeHeader();
	BYTE Decode1();
	WORD Decode2();
	DWORD Decode4();
	std::wstring DecodeWStr1();
	std::wstring DecodeWStr2();
	ULONGLONG Decode8();
	float DecodeFloat();
};

void SetClientPacketHeader_JP_v127();
void SetClientPacketHeader_CN_v126();
void SetClientPacketHeader_HK_v102();
void SetClientPacketHeader_KR_v107();

#endif