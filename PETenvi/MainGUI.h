#ifndef __MAINGUI_H__
#define __MAINGUI_H__

#include"../Share/Simple/Simple.h"
#include"ControlList.h"
#include"PETenvi.h"
#include"PacketLogger.h"
#include"PacketSender.h"
#include"FormatGUI.h"
#include"FilterGUI.h"
#include"Config.h"
#include"PacketScript.h"

#define PE_WIDTH 800
#define PE_HEIGHT 600
#define PE_LOGGER_HEIGHT 200

#define PE_DEBUG 1


bool MainGUI(HINSTANCE hInstance);
Alice& GetMainGUI();
bool UpdateLogger(PacketEditorMessage &pem, bool &bBlock);
bool UpdateStatus(PacketEditorMessage &pem);
void SetInfo(std::wstring wText);
int GetHeaderSize();

std::wstring GetPipeNameLogger();
std::wstring GetPipeNameSender();

#endif