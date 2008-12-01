#pragma once

#include <wx/wx.h>
#include "settings.h"

class MainWindow;
class Twitter;

class Application : public wxApp
{
	MainWindow *main;
	Settings settings;
	Twitter *twitter;

public:
	inline Twitter& GetTwitter() { return *twitter; }
	inline Settings& GetSettings() { return settings; }

	bool OnInit();
	int OnExit();

	void Login(const wxString& user, const wxString& pass);
	void Logout();

	const wxString GetFeedsPath();
	const wxString GetImagesPath();
	const wxString GetSettingsPath();
};

DECLARE_APP(Application)
