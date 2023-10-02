#ifndef __PACKET_H__
#define __PACKET_H__

#include<Windows.h>
#include<string>
#include<vector>

class ServerPacket {
private:
	std::vector<BYTE> packet;

public:
	ServerPacket(BYTE header);

	std::vector<BYTE>& get();
	void Encode1(BYTE val);
	void Encode2(WORD val);
	void Encode4(DWORD val);
	void EncodeWStr(std::wstring val);
};

#endif