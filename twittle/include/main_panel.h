#pragma once

#include <wx/wx.h>
#include <wx/toolbar.h>
#include "status_text_ctrl.h"
#include "feed_panel.h"

class MainPanel : public wxPanel
{
	enum ControlIdentifiers {
		ID_EDIT, ID_CONTENT, ID_TINYURL, ID_TWITPIC,
		ID_COUNTER, ID_PUBLIC, ID_FRIEND, ID_FILTER_AT
	};

	StatusTextCtrl editbox;
	FeedPanel content;
	wxButton tinyurl;
	wxButton twitpic;
	wxStaticText charcounter;
	wxFont font;
	wxButton publicButton;
	wxButton followButton;
	wxButton atButton;

	void InitializeComponents();
	void SetAccelerators();

	void InsertUrl(const wxString& text);

public:
	MainPanel(wxWindow *parent);
	virtual ~MainPanel();

	// Shorten URL functionality (thread callback)
	void ShortenUrl(wxString& shortUrl);

	// Called after options have been changed
	// and the feed panel UI needs to be updated
	void ForceUpdateUI();

	// Events
	void OnButtonClick(wxCommandEvent& evt);
	void OnImageClick(wxCommandEvent& evt);
	void OnShortenUrl(wxCommandEvent& evt);
	void OnEditText(wxCommandEvent& evt);
	void OnEditEnter(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()
};
