#ifndef __TENVI_DATA_H__
#define __TENVI_DATA_H__

#include"TenviMap.h"

// loaded xml data list
class TenviData {
private:
	std::vector<TenviMap*> data_map;
	std::string xml_path;
	std::string region_str;

public:
	TenviMap* get_map(DWORD id);
	void set_xml_path(std::wstring path);
	std::string get_xml_path();
	std::string get_region_str();
};

extern TenviData tenvi_data;
#endif