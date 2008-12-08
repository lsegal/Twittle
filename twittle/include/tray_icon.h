#pragma once

#include <wx/taskbar.h>

/**
 * Represents an icon located in the system tray.
 * This class is subclassed to add popup functionality when the
 * user right clicks the icon.
 */
class TrayIcon : public wxTaskBarIcon
{
public:
	/**
	 * Overrides the wxTaskbarIcon::CreatePopupMenu method to return
	 * a menu which the user on a right click event. Currently returns 
	 * whatever is in the File menu from the MainWindow but without the
	 * accelerator codes.
	 *
	 * @return the popup menu to show to the user on right click.
	 */
	wxMenu* CreatePopupMenu();

	///
	/// @name Window Events
	///
	//@{ 

	/**
	 * Activates the main window when the icon is double clicked. 
	 */
	void OnDoubleClick(wxTaskBarIconEvent& evt);

	/**
	 * Handles a menu event, passing it directly to the main window
	 * (which handles the menu events since the menu items are directly
	 * from its File menu).
	 */
	void OnMenu(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();

	//@}
};
