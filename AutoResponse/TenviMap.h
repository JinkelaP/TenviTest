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


class TenviMap {
private:
	DWORD id; // mapid
	std::vector<TenviSpawnPoint> data_spawn_point;
	std::vector<TenviPortal> data_portal;

	bool LoadXML();

public:
	TenviMap(DWORD mapid);
	DWORD GetID();
	void AddSpawnPoint(TenviSpawnPoint &spawn_point);
	void AddPortal(TenviPortal &portal);
	TenviSpawnPoint FindSpawnPoint(DWORD id = 0);
	TenviPortal FindPortal(DWORD id);
};

#endif