#include"ServerPacket.h"

// static
BYTE ServerPacket::opcode[SP_END] = { 0 };

BYTE* ServerPacket::GetOpcode() {
	return opcode;
}

// header = byte
ServerPacket::ServerPacket(SERVER_PACKET header) {
	Encode1(opcode[header]);
}

std::vector<BYTE>& ServerPacket::get() {
	return packet;
}

void ServerPacket::Encode1(BYTE val) {
	packet.push_back(val);
}
void ServerPacket::Encode2(WORD val) {
	packet.push_back(val & 0xFF);
	packet.push_back((val >> 8) & 0xFF);
}
void ServerPacket::Encode4(DWORD val) {
	packet.push_back(val & 0xFF);
	packet.push_back((val >> 8) & 0xFF);
	packet.push_back((val >> 16) & 0xFF);
	packet.push_back((val >> 24) & 0xFF);
}

// legnth = byte
void ServerPacket::EncodeWStr1(std::wstring val) {
	Encode1((BYTE)val.length());
	for (size_t i = 0; i < val.length(); i++) {
		Encode2((WORD)val.at(i));
	}
}

void ServerPacket::EncodeWStr2(std::wstring val) {
	Encode2((WORD)val.length());
	for (size_t i = 0; i < val.length(); i++) {
		Encode2((WORD)val.at(i));
	}
}

void ServerPacket::Encode8(ULONGLONG val) {
	Encode4((DWORD)val);
	Encode4((DWORD)(val >> 32));
}

void ServerPacket::EncodeFloat(float val) {
	Encode4(*(DWORD *)&val); // no cast
}