#pragma once

#include <wx/wx.h>

DECLARE_EVENT_TYPE(wxEVT_LOGIN_SUCCESS, -1)
DECLARE_EVENT_TYPE(wxEVT_LOGIN_FAILED, -2)

class LoginPanel : public wxPanel
{
	wxTextCtrl username;
	wxTextCtrl password;
	wxButton loginButton;
	wxStaticText errorLabel;
	wxCheckBox rememberCheck;

	enum ControlIdentifiers {
		ID_USERNAME, ID_PASSWORD, ID_BUTTON
	};

	void InitializeComponents();

public:
	LoginPanel(wxWindow *parent, bool autoLogin = false);

	void Login();

	// events
	void OnPaint(wxPaintEvent& evt);
	void OnLogin(wxCommandEvent& evt);
	void OnLoginSuccess(wxCommandEvent& evt);
	void OnLoginFailed(wxCommandEvent& evt);
	void OnText(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};
