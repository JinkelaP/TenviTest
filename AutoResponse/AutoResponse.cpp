#include"AutoResponse.h"

DWORD Addr_OnPacketClass = 0;

// ignore packet encryption
void OnPacketDirectExec(InPacket *p) {
	void *OnPacketClass = (void *)(*(DWORD *)(*(DWORD *)Addr_OnPacketClass + 0x160));
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
	WorldListPacket();
	return 0;
}

DWORD (__thiscall *_LoginButton_KR)(void *ecx, void *, void *, void *) = NULL;
DWORD __fastcall LoginButton_KR_Hook(void *ecx, void *, void *, void *) {
	WorldListPacket();
	return 0;
}


void (__thiscall *_WorldSelectButton)(void *) = NULL;
void __fastcall WorldSelectButton_Hook(void *ecx) {
	_WorldSelectButton(ecx);
	CharacterSelectPacket(); // go to character select
	//CharacterListPacket(); // character list
	CharacterListPacket_Test();
}

bool (__thiscall *_ConnectCaller)(void *) = NULL;
bool __fastcall ConnectCaller_Hook(void *ecx, void *edx, void *v1, void *v2, void *v3) {
	DEBUG(L"Connect is called!");
	// ignore connect checks
	return true;
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

bool AutoResponseHook() {
	Rosemary r;

	switch (GetRegion()) {
	case TENVI_JP: {
		Addr_OnPacketClass = 0x006DB164;
		// press button to go world select
		SHookFunction(LoginButton, 0x0052E43B);
		// world select to go character select
		SHookFunction(WorldSelectButton, 0x0052F038);
		// read send packet buffer without using server
		SHookFunction(EnterSendPacket, 0x0055F2A8);
		// ignore connect checks for world select and character select
		SHookFunction(ConnectCaller, 0x0055EFE2);

		// patch
		r.Patch(0x0042D3DC + 2, L"18"); // portal id to map id
		return true;
	}
	case TENVI_CN: {
		Addr_OnPacketClass = 0x006FAF44;
		SHookFunction(LoginButton, 0x00532FEF);
		SHookFunction(WorldSelectButton, 0x00533D74);
		SHookFunction(EnterSendPacket, 0x0056AADB);
		SHookFunction(ConnectCaller, 0x0056A4FD);
		return true;
	}
	case TENVI_HK: {
		Addr_OnPacketClass = 0x0075CF84;
		SHookFunction(LoginButton, 0x0052CFC2);
		SHookFunction(WorldSelectButton, 0x0052DC5A);
		SHookFunction(EnterSendPacket, 0x005AC927);
		SHookFunction(ConnectCaller, 0x005832FE);
		return true;
	}
	case TENVI_KR: {
		Addr_OnPacketClass = 0x0075E184;
		SHookFunction(LoginButton_KR, 0x004767A3);
		SHookFunction(WorldSelectButton, 0x00540E22);
		SHookFunction(EnterSendPacket, 0x005CBA0F);
		SHookFunction(ConnectCaller, 0x0059DED0);
		return true;
	}
	default: {
		break;
	}
	}

	return false;
}