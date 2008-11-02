#pragma once

#include <vector>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/htmllbox.h>
#include "twitter/twitter.h"

DECLARE_EVENT_TYPE(wxEVT_FEED_UPDATED, -1)
DECLARE_EVENT_TYPE(wxEVT_IMAGE_UPDATED, -2)

class FeedPanel : public wxHtmlListBox, public TwitterUpdateListener
{
	wxString feedResource;

public:
	FeedPanel() : wxHtmlListBox() { }
	FeedPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);
	void Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);
	~FeedPanel();

	void SetFeed(const wxString &resource, int delay = 300);

	// @override wxHtmlListBox
	wxString OnGetItem(size_t n) const;
	//wxCoord OnMeasureItem(size_t n) const;

	// @override TwitterUpdateListener
	void TwitterUpdateReceived(const Twitter& twitter, const wxString& resource);

	void OnFeedUpdated(wxCommandEvent &event);
	void OnImageUpdated(wxCommandEvent &event);

	DECLARE_EVENT_TABLE();
};
