#pragma once

#include <wx/wx.h>
#include "main_window.h"

class Application : public wxApp
{
	MainWindow *main;
	Twitter twitter;

public:
	inline Twitter& GetTwitter() { return twitter; }

	bool OnInit();
	int OnExit();

	void Login(const wxString& user, const wxString& pass);
};

DECLARE_APP(Application)
