#pragma once

#include <vector>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/htmllbox.h>
#include "twitter/twitter.h"

class FeedPanel;

DECLARE_EVENT_TYPE(wxEVT_FEED_UPDATED, -1)

class FeedPanelUpdater : public wxThread
{
	const wxString& url;
	FeedPanel& panel;

	friend class FeedPanel;

public:
	FeedPanelUpdater(const wxString& url, FeedPanel& panel) : 
		url(url), panel(panel), wxThread(wxTHREAD_DETACHED) { }

	static void FeedPanelUpdater::Update(const wxString& url, FeedPanel& panel);

	void* Entry(); // implemented from wxThread
};

class FeedPanel : public wxHtmlListBox
{
	std::vector<TwitterStatus> items;

	void SetItems(std::vector<TwitterStatus> items_); 

public:
	FeedPanel() : wxHtmlListBox() { }
	FeedPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);
	void Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);

	wxString OnGetItem(size_t n) const;
	//wxCoord OnMeasureItem(size_t n) const;

	void BeginUpdate(const wxString& resource);
	void OnFeedUpdated(wxCommandEvent &event);

	DECLARE_EVENT_TABLE();
};
