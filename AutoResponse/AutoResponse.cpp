#include"AutoResponse.h"

#if REGION == REGION_JP
#define Addr_EnterSendPacket 0x0055F2A8
#define Addr_OnPacketClass 0x006DB164
#define Addr_LoginButton 0x0052E43B
#define Addr_WorldSelectButton 0x0052F038
//#define Addr_CharacterSelectButton 0x00531430
#define Addr_CharacterLoginButonOffset 0x1BC
#define Addr_LoginConnect_Caller 0x0055EFE2
#elif REGION == REGION_CN
#define Addr_EnterSendPacket 0x0056AADB
#define Addr_OnPacketClass 0x006FAF44
#define Addr_LoginButton 0x00532FEF
//#define Addr_CharacterSelectButton 0x00535F02
#define Addr_CharacterLoginButonOffset 0x1BC
#define Addr_LoginConnect_Caller 0x0056A4FD
#define Addr_WorldSelectButton 0x00533D74
#elif REGION == REGION_HK
#define Addr_EnterSendPacket 0x005AC927
#define Addr_OnPacketClass 0x0075CF84
#define Addr_LoginButton 0x0052CFC2
//#define Addr_CharacterSelectButton 0x0053009D
#define Addr_CharacterLoginButonOffset 0x1B8
#define Addr_LoginConnect_Caller 0x005832FE
#define Addr_WorldSelectButton 0x0052DC5A
#elif REGION == REGION_KR
#define Addr_EnterSendPacket 0x005CBA0F
#define Addr_OnPacketClass 0x0075E184
#define Addr_LoginButton 0x004767A3 // login error dialog
//#define Addr_CharacterSelectButton 0x0054327B
#define Addr_CharacterLoginButonOffset 0x1B8
#define Addr_LoginConnect_Caller 0x0059DED0
#define Addr_WorldSelectButton 0x00540E22
#endif


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
#if REGION == REGION_KR
DWORD(__thiscall *_LoginButton)(void *ecx, void *, void *, void *) = NULL;
DWORD __fastcall LoginButton_Hook(void *ecx, void *, void *, void *) {
#else
DWORD (__thiscall *_LoginButton)(void *ecx) = NULL;
DWORD __fastcall LoginButton_Hook(void *ecx) {
#endif
	//_LoginButton(ecx);
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
	// press button to go world select
	SHookFunction(LoginButton, Addr_LoginButton);
	// world select to go character select
	SHookFunction(WorldSelectButton, Addr_WorldSelectButton);
	// read send packet buffer without using server
	SHookFunction(EnterSendPacket, Addr_EnterSendPacket);
	// ignore connect checks for world select and character select
	SHookFunction(ConnectCaller, Addr_LoginConnect_Caller);
	return true;
}