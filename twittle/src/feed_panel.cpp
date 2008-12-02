#include <wx/wx.h>
#include <wx/clipbrd.h>
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
	EVT_RIGHT_DOWN(FeedPanel::OnRightClick)
	EVT_MENU(ID_COPYTEXT, FeedPanel::CopyItemAsText)
	EVT_MENU(ID_COPYHTML, FeedPanel::CopyItemAsHtml)
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
	CreateItemMenu();
	CreateAccelerators();
}

void FeedPanel::CreateAccelerators()
{
	wxAcceleratorEntry entries[1];
	entries[0].Set(wxACCEL_CTRL, (int)'C', ID_COPYTEXT);
	wxAcceleratorTable accel(1, entries);
	SetAcceleratorTable(accel);
}

void FeedPanel::CreateItemMenu()
{
	itemMenu.Append(ID_COPYTEXT, _T("Copy as text"));
	itemMenu.Append(ID_COPYHTML, _T("Copy as HTML"));
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
	n = (GetStatusSize() - 1) - n; // reverse n index
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

	TwitterStatus status = GetStatusItem(n);
	const TwitterUser &user = status.GetUser();

	wxString list;
	list << _T("<table cellpadding='5'><tr><td valign='top'>");
	list << _T("<img id='") << user.GetId() <<_T("' width=48 height=48 src='");
	list << user.GetProfileImageFilename();
	list << _T("' align='left'>");
	list << _T("</td><td valign='top'>");
	list << _T("<b>") + user.GetScreenName() + _T("</b>: ");
	list << DecorateStatusText(status.GetText());

	list << _T("<p><font color='#aaaaaa' size='2'>");
	list << user.GetName() << _T(" (");
	list << _T("<a href='http://twitter.com/") << user.GetScreenName() << _T("'>");
	list << _T("<font color='#555555'>") << user.GetScreenName() << _T("</font></a>");
	list << _T(") <font color='#555555'>") << status.GetTimeSincePost();
	list << _T("</font> via <font color='#555555'>") << status.GetSource() << _T("</font>");
	list << _T("</font>");

	list << _T("</td></tr></table>");

	getItemSec.Leave();

	return list;
}

/**
 * Opens a URL in the system default browser 
 */
void FeedPanel::OnLinkClicked(wxHtmlLinkEvent &evt)
{
	wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromMimeType(_T("text/html"));
	if (ft == NULL) return;

	wxString url = evt.GetLinkInfo().GetHref();
	if (!url.StartsWith(_T("http://"))) {
		url = _T("http://") + url;
	}

	wxString cmd = ft->GetOpenCommand(url);
	cmd.Replace(_T("file://"), _T("")); // hack to remove wx file:// prefix 'bug'
	wxExecute(cmd);
	delete ft;
}

void FeedPanel::OnRightClick(wxMouseEvent& evt)
{
	// make sure an item gets selected
	wxMouseEvent clickevt = evt;
	clickevt.SetEventType(wxEVT_LEFT_DOWN);
	wxPostEvent(this, clickevt);

	// show popup menu
	PopupMenu(&itemMenu, evt.GetPosition());
}

void FeedPanel::CopyItemAsText(wxCommandEvent& evt) 
{
	int selId = GetSelection();
	if (selId != wxNOT_FOUND && wxTheClipboard->Open()) {
		TwitterStatus status = GetStatusItem(selId);
		wxString text = status.GetUser().GetScreenName() + _T(": ") + status.GetText();
		wxTheClipboard->SetData(new wxTextDataObject(text));
		wxTheClipboard->Close();
	}
}

void FeedPanel::CopyItemAsHtml(wxCommandEvent& evt) 
{
	int selId = GetSelection();
	if (selId != wxNOT_FOUND && wxTheClipboard->Open()) {
		TwitterStatus status = GetStatusItem(selId);
		wxString decoratedText = DecorateStatusText(status.GetText());
		decoratedText = status.GetUser().GetScreenName() + _T(": ") + decoratedText;
		wxTheClipboard->SetData(new wxTextDataObject(decoratedText));
		wxTheClipboard->Close();
	}
}
