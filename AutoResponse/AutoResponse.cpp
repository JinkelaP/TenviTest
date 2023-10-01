#include"AutoResponse.h"

#pragma pack(push, 1)
// TENVI v127
typedef struct {
	DWORD unk1; // 0x00
	BYTE *packet;
	DWORD unk3; // 0x100
	DWORD unk4; // 0x100
	DWORD unk5; // 0x0
	DWORD encoded;
} OutPacket;

typedef struct {
	DWORD unk1; // 0
	DWORD unk2; // 4
	BYTE *packet; // +0x08
	DWORD unk4; // C
	DWORD unk5; // 10
	DWORD unk6; // 14
	WORD length; // + 0x18 ???
	WORD unk8; // 1A
	DWORD unk9; // 1C
	DWORD decoded; // +0x20
	DWORD unk10;
} InPacket;
#pragma pack(pop)

// ignore packet encryption
void OnPacketDirectExec(InPacket *p) {
	void *OnPacketClass = (void *)(*(DWORD *)(*(DWORD *)0x006DB164 + 0x160));
	void (__thiscall *_OnPacket)(void *ecx, InPacket *p) = (decltype(_OnPacket)(*(DWORD *)(*(DWORD *)OnPacketClass + 0x2C)));
	_OnPacket(OnPacketClass, p);
}

void ProcessPacketExec(std::vector<BYTE> &packet) {
	std::vector<BYTE> buffer;
	// first 4 bytes
	buffer.push_back(0);
	buffer.push_back(0);
	buffer.push_back(0);
	buffer.push_back(0);
	// packet
	buffer.insert(buffer.end(), packet.begin(), packet.end());

	InPacket ip = {};
	ip.unk2 = 2; // always 2
	ip.unk4 = 1; // always 1
	ip.decoded = 4; // ignore first 4 bytes
	ip.packet = &buffer[0]; // real buffer
	ip.length = buffer.size(); // real buffer size

	return OnPacketDirectExec(&ip);
}

// Login Button Click
DWORD (__thiscall *_LoginButton)(void *ecx) = NULL;
DWORD __fastcall LoginButton_Hook(void *ecx) {
	// Login
	{
		std::vector<BYTE> ip;
		ip.push_back(0x08); // header
		ip.push_back(0x02);
		ip.push_back(0x31);
		ip.push_back(0x00);
		ip.push_back(0x32);
		ip.push_back(0x00);

		ProcessPacketExec(ip);
	}
	// Character Select
	{
		std::vector<BYTE> ip;
		ip.push_back(0x04); // header
		ip.push_back(0x00);
		ip.push_back(0xFF);
		ip.push_back(0xFF);
		ip.push_back(0xFF);
		ip.push_back(0xFF);
		ip.push_back(0x00);

		ProcessPacketExec(ip);
	}
	// Character Data
	{
		std::vector<BYTE> ip;
		ip.push_back(0x05);
		ip.push_back(0x01);
		ip.push_back(0x01);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x04);
		ip.push_back(0x20);
		ip.push_back(0x04);
		ip.push_back(0xEA);
		ip.push_back(0x30);
		ip.push_back(0xEC);
		ip.push_back(0x30);
		ip.push_back(0xDF);
		ip.push_back(0x30);
		ip.push_back(0xC8);
		ip.push_back(0x30);
		ip.push_back(0x04);
		ip.push_back(0xEA);
		ip.push_back(0x30);
		ip.push_back(0xEC);
		ip.push_back(0x30);
		ip.push_back(0xDF);
		ip.push_back(0x30);
		ip.push_back(0xC8);
		ip.push_back(0x30);
		ip.push_back(0x06);
		ip.push_back(0x00);
		ip.push_back(0x03);
		ip.push_back(0x00);
		ip.push_back(0x11);
		ip.push_back(0x00);
		ip.push_back(0x19);
		ip.push_back(0x00);
		ip.push_back(0xDF);
		ip.push_back(0x01);
		ip.push_back(0x9D);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x03);

		ProcessPacketExec(ip);
	}
	return 0;
}

DWORD (__thiscall *_CharacterSelectButton)(void *, DWORD, DWORD) = NULL;
DWORD __fastcall CharacterSelectButton_Hook(void *ecx, void *edx, DWORD id, DWORD UI) {
	DWORD ret = _CharacterSelectButton(ecx, id, UI);

	if (!(id == 257 && UI == *(DWORD *)((DWORD)ecx + 0x1BC))) {
		return ret;
	}

	// Game Start
	{
		std::vector<BYTE> ip;
		ip.push_back(0x0C); // header
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);

		ProcessPacketExec(ip);
	}

	// In Game test
	{
		std::vector<BYTE> ip;
		ip.push_back(0x0E); // header
		ip.push_back(0x00);

		ProcessPacketExec(ip);
	}

	// Enter Map test
	{
		std::vector<BYTE> ip;
		ip.push_back(0x10); // header
		ip.push_back(0x00);
		ip.push_back(0xD1);
		ip.push_back(0x07);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);
		ip.push_back(0x00);

		ProcessPacketExec(ip);
	}
	return ret;
}

// ([0-9A-F]{2})
// ip.push_back(0x$1);\n
bool AutoResponseHook() {
	SHookFunction(LoginButton, 0x0052E43B);
	SHookFunction(CharacterSelectButton, 0x00531430);
	return true;
}