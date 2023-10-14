#ifndef __AUTORESPONSE_H__
#define __AUTORESPONSE_H__

#include"../Share/Simple/Simple.h"
#include"../Share/Hook/SimpleHook.h"
#include"TenviPacket.h"
#include"ClientPacket.h"
#include"ServerPacket.h"
#include"FakeServer.h"

#define REGION_JP 0
#define REGION_CN 1
#define REGION_HK 2
#define REGION_KR 3

#define REGION REGION_JP

bool AutoResponseHook();
void SendPacket(ServerPacket &sp);

#endif
