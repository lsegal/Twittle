#include "application.h"

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
	wxInitAllImageHandlers();
	main = new MainWindow();
	main->Show(true);
	return true;
}

