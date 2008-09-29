#pragma once

#include <wx/wx.h>

class LoginPanel : public wxPanel
{
	wxTextCtrl username;
	wxTextCtrl password;
	wxButton loginButton;
	wxStaticText errorLabel;

	enum ControlIdentifiers {
		ID_USERNAME, ID_PASSWORD, ID_BUTTON
	};

	void InitializeComponents();

public:
	LoginPanel(wxWindow *parent);

	// events
	void OnLogin(wxCommandEvent& evt);
	void OnText(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};
