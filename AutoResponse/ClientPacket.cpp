#include"ClientPacket.h"

ClientPacket::ClientPacket(BYTE *spacket, DWORD size) {
	decoded = 0;
	packet.resize(size);
	memcpy_s(&packet[0], size, spacket, size);
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

std::wstring ClientPacket::DecodeWStr() {
	BYTE length = Decode1();
	std::vector<WCHAR> wstr;

	for (size_t i = 0; i < length; i++) {
		wstr.push_back(Decode2());
	}
	wstr.push_back(L'\0');

	//std::wstring val = std::wstring(*(WCHAR *)&packet[decoded], length);
	//decoded += length * sizeof(WCHAR);
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