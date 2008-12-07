#include "main_window.h"
#include "login_panel.h"
#include "feed_panel.h"
#include "main_panel.h"
#include "application.h"
#include "options_dialog.h"
#include "about_dialog.h"

#ifdef __WXMSW__
#	include <strsafe.h>
#	include <shellapi.h>
#endif

#include "resources/icon.xpm"
static wxIcon appIcon(icon);

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_MENU(wxID_PREFERENCES, MainWindow::OnOptions)
	EVT_MENU(ID_LOGOUT, MainWindow::OnLogout)
	EVT_MENU(wxID_EXIT, MainWindow::OnExit)
	EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
	EVT_COMMAND(wxID_ANY, wxEVT_CLEAR_PANEL, MainWindow::OnClearPanel)
	EVT_ICONIZE(MainWindow::OnIconize)
	EVT_ACTIVATE(MainWindow::OnActivate)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_CLEAR_PANEL);

MainWindow::MainWindow(bool showInTaskbar) : 
	wxFrame(NULL, wxID_ANY, wxGetApp().APPNAME, wxDefaultPosition, wxSize(320, 540), 
		showInTaskbar ? wxDEFAULT_FRAME_STYLE : wxDEFAULT_FRAME_STYLE | wxFRAME_NO_TASKBAR),
	panel(NULL), loggedIn(false)
{
	wxGetApp().SetTopWindow(this); // set top level window immediately
	SetIcon(appIcon); // show icon
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
	if (IsActive()) {
		SetTransparent(255); // full opacity when app is active
	}
	else {
		// use transparency settings
		SetTransparent(wxGetApp().GetSettings().GetLong(_T("window.transparency")));
	}
}

void MainWindow::OnActivate(wxActivateEvent& evt)
{
	if (!IsActive()) {
		SetTransparent(255); // full opacity when app is active
	}
	else {
		// use transparency settings
		SetTransparent(wxGetApp().GetSettings().GetLong(_T("window.transparency")));
	}
}

void MainWindow::SetTrayIcon()
{
	bool showInTray = wxGetApp().GetSettings().GetBool(_T("window.showintray"));
	bool minimizeToTray = wxGetApp().GetSettings().GetBool(_T("window.minimizetotray"));
	if (showInTray || (IsIconized() && minimizeToTray)) {
		wxString tooltip = wxGetApp().APPNAME;
		if (loggedIn) {
			tooltip.Append(_T(" (logged in as ") + wxGetApp().GetTwitter().GetUsername() + _T(")"));
		}

		trayIcon.SetIcon(appIcon, tooltip);
	}
	else if (trayIcon.IsIconInstalled()) {
		trayIcon.RemoveIcon();
	}
}

// HACK to get the icon handle from the TaskBarIconWindow 
#ifdef __WXMSW__

static WXHWND trayIconHWND = NULL;

static BOOL CALLBACK FindTrayIconWindow(HWND hwnd, LPARAM param)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	if (GetWindowTextLength(hwnd) == 0 &&
			rect.right == 400 && rect.bottom == 250 && 
			rect.top == 0 && rect.left == 0) {
		trayIconHWND = hwnd;
		return FALSE;
	}
	return TRUE;
}

static void GetTrayIconHWND()
{
	EnumWindows(FindTrayIconWindow, 0);
}
#endif

// Shows a Tray notification (bubble)
// Currently implemented in Win32 only.
// wxWidgets 2.9 has this functionality,
// but it is not yet released 
void MainWindow::TrayNotification(const wxString& text, const wxString& title, const wxIcon* cIcon, int delay)
{
	// return if the window is active
	if (IsActive()) return;

#ifdef __WXMSW__ // win32 only
	GetTrayIconHWND();
	NOTIFYICONDATA data;

	if (text.length() > 128) {
		wxString trunc = text.SubString(0, 123) + _T("...");
		StringCchCopy(data.szInfo, 128, trunc.data());
	}
	else {
		StringCchCopy(data.szInfo, 128, text.data());
	}

	StringCchCopy(data.szInfoTitle, 64, title.data());

	data.cbSize = sizeof(NOTIFYICONDATA);
	data.hWnd = (HWND)trayIconHWND;
	data.uID = 99;
	data.uFlags = NIF_INFO;
	data.uCallbackMessage = 0;
	data.hIcon = NULL;
	data.uVersion = NOTIFYICON_VERSION;
	data.uTimeout = delay * 1000;
	data.dwInfoFlags = cIcon ? NIIF_USER : 0;
#if (NTDDI_VERSION >= NTDDI_VISTA)
	data.hBalloonIcon = cIcon ? (HICON)cIcon->GetHICON() : NULL;
#endif
	HRESULT result = Shell_NotifyIcon(NIM_MODIFY, &data);
#endif
}

void MainWindow::OnIconize(wxIconizeEvent& evt)
{
	bool minimizeToTray = wxGetApp().GetSettings().GetBool(_T("window.minimizetotray"));
	bool showInTaskbar = wxGetApp().GetSettings().GetBool(_T("window.showintaskbar"));
	if (minimizeToTray || showInTaskbar) {
		Hide();
		SetTrayIcon();
	}
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

	SetTrayIcon();
}

void MainWindow::ShowLogin(bool autoLogin)
{
	loggedIn = false;
	SwapPanels(new LoginPanel(this, autoLogin));
	SetMenuBar(LoginMenuBar());
}

void MainWindow::ShowMainPanel()
{
	loggedIn = true;
	SwapPanels(new MainPanel(this));
	SetMenuBar(MainMenuBar());
}

wxMenuBar *MainWindow::LoginMenuBar()
{
	wxMenuBar *menuBar = new wxMenuBar;

	wxMenu* file = new wxMenu;
	file->Append(wxID_PREFERENCES, _T("&Options...\tCtrl+O"));
	file->AppendSeparator();
	file->Append(wxID_EXIT, _T("E&xit\tCtrl+Q"));

	wxMenu *help = new wxMenu;
	help->Append(wxID_ABOUT, _T("About..."));

	menuBar->Append(file, _T("&File"));
	menuBar->Append(help, _T("&Help"));
	return menuBar;
}

wxMenuBar *MainWindow::MainMenuBar()
{
	wxMenuBar *menuBar = new wxMenuBar;

	wxMenu* file = new wxMenu;
	file->Append(wxID_PREFERENCES, _T("&Options...\tCtrl+O"));
	file->Append(ID_LOGOUT, _T("&Logout"));
	file->AppendSeparator();
	file->Append(wxID_EXIT, _T("E&xit\tCtrl+Q"));

	wxMenu *help = new wxMenu;
	help->Append(wxID_ABOUT, _T("About..."));

	menuBar->Append(file, _T("&File"));
	menuBar->Append(help, _T("&Help"));
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
		SetTrayIcon();
	}

	// update transparency
	SetTransparency();
}

void MainWindow::OnLogout(wxCommandEvent& evt)
{
	wxGetApp().Logout();
}

void MainWindow::OnAbout(wxCommandEvent& evt)
{
	AboutDialog(this).ShowModal();
}

void MainWindow::OnExit(wxCommandEvent& evt)
{
	Close(true);
}
