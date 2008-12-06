#include "options_dialog.h"
#include "application.h"

BEGIN_EVENT_TABLE(OptionsDialog, wxDialog)
	EVT_SCROLL(OptionsDialog::TransparencyChanged)
END_EVENT_TABLE()

OptionsDialog::OptionsDialog(wxWindow *parent) :
		wxDialog(parent, wxID_ANY, wxGetApp().APPNAME + _T(" Options"), wxDefaultPosition, wxSize(375, 350)),
		settings(wxGetApp().GetSettings())
{
	wxScrollEvent evt;
	InitializeComponents();
	InitializeValues();
	TransparencyChanged(evt);
}

void OptionsDialog::InitializeComponents()
{
	wxString editLabels[] = { _T("Top"), _T("Bottom") };
	autoLogin.Create(this, wxID_ANY, _T("Auto-login using saved account settings"));
	showScreenNamePrefix.Create(this, wxID_ANY, _T("Show screen name prefix in Twitter statuses"));
	minimizeToTray.Create(this, wxID_ANY, _T("Minimize application icon to tray"));
	showInTray.Create(this, wxID_ANY, _T("Always show application icon in tray"));
	showInTaskbar.Create(this, wxID_ANY, _T("Hide window from taskbar (requires restart)"));
	showNotifications.Create(this, wxID_ANY, _T("Show tray notifications on feed updates"));
	transparency.Create(this, wxID_ANY, 0, 50, 255);
	transLabel.Create(this, wxID_ANY, _T(""));
	editPosition.Create(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 2, editLabels, wxCB_READONLY);

	wxSizer *transSizer = new wxBoxSizer(wxHORIZONTAL);
	transSizer->Add(new wxStaticText(this, wxID_ANY, _T("Window Transparency")));
	transSizer->Add(&transparency, wxSizerFlags().Border(wxLEFT, 5));
	transSizer->Add(&transLabel, wxSizerFlags().Border(wxLEFT, 5));

	wxSizer *editPositionSizer = new wxBoxSizer(wxHORIZONTAL);
	editPositionSizer->Add(new wxStaticText(this, wxID_ANY, _T("Show editbox on")), wxSizerFlags().Center());
	editPositionSizer->AddSpacer(5);
	editPositionSizer->Add(&editPosition, wxSizerFlags().Center());

	wxSizer *buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->Add(&autoLogin, wxSizerFlags().Border(wxTOP, 10));
	panelSizer->AddSpacer(10);
	panelSizer->Add(&showScreenNamePrefix, wxSizerFlags().Border(wxTOP, 10));
	panelSizer->Add(editPositionSizer, wxSizerFlags().Border(wxTOP, 10));
	panelSizer->AddSpacer(10);
	panelSizer->Add(&minimizeToTray, wxSizerFlags().Border(wxTOP, 10));
	panelSizer->Add(&showInTray, wxSizerFlags().Border(wxTOP, 10));
	panelSizer->Add(&showInTaskbar, wxSizerFlags().Border(wxTOP, 10));
#ifdef __WXMSW__
	panelSizer->Add(&showNotifications, wxSizerFlags().Border(wxTOP, 10));
#endif
	panelSizer->AddSpacer(10);
	panelSizer->Add(transSizer, wxSizerFlags().Border(wxTOP, 8));
	panelSizer->AddStretchSpacer();
	if (buttonSizer) panelSizer->Add(buttonSizer, wxSizerFlags(0).Right());

	wxSizer *borderedSizer = new wxBoxSizer(wxVERTICAL);
	borderedSizer->Add(panelSizer, wxSizerFlags(1).Expand().Border(wxALL, 20));

	SetSizer(borderedSizer);
	Center();
}

void OptionsDialog::InitializeValues()
{
	autoLogin.SetValue(settings.GetBool(_T("account.autologin")));
	showScreenNamePrefix.SetValue(settings.GetBool(_T("feedpanel.showscreenname")));
	transparency.SetValue(settings.GetLong(_T("window.transparency")));
	minimizeToTray.SetValue(settings.GetBool(_T("window.minimizetotray")));
	showInTray.SetValue(settings.GetBool(_T("window.showintray")));
	showInTaskbar.SetValue(!settings.GetBool(_T("window.showintaskbar")));
	showNotifications.SetValue(settings.GetBool(_T("window.shownotifications")));

	wxString editPos = settings.GetString(_T("feedpanel.editposition"));
	if (editPos == _T("top")) editPosition.SetStringSelection(_T("Top"));
	else editPosition.SetStringSelection(_T("Bottom"));
}

void OptionsDialog::TransparencyChanged(wxScrollEvent& evt)
{
	transLabel.SetLabel(wxString::Format(_T("%.0f%%"),
		(double)transparency.GetValue() / transparency.GetMax() * 100));

	wxGetApp().GetMainWindow().SetTransparent(transparency.GetValue());
}

/**
 * Moves settings from controls to settings object
 */
bool OptionsDialog::TransferDataFromWindow()
{
	// Pull settings out of controls
	settings.Set(_T("account.autologin"), autoLogin.GetValue());
	settings.Set(_T("feedpanel.showscreenname"), showScreenNamePrefix.GetValue());
	settings.Set(_T("window.transparency"), (long)transparency.GetValue());
	settings.Set(_T("feedpanel.editposition"), editPosition.GetStringSelection().MakeLower());
	settings.Set(_T("window.showintray"), showInTray.GetValue());
	settings.Set(_T("window.showintaskbar"), !showInTaskbar.GetValue());
	settings.Set(_T("window.minimizetotray"), minimizeToTray.GetValue());
	settings.Set(_T("window.shownotifications"), showNotifications.GetValue());

	return true;
}
