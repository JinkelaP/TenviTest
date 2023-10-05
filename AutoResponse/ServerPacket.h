#ifndef __SERVERPACKET_H__
#define __SERVERPACKET_H__

// Server to Client (Recv, In)

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
	void Encode8(ULONGLONG val);
	void EncodeFloat(float val);
};

#endif