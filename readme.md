# TenviTest
## Supported
+ JP v127
+ CN v126
+ HK v102
+ KR v200
+ KRX v107 (Xtream)

## Files
+ https://github.com/Riremito/tv_xml
	+ data xml
+ RunEmuTenvi.exe
	+ DLL injector
	+ you need to edit RunEmuTenvi.ini to use it
+ EmuLoaderTenvi.dll
	+ loads DLLs
+ LocalHost.dll
	+ modify server IP
	+ source : https://github.com/Riremito/LocalHost
	+ this is not needed now
+ EmuMainTenvi.dll
	+ remove HackShield and Game Guard
+ AutoResponse.dll
	+ work as local server
+ PacketTenvi.dll
	+ packet editor
+ PETenvi.exe
	+ packet editor gui

## How to build
+ you have to build this library and put .lib and headers in Share folder
	+ https://github.com/Riremito/tools

## How to use
+ drag and drop exe file to RunEmuTenvi.exe at first time
	+ you should see default RunEmuTenvi.ini settings
	+ if you change Region, you can use it for CN/HK/KR versions