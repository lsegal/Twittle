#pragma once

#include <wx/dialog.h>
#include "settings.h"

/**
 * Allows user to modify application settings.
 * Example usage:
 * @code
 *   OptionsDialog dlg(this);
 *   if (dlg.ShowModal() == wxID_OK) {
 *     const Settings& settings = dlg.GetSettings();
 *
 *     // merge settings with application settings
 *     wxGetApp().GetSettings().Merge(settings);
 *   }
 * @endcode
 *
 * @see Settings
 */
class OptionsDialog : public wxDialog
{
	/// The settings object used to store modified settings.
	Settings settings; 

	wxCheckBox autoLogin;
	wxCheckBox showScreenNamePrefix;
	wxCheckBox showInTray;
	wxCheckBox showInTaskbar;
	wxCheckBox minimizeToTray;
	wxCheckBox showNotifications;
	wxCheckBox alwaysOnTop;
	wxCheckBox transWhenActive;
	wxSlider transparency;
	wxStaticText transLabel;
	wxComboBox editPosition;

	/** Initializes all window controls */
	void InitializeComponents();

	/** Initializes all control values from the #settings object */
	void InitializeValues();

public:
	/** Creates a new options dialog attached to the parent window (MainWindow) */
	OptionsDialog(wxWindow *parent);

	/** Allow override of destructor */
	virtual ~OptionsDialog() { }

	/** 
	 * Returns the settings object which might have been modified by the dialog on close.
	 *
	 * @see TransferDataFromWindow
	 */
	inline const Settings & GetSettings() const { return settings; }

	/**
	 * Pulls the data out of the controls and modifies the settings object
	 * before closing the window. The modified settings should be accessible
	 * from the #GetSettings method after closing the window.
	 *
	 * @see GetSettings
	 */
	bool TransferDataFromWindow();

	///
	/// @name Window Events
	///
	//@{

	/**
	 * Handles the transparency slider to automatically
	 * adjust the main window transparency (for preview purposes).
	 * The transparency should be reset by the caller when the dialog
	 * closes.
	 */
	void TransparencyChanged(wxScrollEvent& evt);

	DECLARE_EVENT_TABLE();

	//@}
};
