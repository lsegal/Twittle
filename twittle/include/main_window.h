#pragma once

#include <wx/wx.h>

class FeedPanel;

DECLARE_EVENT_TYPE(wxEVT_CLEAR_PANEL, -1)

class MainWindow : public wxFrame
{
	enum ControlIdentifiers {
		ID_LOGOUT = 1
	};

	wxPanel *panel;
	bool loggedIn;

	void SetTransparency();
	void SwapPanels(wxPanel *newPanel);
	wxMenuBar *LoginMenuBar();
	wxMenuBar *MainMenuBar();

public:
	MainWindow();
	virtual ~MainWindow();

	void ShowMainPanel();
	void ShowLogin(bool autoLogin = false);

	// Events
	void OnClearPanel(wxCommandEvent& evt);
	void OnOptions(wxCommandEvent& evt);
	void OnLogout(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};
