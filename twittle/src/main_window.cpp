#include "main_window.h"
#include "login_panel.h"
#include "main_panel.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_COMMAND(wxID_ANY, wxEVT_CLEAR_PANEL, MainWindow::OnClearPanel)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_CLEAR_PANEL);

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, _T("Twittle"), wxDefaultPosition, wxSize(320, 425))
{
	panel = NULL;
	ShowLogin();
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
	SetClientSize(GetSize());
}

void MainWindow::ShowLogin()
{
	SwapPanels(new LoginPanel(this));
}

void MainWindow::ShowMainPanel()
{
	SwapPanels(new MainPanel(this));
}
