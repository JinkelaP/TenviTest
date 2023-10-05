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
		p.Encode2(2002); // mapid
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
		p.Encode2(2002); // mapid
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
	// Spawn Character test
	{
		ServerPacket sp(SP_CHARACTER_SPAWN);
		sp.Encode4(0); // 0048DB9B
		sp.Encode4(0); // 0048DBA5
		sp.Encode4(0); // 0048DBAF
		sp.Encode1(0); // 0048DBB9
		sp.Encode1(0); // 0048DBC6
		sp.Encode1(1); // 0048DBD3, death
		sp.Encode1(0); // 0048DBE0
		sp.Encode4(1); // 0048DBFB
		sp.Encode1(36); // 0048DC08
		sp.Encode1(10); // 0048DC2B
		sp.EncodeWStr(L"ƒVƒ‹ƒ”ƒ@"); // name
		sp.EncodeWStr(L"GUARDIAN"); // guardian name
		sp.Encode1(0); // 0048DC8F
		sp.Encode1(1); // 0048DC9C
		sp.Encode2(6); // 0048DCA9
		sp.Encode2(3); // 0048DCB7
		sp.Encode2(19); // 0048DCC5
		sp.Encode2(24); // 0048DCD3
		sp.Encode2(478); // 0048DCE1
		sp.Encode2(156); // 0048DCEF
		sp.Encode1(0); // 0048DCFD

		/*
		for (int i = 0; i < 30; i++) {
			sp.Encode8(0);
			sp.Encode2(0);
		}
		*/
		for (int i = 0; i < 15; i++) {
			sp.Encode8(0);
			sp.Encode2(0);
		}
		for (int i = 0; i < 15; i++) {
			sp.Encode8(0);
			sp.Encode2(0);
		}

		sp.Encode2(0); // 0048DDC3
		sp.Encode2(0); // 0048DDD1
		sp.Encode2(0); // 0048DDE1
		sp.Encode2(0); // 0048DDF1
		sp.Encode2(0); // 0048DE01
		sp.Encode1(0); // 0048DE11
		sp.Encode1(0); // 0048DE21
		sp.Encode1(0); // 0048DE35
		sp.EncodeWStr(L"TEST1"); // guild
		sp.Encode1(0); // 0057B508
		sp.Encode1(0); // 0057B515
		sp.Encode1(0); // 0057B522
		sp.Encode1(0); // 0057B52F
		sp.Encode1(0); // 0057B53C
		sp.Encode1(0); // 0057B549
		sp.Encode1(0); // 0048DE7F
		sp.Encode1(0); // 0048DE8C
		sp.Encode1(0); // 0048DE9F
		sp.Encode1(0); // 0048DEAC
		sp.Encode1(0); // 0048DEB9
		sp.Encode1(0); // 0048E4F3
		sp.Encode4(0); // 0048E513
		sp.Encode1(0); // 0048E51D
		sp.Encode1(0); // 0048E5F9
		sp.Encode1(0); // 0048E60D
		sp.Encode1(0); // 0048E621
		sp.Encode4(0); // 0048E6A1
		sp.Encode2(0); // 0048E6AB
		sp.Encode4(0); // 0048E6C7
		sp.Encode4(0); // 0048E6D1
		sp.Encode4(0); // 0048E6DB
		sp.Encode4(0); // 0048E6E5
		sp.Encode4(0); // 0048E6EF
		sp.Encode1(0); // 0048E6F9
		sp.Encode1(0); // 0048E706
		sp.Encode4(0); // 0048E713
		sp.Encode4(0); // 0048E71D
		sp.Encode4(0); // 0048E727
		sp.Encode4(0); // 0048E731
		sp.EncodeWStr(L"TEST2"); // 0048E73F
		sp.Encode2(0); // 0048E74A
		sp.Encode2(0); // 0048E757
		sp.EncodeWStr(L"TEST3"); // 0048E768
		sp.Encode1(0); // 0048E773
		sp.Encode1(0); // 0048E780
		SendPacket(sp);
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