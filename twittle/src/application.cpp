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

	// load settings
	settings.Load();

	// make the images, feeds, settings directories if they do not exist
	mkdir("imgs");
	mkdir("feeds");
	mkdir("settings");

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
	settings.Save();
	delete twitter; 
	return 0;
}

void Application::Login(const wxString& user, const wxString& pass)
{
	twitter->SetAuth(user, pass);
	main->ShowMainPanel();
}

void Application::Logout()
{
	delete twitter;
	twitter = new Twitter();
	main->ShowLogin();
}

const wxString Application::GetFeedsPath()
{
	wxString path = _T("feeds/") + twitter->GetUsername() + _T("/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}

const wxString Application::GetImagesPath()
{
	wxString path = _T("imgs/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}

const wxString Application::GetSettingsPath()
{
	wxString path = _T("settings/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}
