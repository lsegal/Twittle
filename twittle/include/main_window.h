#pragma once

#include <wx/wx.h>

class FeedPanel;

DECLARE_EVENT_TYPE(wxEVT_CLEAR_PANEL, -1)

class MainWindow : public wxFrame
{
	wxPanel *panel;

	void SwapPanels(wxPanel *newPanel);

public:
	MainWindow();

	void ShowMainPanel();
	void ShowLogin();

	// Events
	void OnClearPanel(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};
