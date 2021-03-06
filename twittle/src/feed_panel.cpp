#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <wx/regex.h>
#include "feed_panel.h"
#include "application.h"
#include "twitter/twitter_feed.h"
#include "twitter/twitter_user.h"
#include "twitter/twitter_status.h"
#include "thread_callback.h"

BEGIN_EVENT_TABLE(FeedPanel, wxHtmlListBox)
	EVT_COMMAND(wxID_ANY, EVT_FEED_UPDATED, FeedPanel::OnFeedUpdated)
	EVT_COMMAND(wxID_ANY, EVT_IMAGE_UPDATED, FeedPanel::OnImageUpdated)
	EVT_HTML_LINK_CLICKED(1, FeedPanel::OnLinkClicked)
	EVT_RIGHT_DOWN(FeedPanel::OnRightClick)
	EVT_MENU(ID_COPYTEXT, FeedPanel::CopyItemAsText)
	EVT_MENU(ID_COPYHTML, FeedPanel::CopyItemAsHtml)
	EVT_TIMER(ID_UPDATEUI, FeedPanel::OnUpdateUI)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(EVT_FEED_UPDATED)
DEFINE_EVENT_TYPE(EVT_IMAGE_UPDATED)

FeedPanel::FeedPanel() : wxHtmlListBox(), filter(FilteredIterator::FILTER_NONE) 
{ 
}

FeedPanel::FeedPanel(wxWindow* parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size,
					 long style, const wxString& name)
: wxHtmlListBox(parent, id, pos, size, style, name), filter(FilteredIterator::FILTER_NONE)
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
	SetMargins(0, 0);
	SetSelectionBackground(wxColour(240, 240, 240));
	CreateItemMenu();
	CreateAccelerators();

	// Start update UI timer
	updateUITimer.SetOwner(this, ID_UPDATEUI);
	updateUITimer.Start(60000, false); // 60 second update
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

void FeedPanel::StatusNotification(const TwitterStatus& status)
{
	wxString title = status.GetUser().GetName() + _T(" (") + 
		status.GetUser().GetScreenName() + _T(") tweets:");
	if (title.Length() > 64) {
		title = status.GetUser().GetScreenName() + _T(" tweets:");
	}
	wxString notification = status.GetText();
	wxGetApp().GetMainWindow().TrayNotification(notification, title);
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

	bool showNotifications = wxGetApp().GetSettings().GetBool(_T("window.shownotifications"));
	if (showNotifications && resource == Twitter::FriendsTimelineUrl) {
		// show update notifications for friends
		TwitterFeed *feed = twitter.GetFeed(resource);
		if (feed && feed->GetStatuses().size() > 0) {
			const TwitterStatus& last = feed->GetStatuses().back();
			StatusNotification(last);
		}
	}

	// post event
	wxCommandEvent evt(EVT_FEED_UPDATED, wxID_ANY);
	wxPostEvent(this, evt);
}

void FeedPanel::OnFeedUpdated(wxCommandEvent &event)
{
	SetItemCount(GetStatusSize());
	RefreshAll();
}

// unused
void FeedPanel::OnImageUpdated(wxCommandEvent &event)
{
}

wxString FeedPanel::DecorateStatusText(wxString text) const
{
	wxRegEx links(_T("((https?|www\\.)://[^[:space:]]+)"), wxRE_ICASE);
	wxRegEx hashtags(_T("#([[:alnum:]_]+)"));
	wxRegEx refs(_T("@([[:alnum:]_]+)"));

	links.ReplaceAll(&text, _T("<a href='\\1'>\\1</a>"));
	refs.ReplaceAll(&text, _T("@<a href='http://twitter.com/\\1'>\\1</a>"));
	hashtags.ReplaceAll(&text, _T("#<a href='http://hashtags.org/tag/\\1'>\\1</a>"));

	return text;
}

wxString FeedPanel::DecorateSource(wxString text) const
{
	wxRegEx links(_T("(<a[^>]+>)([^<]+)"), wxRE_ICASE);
	links.ReplaceAll(&text, _T("\\1<font color='#555555'>\\2</font>"));
	return text;
}

wxString FeedPanel::OnGetItem(size_t n) const
{
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
	if (wxGetApp().GetSettings().GetBool(_T("feedpanel.showscreenname"))) {
		list << _T("<b>") + user.GetScreenName() + _T("</b>: ");
	}
	list << DecorateStatusText(status.GetText());

	list << _T("<p><font color='#aaaaaa' size='2'>");
	list << user.GetName() << _T(" (");
	list << _T("<a href='http://twitter.com/") << user.GetScreenName() << _T("'>");
	list << _T("<font color='#555555'>") << user.GetScreenName() << _T("</font></a>");
	list << _T(") <font color='#555555'>") << status.GetTimeSincePost();
	list << _T("</font> via <font color='#555555'>") << DecorateSource(status.GetSource()) << _T("</font>");
	list << _T("</font>");

	list << _T("</td></tr></table>");

	return list;
}

void FeedPanel::OnLinkClicked(wxHtmlLinkEvent &evt)
{
	wxGetApp().OpenUrl(evt.GetLinkInfo().GetHref());
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

void FeedPanel::OnUpdateUI(wxTimerEvent& evt)
{
	RefreshAll();
}

wxColour FeedPanel::GetSelectedTextColour(const wxColour& colFg) const
{
	return *wxBLACK;
}
