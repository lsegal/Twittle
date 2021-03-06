#include "login_panel.h"
#include "application.h"
#include "settings.h"
#include "twitter/twitter.h"
#include "thread_callback.h"

#include "resources/logo.xpm"

BEGIN_EVENT_TABLE(LoginPanel, wxPanel)
	EVT_TEXT(ID_USERNAME, LoginPanel::OnText)
	EVT_TEXT(ID_PASSWORD, LoginPanel::OnText)
	EVT_BUTTON(ID_BUTTON, LoginPanel::OnLogin)
	EVT_TEXT(ID_BUTTON, LoginPanel::OnLogin)
	EVT_TEXT_ENTER(ID_PASSWORD, LoginPanel::OnLogin)

	EVT_COMMAND(wxID_ANY, wxEVT_LOGIN_SUCCESS, LoginPanel::OnLoginSuccess)
	EVT_COMMAND(wxID_ANY, wxEVT_LOGIN_FAILED, LoginPanel::OnLoginFailed)
	EVT_PAINT(LoginPanel::OnPaint)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_LOGIN_SUCCESS)
DEFINE_EVENT_TYPE(wxEVT_LOGIN_FAILED)

LoginPanel::LoginPanel(wxWindow *parent, bool autoLogin) : wxPanel(parent)
{
	InitializeComponents();
	wxCommandEvent c;
	OnText(c);

	// no maximize ability
	parent->SetWindowStyle(parent->GetWindowStyle() & ~wxMAXIMIZE_BOX);

	// Give size hints for min and max
	int w, h;
	parent->GetSize(&w, &h);
	SetMinSize(wxSize(320, 540));
	SetMaxSize(wxSize(400, 600));
	SetSize(GetMinSize());

	// login if we have auto login set
	if (autoLogin && wxGetApp().GetSettings().GetBool(_T("account.remember"))) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, ID_BUTTON);
		wxPostEvent(this, evt);
	}
}

void LoginPanel::InitializeComponents()
{
	wxString user = wxGetApp().GetSettings().GetString(_T("account.username"));
	wxString pass = wxGetApp().GetSettings().GetString(_T("account.password"));
	bool remember = wxGetApp().GetSettings().GetBool(_T("account.remember"));

	wxStaticText *uLabel = new wxStaticText(this, wxID_ANY, _T("Username"));
	wxStaticText *pLabel = new wxStaticText(this, wxID_ANY, _T("Password"));
	username.Create(this, ID_USERNAME, user);
	password.Create(this, ID_PASSWORD, pass,
		wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER);
	loginButton.Create(this, ID_BUTTON, _T("Login"));
	errorLabel.Create(this, wxID_ANY, wxEmptyString);
	rememberCheck.Create(this, wxID_ANY, _T("Remember Password"));
	rememberCheck.SetValue(remember);

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
	vSizer->Add(&rememberCheck, wxSizerFlags().Right());
	vSizer->AddSpacer(10);
	vSizer->Add(&loginButton, wxSizerFlags().Right());
	vSizer->AddSpacer(10);
	vSizer->Add(&errorLabel);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->AddSpacer(270);
	panelSizer->Add(vSizer, wxSizerFlags().Bottom().Center().Border(wxALL, 40));

	SetSizer(panelSizer);
}

static wxBitmap logoBitmap(logo);
void LoginPanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	wxRegionIterator upd = GetUpdateRegion();

	// iterate over all regions that need updating
	wxSize s = GetParent()->GetClientSize();
	// try to draw image in center
	wxRect logoRect((s.GetWidth() - 256) / 2, 20, 256, 256);
	while (upd) {
		wxRect rect = upd.GetRect();
		dc.Clear();
		if (logoRect.Intersects(rect)) {
			// logo region, redraw logo.
			dc.DrawBitmap(logoBitmap, logoRect.GetLeft(), logoRect.GetTop(), true);
		}
		++upd;
	}
}

void LoginPanel::Login()
{
	loggedIn = wxGetApp().GetTwitter().VerifyCredentials(s_user, s_pass);
}

void LoginPanel::OnLoginSuccess(wxCommandEvent& evt)
{
	// If we're told to save the user/pass, set them in the settings
	if (s_saveUserPass) {
		wxGetApp().GetSettings().Set(_T("account.username"), s_user);
		wxGetApp().GetSettings().Set(_T("account.password"), s_pass);
	}
	else {
		wxGetApp().GetSettings().Set(_T("account.username"), (const wxString&)_T(""));
		wxGetApp().GetSettings().Set(_T("account.password"), (const wxString&)_T(""));
	}
	wxGetApp().GetSettings().Set(_T("account.remember"), s_saveUserPass);

	// Do login
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
	username.Disable();
	password.Disable();
	loginButton.Disable();
	rememberCheck.Disable();
	errorLabel.SetForegroundColour(*wxBLACK);
	errorLabel.SetLabel(_T("Logging in..."));

	s_user = username.GetValue();
	s_pass = password.GetValue();
	s_saveUserPass = rememberCheck.GetValue();
	ThreadCallback<LoginPanel> cb(*this, &LoginPanel::Login);

	time_t timeNow = time(0);
	wxBeginBusyCursor();
	while (cb.IsAlive()) {
		wxYield();

		if (time(0) > timeNow + 10) {
			cb.Kill();
			errorLabel.SetForegroundColour(*wxRED);
			errorLabel.SetLabel(_T("Error: login timeout."));
		}
	}
	wxEndBusyCursor();
	cb.Wait();

	// do stuff
	wxCommandEvent cEvt;
	if (loggedIn) {
		OnLoginSuccess(cEvt);
	}
	else {
		OnLoginFailed(cEvt);
		username.Enable();
		password.Enable();
		rememberCheck.Enable();
		loginButton.Enable();
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

