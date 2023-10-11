#include"FakeServer.h"
#include"AutoResponse.h"

#define TENVI_VERSION 0x1023

// 0x01
void VersionPacket() {
	ServerPacket sp(SP_VERSION);
	sp.Encode4(TENVI_VERSION); // version
	sp.Encode4(1);
	sp.Encode4(1);
	sp.Encode4(1);
	SendPacket(sp);
}

// 0x02
void CrashPacket() {
	ServerPacket sp(SP_CRASH);
	sp.Encode4(TENVI_VERSION); // version
	SendPacket(sp);
}

// 0x09
void WorldSelect() {
	// @09 01 00 40 00 00 01 01 04 74 00 65 00 73 00 74 00 00 00 00 00 00 00 00 00 01 04 41 00 41 00 41 00 41 00 00 00 01 00 00 01 00 00 00
	/*
	Header(09); // 00493B07
	EncodeWStr2(L"@"); // 004938BC
	Encode1(#0); // 004938C8
	Encode1(#1); // 00493934
	Encode1(#0); // 00493979, World Mark
	EncodeWStr(L"Spica"); // 0049398A World Name
	Encode8(0000000000000000); // 004939AD
	Encode1(#1); // 004939BA
	EncodeWStr(L"Spica - 1"); // 004939D2 Channel Name?
	Encode2(#0); // 004939F5
	Encode1(#1); // 00493A0D
	Encode1(#0); // 00493A1E
	Encode1(#0); // 00493A28
	Encode1(#1); // 00493A6A
	Encode1(#0); // 00493A7B
	Encode1(#0); // 00493A88
	Encode1(#0); // 00493A92
	*/
}