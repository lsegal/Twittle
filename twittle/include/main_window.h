#pragma once

#include <wx/wx.h>
#include "feed_panel.h"

class MainWindow : public wxFrame
{
	enum {
		ID_EDIT, ID_CONTENT, ID_SUBMIT, ID_COUNTER
	};

	wxTextCtrl editbox;
	FeedPanel content;
	wxButton submit;
	wxStaticText charcounter;
	wxFont font;

public:
	MainWindow();

	// Events
	void OnEditText(wxCommandEvent &evt);
	void OnEditEnter(wxCommandEvent &evt);

	DECLARE_EVENT_TABLE()
};
