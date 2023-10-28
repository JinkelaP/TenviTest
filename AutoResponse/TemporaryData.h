#ifndef __TEMPORARYDATA_H__
#define __TEMPORARYDATA_H__

#include<Windows.h>
#include<string>
#include<vector>

typedef struct{
	WORD id;
	BYTE level;
} TenviSkill;

enum TenviStat {
	TS_STR,
	TS_DEX,
	TS_HP,
	TS_INT,
	TS_MP,
};

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
	WORD sp; // skill point
	WORD ap;
	// stat
	WORD stat_str; // óÕ
	WORD stat_dex; // ïqè∑
	WORD stat_hp; // ëÃóÕ
	WORD stat_int; // ímî\
	WORD stat_mp; //ímåb

	std::vector<WORD> equipped;
	std::vector<WORD> gequipped;
	std::vector<TenviSkill> skill;

	TenviCharacter(std::wstring nName, BYTE nJob_Mask, WORD nJob, WORD nSkin, WORD nHair, WORD nFace, WORD nCloth, WORD nGColor, std::vector<WORD> &nGEquipped);

	void TestSilva();
	bool UseSP(WORD skill_id);
	bool UseAP(BYTE stat_id);
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