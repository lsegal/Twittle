#pragma once

#include <wx/dialog.h>
#include "settings.h"

class OptionsDialog : public wxDialog
{
	Settings settings;
	wxCheckBox autoLogin;
	wxCheckBox showScreenNamePrefix;
	wxSlider transparency;
	wxStaticText transLabel;
	wxComboBox editPosition;

	void InitializeComponents();
	void InitializeValues();

public:
	OptionsDialog(wxWindow *parent);
	virtual ~OptionsDialog() { } 

	inline const Settings & GetSettings() { return settings; }

	// @override wxWindow
	bool TransferDataFromWindow();

	// events
	void TransparencyChanged(wxScrollEvent& evt);

	DECLARE_EVENT_TABLE();
};