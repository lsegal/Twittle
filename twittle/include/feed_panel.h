#pragma once

#include <vector>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/htmllbox.h>
#include "twitter/twitter.h"

class FeedPanel;

DECLARE_EVENT_TYPE(wxEVT_FEED_UPDATED, -1)
DECLARE_EVENT_TYPE(wxEVT_IMAGE_UPDATED, -2)

class FeedImageUpdater : public wxThread
{
	bool singleitem;
	unsigned int index;
	std::vector<TwitterStatus>& items;
	FeedPanel& panel;

	friend class FeedPanel;

public:
	FeedImageUpdater(std::vector<TwitterStatus> &items, FeedPanel& panel) :
		wxThread(wxTHREAD_DETACHED), items(items), index(0),
		panel(panel), singleitem(false) { }
	FeedImageUpdater(TwitterStatus &item, unsigned int index, FeedPanel& panel);
	~FeedImageUpdater();

	static void Update(std::vector<TwitterStatus> &items, FeedPanel& panel);
	static void Update(TwitterStatus &item, unsigned int index, FeedPanel& panel);

	void* Entry();  // @override wxThread
};

class FeedPanelUpdater : public wxThread
{
	const wxString& url;
	FeedPanel& panel;

	friend class FeedPanel;

public:
	FeedPanelUpdater(const wxString& url, FeedPanel& panel) :
		wxThread(wxTHREAD_DETACHED), url(url), panel(panel) { }

	static void Update(const wxString& url, FeedPanel& panel);

	void* Entry();  // @override wxThread
	void  OnExit(); // @override wxThread
};

class FeedPanel : public wxHtmlListBox
{
	std::vector<TwitterStatus> items;

public:
	FeedPanel() : wxHtmlListBox() { }
	FeedPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);
	void Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);

	void AddItems(std::vector<TwitterStatus> &items_);
	void AddItem(TwitterStatus &item);

	wxString OnGetItem(size_t n) const;
	//wxCoord OnMeasureItem(size_t n) const;

	void OnFeedUpdated(wxCommandEvent &event);
	void OnImageUpdated(wxCommandEvent &event);

	DECLARE_EVENT_TABLE();
};
