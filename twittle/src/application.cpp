#include "application.h"
#include <wx/image.h>
#include <wx/socket.h>
#include <wx/fs_inet.h>

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
	// handlers
	wxSocketBase::Initialize();
	wxInitAllImageHandlers();
	wxFileSystem::AddHandler(new wxInternetFSHandler);

	// twitter
	twitter = new Twitter();

	// main window
	main = new MainWindow();
	main->Show(true);
	//main->SetTransparent(200);
	SetTopWindow(main);
	return true;
}

int Application::OnExit()
{
	delete twitter; 
	return 0;
}

void Application::Login(const wxString& user, const wxString& pass)
{
	twitter->SetAuth(user, pass);
	main->ShowMainPanel();
}