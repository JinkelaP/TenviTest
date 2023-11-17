#include"FakeServer.h"
#include"AutoResponse.h"
#include"TemporaryData.h"

TenviAccount TA;
// ========== TENVI Packet Response ==========
#define TENVI_VERSION 0x1023

// 0x01
void VersionPacket() {
	ServerPacket sp(SP_VERSION);
	sp.Encode4(TENVI_VERSION); // 00491388, version
	sp.Encode4(1); // 00491391
	sp.Encode4(1); // 0049139A
	sp.Encode4(1); // 004913A4
	SendPacket(sp);
}

// 0x02
void CrashPacket() {
	ServerPacket sp(SP_CRASH);
	sp.Encode4(TENVI_VERSION); // version
	SendPacket(sp);
}

// 0x03
void LoginFailedPacket() {
	ServerPacket sp(SP_LOGIN_FAILED);
	SendPacket(sp);
}

// 0x04
void CharacterSelectPacket() {
	ServerPacket sp(SP_CHARACTER_SELECT);
	sp.Encode1(0); // 00492CE6, 0 = OK, 1 = login error
	sp.Encode4(-1); // 00492D5B saved to pointer
	sp.Encode1(0); // 00492D65 saved to pointer

	if (GetRegion() == TENVI_KR) {
		sp.Encode1(0);
		sp.EncodeWStr1(L"Tenvi KR v200");
	}

	SendPacket(sp);
}

// 0x05
int character_slot = 6;
void CharacterListPacket() {
	ServerPacket sp(SP_CHARACTER_LIST);
	sp.Encode1(0); // Number of Characters
	sp.Encode1(character_slot); // Number of Character Slot
	DelaySendPacket(sp);
}

void CharacterListPacket_Test() {
	ServerPacket sp(SP_CHARACTER_LIST);

	sp.Encode1((BYTE)TA.GetCharacters().size()); // characters
	for (auto &chr : TA.GetCharacters()) {
		sp.Encode4(chr.id); // ID
		sp.Encode1(chr.job_mask);
		sp.Encode1((BYTE)chr.level); // level
		sp.EncodeWStr1(chr.name); // name
		sp.EncodeWStr1(L"");
		sp.Encode2(chr.job);
		sp.Encode2(chr.skin);
		sp.Encode2(chr.hair);
		sp.Encode2(chr.face);
		sp.Encode2(chr.cloth);
		sp.Encode2(chr.gcolor);
		// character equip, max 15
		for (int i = 0; i < 15; i++) {
			sp.Encode2(0);
		}
		// guardian equip, max 15
		for (auto gequip : chr.gequipped) {
			sp.Encode2(gequip);
		}
		sp.Encode2(chr.map); // mapid
	}

	if (GetRegion() == TENVI_KR) {
		sp.Encode1(0);
	}

	sp.Encode1(TA.slot); // character slots
	SendPacket(sp);
}

// 0x07
void DeleteCharacter() {
	ServerPacket sp(SP_DELETE_CHARACTER_MSG);
	sp.Encode1(0); // 00491C61, error code 1,2,4
	SendPacket(sp);
}

// 0x08
void WorldSelectPacket() {
	ServerPacket sp(SP_WORLD_SELECT);
	SendPacket(sp);
}

// 0x09
int worlds = 1;
int channels = 5;
void WorldListPacket() {
	ServerPacket sp(SP_WORLD_LIST);
	sp.EncodeWStr2(L""); // 004938BC, Message

	if (GetRegion() == TENVI_JP || GetRegion() == TENVI_CN) {
		sp.Encode1(0); // 004938C8, NetCafe
	}

	sp.Encode1(worlds); // 00493934, Number of Worlds
	for(int world = 0; world < worlds; world++) {
		sp.Encode1(2); // 00493979, World Mark
		sp.EncodeWStr1(L"Spica"); // 0049398A World Name
		sp.Encode8(0); // 004939AD

		sp.Encode1(1); // 004939BA
		{
			sp.EncodeWStr1(L"127.0.0.1"); // 004939D2, IP
			sp.Encode2(8787); // 004939F5, Port
		}

		sp.Encode1(channels); // 00493A0D, Number of channels
		for (int channel = 0; channel < channels; channel++) {
			sp.Encode1(channel + 1); // 00493A1E
			sp.Encode1(50); // 00493A28, Population (CH1 and CH2 are edited by client side lol)
		}
	}

	sp.Encode1(1); // 00493A6A
	sp.Encode1(0); // 00493A7B

	if (GetRegion() == TENVI_JP || GetRegion() == TENVI_CN) {
		sp.Encode1(0); // 00493A88
	}

	sp.Encode1(0); // 00493A92

	SendPacket(sp);
}

// 0x0A not used in JP
void CharacterSelectInvitedPacket() {
	ServerPacket sp(SP_CS_INVITED);
	sp.Encode1(0); // 00491CEF, 0-2
	sp.Encode4(0); // 00491E20, unique code
	SendPacket(sp);
}

// 0x0B not used in JP
void CharacterSelectUnknownMsgPacket() {
	ServerPacket sp(SP_CS_KOREAN_MSG);
	sp.Encode1(1); // 00491AF9, show dialog
	sp.Encode1(0); // 00491B1E, not used
	SendPacket(sp);
}

// 0x0C
void GetGameServerPacket() {
	ServerPacket sp(SP_GET_GAME_SERVER);
	sp.Encode1(0); // error code
	sp.Encode4(0x0100007F); // IP
	sp.Encode2(8787); // port
	sp.Encode4(0);
	sp.Encode4(0);
	SendPacket(sp);
}

// 0x0D
void GetLoginServerPacket() {
	ServerPacket sp(SP_GET_LOGIN_SERVER);
	sp.Encode1(0); // error code
	sp.Encode4(0);
	sp.Encode4(0);
	sp.Encode4(0x0100007F); // IP
	sp.Encode2(8787); // port
	SendPacket(sp);
}

// 0x0E
void ConnectedPacket() {
	ServerPacket sp(SP_CONNECTED);
	sp.Encode1(0); // error code
	SendPacket(sp);
}

// 0x0F
void MapResetPacket() {
	ServerPacket sp(SP_MAP_RESET);
	sp.Encode1(0); // error code
	sp.Encode4(0);
	sp.Encode4(0);
	sp.Encode4(0);
	sp.Encode2(0);
	sp.Encode2(0);
	SendPacket(sp);
}

// 0x10
void ChangeMapPacket(WORD mapid, float x = 0, float y = 0) {
	ServerPacket sp(SP_MAP_CHANGE);
	sp.Encode1(0); // error code = 37
	sp.Encode2(mapid); // mapid
	sp.Encode1(0); // 1 = empty map?
	sp.Encode4(0);
	sp.EncodeFloat(x); // float value
	sp.EncodeFloat(y); // float value
	sp.Encode1(0);
	sp.Encode1(0);
	sp.Encode1(0); // disable item shop and park
	sp.Encode1(0);
	sp.Encode4(0);
	SendPacket(sp);
}

// 0x11
void CharacterSpawnPacket(TenviCharacter &chr, float x = 0, float y = 0) {
	ServerPacket sp(SP_CHARACTER_SPAWN);
	sp.Encode4(chr.id); // 0048DB9B id, where checks id?
	sp.EncodeFloat(x); // 0048DBA5, coordinate x
	sp.EncodeFloat(y); // 0048DBAF, corrdinate y
	sp.Encode1(0); // 0048DBB9, direction 0 = left, 1 = right
	sp.Encode1(1); // 0048DBC6, guardian, 0 = guardian off, 1 = guardian on
	sp.Encode1(1); // 0048DBD3, death, 0 = death, 1 = alive
	sp.Encode1(0); // 0048DBE0, battle, 0 = change channel OK, 1 = change channel NG
	sp.Encode4(0); // 0048DBFB, ???
	sp.Encode1(chr.job_mask); // 0048DC08
	sp.Encode1((BYTE)chr.level); // 0048DC2B

	if (GetRegion() == TENVI_HK || GetRegion() == TENVI_KRX) {
		sp.Encode1(1);
	}

	sp.EncodeWStr1(chr.name); // name
	sp.EncodeWStr1(L""); // guardian name
	sp.Encode1(0); // 0048DC8F
	sp.Encode1(1); // 0048DC9C
	sp.Encode2(chr.job); // 0048DCA9
	sp.Encode2(chr.skin); // 0048DCB7
	sp.Encode2(chr.hair); // 0048DCC5
	sp.Encode2(chr.face); // 0048DCD3
	sp.Encode2(chr.cloth); // 0048DCE1
	sp.Encode2(chr.gcolor); // 0048DCEF
	sp.Encode1(0); // 0048DCFD

	// character equip
	for (auto equip : chr.equipped) {
		sp.Encode8(0);
		sp.Encode2(equip);
	}

	// guardian equip
	for (auto gequip : chr.gequipped) {
		sp.Encode8(0);
		sp.Encode2(gequip);
	}

	sp.Encode2(0); // 0048DDC3
	sp.Encode2(0); // 0048DDD1
	sp.Encode2(0); // 0048DDE1
	sp.Encode2(0); // 0048DDF1
	sp.Encode2(0); // 0048DE01

	if (GetRegion() == TENVI_JP || GetRegion() == TENVI_CN) {
		sp.Encode1(0); // 0048DE11
		sp.Encode1(0); // 0048DE21
		sp.Encode1(0); // 0048DE35
	}

	sp.EncodeWStr1(L""); // guild
	sp.Encode1(0); // 0057B508
	sp.Encode1(0); // 0057B515
	sp.Encode1(0); // 0057B522
	sp.Encode1(0); // 0057B52F
	sp.Encode1(0); // 0057B53C
	sp.Encode1(0); // 0057B549
	sp.Encode1(0); // 0048DE7F
	sp.Encode1(0); // 0048DE8C
	sp.Encode1(0); // 0048DE9F
	sp.Encode1(0); // 0048DEAC
	sp.Encode1(0); // 0048DEB9
	sp.Encode1(0); // 0048E4F3
	sp.Encode4(0); // 0048E513
	sp.Encode1(0); // 0048E51D
	sp.Encode1(0); // 0048E5F9
	sp.Encode1(0); // 0048E60D
	sp.Encode1(0); // 0048E621
	sp.Encode4(0); // 0048E6A1
	sp.Encode2(0); // 0048E6AB
	sp.Encode4(0); // 0048E6C7
	sp.Encode4(0); // 0048E6D1
	sp.Encode4(0); // 0048E6DB
	sp.Encode4(0); // 0048E6E5
	sp.Encode4(0); // 0048E6EF
	sp.Encode1(0); // 0048E6F9
	sp.Encode1(0); // 0048E706
	sp.Encode4(0); // 0048E713
	sp.Encode4(0); // 0048E71D
	sp.Encode4(0); // 0048E727
	sp.Encode4(0); // 0048E731
	sp.EncodeWStr1(L""); // 0048E73F
	sp.Encode2(0); // 0048E74A
	sp.Encode2(0); // 0048E757
	sp.EncodeWStr1(L""); // 0048E768
	sp.Encode1(0); // 0048E773
	sp.Encode1(0); // 0048E780

	if (GetRegion() == TENVI_HK || GetRegion() == TENVI_KR || GetRegion() == TENVI_KRX) {
		sp.Encode1(0);
		sp.Encode1(0);
	}

	SendPacket(sp);
}

// 0x12
void RemoveObjectPacket(DWORD object_id) {
	ServerPacket sp(SP_REMOVE_OBJECT);
	sp.Encode4(object_id); // not only for character
	SendPacket(sp);
}

// 0x14
void CreateObjectPacket(TenviRegen &regen) {
	ServerPacket sp(SP_CREATE_OBJECT);
	sp.Encode4(regen.id);
	sp.Encode2(regen.object.id); // npc, mob id
	sp.Encode1(0);
	sp.Encode4(0);
	sp.Encode1(0);
	sp.Encode1(0);
	sp.Encode4(0);
	sp.Encode1(0);
	sp.Encode2(0);
	sp.EncodeFloat(regen.area.left);
	sp.EncodeFloat(regen.area.top);
	sp.Encode2(0);
	sp.EncodeFloat(regen.area.left);
	sp.EncodeFloat(regen.area.top);
	sp.EncodeFloat(regen.area.right);
	sp.EncodeFloat(regen.area.bottom);
	sp.Encode1(0);
	sp.Encode1(0);
	SendPacket(sp);
}

// 0x20
void ActivateObjectPacket(TenviRegen &regen) {
	ServerPacket sp(SP_ACTIVATE_OBJECT);
	sp.Encode4(regen.id);
	sp.Encode1(3); // ?
	SendPacket(sp);
}

// 0x23
void ShowObjectPacket(TenviRegen &regen) {
	ServerPacket sp(SP_SHOW_OBJECT);
	sp.Encode4(regen.id);
	sp.Encode1(1);
	sp.Encode1(1);
	sp.Encode2(0);
	sp.EncodeFloat(regen.area.left);
	sp.EncodeFloat(regen.area.top);
	SendPacket(sp);
}

// 0x3C
void InMapTeleportPacket(TenviCharacter &chr) {
	ServerPacket sp(SP_IN_MAP_TELEPORT);
	sp.Encode4(chr.id); // 00489222, character id
	sp.Encode1(1); // 0048923E, 0 = fall down, 1 = teleport to platform
	sp.Encode4(0); // 0048924B, x?
	sp.Encode4(0); // 00489255, y?
	sp.Encode1(0); // 0048925F, 0 = face left, 1 = face right
	sp.Encode1(0); // 00489269
	sp.Encode1(0); // 00489276
	sp.Encode1(1); // 00489283, 0 = no guardian, 1 = with guardian
	SendPacket(sp);
}

// 0x3D
void AccountDataPacket(TenviCharacter &chr) {
	ServerPacket sp(SP_ACCOUNT_DATA);
	sp.Encode4(0); // 00498E4F, ???
	sp.Encode4(chr.id); // 00498E5C, character id
	sp.EncodeWStr1(chr.name); // 00498E79, character name
	sp.EncodeWStr1(L""); // 00498EA5, ???
	sp.Encode1(chr.job_mask); // 00498ECD

	if (GetRegion() == TENVI_HK || GetRegion() == TENVI_KRX) {
		sp.Encode1(10); // unk
	}

	sp.Encode1((BYTE)chr.level); // 00498EF0
	sp.Encode8(1234); // 00498F0C, EXP
	sp.Encode8(77770503); // 00498F28, Coin (Gold, Silver, Bronze)
	sp.Encode8(0); // 00498F44, ???
	sp.Encode1(0); // 00498F60
	sp.Encode1(0); // 00498F70

	if (GetRegion() == TENVI_JP || GetRegion() == TENVI_CN) {
		sp.Encode1(0); // 00498F80
	}

	{
		sp.EncodeWStr1(L"TENVI"); // 0057A877, Guild Name
		{
			sp.Encode1(0); // 0057B508
			sp.Encode1(0); // 0057B515
			sp.Encode1(0); // 0057B522
			sp.Encode1(0); // 0057B52F
			sp.Encode1(0); // 0057B53C
			sp.Encode1(0); // 0057B549
		}
	}
	sp.EncodeWStr1(L"chr unk1"); // 00498FA0

	if (GetRegion() == TENVI_JP) {
		sp.Encode1(0); // 00498FC8
		sp.Encode1(0); // 00498FD8
		sp.Encode1(0); // 00498FF0
	}

	// loop 4
	{
		sp.Encode1(5 * 8); // 00499024, Equip Slot
		sp.Encode1(5 * 8); // 00499024, Item Slot
		sp.Encode1(5 * 8); // 00499024, Cash Slot
		sp.Encode1(4 * 10); // 00499024, Card Slots

		if (GetRegion() == TENVI_HK || GetRegion() == TENVI_KR || GetRegion() == TENVI_KRX) {
			sp.Encode1(4); // unk
		}

	}
	sp.EncodeWStr1(L"TenviTest"); // 00499044, Profile Message
	sp.Encode1(0); // 0049906C, ???
	sp.Encode1(0); // 004990B3
	// loop
	{
		/*
		sp.Encode1(0);
		sp.Encode2(0);
		sp.Encode1(0);
		*/
	}
	sp.Encode1(0); // 00499101, ???
	sp.Encode4(0); // 00499117
	sp.Encode1(0); // 00499124
	sp.Encode1(0); // 00499134, Married?
	sp.Encode4(0); // 00499144, Partner Character ID?
	sp.EncodeWStr1(L""); // 00499155, Partner Name?
	sp.Encode1(0); // 0049917D, Item Shop New Icon
	sp.Encode1(0); // 0049918D, Item Shop Box Icon
	sp.Encode1(0); // 0049919D, ???

	if (GetRegion() == TENVI_CN) {
		sp.Encode1(0);
	}

	SendPacket(sp);
}

// 0x42
void PlayerLevelUpPacket(TenviCharacter &chr) {
	ServerPacket sp(SP_PLAYER_LEVEL_UP);
	sp.Encode4(chr.id); // 00488FD1, id
	sp.Encode1(chr.level + 1); // 00488FDB, 0 = rank up, others are level
	SendPacket(sp);
}

// 0x45
void PlayerSPPacket(TenviCharacter &chr) {
	ServerPacket sp(SP_PLAYER_STAT_SP);
	sp.Encode2(chr.sp);
	SendPacket(sp);
}
// 0x46
void PlayerAPPacket(TenviCharacter &chr) {
	ServerPacket sp(SP_PLAYER_STAT_AP);
	sp.Encode2(chr.ap);
	SendPacket(sp);
}

// 0x47
void PlayerStatPacket(TenviCharacter &chr) {
	ServerPacket sp(SP_PLAYER_STAT_ALL);
	sp.Encode2(3000); // 004956F5, HP
	sp.Encode2(4000); // 00495713, MAXHP
	sp.Encode2(1000); // 0049572F, MP
	sp.Encode2(2000); // 0049574B, MAXMP
	sp.Encode2(chr.stat_str); // 00495767, 力 (STR)
	sp.Encode2(chr.stat_dex); // 00495783, 敏捷 (DEX)
	sp.Encode2(chr.stat_hp); // 0049579F, 体力 (HP)
	sp.Encode2(chr.stat_int); // 004957BB, 知能 (INT)
	sp.Encode2(chr.stat_mp); // 004957D7, 知恵 (MP)
	sp.Encode2(988); // 004957F3, 物理ダメージ Min
	sp.Encode2(1006); // 0049580F, 物理ダメージ Max
	sp.Encode2(1000); // 0049582B, 物理攻撃力
	sp.Encode2(2718); // 00495847, 魔法攻撃力
	sp.Encode2(1887); // 00495863, 防御力
	sp.Encode2(9130); // 0049587F, 物理命中率
	sp.Encode2(9763); // 004958A7, 魔法命中率
	sp.Encode2(129); // 004958CF, 回避率
	sp.Encode2(189); // 004958F7, 物理クリティカル
	sp.Encode2(2279); // 0049591F, 魔法クリティカル

	if (GetRegion() == TENVI_KRX) {
		sp.Encode2(0);
	}

	sp.Encode2(131); // 00495947, 飛行スピード
	sp.Encode2(100); // 0049596F, 歩行スピード
	sp.Encode2(22); // 00495997, 炎抵抗力
	sp.Encode2(23); // 004959B3, 氷抵抗力

	if (GetRegion() != TENVI_KRX) {
		sp.Encode2(24); // 004959CF, 生抵抗力
	}

	sp.Encode2(25); // 004959EB, 光抵抗力
	sp.Encode2(26); // 00495A07, 闇抵抗力

	if (GetRegion() != TENVI_KRX) {
		sp.Encode2(0); // 00495A23, 力差分
		sp.Encode2(0); // 00495A42, 敏捷差分
		sp.Encode2(0); // 00495A61
		sp.Encode2(0); // 00495A80
		sp.Encode2(0); // 00495A9F
	}
	else {
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.EncodeFloat(0.0);
		sp.Encode4(0);
	}


	SendPacket(sp);
}

// 0x4B
void EmotionPacket(TenviCharacter &chr, BYTE emotion) {
	ServerPacket sp(SP_EMOTION);
	sp.Encode4(chr.id); // 0048608E, character id
	sp.Encode1(emotion); // 00486099, emotion
	SendPacket(sp);
}

// 0x54
void WorldMapUpdatePacket(BYTE area_code) {
	ServerPacket sp(SP_WORLD_MAP_UPDATE);
	sp.Encode1(area_code); // 00496E95
	sp.Encode1(0); // 00496EB7
	sp.Encode1(1); // 00496ED7
	sp.Encode1(area_code); // 00496EE7
	DelaySendPacket(sp);
}

void WorldMapUpdatePacketTest(BYTE area_code) {
	ServerPacket sp(SP_WORLD_MAP_UPDATE);
	sp.Encode1(area_code);

	for (int i = 1; i < 256; i++) {
		sp.Encode1((BYTE)i);
	}
	sp.Encode1(0);

	std::vector<BYTE> activated_area;

	activated_area.push_back(8); // シルヴァアイランド
	activated_area.push_back(2); // リブラアイランド
	activated_area.push_back(5); // タリーB1アイランド
	activated_area.push_back(6); // ミノスアイランド

	if (GetRegion() != TENVI_HK) {
		activated_area.push_back(1); // ビキウィニーアイランド
		activated_area.push_back(3); // ファントムアイランド
		activated_area.push_back(4); // プチポチパーク
	}

	if (GetRegion() == TENVI_HK || GetRegion() == TENVI_KR || GetRegion() == TENVI_KRX) {
		activated_area.push_back(7); // 蓋亞藩
	}

	sp.Encode1(activated_area.size()); // Number of Islands
	for (auto &v : activated_area) {
		sp.Encode1(v);
	}

	DelaySendPacket(sp);
}


// 0x5C
void EnterItemShopErrorPacket() {
	ServerPacket sp(SP_ITEM_SHOP_ERROR);
	sp.Encode1(1); // 004C93B9, error code 1-7
	SendPacket(sp);
}

// 0x66
void UpdateSkillPacket(TenviCharacter &chr, WORD skill_id) {
	ServerPacket sp(SP_UPDATE_SKILL);
	sp.Encode4(chr.id); // 00485E65, character id
	sp.Encode2(skill_id); // 00485E6F, skill id
	sp.Encode1(1); // 00485E7A, 0 = failed, 1 = success
	DelaySendPacket(sp);
}

// 0x6D
void InitSkillPacket(TenviCharacter &chr) {
	ServerPacket sp(SP_PLAYER_SKILL_ALL);
	sp.Encode1((BYTE)chr.skill.size()); // 0049977E, number of skills

	for (auto v : chr.skill) {
		sp.Encode1(1); // 00499792, idk
		sp.Encode2(v.id); // 0049979F, skill id
		sp.Encode1(v.level); // 004997AA, skill point
	}

	SendPacket(sp);
}

// 0xE0
enum BoardAction {
	Board_Spawn = 0,
	Board_Remove = 1,
	Board_AddInfo = 2,
};
void BoardPacket(BoardAction action, std::wstring owner = L"", std::wstring msg = L"") {
	ServerPacket sp(SP_BOARD);

	sp.Encode1(action); // 0048F653, 0 = spawn object, 1 = remove object, 2 = insert info

	switch (action) {
	case Board_Spawn: {
		sp.Encode4(3131); // 0048AEF6 object id
		sp.Encode4(1337); // 0048AF00 ???
		sp.EncodeWStr1(owner); // 0048AF0E ???
		sp.EncodeWStr1(msg); // 0048AF1D message
		sp.Encode4(0); // 0048AF28
		sp.Encode4(0); // 0048AF32
		sp.Encode1(0); // 0048AF3C
		sp.Encode1(3); // 0048AF46 board type
		break;
	}
	case Board_Remove: {
		sp.Encode4(3131); // object id
		break;
	}
	case Board_AddInfo: {
		sp.Encode4(3131); // object id
		sp.EncodeWStr1(msg); // 0048AF1D message
		break;
	}
	default: {
		// error
		return;
	}
	}

	SendPacket(sp);
}

// ========== Functions ==================

void SpawnObjects(TenviCharacter &chr, WORD map_id) {
	for (auto &regen : tenvi_data.get_map(map_id)->GetRegen()) {
		CreateObjectPacket(regen);
		ShowObjectPacket(regen);
		ActivateObjectPacket(regen);
	}
}

// go to map
void ChangeMap(TenviCharacter &chr, WORD map_id, float x, float y) {
	ChangeMapPacket(map_id, x, y);
	
	switch (map_id) {
	case MAPID_ITEM_SHOP:
	case MAPID_EVENT:
	case MAPID_PARK:
	{
		// do not change last map id
		chr.SetMapReturn(chr.map);
		break;
	}
	default:
	{
		chr.SetMapReturn(chr.map);
		chr.map = map_id;
		break;
	}
	}
	CharacterSpawnPacket(chr, x, y);
	SpawnObjects(chr, map_id);
}

// enter map by login or something
void SetMap(TenviCharacter &chr, WORD map_id) {
	TenviSpawnPoint spawn_point = tenvi_data.get_map(map_id)->FindSpawnPoint(0);
	ChangeMap(chr, map_id, spawn_point.x, spawn_point.y);
}

// enter map by portal
void UsePortal(TenviCharacter &chr, DWORD portal_id) {
	TenviPortal portal = tenvi_data.get_map(chr.map)->FindPortal(portal_id); // current map
	TenviPortal next_portal = tenvi_data.get_map(portal.next_mapid)->FindPortal(portal.next_id); // next map

	ChangeMap(chr, portal.next_mapid, next_portal.x, next_portal.y);
}

void ItemShop(TenviCharacter &chr, bool bEnter) {
	if (bEnter) {
		SetMap(chr, MAPID_ITEM_SHOP);
	}
	else {
		SetMap(chr, chr.map_return);
	}
}

void Park(TenviCharacter &chr, bool bEnter) {
	if (bEnter) {
		SetMap(chr, MAPID_PARK);
	}
	else {
		SetMap(chr, chr.map_return);
	}
}

void Event(TenviCharacter &chr, bool bEnter) {
	if (bEnter) {
		SetMap(chr, MAPID_EVENT);
	}
	else {
		SetMap(chr, chr.map_return);
	}
}

// ========== TENVI Server Main ==========
bool FakeServer(ClientPacket &cp) {
	CLIENT_PACKET header = cp.DecodeHeader();

	switch (header) {
	// Select Character
	case CP_GAME_START: {
		DWORD character_id = cp.Decode4();
		BYTE channel = cp.Decode1();

		TA.Login(character_id);

		for (auto &chr : TA.GetCharacters()) {
			if (chr.id == character_id) {
				chr.x = 0.0;
				chr.y = 0.0;
				GetGameServerPacket(); // notify game server ip
				ConnectedPacket(); // connected
				AccountDataPacket(chr);
				PlayerStatPacket(chr);
				PlayerSPPacket(chr);
				PlayerAPPacket(chr);
				InitSkillPacket(chr);

				SetMap(chr, chr.map);
				BoardPacket(Board_Spawn, L"Riremito", L"Tenvi JP v127");
				BoardPacket(Board_AddInfo, L"Riremito", L"Tenvi JP v127");
				return true;
			}
		}
		return false;
	}
	// Create New Character
	case CP_CREATE_CHARACTER: {
		std::wstring character_name = cp.DecodeWStr1();
		BYTE job_mask = cp.Decode1(); // 0x11 to 0x24
		WORD job_id = cp.Decode2(); // 4,5,6
		WORD character_skin = cp.Decode2(); // 1,2,3
		WORD character_hair = cp.Decode2();
		WORD character_face = cp.Decode2();
		WORD character_cloth = cp.Decode2();
		WORD guardian_color = cp.Decode2();

		WORD guardian_head = cp.Decode2();
		WORD guardian_body = cp.Decode2();
		WORD guardian_weapon = cp.Decode2();

		std::vector<WORD> guardian_equip;
		guardian_equip.push_back(guardian_head);
		guardian_equip.push_back(guardian_body);
		guardian_equip.push_back(guardian_weapon);

		TA.AddCharacter(character_name, job_mask, job_id, character_skin, character_hair, character_face, character_cloth, guardian_color, guardian_equip);
		CharacterListPacket_Test();
		return true;
	}
	// Delete Character
	case CP_DELETE_CHARACTER: {
		DWORD character_id = cp.Decode4();
		// not coded
		return true;
	}
	// Character Select to World Select
	case CP_BACK_TO_LOGIN_SERVER: {
		WorldSelectPacket(); // back to login server
		WorldListPacket(); // show world list
		return true;
	}
	// Game Server to Login Server
	case CP_LOGOUT: {
		GetLoginServerPacket();// notify login server ip
		ConnectedPacket(); // connected
		CharacterListPacket_Test();
		return true;
	}
	case CP_USE_AP: {
		TenviCharacter &chr = TA.GetOnline();
		BYTE stat = cp.Decode1();
		chr.UseAP(stat);
		PlayerAPPacket(chr);
		PlayerStatPacket(chr);
		return true;
	}
	case CP_EMOTION: {
		TenviCharacter &chr = TA.GetOnline();
		BYTE emotion = cp.Decode1();
		EmotionPacket(chr, emotion);
		return true;
	}
	case CP_UPDATE_PROFILE: {
		std::wstring wText = cp.DecodeWStr1();
		return true;
	}
	case CP_WORLD_MAP_OPEN:
	{
		BYTE area_code = cp.Decode1();
		//WorldMapUpdatePacket(area_code);
		WorldMapUpdatePacketTest(area_code);
		return true;
	}
	case CP_ITEM_SHOP: {
		BYTE flag = cp.Decode1();
		ItemShop(TA.GetOnline(), flag ? true : false);
		return true;
	}
	case CP_USE_SP: {
		TenviCharacter &chr = TA.GetOnline();
		WORD skill_id = cp.Decode2();
		chr.UseSP(skill_id);
		UpdateSkillPacket(chr, skill_id);
		PlayerSPPacket(chr);
		return true;
	}
	case CP_USE_PORTAL: {
		TenviCharacter &chr = TA.GetOnline();
		DWORD portal_id = cp.Decode4();
		// cp.DecodeWStr1();
		UsePortal(chr, portal_id);
		return true;
	}
	case CP_CHANGE_CHANNEL: {
		BYTE channel = cp.Decode1();
		return true;
	}
	case CP_PLAYER_CHAT: {
		BYTE type = cp.Decode1(); // 0 = map chat
		cp.Decode1(); // 1
		cp.Decode1(); // 0
		std::wstring message = cp.DecodeWStr1();

		// command test
		if (message.length() && message.at(0) == L'@') {
			if (_wcsnicmp(message.c_str(), L"@map ", 5) == 0) {
				int map_id = _wtoi(&message.c_str()[5]);
				SetMap(TA.GetOnline(), map_id);
			}
			return true;
		}

		return true;
	}
	case CP_PARK: {
		BYTE flag = cp.Decode1();
		Park(TA.GetOnline(), flag ? true : false);
		return true;
	}
	case CP_PARK_BATTLE_FIELD: // ???
	case CP_EVENT: {
		BYTE flag = cp.Decode1();
		Event(TA.GetOnline(), flag ? true : false);
		return true;
	}
	case CP_TIME_GET_TIME: {
		cp.Decode1(); // 0
		cp.Decode4(); // time
		return true;
	}
	default:
	{
		break;
	}
	}

	return false;
}