#include"FakeServer.h"
#include"AutoResponse.h"
#include"TemporaryData.h"

TenviAccount TA;
// ========== TENVI Packet Response ==========
#define TENVI_VERSION 0x1023

// 0x01
void VersionPacket() {
	ServerPacket sp(SP_VERSION);
	sp.Encode4(TENVI_VERSION); // version
	sp.Encode4(1);
	sp.Encode4(1);
	sp.Encode4(1);
	SendPacket(sp);
}

// 0x02
void CrashPacket() {
	ServerPacket sp(SP_CRASH);
	sp.Encode4(TENVI_VERSION); // version
	SendPacket(sp);
}

// 0x04
void CharacterSelectPacket() {
	ServerPacket sp(SP_CHARACTER_SELECT);
	sp.Encode1(0);
	sp.Encode4(-1);
	sp.Encode1(0);
	SendPacket(sp);
}

// 0x05
int character_slot = 6;
void CharacterListPacket() {
	ServerPacket sp(SP_CHARACTER_LIST);
	sp.Encode1(0); // Number of Characters
	sp.Encode1(character_slot); // Number of Character Slot
	SendPacket(sp);
}

void CharacterListPacket_Test() {
	ServerPacket sp(SP_CHARACTER_LIST);

	sp.Encode1(TA.GetCharacters().size()); // characters
	for (auto &chr : TA.GetCharacters()) {
		sp.Encode4(chr.id); // ID
		sp.Encode1(chr.job_mask);
		sp.Encode1(chr.level); // level
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

	sp.Encode1(TA.slot); // character slots
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

#if REGION == REGION_JP || REGION == REGION_CN
	sp.Encode1(0); // 004938C8, NetCafe
#endif

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
#if REGION == REGION_JP || REGION == REGION_CN
	sp.Encode1(0); // 00493A88
#endif
	sp.Encode1(0); // 00493A92

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

// 0x10
void ChangeMapPacket(WORD mapid) {
	ServerPacket sp(SP_MAP_CHANGE);
	sp.Encode1(0);
	sp.Encode2(mapid); // mapid
	sp.Encode1(0);
	sp.Encode4(0);
	sp.Encode4(0); // float value
	sp.Encode4(0); // float value
	sp.Encode1(0);
	sp.Encode1(0);
	sp.Encode1(0);
	sp.Encode1(0);
	sp.Encode4(0);
	SendPacket(sp);
}

// 0x11
void CharacterSpawn(TenviCharacter &chr) {
	ServerPacket sp(SP_CHARACTER_SPAWN);
	sp.Encode4(chr.id); // 0048DB9B id, where checks id?
	sp.Encode4(0); // 0048DBA5 x
	sp.Encode4(0); // 0048DBAF y
	sp.Encode1(0); // 0048DBB9 direction
	sp.Encode1(1); // 0048DBC6 guardian
	sp.Encode1(1); // 0048DBD3 death
	sp.Encode1(1); // 0048DBE0 guardian gasping
	sp.Encode4(1); // 0048DBFB
	sp.Encode1(chr.job_mask); // 0048DC08
	sp.Encode1(chr.level); // 0048DC2B
#if REGION == REGION_HK || REGION == REGION_KR
	sp.Encode1(1);
#endif
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

	for (int i = 0; i < 15; i++) {
		sp.Encode8(0);
		sp.Encode2(0);
	}

	// fixed size (15)
	for (auto gequip : chr.gequipped) {
		sp.Encode8(0);
		sp.Encode2(gequip);
	}

	sp.Encode2(0); // 0048DDC3
	sp.Encode2(0); // 0048DDD1
	sp.Encode2(0); // 0048DDE1
	sp.Encode2(0); // 0048DDF1
	sp.Encode2(0); // 0048DE01
#if REGION == REGION_JP || REGION == REGION_CN
	sp.Encode1(0); // 0048DE11
	sp.Encode1(0); // 0048DE21
	sp.Encode1(0); // 0048DE35
#endif
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
#if REGION == REGION_HK || REGION == REGION_KR
	sp.Encode1(0);
	sp.Encode1(0);
#endif
	SendPacket(sp);
}

// 0x3D
void AccountDataPacket(TenviCharacter &chr) {
	ServerPacket sp(SP_ACCOUNT_DATA);
	sp.Encode4(0); // 00498E4F
	sp.Encode4(chr.id); // 00498E5C
	sp.EncodeWStr1(chr.name); // 00498E79
	sp.EncodeWStr1(L""); // 00498EA5
	sp.Encode1(chr.job_mask); // 00498ECD
	sp.Encode1(chr.level); // 00498EF0
	sp.Encode8(1234); // 00498F0C
	sp.Encode8(4567); // 00498F28
	sp.Encode8(7890); // 00498F44
	sp.Encode1(0); // 00498F60
	sp.Encode1(0); // 00498F70
	sp.Encode1(0); // 00498F80
	sp.EncodeWStr1(L""); // 0057A877
	sp.Encode1(0); // 0057B508
	sp.Encode1(0); // 0057B515
	sp.Encode1(0); // 0057B522
	sp.Encode1(0); // 0057B52F
	sp.Encode1(0); // 0057B53C
	sp.Encode1(0); // 0057B549
	sp.EncodeWStr1(L""); // 00498FA0
	sp.Encode1(0); // 00498FC8
	sp.Encode1(0); // 00498FD8
	sp.Encode1(0); // 00498FF0
	sp.Encode1(0); // 00499024
	sp.Encode1(0); // 00499024
	sp.Encode1(0); // 00499024
	sp.Encode1(0); // 00499024
	sp.EncodeWStr1(L""); // 00499044
	sp.Encode1(0); // 0049906C
	sp.Encode1(0); // 004990B3
	sp.Encode1(0); // 00499101
	sp.Encode4(0); // 00499117
	sp.Encode1(0); // 00499124
	sp.Encode1(0); // 00499134
	sp.Encode4(0); // 00499144
	sp.EncodeWStr1(L""); // 00499155
	sp.Encode1(0); // 0049917D
	sp.Encode1(0); // 0049918D
	sp.Encode1(0); // 0049919D


	for (int i = 0; i < 100; i++) {
		sp.Encode8(0);
	}
	SendPacket(sp);
}

// ========== TENVI Server Main ==========
bool FakeServer(ClientPacket &cp) {
	CLIENTPACKET header = (CLIENTPACKET)cp.Decode1();

	switch (header) {
	// Select Character
	case CP_GAME_START: {
		DWORD character_id = cp.Decode4();
		BYTE channel = cp.Decode1();

		TA.Login(character_id);

		for (auto &chr : TA.GetCharacters()) {
			if (chr.id == character_id) {
				GetGameServerPacket(); // notify game server ip
				ConnectedPacket(); // connected
				AccountDataPacket(chr);
				ChangeMapPacket(chr.map); // map change
				CharacterSpawn(chr); // character spawn
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
	default:
	{
		break;
	}
	}

	return false;
}