#include "application.h"
#include <wx/image.h>
#include <wx/socket.h>

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
	wxSocketBase::Initialize();
	wxInitAllImageHandlers();
	main = new MainWindow();
	main->Show(true);
	return true;
}

