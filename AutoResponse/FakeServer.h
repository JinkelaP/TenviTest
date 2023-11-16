#ifndef __FAKESERVER_H__
#define __FAKESERVER_H__

#include"ClientPacket.h"
#include"ServerPacket.h"
#include"TenviData.h"

#define MAPID_ITEM_SHOP 65535
#define MAPID_PARK 65534
#define MAPID_EVENT 62501

bool FakeServer(ClientPacket &cp);
// test
void WorldListPacket();
void CharacterSelectPacket();
void CharacterListPacket();
void CharacterListPacket_Test();

#endif