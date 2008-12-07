#pragma once

#include <wx/wx.h>

class AboutDialog : public wxDialog
{
	void InitializeComponents();

public:
	AboutDialog(wxWindow *parent);
};
