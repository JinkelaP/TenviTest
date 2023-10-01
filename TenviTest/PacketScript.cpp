#include"PacketScript.h"

Frost::Frost(std::wstring wScript) {
	text_original = wScript;
}

Frost::~Frost() {

}

bool Frost::Parse() {
	text_debug = text_original;
	return true;
}


std::wstring Frost::GetText() {
	return text_debug;
}