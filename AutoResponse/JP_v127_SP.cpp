#include"ServerPacket.h"

void SetServerPacketHeader_JP_v127() {
	BYTE *header = ServerPacket::GetOpcode();

	header[SP_VERSION] = 0x01;
	header[SP_CRASH] = 0x02;
	header[SP_LOGIN_FAILED] = 0x03;
	header[SP_CHARACTER_SELECT] = 0x04;
	header[SP_CHARACTER_LIST] = 0x05;
	header[SP_DELETE_CHARACTER_MSG] = 0x07;
	header[SP_WORLD_SELECT] = 0x08;
	header[SP_WORLD_LIST] = 0x09;
	header[SP_CS_INVITED] = 0x0A;
	header[SP_CS_KOREAN_MSG] = 0x0B;
	header[SP_GET_GAME_SERVER] = 0x0C;
	header[SP_GET_LOGIN_SERVER] = 0x0D;
	header[SP_CONNECTED] = 0x0E;
	header[SP_MAP_RESET] = 0x0F;
	header[SP_MAP_CHANGE] = 0x10;
	header[SP_CHARACTER_SPAWN] = 0x11;
	header[SP_REMOVE_OBJECT] = 0x12;
	header[SP_CREATE_OBJECT] = 0x14;
	header[SP_ACTIVATE_OBJECT] = 0x20;
	// 0x22 Object Movement
	header[SP_HIT] = 0x21;
	header[SP_SHOW_OBJECT] = 0x23;
	// 0x29 Object Teleport
	header[SP_IN_MAP_TELEPORT] = 0x3C;
	header[SP_ACCOUNT_DATA] = 0x3D;
	header[SP_PLAYER_HIT] = 0x41;
	header[SP_PLAYER_LEVEL_UP] = 0x42;
	header[SP_PLAYER_STAT_EXP] = 0x43;
	header[SP_PLAYER_STAT_SP] = 0x45;
	header[SP_PLAYER_STAT_AP] = 0x46;
	header[SP_PLAYER_STAT_ALL] = 0x47;
	header[SP_GUARDIAN_SUMMON] = 0x4A,
	header[SP_EMOTION] = 0x4B;
	header[SP_WORLD_MAP_UPDATE] = 0x54;
	header[SP_PLAYER_REVIVE] = 0x5B,
	header[SP_ITEM_SHOP_ERROR] = 0x5C;
	header[SP_UPDATE_SKILL] = 0x66;
	header[SP_PLAYER_SKILL_ALL] = 0x6D;
	header[SP_FRIEND_REQUEST] = 0x87;
	header[SP_GUILD_RANK_DOWN] = 0x92;
	header[SP_GM_MSG] = 0xD0;
	header[SP_BOARD] = 0xE0;
}