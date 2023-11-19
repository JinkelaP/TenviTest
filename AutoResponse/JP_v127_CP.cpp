#include"ClientPacket.h"

void SetClientPacketHeader_JP_v127() {
	BYTE *header = ClientPacket::GetOpcode();

	header[CP_VERSION] = 0x01;
	header[CP_CONNECT_RESPONSE] = 0x03;
	header[CP_GAME_START] = 0x04;
	header[CP_CREATE_CHARACTER] = 0x05;
	header[CP_DELETE_CHARACTER] = 0x06;
	header[CP_INVITED] = 0x07;
	header[CP_BACK_TO_LOGIN_SERVER] = 0x0A;
	header[CP_LOGOUT] = 0x09;
	header[CP_PLAYER_MOVEMENT] = 0x0C;
	header[CP_DROP_COIN] = 0x11;
	header[CP_GUARDIAN_RIDE] = 0x18;
	header[CP_GUARDIAN_MOVEMENT] = 0x19;
	header[CP_USE_AP] = 0x1A;
	header[CP_GUARDIAN_SUMMON] = 0x1B;
	header[CP_EMOTION] = 0x1C;
	header[CP_UPDATE_PROFILE] = 0x20;
	header[CP_TUTORIAL_LEAVE] = 0x21;
	header[CP_WORLD_MAP_OPEN] = 0x23;
	header[CP_ITEM_SHOP] = 0x38;
	// 0x43 player hit or mob hit
	// 0x44 use skill
	header[CP_USE_SP] = 0x45;
	// 0x46 skill effect or skill charge
	// 0x47 skill hit mob
	// 0x49 skill attack
	header[CP_USE_PORTAL] = 0x4E;
	header[CP_PLAYER_REVIVE] = 0x4F,
	header[CP_CHANGE_CHANNEL] = 0x50;
	header[CP_NPC_MOVEMENT] = 0x52;
	header[CP_NPC_TALK] = 0x53;
	header[CP_PLAYER_CHAT] = 0x55;
	header[CP_FRIEND_REQUEST] = 0x74;
	header[CP_ITEM_SHOP_OPERATION] = 0xAE;
	header[CP_PARK] = 0xC0;
	header[CP_PARK_BATTLE_FIELD] = 0xC3;
	header[CP_EVENT] = 0xC6;
	header[CP_TIME_GET_TIME] = 0xC8;
	header[CP_PING] = 0xF1;
}