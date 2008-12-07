#pragma once

#include <wx/taskbar.h>

class TrayIcon : public wxTaskBarIcon
{
public:
	wxMenu* CreatePopupMenu();

	// events
	void OnDoubleClick(wxTaskBarIconEvent& evt);
	void OnMenu(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};
