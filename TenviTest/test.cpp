#include"../Share/Simple/Simple.h"
#include"PacketScript.h"

enum SubControl {
	RESERVED,
	BUTTON_TEST,
	EDIT_INPUT,
	EDIT_OUTPUT,
};

bool OnCreate(Alice &a) {
	a.Button(BUTTON_TEST, L"test", 3, 3);
	a.TextArea(EDIT_INPUT, 3, 20, 390, 300);
	a.TextArea(EDIT_OUTPUT, 400, 20, 390, 300);
	a.ReadOnly(EDIT_OUTPUT);
	return true;
}

void CreatePacketBuffer(std::wstring wInput, std::wstring &wOutput) {

}

bool OnCommand(Alice &a, int nIDDlgItem) {

	if (nIDDlgItem == BUTTON_TEST) {
		std::wstring wText = a.GetText(EDIT_INPUT);
		Frost f(wText);

		if (f.Parse()) {
			wText = f.GetText();
		}
		else {
			wText = L"parse error";
		}

		/*
		ServerPacket p(0x00);
		p.Encode4(-1);
		p.Encode2(1);
		p.EncodeWStr(L"Riremito");

		std::vector<BYTE> &packet = p.get();
		wText = L"@" + DatatoString(&packet[0], packet.size(), true);
		*/

		a.SetText(EDIT_OUTPUT, wText);
		return true;
	}

	return true;
}

bool MainGUI(HINSTANCE hInstance) {
	Alice a(L"TenviTesterClass", L"TenviTest v127", 800, 600, hInstance);

	a.SetOnCreate(OnCreate);
	a.SetOnCommand(OnCommand);
	a.Run();
	a.Wait(); // lock
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MainGUI(hInstance);
	return 0;
}