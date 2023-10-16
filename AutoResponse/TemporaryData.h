#ifndef __TEMPORARYDATA_H__
#define __TEMPORARYDATA_H__

#include<Windows.h>
#include<string>
#include<vector>

class TenviCharacter {
private:
	static DWORD id_counter;

public:
	DWORD id;
	std::wstring name;
	BYTE job_mask;
	WORD job;
	WORD skin;
	WORD hair;
	WORD face;
	WORD cloth;
	WORD gcolor; // guadian
	WORD map;
	BYTE level;
	std::vector<WORD> equipped;
	std::vector<WORD> gequipped;

	TenviCharacter(std::wstring nName, BYTE nJob_Mask, WORD nJob, WORD nSkin, WORD nHair, WORD nFace, WORD nCloth, WORD nGColor, std::vector<WORD> &nGEquipped);

	void TestSilva();
};

class TenviAccount {
private:
	std::vector<TenviCharacter> characters;
	DWORD online_id;

public:
	BYTE slot;

	TenviAccount();
	bool FindCharacter(DWORD id, TenviCharacter *found);
	std::vector<TenviCharacter>& GetCharacters();
	bool AddCharacter(std::wstring nName, BYTE nJob_Mask, WORD nJob, WORD nSkin, WORD nHair, WORD nFace, WORD nCloth, WORD nGColor, std::vector<WORD> &nGEquipped);
	bool Login(DWORD id);
	TenviCharacter& GetOnline();
};

#endif