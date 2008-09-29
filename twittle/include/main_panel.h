#pragma once

#include <wx/wx.h>
#include "feed_panel.h"

class MainPanel : public wxPanel
{
	enum ControlIdentifiers {
		ID_EDIT, ID_CONTENT, ID_SUBMIT, ID_COUNTER
	};

	wxTextCtrl editbox;
	FeedPanel content;
	wxButton submit;
	wxStaticText charcounter;
	wxFont font;

	void InitializeComponents();
	void CheckEditboxEmpty();

public:
	MainPanel(wxWindow *parent);

	// Events
	void OnEditText(wxCommandEvent &evt);
	void OnEditEnter(wxCommandEvent &evt);

	DECLARE_EVENT_TABLE()
};
