#pragma once

#include <wx/wx.h>
#include "main_window.h"
#include "settings.h"

class Twitter;

/**
 * The main application class. 
 */
class Application : public wxApp
{
	MainWindow *main;
	Settings settings;
	Twitter *twitter;

public:
	/** The application name */
	static const wxString APPNAME;

	/** The application version */
	static const wxString APPVERSION;

	/**
	 * Opens a URL in the default browser. Uses default browser
	 * settings (Firefox can be set to open in a new tab)
	 *
	 * @param url the URL to open in the browser 
	 */
	static void OpenUrl(const wxString& url);

	/** Returns the main window frame */
	inline MainWindow& GetMainWindow() { return *main; }

	/** Returns the Twitter client controlling the connection */
	inline Twitter& GetTwitter() { return *twitter; }

	/** Returns the application settings */
	inline Settings& GetSettings() { return settings; }

	/** 
	 * Initialization code.
	 */
	bool OnInit();

	/** 
	 * Called when the application exits.
	 * Cleans up twitter objects and deserializes all
	 * settings and data.
	 */
	int OnExit();

	/**
	 * Sets the auth info for the twitter account and shows the main
	 * panel. Does not do any verification that the user/password
	 * is valid; use Twitter::VerifyCredentials
	 *
	 * @see Twitter::VerifyCredentials
	 * @pre login credentials have been verified
	 * @post main window is shown
	 * @param user the username 
	 * @param pass the Twitter account password
	 */
	void Login(const wxString& user, const wxString& pass);

	/** 
	 * Logs out of the Twitter account by showing the login panel.
	 * Does not actually talk to the Twitter server to formally "log out".
	 *
	 * @pre user is logged in (main window is shown)
	 * @post login window is shown
	 */
	void Logout();

	/**
	 * Returns the path on disk to store all feed related data for the logged in user.
	 *
	 * @pre the Twitter login credentials must be set (via #Login or Twitter::SetAuth)
	 */
	const wxString GetFeedsPath();

	/**
	 * Returns the path on disk to store all image related data for profile images
	 */
	const wxString GetImagesPath();

	/**
	 * Returns the path on disk where the settings.xml file is stored
	 */
	const wxString GetSettingsPath();
};

DECLARE_APP(Application)
