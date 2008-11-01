#pragma once

#include <wx/wx.h>
#include <wx/toolbar.h>
#include "feed_panel.h"

class MainPanel : public wxPanel
{
	enum ControlIdentifiers {
		ID_EDIT, ID_CONTENT, ID_SUBMIT, ID_COUNTER, 
		ID_PUBLIC, ID_FRIEND
	};

	wxTextCtrl editbox;
	FeedPanel content;
	wxButton submit;
	wxStaticText charcounter;
	wxFont font;
	wxButton publicButton;
	wxButton followButton;

	void InitializeComponents();
	void CheckEditboxEmpty();

public:
	MainPanel(wxWindow *parent);

	// Events
	void OnButtonClick(wxCommandEvent& evt);
	void OnEditText(wxCommandEvent& evt);
	void OnEditEnter(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()
};
