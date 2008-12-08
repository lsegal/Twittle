#include "application.h"
#include "main_window.h"
#include "twitter/twitter.h"
#include <wx/image.h>
#include <wx/socket.h>
#include <wx/fs_inet.h>
#include <wx/mimetype.h>
#include <wx/stdpaths.h>

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

const wxString Application::APPNAME = _T("Twittle");
const wxString Application::APPVERSION = _T("0.5 beta");

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

	// twitter
	twitter = new Twitter();

	// make the images, feeds, settings directories if they do not exist
	GetImagesPath();
	GetFeedsPath();

	// main window
	main = new MainWindow(settings.GetBool(_T("window.showintaskbar")));
	main->Show(true);

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

void Application::OpenUrl(const wxString& url)
{
	wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromMimeType(_T("text/html"));
	if (ft == NULL) return;

	wxString totalUrl = url;
	if (!url.StartsWith(_T("http://"))) {
		totalUrl = _T("http://") + totalUrl;
	}

	wxString cmd = ft->GetOpenCommand(totalUrl);
	cmd.Replace(_T("file://"), _T("")); // hack to remove wx file:// prefix 'bug'
	wxExecute(cmd);
	delete ft;
}

wxString Application::GetAppName() const
{
	return APPNAME;
}

wxString Application::GetFeedsPath() const
{
	wxStandardPathsBase& sPath = wxStandardPathsBase::Get();
	wxString path = sPath.GetUserDataDir() + _T("/accounts/") + twitter->GetUsername() + _T("/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}

wxString Application::GetImagesPath() const
{
	wxStandardPathsBase& sPath = wxStandardPathsBase::Get();
	wxString path = sPath.GetUserDataDir() + _T("/images/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}

wxString Application::GetSettingsPath() const
{
	wxStandardPathsBase& sPath = wxStandardPathsBase::Get();

	// this checks for and creates an empty directory,
	// but we keep it around just in case we decide to change the path name
	wxString path = sPath.GetUserDataDir() + _T("/");
	mkdir(path.mb_str()); // make sure path exists
	return path;
}
