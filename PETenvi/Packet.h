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
	ServerPacket();

	std::vector<BYTE>& get();
	void Encode1(BYTE val);
	void Encode2(WORD val);
	void Encode4(DWORD val);
	void EncodeWStr1(std::wstring val);
	void EncodeWStr2(std::wstring val);
	void Encode8(ULONGLONG val);
	void EncodeFloat(float val);
};

#endif