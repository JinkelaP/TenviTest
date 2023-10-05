#include"FakeServer.h"
#include"AutoResponse.h"

bool FakeServer(ClientPacket &cp) {
	CLIENTPACKET header = (CLIENTPACKET)cp.Decode1();

	switch (header) {
	// Login
	case CP_CREATE_CHARACTER: {
		std::wstring character_name = cp.DecodeWStr();
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
		guardian_equip.resize(15);

		{
			ServerPacket sp(SP_CHARACTER_LIST);
			sp.Encode1(1); // characters
			sp.Encode4(1234); // ID
			sp.Encode1(job_mask); // job + gender
			sp.Encode1(10); // level
			sp.EncodeWStr(character_name); // name
			sp.EncodeWStr(L"str");
			sp.Encode2(job_id);
			sp.Encode2(character_skin);
			sp.Encode2(character_hair);
			sp.Encode2(character_face);
			sp.Encode2(character_cloth);
			sp.Encode2(guardian_color);

			// character equip, max 15
			for (int i = 0; i < 15; i++) sp.Encode2(0);
			// guardian equip, max 15
			for(auto g_equip: guardian_equip){
				sp.Encode2(g_equip);
			}

			sp.Encode2(2002); // mapid
			sp.Encode1(3); // character slots
			SendPacket(sp);
		}
		return true;
	}
	case CP_GOTO_LOGIN: {
		ServerPacket sp(SP_GOTO_LOGIN);
		SendPacket(sp);
		return true;
	}
	// Game
	case CP_LOGOUT: {
		{
			ServerPacket sp(SP_GOTO_LOGIN_FROM_GAME);
			sp.Encode1(0); // error code
			sp.Encode4(0);
			sp.Encode4(0);
			sp.Encode4(0x0100007F); // IP
			sp.Encode2(8787); // port
			SendPacket(sp);
		}
		{
			ServerPacket sp(SP_CONNECTED);
			sp.Encode1(0); // error code
			SendPacket(sp);
		}
		return true;
	}
	default:
	{
		break;
	}
	}

	return false;
}