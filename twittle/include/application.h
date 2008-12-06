#pragma once

#include <wx/wx.h>
#include "main_window.h"
#include "settings.h"

class Twitter;

class Application : public wxApp
{
	MainWindow *main;
	Settings settings;
	Twitter *twitter;

public:
	static const wxString APPNAME;
	static const wxString APPVERSION;

	inline MainWindow& GetMainWindow() { return *main; }
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
