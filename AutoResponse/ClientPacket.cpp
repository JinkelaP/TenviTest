#include"ClientPacket.h"

// static
BYTE ClientPacket::opcode[CP_END] = { 0 };

BYTE* ClientPacket::GetOpcode() {
	return opcode;
}

ClientPacket::ClientPacket(BYTE *spacket, DWORD size) {
	decoded = 0;
	packet.resize(size);
	memcpy_s(&packet[0], size, spacket, size);
}

CLIENT_PACKET ClientPacket::DecodeHeader() {
	BYTE header = Decode1();

	for (size_t i = 0; i < CP_END; i++) {
		if (opcode[i] == header) {
			return (CLIENT_PACKET)i;
		}
	}

	return CP_UNKNOWN;
}

BYTE ClientPacket::Decode1() {
	BYTE val = *(BYTE *)&packet[decoded];
	decoded++;
	return val;
}
WORD ClientPacket::Decode2() {
	WORD val = *(WORD *)&packet[decoded];
	decoded += 2;
	return val;
}
DWORD ClientPacket::Decode4() {
	DWORD val = *(DWORD *)&packet[decoded];
	decoded += 4;
	return val;
}

std::wstring ClientPacket::DecodeWStr1() {
	BYTE length = Decode1();
	std::vector<WCHAR> wstr;

	for (size_t i = 0; i < length; i++) {
		wstr.push_back(Decode2());
	}
	wstr.push_back(L'\0');

	return std::wstring(&wstr[0]);
}

std::wstring ClientPacket::DecodeWStr2() {
	WORD length = Decode2();
	std::vector<WCHAR> wstr;

	for (size_t i = 0; i < length; i++) {
		wstr.push_back(Decode2());
	}
	wstr.push_back(L'\0');

	return std::wstring(&wstr[0]);
}

ULONGLONG ClientPacket::Decode8() {
	ULONGLONG val = *(ULONGLONG *)&packet[decoded];
	decoded += 8;
	return val;
}

float ClientPacket::DecodeFloat() {
	float val = *(float *)&packet[decoded];
	decoded += 4;
	return val;
}