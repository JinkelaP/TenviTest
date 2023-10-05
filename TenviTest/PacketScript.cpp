#include"PacketScript.h"
#include"../Share/Simple/Simple.h"
#include <sstream>
#include <istream>
#include <iostream>

Frost::Frost(std::wstring wScript) {
	//input = wScript;

	std::wistringstream MyStream(wScript);
	std::wstring s;

	while (std::getline(MyStream, s)) {
		input_list.push_back(s);
		std::wcout << s << std::endl;
	}
}

Frost::~Frost() {

}

enum FORMAT_TYPE {
	TYPE_UNK,
	TYPE_BYTE,
	TYPE_WORD,
	TYPE_DWORD,
	TYPE_WSTR,
	TYPE_QWORD,
	TYPE_FLOAT,
};

bool Frost::DataParse(std::wstring data, ULONGLONG &uData) {
	uData = 0;

	std::wsmatch match;
	// hex
	if (std::regex_search(data, match, std::wregex(LR"(^\s*(0x|)([0-9A-Fa-f]+))")) && match.size() >= 2) {
		swscanf_s(match[2].str().c_str(), L"%llX", &uData);
		DEBUG(match[2].str());
		return true;
	}
	// int
	if (std::regex_search(data, match, std::wregex(LR"(^\s*(#-)(\d+))")) && match.size() >= 2) {
		swscanf_s(match[2].str().c_str(), L"%lld", &uData);
		uData = ~uData + 1;
		DEBUG(match[2].str());
		return true;
	}
	// int
	if (std::regex_search(data, match, std::wregex(LR"(^\s*(#)(\d+))")) && match.size() >= 2) {
		swscanf_s(match[2].str().c_str(), L"%lld", &uData);
		DEBUG(match[2].str());
		return true;
	}

	DEBUG(L"DataParse Fail");

	return false;
}

bool Frost::WStrParse(std::wstring data, std::wstring &wData) {
	wData = L"";

	std::wsmatch match;

	// "Str"
	if (std::regex_search(data, match, std::wregex(LR"(^\s*(L|)\"(.*)\")")) && match.size() >= 2) {
		wData = match[2].str();
		return true;
	}

	wData = data;
	return true;
}


bool Frost::Parse() {
	for (auto input : input_list) {
		Parse(input);
	}
	return true;
}

bool Frost::Parse(std::wstring input) {
	std::wsmatch match;

	FORMAT_TYPE type = TYPE_UNK;
	std::wstring data;

	// LR"(^\s*(Encode1)\s*\(([0-9A-Fa-f]+)\))"
	if (std::regex_search(input, match, std::wregex(LR"(^\s*(EncodeHeader|Header|Encode1)\s*\((.*)\))")) && match.size() >= 2) {
		type = TYPE_BYTE;
		data = match[2];
	}
	else if (std::regex_search(input, match, std::wregex(LR"(^\s*(Encode2)\s*\((.*)\))")) && match.size() >= 2) {
		type = TYPE_WORD;
		data = match[2];
	}
	else if (std::regex_search(input, match, std::wregex(LR"(^\s*(Encode4)\s*\((.*)\))")) && match.size() >= 2) {
		type = TYPE_DWORD;
		data = match[2];
	}
	else if (std::regex_search(input, match, std::wregex(LR"(^\s*(Encode8)\s*\((.*)\))")) && match.size() >= 2) {
		type = TYPE_QWORD;
		data = match[2];
	}
	else if (std::regex_search(input, match, std::wregex(LR"(^\s*(EncodeWStr)\s*\((.*)\))")) && match.size() >= 2) {
		type = TYPE_WSTR;
		data = match[2];
	}
	else {
		DEBUG(L"Parse Failvvv");
		return false;
	}

	/*
	if (std::regex_search(input, match, std::wregex(LR"(^\s*(EncodeFloat)\s*\((.*)\))")) && match.size() >= 2) {
		type = TYPE_FLOAT;
		data = match[2];
	}
	*/
	switch (type) {
	case TYPE_BYTE:
	{
		ULONGLONG val = 0;
		if (!DataParse(data, val)) {
			return false;
		}
		p.Encode1(val);
		return true;
	}
	case TYPE_WORD:
	{
		ULONGLONG val = 0;
		if (!DataParse(data, val)) {
			return false;
		}
		p.Encode2(val);
		return true;
	}
	case TYPE_DWORD:
	{
		ULONGLONG val = 0;
		if (!DataParse(data, val)) {
			return false;
		}
		p.Encode4(val);
		return true;
	}
	case TYPE_QWORD:
	{
		ULONGLONG val = 0;
		if (!DataParse(data, val)) {
			return false;
		}
		p.Encode8(val);
		return true;
	}
	//case TYPE_FLOAT:
	case TYPE_WSTR: {
		std::wstring val;
		WStrParse(data, val);
		p.EncodeWStr(val);
		return true;
	}
	default:
	{
		return false;
	}
	}

	DEBUG(L"Parse Fail");
	return false;
}


std::wstring Frost::GetText() {
	return L"@" + DatatoString(&p.get()[0], p.get().size(), true);
}