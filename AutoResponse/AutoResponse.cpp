#include"AutoResponse.h"

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
	ip.length = (WORD)buffer.size(); // real buffer size

	return OnPacketDirectExec(&ip);
}

void SendPacket(ServerPacket &sp) {
	return ProcessPacketExec(sp.get());
}

// Login Button Click
DWORD (__thiscall *_LoginButton)(void *ecx) = NULL;
DWORD __fastcall LoginButton_Hook(void *ecx) {
	// go to World Select
	{
		ServerPacket p(0x08);
		p.EncodeWStr(L"HelloWorld");
		SendPacket(p);
	}
	// go to Character Select
	{
		ServerPacket p(0x04);
		p.Encode1(0);
		p.Encode4(-1);
		p.Encode1(0);
		SendPacket(p);
	}
	// update Character Data
	{
		ServerPacket p(0x05);
		p.Encode1(1); // characters
		p.Encode4(1); // ID
		p.Encode1(4);
		p.Encode1(30); // level
		p.EncodeWStr(L"Riremito"); // name
		p.EncodeWStr(L"str");
		p.Encode2(6);
		p.Encode2(3);
		p.Encode2(17);
		p.Encode2(25);
		p.Encode2(479);
		p.Encode2(157);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(0);
		p.Encode2(2001); // mapid
		p.Encode1(3); // character slots
		SendPacket(p);
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
		ServerPacket p(0x0C);
		p.Encode1(0);
		p.Encode4(0);
		p.Encode2(0);
		p.Encode4(0);
		p.Encode4(0);
		SendPacket(p);
	}

	// In Game test
	{
		ServerPacket p(0x0E);
		p.Encode1(0);
		SendPacket(p);
	}

	// Enter Map test
	{
		ServerPacket p(0x10);
		p.Encode1(0);
		p.Encode2(2001); // mapid
		p.Encode1(0);
		p.Encode4(0);
		p.Encode4(0); // float value
		p.Encode4(0); // float value
		p.Encode1(0);
		p.Encode1(0);
		p.Encode1(0);
		p.Encode1(0);
		p.Encode4(0);
		SendPacket(p);
	}
	return ret;
}

void(__thiscall *_EnterSendPacket)(OutPacket *) = NULL;
void __fastcall EnterSendPacket_Hook(OutPacket *op) {
	// save packet data
	ClientPacket cp(op->packet, op->encoded);
	// execute original send packet
	_EnterSendPacket(op);
	// fake server request
	FakeServer(cp);
}

// ([0-9A-F]{2})
// ip.push_back(0x$1);\n
bool AutoResponseHook() {
	SHookFunction(LoginButton, 0x0052E43B);
	SHookFunction(CharacterSelectButton, 0x00531430);
	SHookFunction(EnterSendPacket, 0x0055F2A8);
	return true;
}