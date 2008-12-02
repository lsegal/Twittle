#pragma once

#include <vector>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/htmllbox.h>
#include "twitter/twitter_update_listener.h"

class Twitter;

DECLARE_EVENT_TYPE(EVT_FEED_UPDATED, -1)
DECLARE_EVENT_TYPE(EVT_IMAGE_UPDATED, -2)
DECLARE_EVENT_TYPE(EVT_REFRESH_FEED, -3)

class FeedPanel : public wxHtmlListBox, public TwitterUpdateListener
{
	enum ControlIdentifiers {
		ID_COPYTEXT, ID_COPYHTML
	};

	wxThread *refreshThread;
	wxString feedResource;
	unsigned int filter;
	wxMenu itemMenu;

	void CreateItemMenu();
	void CreateAccelerators();

protected:
	wxString DecorateStatusText(wxString text) const;
	unsigned int GetStatusSize() const;
	const TwitterStatus GetStatusItem(unsigned int n) const;

public:
	FeedPanel() : wxHtmlListBox(), filter(0) { }
	FeedPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);
	void Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);
	~FeedPanel();

	enum FilterTypes {
		FILTER_NONE = 0, FILTER_REPLIES
	};

	void SetFeed(const wxString &resource, int delay = 300);

	void ResetFilter() { filter = 0; SetItemCount(GetStatusSize()); }
	void AddFilter(unsigned int ftype) { filter |= ftype; SetItemCount(GetStatusSize()); }
	void RemoveFilter(unsigned int ftype) { filter &= ~ftype; SetItemCount(GetStatusSize()); }

	// @override wxHtmlListBox
	wxString OnGetItem(size_t n) const;
	//wxCoord OnMeasureItem(size_t n) const;

	// @override TwitterUpdateListener
	void TwitterUpdateReceived(const Twitter& twitter, const wxString& resource);

	void OnLinkClicked(wxHtmlLinkEvent &event);

	void OnFeedUpdated(wxCommandEvent &event);
	void OnImageUpdated(wxCommandEvent &event);

	// Popup menu events
	void OnRightClick(wxMouseEvent &event);
	void CopyItemAsText(wxCommandEvent& evt);
	void CopyItemAsHtml(wxCommandEvent& evt);

	// Refresh thread
	void RefreshThread();
	void OnUpdate(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};
