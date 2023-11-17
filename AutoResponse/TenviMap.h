#ifndef __TEMVI_MAP_H__
#define __TEMVI_MAP_H__

#include<Windows.h>
#include<string>
#include<vector>

typedef struct {
	DWORD id;
	DWORD next_id;
	DWORD next_mapid;
	float x;
	float y;
} TenviPortal;

typedef struct {
	DWORD id; // no id?
	float x;
	float y;
} TenviSpawnPoint;

typedef struct {
	DWORD id;
	//DWORD object_id;
} TenviObject;

typedef struct {
	float left;
	float top;
	float right;
	float bottom;
} TenviArea;

typedef struct {
	DWORD id;
	DWORD delay;
	DWORD flip;
	DWORD once;
	DWORD population;
	TenviArea area;
	TenviObject object;
} TenviRegen;

class TenviMap {
private:
	DWORD id; // mapid
	std::vector<TenviSpawnPoint> data_spawn_point;
	std::vector<TenviPortal> data_portal;
	std::vector<TenviRegen> data_regen;

	bool LoadXML();
	bool LoadSubXML();

public:
	TenviMap(DWORD mapid);
	DWORD GetID();
	void AddSpawnPoint(TenviSpawnPoint &spawn_point);
	void AddPortal(TenviPortal &portal);
	void AddRegen(TenviRegen &regen);
	std::vector<TenviRegen>& GetRegen();
	TenviSpawnPoint FindSpawnPoint(DWORD id = 0);
	TenviPortal FindPortal(DWORD id);
};

#endif