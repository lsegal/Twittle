#include "login_panel.h"
#include "application.h"

BEGIN_EVENT_TABLE(LoginPanel, wxPanel)
	EVT_TEXT(ID_USERNAME, LoginPanel::OnText)
	EVT_TEXT(ID_PASSWORD, LoginPanel::OnText)
	EVT_BUTTON(ID_BUTTON, LoginPanel::OnLogin)
	EVT_TEXT(ID_BUTTON, LoginPanel::OnLogin)
	EVT_TEXT_ENTER(ID_PASSWORD, LoginPanel::OnLogin)
END_EVENT_TABLE()

LoginPanel::LoginPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
{
	InitializeComponents();
	OnText(wxCommandEvent());
}

void LoginPanel::InitializeComponents()
{
	wxStaticText *uLabel = new wxStaticText(this, wxID_ANY, _T("Username"));
	wxStaticText *pLabel = new wxStaticText(this, wxID_ANY, _T("Password"));
	username.Create(this, ID_USERNAME);
	password.Create(this, ID_PASSWORD, wxEmptyString, 
		wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER);
	loginButton.Create(this, ID_BUTTON, _T("Login"));
	errorLabel.Create(this, wxID_ANY, wxEmptyString);
	errorLabel.SetForegroundColour(*wxRED);

	wxBoxSizer *uSizer = new wxBoxSizer(wxHORIZONTAL);
	uSizer->Add(uLabel, wxSizerFlags().Border(wxTOP, 5));
	uSizer->AddSpacer(20);
	uSizer->Add(&username);

	wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
	pSizer->Add(pLabel, wxSizerFlags().Border(wxTOP, 5));
	pSizer->AddSpacer(20);
	pSizer->Add(&password);

	wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
	vSizer->Add(uSizer);
	vSizer->AddSpacer(10);
	vSizer->Add(pSizer);
	vSizer->AddSpacer(10);
	vSizer->Add(&loginButton, wxSizerFlags().Right());
	vSizer->AddSpacer(10);
	vSizer->Add(&errorLabel);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->AddStretchSpacer(500);
	panelSizer->Add(vSizer, wxSizerFlags().Bottom().Center().Border(wxALL, 40));

	SetSizer(panelSizer);
}

void LoginPanel::OnLogin(wxCommandEvent& evt)
{
	const wxString& user = username.GetValue();
	const wxString& pass = password.GetValue();

	if (wxGetApp().GetTwitter().VerifyCredentials(user, pass)) {
		errorLabel.SetLabel(wxEmptyString);

		wxGetApp().Login(user, pass);
	}
	else {
		errorLabel.SetLabel(_T("Invalid login."));
	}
}

void LoginPanel::OnText(wxCommandEvent& evt)
{
	if (!username.GetValue() || !password.GetValue()) {
		loginButton.Disable();
	}
	else {
		loginButton.Enable();
	}
}

