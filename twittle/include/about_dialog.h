#pragma once

#include <wx/wx.h>

/**
 * Represents a modal About dialog in the application showing 
 * version and author information.
 */
class AboutDialog : public wxDialog
{
	/** Initializes all of the dialog controls */
	void InitializeComponents();

public:
	/**
	 * Creates a new About dialog with the parent window set.
	 *
	 * @param parent the parent window of the modal dialog (most likely MainWindow)
	 */
	AboutDialog(wxWindow *parent);
};
