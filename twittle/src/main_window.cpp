#include "main_window.h"
#include "login_panel.h"
#include "feed_panel.h"
#include "main_panel.h"
#include "application.h"

#include "resources/icon.xpm"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_MENU(ID_LOGOUT, MainWindow::OnLogout)
	EVT_MENU(wxID_EXIT, MainWindow::OnExit)
	EVT_COMMAND(wxID_ANY, wxEVT_CLEAR_PANEL, MainWindow::OnClearPanel)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_CLEAR_PANEL);

MainWindow::MainWindow() : 
	wxFrame(NULL, wxID_ANY, _T("Twittle"), wxDefaultPosition, wxSize(320, 520)), panel(NULL)
{
	SetIcon(wxIcon(icon)); // show icon
	SetWindowStyle(GetWindowStyle() & ~wxMAXIMIZE_BOX); // no maximize ability

	// Set last known position
	long x = wxGetApp().GetSettings().GetLong(_T("window.x"));
	long y = wxGetApp().GetSettings().GetLong(_T("window.y"));
	if (x > 0 && y > 0) {
		SetPosition(wxPoint(x, y));
	}

	ShowLogin(true);
}

MainWindow::~MainWindow()
{
	long x, y;
	GetPosition((int*)&x, (int*)&y);
	wxGetApp().GetSettings().Set(_T("window.x"), x);
	wxGetApp().GetSettings().Set(_T("window.y"), y);
}

void MainWindow::SwapPanels(wxPanel *newPanel)
{
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
}

void MainWindow::ShowLogin(bool autoLogin)
{
	SwapPanels(new LoginPanel(this, autoLogin));
	SetMenuBar(LoginMenuBar());
}

void MainWindow::ShowMainPanel()
{
	SwapPanels(new MainPanel(this));
	SetMenuBar(MainMenuBar());
}

wxMenuBar *MainWindow::LoginMenuBar()
{
	wxMenuBar *menuBar = new wxMenuBar;

	wxMenu* file = new wxMenu;
	file->Append(wxID_EXIT, _T("&Quit"));

	menuBar->Append(file, _T("&File"));
    return menuBar;
}

wxMenuBar *MainWindow::MainMenuBar()
{
	wxMenuBar *menuBar = new wxMenuBar;

	wxMenu* file = new wxMenu;
	file->Append(ID_LOGOUT, _T("&Logout"));
	file->AppendSeparator();
	file->Append(wxID_EXIT, _T("&Quit"));

	menuBar->Append(file, _T("&File"));
    return menuBar;
}

void MainWindow::OnLogout(wxCommandEvent& evt)
{
	wxGetApp().Logout();
}

void MainWindow::OnExit(wxCommandEvent& evt)
{
	Close(true);
}