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
	gequipped = nGEquipped;
	gequipped.resize(15);
	map = 2002;
	level = 30;
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