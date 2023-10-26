#include"PacketTenvi.h"
#include<vector>
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)

PipeClient *pc = NULL;
CRITICAL_SECTION cs;

bool StartPipeClient() {
	pc = new PipeClient(PE_LOGGER_PIPE_NAME);
	return pc->Run();
}

bool RestartPipeClient() {
	if (pc) {
		delete pc;
	}
	return StartPipeClient();
}

// ShiftJIS to UTF16
bool ShiftJIStoUTF8(std::string sjis, std::wstring &utf16) {
	// UTF16�֕ϊ�����ۂ̕K�v�ȃo�C�g�����擾
	int len = MultiByteToWideChar(CP_ACP, 0, sjis.c_str(), -1, 0, 0);
	if (!len) {
		return false;
	}

	// UTF16�֕ϊ�
	std::vector<BYTE> b((len + 1) * sizeof(WORD));
	if (!MultiByteToWideChar(CP_ACP, 0, sjis.c_str(), -1, (WCHAR *)&b[0], len)) {
		return false;
	}

	utf16 = std::wstring((WCHAR *)&b[0]);
	return true;
}

// �o�C�g�z�񂩂�ShiftJIS��������擾
bool BYTEtoShiftJIS(BYTE *text, size_t len, std::string &sjis) {
	std::vector<BYTE> b(len + 1);
	for (size_t i = 0; i < len; i++) {
		b[i] = text[i];
	}
	sjis = std::string((char *)&b[0]);
	return true;
}

void(__thiscall *_SendPacket)(void *ecx, OutPacket *p);
void(__thiscall *_COutPacket)(OutPacket *p, BYTE b, void *v);
void(__thiscall *_Encode1)(OutPacket *p, BYTE b);
void(__thiscall *_Encode2)(OutPacket *p, WORD w);
void(__thiscall *_Encode4)(OutPacket *p, DWORD dw);
void(__thiscall *_EncodeStr)(OutPacket *p, char *s);
void(__thiscall *_EncodeWStr1)(OutPacket *p, WCHAR *wc);
void(__thiscall *_EncodeBuffer)(OutPacket *p, BYTE *b, DWORD len);

void(__thiscall *_ProcessPacket)(void *ecx, InPacket *p);
BYTE(__thiscall *_DecodeHeader)(InPacket *p);
BYTE(__thiscall *_Decode1)(InPacket *p);
WORD(__thiscall *_Decode2)(InPacket *p);
DWORD(__thiscall *_Decode4)(InPacket *p);
char** (__thiscall *_DecodeStr)(InPacket *p, char **s);
WCHAR** (__thiscall *_DecodeWStr1)(InPacket *p, WCHAR **wc);
WCHAR** (__thiscall *_DecodeWStr2)(InPacket *p, WCHAR **wc);
void(__thiscall *_DecodeBuffer)(InPacket *p, BYTE *b, DWORD len);

DWORD packet_id_out = 0;
DWORD packet_id_in = 0;

typedef struct {
	ULONG_PTR id; // �p�P�b�g���ʎq
	ULONG_PTR addr; // ���^�[���A�h���X
	MessageHeader fmt; // �t�H�[�}�b�g�̎��
	ULONG_PTR pos; // �ꏊ
	ULONG_PTR size; // �f�[�^�̒��� (DecodeBuffer�ȊO�s�v)

} PacketExtraInformation;


void AddExtra(PacketExtraInformation &pxi) {
	EnterCriticalSection(&cs);
	union {
		PacketEditorMessage *pem;
		BYTE *b;
	};

	pem = new PacketEditorMessage;

	if (!pem) {
		LeaveCriticalSection(&cs);
		return;
	}

	pem->header = pxi.fmt;
	pem->id = pxi.id;
	pem->addr = pxi.addr;
	pem->Extra.pos = pxi.pos;
	pem->Extra.size = pxi.size;

	if (!pc->Send(b, sizeof(PacketEditorMessage))) {
		RestartPipeClient();
	}

	delete pem;
	LeaveCriticalSection(&cs);
}

void AddSendPacket(OutPacket *p, ULONG_PTR addr, bool &bBlock) {
	EnterCriticalSection(&cs);
	union {
		PacketEditorMessage *pem;
		BYTE *b;
	};

	b = new BYTE[sizeof(PacketEditorMessage) + p->encoded];

	if (!b) {
		LeaveCriticalSection(&cs);
		return;
	}

	pem->header = SENDPACKET;
	pem->id = packet_id_out; // ???
	pem->addr = addr;
	pem->Binary.length = p->encoded;
	memcpy_s(pem->Binary.packet, p->encoded, p->packet, p->encoded);

	if (!pc->Send(b, sizeof(PacketEditorMessage) + p->encoded)) {
		RestartPipeClient();
	}
	else {
		std::wstring wResponse;
		pc->Recv(wResponse);
		if (wResponse.compare(L"Block") == 0) {
			bBlock = true;
		}
		else {
			bBlock = false;
		}
	}

	delete[] b;
	LeaveCriticalSection(&cs);
}

void AddRecvPacket(InPacket *p, ULONG_PTR addr, bool &bBlock) {
	EnterCriticalSection(&cs);
	union {
		PacketEditorMessage *pem;
		BYTE *b;
	};
	b = new BYTE[sizeof(PacketEditorMessage) + p->length - 0x04];
	if (!b) {
		LeaveCriticalSection(&cs);
		return;
	}

	pem->header = RECVPACKET;
	pem->id = packet_id_in;
	pem->addr = addr;
	pem->Binary.length = p->length - 0x04;
	memcpy_s(pem->Binary.packet, p->length - 0x04, &p->packet[4], p->length - 0x04);
	if (!pc->Send(b, sizeof(PacketEditorMessage) + p->length - 0x04)) {
		RestartPipeClient();
	}
	else {
		std::wstring wResponse;
		pc->Recv(wResponse);
		if (wResponse.compare(L"Block") == 0) {
			bBlock = true;
		}
		else {
			bBlock = false;
		}
	}

	delete[] b;
	LeaveCriticalSection(&cs);
}

// ��Ƀt�H�[�}�b�g���͑��M�����
void __fastcall SendPacket_Hook(void *ecx, void *edx, OutPacket *p) {
	if (uEnterSendPacket_ret != (ULONG_PTR)_ReturnAddress()) {
		bool bBlock = false;
		AddSendPacket(p, (DWORD)_ReturnAddress(), bBlock);
		if (bBlock) {
			return;
		}
	}
	return _SendPacket(ecx, p);
}

void __fastcall COutPacket_Hook(OutPacket *p, void *edx, BYTE b, void *v) {
	packet_id_out++;
	PacketExtraInformation pxi = { packet_id_out, (ULONG_PTR)_ReturnAddress(), TENVI_ENCODE_HEADER_1, 0, sizeof(BYTE) };
	AddExtra(pxi);
	return _COutPacket(p, b, v);
}

void __fastcall Encode1_Hook(OutPacket *p, void *edx, BYTE b) {
	if (p->encoded) {
		PacketExtraInformation pxi = { packet_id_out, (ULONG_PTR)_ReturnAddress(), ENCODE1, p->encoded, sizeof(BYTE) };
		AddExtra(pxi);
	}
	return _Encode1(p, b);
}

void __fastcall Encode2_Hook(OutPacket *p, void *edx, WORD w) {
	if (p->encoded) {
		PacketExtraInformation pxi = { packet_id_out, (ULONG_PTR)_ReturnAddress(), ENCODE2, p->encoded, sizeof(WORD) };
		AddExtra(pxi);
	}
	return _Encode2(p, w);

}

void __fastcall Encode4_Hook(OutPacket *p, void *edx, DWORD dw) {
	PacketExtraInformation pxi = { packet_id_out, (ULONG_PTR)_ReturnAddress(), ENCODE4, p->encoded, sizeof(DWORD) };
	AddExtra(pxi);
	return _Encode4(p, dw);
}

void __fastcall EncodeStr_Hook(OutPacket *p, void *edx, char *s) {
	PacketExtraInformation pxi = { packet_id_out, (DWORD)_ReturnAddress(), ENCODESTR, p->encoded, sizeof(WORD) + strlen(s) };
	AddExtra(pxi);
	return _EncodeStr(p, s);
}

void __fastcall EncodeWStr1_Hook(OutPacket *p, void *edx, WCHAR *wc) {
	BYTE len = 0;
	while (wc[len] && len < 0xFF) {
		len++;
	}
	PacketExtraInformation pxi = { packet_id_out, (DWORD)_ReturnAddress(), TENVI_ENCODE_WSTR_1, p->encoded, sizeof(BYTE) + len * sizeof(WORD) };
	AddExtra(pxi);
	return _EncodeWStr1(p, wc);
}

void __fastcall EncodeBuffer_Hook(OutPacket *p, void *edx, BYTE *b, DWORD len) {
	PacketExtraInformation pxi = { packet_id_out, (ULONG_PTR)_ReturnAddress(), ENCODEBUFFER, p->encoded, len };
	AddExtra(pxi);
	return _EncodeBuffer(p, b, len);
}

void __fastcall ProcessPacket_Hook(void *pCClientSocket, void *edx, InPacket *p) {
	bool bBlock = false;
	AddRecvPacket(p, (ULONG_PTR)_ReturnAddress(), bBlock);
	if (!bBlock) {
		_ProcessPacket(pCClientSocket, p);
	}
	PacketExtraInformation pxi = { packet_id_in++, (ULONG_PTR)0, DECODE_END, 0, 0 };
	AddExtra(pxi);
}

WORD __fastcall DecodeHeader_Hook(InPacket *p, void *edx) {
	if (p->decoded == 0x04) {
		{
			PacketExtraInformation pxi = { packet_id_in++, (ULONG_PTR)0, DECODE_END, 0, 0 };
			AddExtra(pxi);

			packet_id_in++;
			bool bBlock = false;
			AddRecvPacket(p, (ULONG_PTR)_ReturnAddress(), bBlock);
		}
		PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), TENVI_DECODE_HEADER_1, p->decoded - 4, sizeof(BYTE) };
		AddExtra(pxi);
	}
	return _DecodeHeader(p);
}

BYTE __fastcall Decode1_Hook(InPacket *p, void *edx) {
	if (p->decoded != 0x04) {
		PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), DECODE1, p->decoded - 4, sizeof(BYTE) };
		AddExtra(pxi);
	}
	return _Decode1(p);
}

WORD __fastcall Decode2_Hook(InPacket *p, void *edx) {
	PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), DECODE2, p->decoded - 4, sizeof(WORD) };
	AddExtra(pxi);
	return _Decode2(p);
}

DWORD __fastcall Decode4_Hook(InPacket *p, void *edx) {
	PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), DECODE4, p->decoded - 4, sizeof(DWORD) };
	AddExtra(pxi);
	return _Decode4(p);
}

char** __fastcall DecodeStr_Hook(InPacket *p, void *edx, char **s) {
	PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), DECODESTR, p->decoded - 4, sizeof(WORD) + *(WORD *)&p->packet[p->decoded] };
	AddExtra(pxi);
	return _DecodeStr(p, s);
}

// Tenvi
WCHAR** __fastcall DecodeWStr1_Hook(InPacket *p, void *edx, WCHAR **wc) {
	PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), TENVI_DECODE_WSTR_1, p->decoded - 4, sizeof(BYTE) + p->packet[p->decoded] * 2 };
	AddExtra(pxi);
	return _DecodeWStr1(p, wc);
}
WCHAR** __fastcall DecodeWStr2_Hook(InPacket *p, void *edx, WCHAR **wc) {
	PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), TENVI_DECODE_WSTR_2, p->decoded - 4, sizeof(WORD) + *(WORD *)&p->packet[p->decoded] * 2 };
	AddExtra(pxi);
	return _DecodeWStr2(p, wc);
}

void __fastcall DecodeBuffer_Hook(InPacket *p, void *edx, BYTE *b, DWORD len) {
	PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), DECODEBUFFER, p->decoded - 4, len };
	AddExtra(pxi);
	return _DecodeBuffer(p, b, len);
}

ULONGLONG(__thiscall *_Decode8)(InPacket *);
ULONGLONG __fastcall Decode8_Hook(InPacket *p) {
	PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), DECODE8, p->decoded - 4, sizeof(ULONGLONG) };
	AddExtra(pxi);
	return _Decode8(p);
}

float(__thiscall *_DecodeFloat)(InPacket *);
float __fastcall DecodeFloat_Hook(InPacket *p) {
	PacketExtraInformation pxi = { packet_id_in, (ULONG_PTR)_ReturnAddress(), DECODE4, p->decoded - 4, sizeof(DWORD) };
	AddExtra(pxi);
	return _DecodeFloat(p);
}


// Packet Injector
ULONG_PTR uSendPacket = 0;
ULONG_PTR uEnterSendPacket = 0;
ULONG_PTR uEnterSendPacket_ret = 0;
ULONG_PTR uCClientSocket = 0;
void(__thiscall *_EnterSendPacket)(OutPacket *p) = NULL;
void __fastcall EnterSendPacket_Hook(OutPacket *p) {
	bool bBlock = false;
	AddSendPacket(p, (ULONG_PTR)_ReturnAddress(), bBlock);
	if (bBlock) {
		return;
	}
	return _EnterSendPacket(p);
}

ULONG_PTR GetCClientSocket() {
	if (!uCClientSocket) {
		return 0;
	}

	return *(ULONG_PTR *)uCClientSocket;
}

// FF 74 24 04 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C3
bool ScannerEnterSendPacket(ULONG_PTR uAddress) {
	if (!uSendPacket) {
		return false;
	}

	ULONG_PTR uCall = uAddress + 0x0A;
	ULONG_PTR uFunction = uCall + 0x05 + *(signed long int *)(uCall + 0x01);
	if (uFunction != uSendPacket) {
		return false;
	}

	uEnterSendPacket = uAddress;
	uEnterSendPacket_ret = uEnterSendPacket + 0x0F;
	uCClientSocket = *(ULONG_PTR *)(uAddress + 0x06);
	return true;
}

bool ListScan(Rosemary &r, ULONG_PTR &result, std::wstring aob[], size_t count, int &used) {
	result = 0; // scan result
	used = -1; // which aob is used
	for (size_t i = 0; i < count; i++) {
		result = r.Scan(aob[i]);
		if (result) {
			used = i;
			return true;
		}
	}
	return false;
}

DWORD Addr_OnPacketClass = 0;

void MyProcessPacket(InPacket *p) {
	void *OnPacketClass = (void *)(*(DWORD *)(*(DWORD *)Addr_OnPacketClass + 0x160));
	void(__thiscall *_OnPacket)(void *ecx, InPacket *p) = (decltype(_OnPacket)(*(DWORD *)(*(DWORD *)OnPacketClass + 0x2C)));
	_OnPacket(OnPacketClass, p);
	PacketExtraInformation pxi = { packet_id_in++, (ULONG_PTR)0, DECODE_END, 0, 0 };
	AddExtra(pxi);
}

#define HOOKDEBUG(func) \
{\
	ListScan(r, u##func, AOB_##func, _countof(AOB_##func), iWorkingAob);\
	DEBUG(L""#func" = " + QWORDtoString(u##func) + L", Aob = " + std::to_wstring(iWorkingAob));\
	SHookFunction(func, u##func);\
}

bool PacketHook() {
	InitializeCriticalSection(&cs);
	Rosemary r;

	DWORD old = 0;
	DWORD addr = 0x00401000;
	VirtualProtect((void *)addr, 0x1000, PAGE_EXECUTE_READWRITE, &old);


	switch (GetRegion()) {
	case TENVI_JP: {
		Addr_OnPacketClass = 0x006DB164;
		SHookFunction(EnterSendPacket, 0x0055F2A8);
		SHookFunction(COutPacket, 0x0055F36D);
		SHookFunction(Encode1, 0x0040F287);
		SHookFunction(Encode2, 0x00402FFD);
		SHookFunction(Encode4, 0x00403025);
		SHookFunction(EncodeWStr1, 0x0040F435);
		SHookFunction(DecodeHeader, 0x0055F357);
		SHookFunction(Decode1, 0x00402EFE);
		SHookFunction(Decode2, 0x00402F30);
		SHookFunction(Decode4, 0x00402F63);
		SHookFunction(DecodeWStr1, 0x0045BBCD);
		SHookFunction(DecodeWStr2, 0x0040921A);
		SHookFunction(Decode8, 0x00402FC7);
		SHookFunction(DecodeFloat, 0x00402F95);
		break;
	}
	case TENVI_CN: {
		Addr_OnPacketClass = 0x006FAF44;
		SHookFunction(EnterSendPacket, 0x0056AADB);
		SHookFunction(COutPacket, 0x0056ABA0);
		SHookFunction(Encode1, 0x0040EEB6);
		SHookFunction(Encode2, 0x0040346A);
		SHookFunction(Encode4, 0x00403492);
		// #define Addr_Encode8 0x004034B8
		SHookFunction(EncodeWStr1, 0x00413ECC);
		// #define Addr_EncodeWStr2 0x00496239
		SHookFunction(DecodeHeader, 0x0056AB8A);
		SHookFunction(Decode1, 0x0040336B);
		SHookFunction(Decode2, 0x0040339D);
		SHookFunction(Decode4, 0x004033D0);
		SHookFunction(DecodeWStr1, 0x0045CB52);
		SHookFunction(DecodeWStr2, 0x00408EEF);
		SHookFunction(Decode8, 0x00403434);
		SHookFunction(DecodeFloat, 0x00403402);
		break;
	}
	case TENVI_HK: {
		Addr_OnPacketClass = 0x0075CF84;
		SHookFunction(EnterSendPacket, 0x005AC927);
		SHookFunction(COutPacket, 0x005AC9EC);
		SHookFunction(Encode1, 0x00417D30);
		SHookFunction(Encode2, 0x00402262);
		SHookFunction(Encode4, 0x0040228A);
		SHookFunction(EncodeWStr1, 0x0040FC19);
		SHookFunction(DecodeHeader, 0x005AC9D6);
		SHookFunction(Decode1, 0x00402163);
		SHookFunction(Decode2, 0x00402195);
		SHookFunction(Decode4, 0x004021C8);
		SHookFunction(DecodeWStr1, 0x0043B921);
		SHookFunction(DecodeWStr2, 0x00469777);
		SHookFunction(Decode8, 0x0040222C);
		SHookFunction(DecodeFloat, 0x004021FA);
		break;
	}
	case TENVI_KR: {
		Addr_OnPacketClass = 0x0075E184;
		SHookFunction(EnterSendPacket, 0x005CBA0F);
		SHookFunction(COutPacket, 0x005CBAD4);
		SHookFunction(Encode1, 0x0040705B);
		SHookFunction(Encode2, 0x00402249);
		SHookFunction(Encode4, 0x00402271);
		// #define Addr_Encode8 0x00402297
		SHookFunction(EncodeWStr1, 0x004143DC);
		SHookFunction(DecodeHeader, 0x005CBABE);
		SHookFunction(Decode1, 0x0040214A);
		SHookFunction(Decode2, 0x0040217C);
		SHookFunction(Decode4, 0x004021AF);
		SHookFunction(DecodeWStr1, 0x0044C2BE);
		SHookFunction(DecodeWStr2, 0x0047B50E);
		SHookFunction(Decode8, 0x00402213);
		SHookFunction(DecodeFloat, 0x004021E1);
		break;
	}
	default: {
		break;
	}
	}


	VirtualProtect((void *)addr, 0x1000, old, &old);

	StartPipeClient();
	RunPacketSender();
	return true;
}
