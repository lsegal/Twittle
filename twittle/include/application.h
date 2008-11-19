#pragma once

#include <wx/wx.h>

class MainWindow;
class Twitter;

class Application : public wxApp
{
	MainWindow *main;
	Twitter *twitter;

public:
	inline Twitter& GetTwitter() { return *twitter; }

	bool OnInit();
	int OnExit();

	void Login(const wxString& user, const wxString& pass);
};

DECLARE_APP(Application)
