#include "application.h"
#include "main_window.h"
#include "twitter/twitter.h"
#include <wx/image.h>
#include <wx/socket.h>
#include <wx/fs_inet.h>

#ifdef WIN32
#	define mkdir _mkdir
#else
#	include <sys/stat.h>
#	define mkdir(dir) mkdir(dir, S_IRWXU|S_IRWXG);
#endif

#ifdef __WXMAC__
#	include <ApplicationServices/ApplicationServices.h>
#endif

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
#ifdef __WXMAC__
	// Bring process to front on a Mac
	ProcessSerialNumber PSN;
	GetCurrentProcess(&PSN);
	TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
#endif

	// conversion
	wxConvCurrent = &wxConvISO8859_1;

	// handlers
	wxSocketBase::Initialize();
	wxInitAllImageHandlers();
	wxFileSystem::AddHandler(new wxInternetFSHandler);

	// load settings
	settings.Load();

	// make the images, feeds, settings directories if they do not exist
	mkdir("images");
	mkdir("accounts");

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
	wxString path = _T("accounts/") + twitter->GetUsername() + _T("/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}

const wxString Application::GetImagesPath()
{
	wxString path = _T("images/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}

const wxString Application::GetSettingsPath()
{
	// this checks for and creates an empty directory,
	// but we keep it around just in case we decide to change the path name
	wxString path = _T("");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}
