#include <wx/wx.h>
#include <wx/mimetype.h>
#include <wx/regex.h>
#include "feed_panel.h"
#include "application.h"
#include "twitter/twitter_feed.h"
#include "twitter/twitter_user.h"
#include "twitter/twitter_status.h"

class FilteredIterator
{
	unsigned int filter;
	std::vector<TwitterStatus>::const_iterator iter;
	std::vector<TwitterStatus>::const_iterator iterend;

	bool valid()
	{
		if (iter == iterend) return true;

		if (filter & FeedPanel::FILTER_REPLIES) {
			if (iter->GetText()[0] != _T('@')) return false;
			if (wxGetApp().GetTwitter().GetUsername() ==
				iter->GetUser().GetScreenName()) return false;
		}
		return true;
	}

public:
	FilteredIterator(std::vector<TwitterStatus>::const_iterator it,
		std::vector<TwitterStatus>::const_iterator end, unsigned int ftype = 0) :
			filter(ftype), iter(it), iterend(end) { }

	const FilteredIterator& operator++() { do { ++iter; } while (!valid()); return *this; }
	const FilteredIterator& operator--() { do { --iter; } while (!valid()); return *this; }
	bool operator==(const std::vector<TwitterStatus>::const_iterator& c) const { return iter == c; }
	bool operator!=(const std::vector<TwitterStatus>::const_iterator& c) const { return iter != c; }
	bool operator<(const std::vector<TwitterStatus>::const_iterator& c) const { return iter < c; }
	bool operator>(const std::vector<TwitterStatus>::const_iterator& c) const { return iter > c; }
	std::vector<TwitterStatus>::const_iterator::value_type operator*() const { return *iter; }
};

BEGIN_EVENT_TABLE(FeedPanel, wxHtmlListBox)
	EVT_COMMAND(wxID_ANY, wxEVT_FEED_UPDATED, FeedPanel::OnFeedUpdated)
	EVT_COMMAND(wxID_ANY, wxEVT_IMAGE_UPDATED, FeedPanel::OnImageUpdated)
	EVT_HTML_LINK_CLICKED(1, FeedPanel::OnLinkClicked)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_FEED_UPDATED)
DEFINE_EVENT_TYPE(wxEVT_IMAGE_UPDATED)

FeedPanel::FeedPanel(wxWindow* parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size,
					 long style, const wxString& name)
: wxHtmlListBox(parent, id, pos, size, style, name), filter(0)
{
	Create(parent, 1, pos, size, style, name);
}

FeedPanel::~FeedPanel()
{
	wxGetApp().GetTwitter().UnregisterListener(*this, feedResource);
}

void FeedPanel::Create(wxWindow* parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size,
					 long style, const wxString& name)
{
	wxHtmlListBox::Create(parent, id, pos, size, style, name);
}

unsigned int FeedPanel::GetStatusSize() const
{
	unsigned int count = 0;
	const TwitterFeed *feed = wxGetApp().GetTwitter().GetFeed(feedResource);
	const std::vector<TwitterStatus>& statuses = feed->GetStatuses();
	FilteredIterator it(statuses.begin(), statuses.end(), filter);
	for (; it != statuses.end(); ++it, ++count);
	return count - 1;
}

const TwitterStatus FeedPanel::GetStatusItem(unsigned int n) const
{
	unsigned int count = 0;
	const TwitterFeed *feed = wxGetApp().GetTwitter().GetFeed(feedResource);
	const std::vector<TwitterStatus>& statuses = feed->GetStatuses();
	FilteredIterator it(statuses.begin(), statuses.end(), filter);
	for (; it != statuses.end(); ++it, ++count) {
		if (count - 1 == n) return *it;
	}
	throw "invalid status item";
}

void FeedPanel::SetFeed(const wxString& resource, int delay)
{
	// Already watching this feed
	if (resource == feedResource) return;

	Twitter &twitter = wxGetApp().GetTwitter();

	if (feedResource != _T("")) {
		// unregister the old resource
		twitter.UnregisterListener(*this, feedResource);
		twitter.GetFeed(feedResource)->Pause();
		SetItemCount(0); // flush the data
	}

	// Load any residual data in the feedResource
	feedResource = resource;
	TwitterFeed *feed = wxGetApp().GetTwitter().GetFeed(feedResource);
	if (feed) {
		SetItemCount(GetStatusSize());
	}
	else { 
		// Attempt to de-serialize any saved data
		feed = Serializer<wxString, TwitterFeed>().Read(feedResource);
		if (feed) {
			twitter.LoadFeed(feedResource, feed);
			SetItemCount(GetStatusSize());
		}
	}

	// register the listener and begin the monitoring threads
	twitter.RegisterListener(*this, feedResource);
	twitter.BeginFeed(feedResource, delay);
}

void FeedPanel::TwitterUpdateReceived(const Twitter& twitter, const wxString& resource)
{
	if (resource != feedResource) return;

	wxCommandEvent evt(wxEVT_FEED_UPDATED, wxID_ANY);
	wxPostEvent(this, evt);
}

static wxCriticalSection feedUpdateSec;

void FeedPanel::OnFeedUpdated(wxCommandEvent &event)
{
	feedUpdateSec.Enter();
	SetItemCount(GetStatusSize());
	feedUpdateSec.Leave();
}

void FeedPanel::OnImageUpdated(wxCommandEvent &event)
{
}

wxString FeedPanel::DecorateStatusText(wxString text) const
{
	wxRegEx links(_T("((?:https?|www\\.)://[^[:space:]]+)"), wxRE_ICASE | wxRE_ADVANCED);
	wxRegEx hashtags(_T("#([[:alnum:]_]+)"));
	wxRegEx refs(_T("@([[:alnum:]_]+)"));

	links.ReplaceAll(&text, _T("<a href='\\1'>\\1</a>"));
	refs.ReplaceAll(&text, _T("@<a href='http://twitter.com/\\1'>\\1</a>"));
	hashtags.ReplaceAll(&text, _T("#<a href='http://hashtags.org/tag/\\1'>\\1</a>"));

	return text;
}

static wxCriticalSection getItemSec;

wxString FeedPanel::OnGetItem(size_t n) const
{
	getItemSec.Enter();

	const TwitterFeed *feed = wxGetApp().GetTwitter().GetFeed(feedResource);
	if (feed == NULL) return _T("");

	size_t len = GetStatusSize() - 1;
	TwitterStatus status = GetStatusItem(len - n);
	const TwitterUser &user = status.GetUser();

	wxString list;
	list << _T("<table cellpadding='5'><tr><td valign='top'>");
	list << _T("<img id='") << user.GetId() <<_T("' width=48 height=48 src='");
	list << user.GetProfileImageFilename();
	list << _T("' align='left'>");
	list << _T("</td><td valign='top'>");
	list << _T("<b>") + user.GetScreenName() + _T("</b>: ");
	list << DecorateStatusText(status.GetText());

	try {
		if (status.GetCreatedAt().IsValid()) {
			list << _T("<p><font color='#aaaaaa'>");
			list << status.GetCreatedAt().FormatISODate() << _T(" ");
			list << status.GetCreatedAt().FormatISOTime();
			list << _T("</font>");
		}
	}
	catch (...) {
		// something funny happens here sometimes.
	}
	list << _T("</td></tr></table>");

	getItemSec.Leave();

	return list;
}

void FeedPanel::OnLinkClicked(wxHtmlLinkEvent &evt)
{
	wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromMimeType(_T("text/html"));
	if (ft == NULL) return;

	wxString url = evt.GetLinkInfo().GetHref();
	if (!url.StartsWith(_T("http://"))) {
		url = _T("http://") + url;
	}

	wxString cmd = ft->GetOpenCommand(url);
	cmd.Replace(_T("file://"), _T("")); // hack to remove wx 'bug'
	wxExecute(cmd);
	delete ft;
}
