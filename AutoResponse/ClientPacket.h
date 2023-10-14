#ifndef __CLIENTPACKET_H__
#define __CLIENTPACKET_H__

// Client to Server (Send, Out)

#include<Windows.h>
#include<string>
#include<vector>

class ClientPacket {
private:
	std::vector<BYTE> packet;
	DWORD decoded;

public:
	ClientPacket(BYTE *spacket, DWORD size);

	BYTE Decode1();
	WORD Decode2();
	DWORD Decode4();
	std::wstring DecodeWStr1();
	std::wstring DecodeWStr2();
	ULONGLONG Decode8();
	float DecodeFloat();
};

#endif