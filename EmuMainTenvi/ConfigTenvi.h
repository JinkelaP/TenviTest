#ifndef __CONFIGTENVI_H__
#define __CONFIGTENVI_H__

#include"../Share/Simple/Simple.h"

enum Region {
	TENVI_JP, // JP v127
	TENVI_CN, // CN v126
	TENVI_HK, // HK v102
	TENVI_KR, // KR v200
	TENVI_KRX, // KR v107 (Xtream)
};

Region GetRegion();
bool LoadRegionConfig(HINSTANCE hinstDLL);


#endif