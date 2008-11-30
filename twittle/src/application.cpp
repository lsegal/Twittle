#include "application.h"
#include "main_window.h"
#include "twitter/twitter.h"
#include <wx/image.h>
#include <wx/socket.h>
#include <wx/fs_inet.h>

#ifdef WIN32
#	define mkdir _mkdir
#endif

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
	// handlers
	wxSocketBase::Initialize();
	wxInitAllImageHandlers();
	wxFileSystem::AddHandler(new wxInternetFSHandler);

	// make the images directory if it does not exist
	mkdir("imgs");

	// make the feeds directory if it does not exist
	mkdir("feeds");

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

const wxString Application::GetFeedsPath()
{
	wxString path = _T("feeds/") + twitter->GetUsername() + _T("/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}
