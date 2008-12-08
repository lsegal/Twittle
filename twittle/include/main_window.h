#pragma once

#include <wx/wx.h>
#include "tray_icon.h"

class FeedPanel;

DECLARE_EVENT_TYPE(wxEVT_CLEAR_PANEL, -1)

/**
 * Represents the main window of the application. Called only once by Application.
 * By design, this is the only modeless window shown in the application.
 * All other views are implemented as panels which are inserted into this
 * frame using the #SwapPanels method, which inserts the panel and resizes the
 * window appropriately to the panel's preferred size.
 */
class MainWindow : public wxFrame
{
	enum ControlIdentifiers {
		ID_LOGOUT = 1
	};

	wxPanel *panel;
	TrayIcon trayIcon;
	bool loggedIn;

	/** Sets the on top settings for the window */
	void SetOnTop();

	/** Sets the transparency settings for the window */
	void SetTransparency();

	/** Sets the tray icon settings for the window */
	void SetTrayIcon();

	/** Replaces the inner contents of the frame with a new panel */
	void SwapPanels(wxPanel *newPanel);
	
	/** Creates a menubar to be inserted when the login panel is shown */
	wxMenuBar *LoginMenuBar();

	/** Creates a menubar to be inserted when the main panel is shown */
	wxMenuBar *MainMenuBar();

public:
	/**
	 * Creates the window frame with position/size stored in settings.
	 *
	 * @param showInTaskbar if set to false, will hide window from the taskbar.
	 *   this can only be done at construction.
	 */
	MainWindow(bool showInTaskbar = true);

	/**
	 * Stores the window position and size before closing.
	 */
	virtual ~MainWindow();

	/** Shows the main panel (after logging in). Usually only called by Application */
	void ShowMainPanel();

	/** 
	 * Shows the login panel. Usually only called by Application.
	 *
	 * @param autoLogin whether or not to automatically attempt to 
	 *   login with stored account settings.
	 */
	void ShowLogin(bool autoLogin = false);

	/**
	 * Shows a Tray notification (bubble); Currently implemented in Win32 only.
	 * wxWidgets 2.9 has this functionality, but it is not yet released.
	 *
	 * @param text the notification text
	 * @param title the title of the notification
	 * @param icon show an icon next to the notification (currently unused)
	 * @param delay how long (in seconds) to show the notification
	 */
	void TrayNotification(const wxString& text, const wxString& title = _T("Twittle"), const wxIcon* icon = NULL, int delay = 5);

	///
	/// @name Window Events
	///
	//@{

	/**
	 * Handled when the window is activated or deactivated. 
	 * This usually matters when transparency settings are set to only
	 * apply when the window is inactive.
	 */
	void OnActivate(wxActivateEvent& evt);

	/** 
	 * Handles a minimize event. If settings are set to minimize to tray,
	 * the window will be hidden here.
	 */
	void OnIconize(wxIconizeEvent& evt);

	/**
	 * Handles the custom wxEVT_CLEAR_PANEL event to clear contents of 
	 * a panel. This event is posted from #SwapPanels.
	 *
	 * @see #SwapPanels
	 */
	void OnClearPanel(wxCommandEvent& evt);

	/**
	 * Handles the clicking of the options menu item (or accelerator Ctrl+O key, 
	 * Command+Comma on OSX). Shows the OptionsDialog and saves/updates application
	 * settings if the dialog is successfully closed.
	 */
	void OnOptions(wxCommandEvent& evt);

	/**
	 * Handles clicking of the logout menu item. Logs the user out and
	 * shows the login panel
	 */
	void OnLogout(wxCommandEvent& evt);

	/**
	 * Handles clicking of the about menu. Shows the AboutDialog.
	 */
	void OnAbout(wxCommandEvent& evt);

	/**
	 * Handles clicking of the Exit menu item (or Ctrl+Q/Command+Q key combo).
	 * Closes the main window which causes the application to exit.
	 */
	void OnExit(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();

	//@}

	friend class TrayIcon;
};
