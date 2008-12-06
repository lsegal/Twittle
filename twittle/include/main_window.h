#pragma once

#include <wx/wx.h>
#include "tray_icon.h"

class FeedPanel;

DECLARE_EVENT_TYPE(wxEVT_CLEAR_PANEL, -1)

class MainWindow : public wxFrame
{
	enum ControlIdentifiers {
		ID_LOGOUT = 1
	};

	wxPanel *panel;
	TrayIcon trayIcon;
	bool loggedIn;

	void SetTransparency();
	void SetTrayIcon();
	void SwapPanels(wxPanel *newPanel);
	wxMenuBar *LoginMenuBar();
	wxMenuBar *MainMenuBar();

public:
	MainWindow(bool showInTaskbar = true);
	virtual ~MainWindow();

	void ShowMainPanel();
	void ShowLogin(bool autoLogin = false);

	// Events
	void OnIconize(wxIconizeEvent& evt);
	void OnClearPanel(wxCommandEvent& evt);
	void OnOptions(wxCommandEvent& evt);
	void OnLogout(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	void TrayNotification(const wxString& text, int delay = 5);

	DECLARE_EVENT_TABLE();

	friend class TrayIcon;
};
