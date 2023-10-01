#ifndef __PACKETSCRIPT_H__
#define __PACKETSCRIPT_H__

#include<Windows.h>
#include<string>
#include<regex>

class Frost {
private:
	std::wstring text_original;
	std::wstring text_debug;

	bool Encode1();
	bool Encode2();
	bool Encode4();
	bool EncodeStr();
	bool EncodeWStr();
public:
	Frost(std::wstring wScript);
	~Frost();
	bool Parse();
	std::wstring GetText();
};

#endif