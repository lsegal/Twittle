#include "tray_icon.h"
#include "application.h"

BEGIN_EVENT_TABLE(TrayIcon, wxTaskBarIcon)
	EVT_TASKBAR_LEFT_DCLICK(TrayIcon::OnDoubleClick) 
	EVT_MENU(wxID_ANY, TrayIcon::OnMenu)
END_EVENT_TABLE()

wxMenu *TrayIcon::CreatePopupMenu()
{
	MainWindow& main = wxGetApp().GetMainWindow(); // friend
	wxMenuBar *mbar = main.loggedIn ? main.MainMenuBar() : main.LoginMenuBar();
	wxMenu *menu = mbar->Remove(0);
	menu->SetInvokingWindow(&main);
	SetNextHandler(&main);

	// remove all accelerators
	wxMenuItemList::iterator it;
	for (it = menu->GetMenuItems().begin(); it != menu->GetMenuItems().end(); ++it) {
		(*it)->SetText((*it)->GetText().BeforeFirst(_T('\t')));
	}

	delete mbar; // free memory without deleting menus (wxDELETE)
	return menu; // "library" will delete this as per docs
}

void TrayIcon::OnDoubleClick(wxTaskBarIconEvent& evt)
{
	MainWindow& main = wxGetApp().GetMainWindow();

	if (main.IsIconized()) {
		main.Iconize(false);
		main.Show();
		main.SetTrayIcon();
	}
	main.Raise();
}

void TrayIcon::OnMenu(wxCommandEvent& evt)
{
	MainWindow& main = wxGetApp().GetMainWindow();
	wxPostEvent(&main, evt);
}
