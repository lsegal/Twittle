#include "login_panel.h"
#include "application.h"
#include "twitter/twitter.h"
#include "thread_callback.h"

BEGIN_EVENT_TABLE(LoginPanel, wxPanel)
	EVT_TEXT(ID_USERNAME, LoginPanel::OnText)
	EVT_TEXT(ID_PASSWORD, LoginPanel::OnText)
	EVT_BUTTON(ID_BUTTON, LoginPanel::OnLogin)
	EVT_TEXT(ID_BUTTON, LoginPanel::OnLogin)
	EVT_TEXT_ENTER(ID_PASSWORD, LoginPanel::OnLogin)

	EVT_COMMAND(wxID_ANY, wxEVT_LOGIN_SUCCESS, LoginPanel::OnLoginSuccess)
	EVT_COMMAND(wxID_ANY, wxEVT_LOGIN_FAILED, LoginPanel::OnLoginFailed)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_LOGIN_SUCCESS)
DEFINE_EVENT_TYPE(wxEVT_LOGIN_FAILED)

LoginPanel::LoginPanel(wxWindow *parent) : wxPanel(parent)
{
	InitializeComponents();
	wxCommandEvent c;
	OnText(c);

	// Give size hints for min and max
	wxSize s = GetSize();
	SetMinSize(wxSize(s.GetWidth(), s.GetHeight() + 100));
	SetMaxSize(wxSize(400, 600));
}

void LoginPanel::InitializeComponents()
{
	wxStaticText *uLabel = new wxStaticText(this, wxID_ANY, _T("Username"));
	wxStaticText *pLabel = new wxStaticText(this, wxID_ANY, _T("Password"));
	username.Create(this, ID_USERNAME, _T("twittletest"));
	password.Create(this, ID_PASSWORD, _T("twittle"),
		wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER);
	loginButton.Create(this, ID_BUTTON, _T("Login"));
	errorLabel.Create(this, wxID_ANY, wxEmptyString);

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

	SetSizerAndFit(panelSizer);
}

static wxString s_user;
static wxString s_pass;

void LoginPanel::Login()
{
	wxCommandEvent evt(wxEVT_LOGIN_FAILED, wxID_ANY);

	if (wxGetApp().GetTwitter().VerifyCredentials(s_user, s_pass)) {
		evt.SetEventType(wxEVT_LOGIN_SUCCESS);
	}

	wxPostEvent(this, evt);
}

void LoginPanel::OnLoginSuccess(wxCommandEvent& evt)
{
	errorLabel.SetLabel(_T(""));
	wxGetApp().Login(s_user, s_pass);
}

void LoginPanel::OnLoginFailed(wxCommandEvent& evt)
{
	errorLabel.SetForegroundColour(*wxRED);
	errorLabel.SetLabel(_T("Invalid login."));
	wxSleep(1);
}

void LoginPanel::OnLogin(wxCommandEvent& evt)
{
	errorLabel.SetForegroundColour(*wxBLACK);
	errorLabel.SetLabel(_T("Logging in..."));

	s_user = username.GetValue();
	s_pass = password.GetValue();
	ThreadCallback<LoginPanel> cb(*this, &LoginPanel::Login);

	time_t timeNow = time(0);
	while (cb.IsAlive()) {
		wxYield();

		if (time(0) > timeNow + 10) {
			cb.Kill();
			errorLabel.SetForegroundColour(*wxRED);
			errorLabel.SetLabel(_T("Error: login timeout."));
		}
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

