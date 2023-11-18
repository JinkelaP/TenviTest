#ifndef __PACKETSCRIPT_H__
#define __PACKETSCRIPT_H__

#include<Windows.h>
#include<string>
#include<regex>
#include"Packet.h"

class Frost {
private:
	//std::wstring input;
	std::vector<std::wstring> input_list;
	ServerPacket p;

	bool DataParse(std::wstring data, ULONGLONG &uData);
	bool DataParseFloat(std::wstring data, float &fData);
	bool WStrParse(std::wstring data, std::wstring &wData);
	bool Parse(std::wstring input);

public:
	Frost(std::wstring wScript);
	~Frost();
	std::wstring GetText();
	bool Parse();
};

#endif