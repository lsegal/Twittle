#include "main_window.h"
#include "login_panel.h"
#include "feed_panel.h"
#include "main_panel.h"
#include "application.h"
#include "options_dialog.h"

#include "resources/icon.xpm"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_MENU(ID_OPTIONS, MainWindow::OnOptions)
	EVT_MENU(ID_LOGOUT, MainWindow::OnLogout)
	EVT_MENU(wxID_EXIT, MainWindow::OnExit)
	EVT_COMMAND(wxID_ANY, wxEVT_CLEAR_PANEL, MainWindow::OnClearPanel)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_CLEAR_PANEL);

MainWindow::MainWindow() : 
	wxFrame(NULL, wxID_ANY, _T("Twittle"), wxDefaultPosition, wxSize(320, 540)), 
		panel(NULL), loggedIn(false)
{
	SetIcon(wxIcon(icon)); // show icon
	SetTransparency(); // set transparency

	// Set last known position
	long x = wxGetApp().GetSettings().GetLong(_T("window.x"));
	long y = wxGetApp().GetSettings().GetLong(_T("window.y"));
	if (x > 0 && y > 0) {
		SetPosition(wxPoint(x, y));
	}
	else {
		Center();
	}

	ShowLogin(wxGetApp().GetSettings().GetBool(_T("account.autologin")));
}

MainWindow::~MainWindow()
{
	long x, y;
	GetPosition((int*)&x, (int*)&y);
	wxGetApp().GetSettings().Set(_T("window.x"), x);
	wxGetApp().GetSettings().Set(_T("window.y"), y);
}

void MainWindow::SetTransparency()
{
	SetTransparent(wxGetApp().GetSettings().GetLong(_T("window.transparency")));
}

void MainWindow::SwapPanels(wxPanel *newPanel)
{
	Freeze();
	wxCommandEvent event(wxEVT_CLEAR_PANEL, wxID_ANY);
	event.SetEventObject(panel);
	event.SetClientData(newPanel);
	wxPostEvent(this, event);
}

void MainWindow::OnClearPanel(wxCommandEvent& evt)
{
	if (evt.GetEventObject()) {
		static_cast<wxWindow*>(evt.GetEventObject())->Destroy();
	}
	panel = static_cast<wxPanel*>(evt.GetClientData());

	// Create a sizer for new panel
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(panel, wxSizerFlags(1).Expand());
	SetSizer(sizer);

	// Propagate the size hints from the main panel
	SetSizeHints(panel->GetMinSize(), panel->GetMaxSize());
	SetClientSize(panel->GetSize());
	Thaw();
}

void MainWindow::ShowLogin(bool autoLogin)
{
	SwapPanels(new LoginPanel(this, autoLogin));
	SetMenuBar(LoginMenuBar());
	loggedIn = false;
}

void MainWindow::ShowMainPanel()
{
	SwapPanels(new MainPanel(this));
	SetMenuBar(MainMenuBar());
	loggedIn = true;
}

wxMenuBar *MainWindow::LoginMenuBar()
{
	wxMenuBar *menuBar = new wxMenuBar;

	wxMenu* file = new wxMenu;
	file->Append(ID_OPTIONS, _T("&Options"));
	file->AppendSeparator();
	file->Append(wxID_EXIT, _T("&Quit"));

	menuBar->Append(file, _T("&File"));
    return menuBar;
}

wxMenuBar *MainWindow::MainMenuBar()
{
	wxMenuBar *menuBar = new wxMenuBar;

	wxMenu* file = new wxMenu;
	file->Append(ID_OPTIONS, _T("&Options"));
	file->Append(ID_LOGOUT, _T("&Logout"));
	file->AppendSeparator();
	file->Append(wxID_EXIT, _T("&Quit"));

	menuBar->Append(file, _T("&File"));
    return menuBar;
}

void MainWindow::OnOptions(wxCommandEvent& evt)
{
	OptionsDialog options(this);
	if (options.ShowModal() == wxID_OK) {
		// save settings
		wxGetApp().GetSettings().Merge(options.GetSettings());
		wxGetApp().GetSettings().Save();

		// update ui
		if (loggedIn) {
			MainPanel *main = static_cast<MainPanel*>(panel);
			main->SetPanelOrder();
			main->ForceUpdateUI();
		}
	}
	else {
		// reset transparency
		SetTransparency();
	}
}

void MainWindow::OnLogout(wxCommandEvent& evt)
{
	wxGetApp().Logout();
}

void MainWindow::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
