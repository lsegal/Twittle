#include "main_window.h"
#include "login_panel.h"
#include "main_panel.h"

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, _T("Twittle"), wxDefaultPosition, wxSize(320, 425))
{
	ShowLogin();
}

void MainWindow::ClearPanels()
{
	GetChildren().Clear();
}

void MainWindow::ShowLogin()
{
	ClearPanels();
	new LoginPanel(this, wxID_ANY);
}

void MainWindow::ShowFollowingPanel()
{
	ClearPanels();
	new MainPanel(this, wxID_ANY);
}
