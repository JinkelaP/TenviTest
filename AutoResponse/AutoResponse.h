#ifndef __AUTORESPONSE_H__
#define __AUTORESPONSE_H__

#include"../Share/Simple/Simple.h"
#include"../Share/Hook/SimpleHook.h"
#include"TenviPacket.h"
#include"ClientPacket.h"
#include"ServerPacket.h"
#include"FakeServer.h"

enum Region {
	TENVI_JP, // JP v127
	TENVI_CN, // CN v126
	TENVI_HK, // HK v102
	TENVI_KR, // KR v107 (Xtream)
};

Region GetRegion();

bool AutoResponseHook();
void SendPacket(ServerPacket &sp);

#endif
