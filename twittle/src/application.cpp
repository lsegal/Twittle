#include "application.h"
#include <wx/image.h>
#include <wx/socket.h>
#include <wx/fs_inet.h>

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
	wxSocketBase::Initialize();
	wxInitAllImageHandlers();
	wxFileSystem::AddHandler(new wxInternetFSHandler);
	main = new MainWindow();
	main->Show(true);
	return true;
}

