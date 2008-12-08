#pragma once

#include <wx/wx.h>

DECLARE_EVENT_TYPE(wxEVT_LOGIN_SUCCESS, -1)
DECLARE_EVENT_TYPE(wxEVT_LOGIN_FAILED, -2)

/**
 * Implements a login window whereby the user can
 * enter account information and have it remembered.
 */
class LoginPanel : public wxPanel
{
	wxTextCtrl username;
	wxTextCtrl password;
	wxButton loginButton;
	wxStaticText errorLabel;
	wxCheckBox rememberCheck;

	// For remembering login user, used by events
	wxString s_user;
	wxString s_pass;
	bool s_saveUserPass;
	bool loggedIn; 

	/// Control identifiers
	enum ControlIdentifiers {
		ID_USERNAME, ID_PASSWORD, ID_BUTTON
	};

	/** Initializes window components */
	void InitializeComponents();

public:
	/** 
	 * Creates the login panel.
	 * 
	 * @param parent the owning frame (most likely MainWindow)
	 * @param autoLogin if set to true, will attempt to automatically
	 *   login using any saved account settings
	 */
	LoginPanel(wxWindow *parent, bool autoLogin = false);

	/**
	 * Performs a verification of the login credentials
	 * using the account settings set in the text boxes. 
	 * Sets the #loggedIn status to true if the authorization succeeds.
	 * 
	 * @warning This code runs in a thread via a ThreadCallback and
	 *   should therefore not attempt to directly interact with the GUI.
	 */
	void Login();

	///
	/// @name Window Events
	///
	// @{

	/**
	 * Catch the paint event to draw the logo. This is done because
	 * wxStaticBitmap does not support images larger than 64x64. 
	 */
	void OnPaint(wxPaintEvent& evt);

	/**
	 * Activated when the login button is pressed. Initiates a 
	 * ThreadCallback to #Login and waits for #loggedIn to be filled.
	 * Times out after 10 seconds.
	 */
	void OnLogin(wxCommandEvent& evt);

	/** 
	 * Activated when Login succeeds. 
	 * Saves account settings if "remember me" is checked.
	 * Shows the main panel
	 */
	void OnLoginSuccess(wxCommandEvent& evt);

	/** 
	 * Displays an error message if authorization fails 
	 */
	void OnLoginFailed(wxCommandEvent& evt);

	/**
	 * Handle the text event in the password box so that the
	 * ENTER key will act as if the user clicked the login button.
	 */
	void OnText(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();

	// @}
};
