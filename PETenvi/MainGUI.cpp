#include"MainGUI.h"

int global_header_size = 1; // default, 0 = raw

int GetHeaderSize() {
	return global_header_size;
}

// 接続状態
void SetInfo(std::wstring wText) {
	Alice &a = GetMainGUI();
	a.SetText(STATIC_INFO, wText);
}

// ListViewの更新
bool UpdateLogger(PacketEditorMessage &pem, bool &bBlock) {
	if (pem.header != SENDPACKET && pem.header != RECVPACKET) {
		return false;
	}

	bBlock = false;
	FilterType ft = NORMAL_PACKET;
	CheckFilter(pem, ft);

	if (ft == BLOCK_PACKET) {
		bBlock = true;
	}

	if (ft == IGNORE_PACKET) {
		return false;
	}

	Alice &a = GetMainGUI();

	std::wstring wType;
	if (pem.header == SENDPACKET) {
		if (!a.CheckBoxStatus(CHECK_SEND)) {
			return false;
		}
		wType = L"Send";
	}
	else if (pem.header == RECVPACKET) {
		if (!a.CheckBoxStatus(CHECK_RECV)) {
			return false;
		}
		wType = L"Recv";
	}

	a.ListView_AddItem(LISTVIEW_LOGGER, LV_TYPE, wType);
	a.ListView_AddItem(LISTVIEW_LOGGER, LV_ID, std::to_wstring(pem.id));
	a.ListView_AddItem(LISTVIEW_LOGGER, LV_LENGTH, std::to_wstring(pem.Binary.length));
	std::wstring wpacket = DatatoString(pem.Binary.packet, (pem.Binary.length > 1024) ? 1024 : pem.Binary.length, true);

	size_t header_size = (size_t)GetHeaderSize();
	if (header_size <= pem.Binary.length) {
		if (header_size) {
			// remove header
			wpacket.erase(wpacket.begin(), wpacket.begin() + (header_size * 2 + (header_size - 1))); // XX YY
			// add header
			std::wstring header_text = L"@";
			if (header_size == 1) {
				header_text += BYTEtoString(pem.Binary.packet[0]);
			}
			else if (header_size == 2) {
				header_text += WORDtoString(*(WORD *)&pem.Binary.packet[0]);
			}
			else if (header_size == 4) {
				header_text += DWORDtoString(*(DWORD *)&pem.Binary.packet[0]);
			}

			if (!bBlock) {
				a.ListView_AddItem(LISTVIEW_LOGGER, LV_PACKET, header_text + wpacket);
			}
			else {
				a.ListView_AddItem(LISTVIEW_LOGGER, LV_PACKET, header_text + L" (Blocked)" + wpacket);
			}
		}
		// Raw
		else {
			if (!bBlock) {
				a.ListView_AddItem(LISTVIEW_LOGGER, LV_PACKET, wpacket);
			}
			else {
				a.ListView_AddItem(LISTVIEW_LOGGER, LV_PACKET, L"(Blocked) " + wpacket);
			}
		}
	}
	else {
		a.ListView_AddItem(LISTVIEW_LOGGER, LV_PACKET, L"(too short)" + wpacket);
	}
	return true;
}

// Status, Format Packetの更新
bool UpdateStatus(PacketEditorMessage &pem) {
	if (pem.header != SENDPACKET && pem.header != DECODE_END) {
		return false;
	}

	std::wstring wID = std::to_wstring(pem.id);
	Alice &a = GetMainGUI();
	int line = 0;

	if (!a.ListView_Find(LISTVIEW_LOGGER, LV_ID, wID, line)) {
		return false;
	}

	std::vector<PacketData> &vpd = (pem.header == SENDPACKET) ? GetOutPacketFormat() : GetInPacketFormat();
	for (auto &pd : vpd) {
		if (pd.id == pem.id) {
			if (pd.status != 1) {
				a.ListView_UpdateItem(LISTVIEW_LOGGER, LV_STATUS, line, L"NG");
				return true;
			}
			std::wstring wFormatPacket;
			for (auto &pf : pd.format) {
				if (wFormatPacket.length()) {
					wFormatPacket += L" ";
				}
				wFormatPacket += GetFormatData(pd, pf);

				if (wFormatPacket.length() > 4096) {
					return false;
				}
			}

			a.ListView_UpdateItem(LISTVIEW_LOGGER, LV_PACKET_FORMAT, line, wFormatPacket);
			return true;
		}
	}

	return false;
}

// ListViewで選択中のPacketをセット
bool SetRawPacket(Alice &a, MessageHeader type, std::wstring &text_packet) {
	if (text_packet.length() > 1024) {
		return false;
	}

	if (type == SENDPACKET) {
		a.SetText(EDIT_PACKET_SEND, text_packet);
		a.SetText(EDIT_PACKET_RECV, L"");
		return true;
	}

	if (type == RECVPACKET) {
		a.SetText(EDIT_PACKET_SEND, L"");
		a.SetText(EDIT_PACKET_RECV, text_packet);
		return true;
	}

	return false;
}

bool ReadFormatPacket(Alice &a) {
	std::wstring wText = a.GetText(EDIT_PACKET_FORMAT);
	Frost f(wText);

	if (!f.Parse()) {
		wText = L"parse error";
		a.SetText(EDIT_PACKET_RECV, wText);
		return false;
	}

	wText = f.GetText();
	a.SetText(EDIT_PACKET_RECV, wText);
	return true;
}



bool OnCreate(Alice &a) {
	a.ListView(LISTVIEW_LOGGER, 3, 3, (PE_WIDTH - 6), PE_LOGGER_HEIGHT);
	a.ListView_AddHeader(LISTVIEW_LOGGER, L"Type", 40);
	a.ListView_AddHeader(LISTVIEW_LOGGER, L"ID", 10);
	a.ListView_AddHeader(LISTVIEW_LOGGER, L"Length", 50);
	a.ListView_AddHeader(LISTVIEW_LOGGER, L"Status", 50);
	a.ListView_AddHeader(LISTVIEW_LOGGER, L"Format Packet", 10);
	a.ListView_AddHeader(LISTVIEW_LOGGER, L"Packet", (PE_WIDTH - 180));


	a.StaticText(STATIC_INFO, L"Disconnected", 10, (PE_LOGGER_HEIGHT + 10));

	a.Button(BUTTON_OPEN_FORMATVIEW, L"Format View", 100, (PE_LOGGER_HEIGHT + 10), 100);
	a.CheckBox(CHECK_HEADER_SIZE, L"Update", 390, (PE_LOGGER_HEIGHT + 10), BST_CHECKED);
	a.Button(BUTTON_OPEN_FILTER, L"Filter", 450, (PE_LOGGER_HEIGHT + 10), 100);

	a.CheckBox(CHECK_SEND, L"Send", (PE_WIDTH - 100), (PE_LOGGER_HEIGHT + 10), BST_CHECKED);
	a.CheckBox(CHECK_RECV, L"Recv", (PE_WIDTH - 50), (PE_LOGGER_HEIGHT + 10), BST_CHECKED);
	a.Button(BUTTON_CLEAR, L"Clear", (PE_WIDTH - 150), (PE_LOGGER_HEIGHT + 10));

	// sender
	a.EditBox(EDIT_PACKET_SEND, 10, (PE_LOGGER_HEIGHT + 50), L"CClientSocket::SendPacket", (PE_WIDTH - 120));
	a.EditBox(EDIT_PACKET_RECV, 10, (PE_LOGGER_HEIGHT + 70), L"CClientSocket::ProcessPacket", (PE_WIDTH -120));
	a.Button(BUTTON_SEND, L"SendPacket", (PE_WIDTH - 100), (PE_LOGGER_HEIGHT + 50));
	a.Button(BUTTON_RECV, L"RecvPacket", (PE_WIDTH - 100), (PE_LOGGER_HEIGHT + 70));
	a.CheckBox(CHECK_LOCK, L"Lock", (PE_WIDTH - 100) + 20, (PE_LOGGER_HEIGHT + 90));
	// debug
#ifdef PE_DEBUG
	a.Button(BUTTON_INC_SEND, L"+", (PE_WIDTH - 25), (PE_LOGGER_HEIGHT + 50));
	a.Button(BUTTON_INC_RECV, L"+", (PE_WIDTH - 25), (PE_LOGGER_HEIGHT + 70));
#endif


	a.TextArea(EDIT_PACKET_FORMAT, 10, (PE_LOGGER_HEIGHT + 90), (PE_WIDTH - 100), 200);
	a.Button(BUTTON_RECV_FORMAT, L"RecvPacket", (PE_WIDTH - 100) + 20, (PE_LOGGER_HEIGHT + 90) + 180);

	PacketLogger(); // logger
	return true;
}

// 色々な処理
bool OnCommand(Alice &a, int nIDDlgItem) {
	if (nIDDlgItem == BUTTON_CLEAR) {
		ClearAll(); // logger
		a.ListView_Clear(LISTVIEW_LOGGER);
		return true;
	}

	if (nIDDlgItem == BUTTON_SEND) {
		PacketSender(a, SENDPACKET);
		return true;
	}

	if (nIDDlgItem == BUTTON_RECV) {
		PacketSender(a, RECVPACKET);
		return true;
	}

	if (nIDDlgItem == BUTTON_RECV_FORMAT) {
		if (ReadFormatPacket(a)) {
			PacketSender(a, RECVPACKET);
		}
		return true;
	}

	// edit lock
	if (nIDDlgItem == CHECK_LOCK) {
		bool read_only = a.CheckBoxStatus(nIDDlgItem);
		a.ReadOnly(EDIT_PACKET_SEND, read_only);
		a.ReadOnly(EDIT_PACKET_RECV, read_only);
		return true;
	}

	if (nIDDlgItem == BUTTON_OPEN_FORMATVIEW) {
		OpenFormatGUI();
		return true;
	}

	if (nIDDlgItem == BUTTON_OPEN_FILTER) {
		OpenFilterGUI();
		return true;
	}

	return true;
}

// ListView上で選択したパケットを入力欄にコピー
bool OnNotify(Alice &a, int nIDDlgItem) {
	if (nIDDlgItem == LISTVIEW_LOGGER) {
		std::wstring text_type;
		std::wstring text_id;
		std::wstring text_packet;
		bool check = true;

		check &= a.ListView_Copy(LISTVIEW_LOGGER, LV_TYPE, text_type, false);
		check &= a.ListView_Copy(LISTVIEW_LOGGER, LV_ID, text_id, false);
		check &= a.ListView_Copy(LISTVIEW_LOGGER, LV_PACKET, text_packet, true, 4096);

		if (!check) {
			return false;
		}

		MessageHeader type = UNKNOWN;

		if (text_type.compare(L"Send") == 0) {
			type = SENDPACKET;
		}
		else if (text_type.compare(L"Recv") == 0) {
			type = RECVPACKET;
		}

		// raw packet
		if (!a.CheckBoxStatus(CHECK_LOCK)) {
			SetRawPacket(a, type, text_packet);
		}

		DWORD id = _wtoi(text_id.c_str());

		if (text_type.compare(L"Send") == 0) {
			SetExtraInfo(GetOutPacketFormat(), id);
			SetFilterHeader(SENDPACKET, text_packet);
			return true;
		}
		if (text_type.compare(L"Recv") == 0) {
			SetExtraInfo(GetInPacketFormat(), id);
			SetFilterHeader(RECVPACKET, text_packet);
			return true;
		}

		return false;
	}
	return true;
}

// global main gui
Alice *global_a = NULL;
void SetMainGUI(Alice *ga) {
	global_a = ga;
}

Alice& GetMainGUI() {
	return *global_a;
}

// 終了処理
LRESULT CALLBACK ExitCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (Msg == WM_DESTROY) {
		// format view
		HWND sub_hwnd = FVGet();
		if (sub_hwnd) {
			CloseWindow(sub_hwnd);
			DestroyWindow(sub_hwnd);
		}
		// 強制終了
		ExitProcess(0);
	}
	return 0;
}

// start gui
bool MainGUI(HINSTANCE hInstance) {
	Alice a(L"PETenviClass", L"RirePE for Tenvi", PE_WIDTH, PE_HEIGHT, hInstance);

	SetMainGUI(&a);
	a.SetOnCreate(OnCreate);
	a.SetOnCommand(OnCommand);
	a.SetOnNotify(OnNotify);
	a.SetCallback(ExitCallback, Alice::CT_CALL);
	a.Run();
	InitFormatGUI(hInstance); // no lock
	InitFilterGUI(hInstance); // no lock
	a.Wait(); // lock
	return true;
}