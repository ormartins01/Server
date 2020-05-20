#include "tray_icon.h"

const UINT WM_TRAY = WM_USER + 1;

HWND create_window(char* window_name)
{
	return 0;
}

LRESULT CALLBACK HiddenWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		NOTIFYICONDATA stData;
		ZeroMemory(&stData, sizeof(stData));
		stData.cbSize = sizeof(stData);
		stData.hWnd = hWnd;
		stData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		stData.uCallbackMessage = WM_TRAY;
		stData.hIcon = ::LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(101));
		//LoadStringSafe(IDS_TIP, stData.szTip, _countof(stData.szTip));
		if (!Shell_NotifyIcon(NIM_ADD, &stData))
			return -1; // oops
	}
	return 0;

	case WM_DESTROY:
	{
		NOTIFYICONDATA stData;
		ZeroMemory(&stData, sizeof(stData));
		stData.cbSize = sizeof(stData);
		stData.hWnd = hWnd;
		Shell_NotifyIcon(NIM_DELETE, &stData);
	}
	return 0;

	/*case WM_TRAY:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			SendMessage(hWnd, WM_COMMAND, ID_SHOW, 0);
			break;

		case WM_RBUTTONDOWN:
		{
			HMENU hMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_POPUP));
			if (hMenu)
			{
				HMENU hSubMenu = GetSubMenu(hMenu, 0);
				if (hSubMenu)
				{
					POINT stPoint;
					GetCursorPos(&stPoint);

					TrackPopupMenu(hSubMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, stPoint.x, stPoint.y, 0, hWnd, NULL);
				}

				DestroyMenu(hMenu);
			}
		}
		break;
		}
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_SHOW:
			if (CPopupWndData::s_pThis)
				SetForegroundWindow(CPopupWndData::s_pThis->m_hWnd);
			else
			{
				CPopupWndData stData;
				ZeroMemory(&stData, sizeof(stData));
				CPopupWndData::s_pThis = &stData;

				DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_CONVERTER), NULL, ConverterProc);

				if (stData.m_pControls)
					delete[] stData.m_pControls;
				CPopupWndData::s_pThis = NULL;
			}
			return 0;

		case ID_QUIT:
			PostQuitMessage(0);
			return 0;
		}
		break;
		*/
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

tray_icon::tray_icon(HINSTANCE h_instance, const wchar_t* window_name)
	: h_instance_(h_instance)
	, hidden_window_(0)
{
	WNDCLASS stWC;
	ZeroMemory(&stWC, sizeof(stWC));
	stWC.lpszClassName = window_name;
	stWC.hInstance = h_instance;
	stWC.lpfnWndProc = HiddenWndProc;
	a_class_ = RegisterClass(&stWC);
	if (!a_class_)
		return;
	hidden_window_ = CreateWindow((LPCTSTR)a_class_, _T(""), 0, 0, 0, 0, 0, NULL, NULL, h_instance, NULL);
}

tray_icon::~tray_icon()
{
	if (IsWindow(hidden_window_))
		DestroyWindow(hidden_window_);
	if (a_class_)
		UnregisterClass((LPCTSTR)a_class_, h_instance_);
}

void tray_icon::close()
{
	PostMessage(hidden_window_, WM_QUIT, 0, 0);
}

HWND tray_icon::GetWindow() const
{
	return hidden_window_;
}

