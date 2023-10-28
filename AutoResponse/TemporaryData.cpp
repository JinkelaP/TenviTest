#include"TemporaryData.h"

DWORD TenviCharacter::id_counter = 1337;

// new character
TenviCharacter::TenviCharacter(std::wstring nName, BYTE nJob_Mask, WORD nJob, WORD nSkin, WORD nHair, WORD nFace, WORD nCloth, WORD nGColor, std::vector<WORD> &nGEquipped) {
	id = id_counter++;
	name = nName;
	job_mask = nJob_Mask; // gender and job
	job = nJob;
	skin = nSkin;
	hair = nHair;
	face = nFace;
	cloth = nCloth;
	gcolor = nGColor;
	equipped.resize(15);
	gequipped = nGEquipped;
	gequipped.resize(15);
	map = 2002;
	level = 30;
	sp = 500;
	ap = 300;
	// test stat
	stat_str = 16;
	stat_dex = 18;
	stat_hp = 199;
	stat_int = 712;
	stat_mp = 158;
}


void TenviCharacter::TestSilva() {
	gcolor = 187;
	hair = 137;
	//map = 8003;
	map = 8037;
	equipped.clear();
	equipped.push_back(228); // hat
	equipped.push_back(105); // overall
	equipped.push_back(22508); // weapon
	//equipped.push_back(28715); // weapon ava?
	equipped.resize(15);
}

// game related
bool TenviCharacter::UseSP(WORD skill_id) {
	if (0 < sp) {
		sp--;
		for (auto &v : skill) {
			if (v.id == skill_id) {
				v.level++;
				return true;
			}
		}

		TenviSkill learn_skill;
		learn_skill.id = skill_id;
		learn_skill.level = 1;
		skill.push_back(learn_skill);
		return true;
	}
	return false;
}

bool TenviCharacter::UseAP(BYTE stat_id) {
	if (0 < ap) {
		ap--;
		switch (stat_id) {
		case TS_STR: {
			stat_str++;
			return true;
		}
		case TS_DEX: {
			stat_dex++;
			return true;
		}
		case TS_HP: {
			stat_hp++;
			return true;
		}
		case TS_INT: {
			stat_int++;
			return true;
		}
		case TS_MP: {
			stat_mp++;
			return true;
		}
		default:
		{
				break;
		}
		}
	}
	return false;
}

// init
TenviAccount::TenviAccount() {
	// account info
	slot = 6;

	// default characters
	std::vector<WORD> silva_equip;
	silva_equip.push_back(20002);
	silva_equip.push_back(20502);
	silva_equip.push_back(22508);
	TenviCharacter silva(L"Silva", (1 << 4) | 4, 6, 3, 19, 24, 479, 157, silva_equip);
	silva.TestSilva(); // test

	std::vector<WORD> talli_equip;
	talli_equip.push_back(20001);
	talli_equip.push_back(20811);
	talli_equip.push_back(22507);
	TenviCharacter talli(L"Talli", (1 << 4) | 2, 5, 2, 18, 25, 476, 155, talli_equip);


	std::vector<WORD> andras_equip;
	andras_equip.push_back(20310);
	andras_equip.push_back(20500);
	andras_equip.push_back(22500);
	TenviCharacter andras(L"Andras", (1 << 4) | 1, 4, 1, 17, 23, 473, 8, andras_equip);

	characters.push_back(silva);
	characters.push_back(talli);
	characters.push_back(andras);
}


bool TenviAccount::AddCharacter(std::wstring nName, BYTE nJob_Mask, WORD nJob, WORD nSkin, WORD nHair, WORD nFace, WORD nCloth, WORD nGColor, std::vector<WORD> &nGEquipped) {
	if (slot <= GetCharacters().size()) {
		return false;
	}

	TenviCharacter character(nName, nJob_Mask, nJob, nSkin, nHair, nFace, nCloth, nGColor, nGEquipped);
	characters.push_back(character);
	return true;
}

bool TenviAccount::FindCharacter(DWORD id, TenviCharacter *found) {
	for (auto &character : characters) {
		if (character.id == id) {
			*found = character;
			return true;
		}
	}

	found = NULL;
	return false;
}

std::vector<TenviCharacter>& TenviAccount::GetCharacters() {
	return characters;
}

bool TenviAccount::Login(DWORD id) {
	online_id = id;
	return true;
}

TenviCharacter& TenviAccount::GetOnline() {
	for (auto &character : characters) {
		if (character.id == online_id) {
			return character;
		}
	}

	return characters[0];
}