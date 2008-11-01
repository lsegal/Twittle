#include "feed_panel.h"
#include "application.h"

BEGIN_EVENT_TABLE(FeedPanel, wxHtmlListBox)
	EVT_COMMAND(wxID_ANY, wxEVT_FEED_UPDATED, FeedPanel::OnFeedUpdated)
	EVT_COMMAND(wxID_ANY, wxEVT_IMAGE_UPDATED, FeedPanel::OnImageUpdated)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_FEED_UPDATED)
DEFINE_EVENT_TYPE(wxEVT_IMAGE_UPDATED)

FeedPanel::FeedPanel(wxWindow* parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size,
					 long style, const wxString& name)
: wxHtmlListBox(parent, id, pos, size, style, name)
{
	Create(parent, id, pos, size, style, name);
}

void FeedPanel::Create(wxWindow* parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size,
					 long style, const wxString& name)
{
	wxHtmlListBox::Create(parent, id, pos, size, style, name);
}

void FeedPanel::SetFeed(const wxString& resource, int delay) 
{
	Twitter &twitter = wxGetApp().GetTwitter();

	if (feedResource != _T("")) {
		// unregister the old resource
		twitter.UnregisterListener(*this, feedResource);
		twitter.EndFeed(feedResource);
		SetItemCount(0); // flush the data
	}

	// Load any residual data in the feedResource
	feedResource = resource;
	TwitterFeed *feed = wxGetApp().GetTwitter().GetFeed(feedResource);
	if (feed) {
		SetItemCount(feed->GetStatuses().size());
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

void FeedPanel::OnFeedUpdated(wxCommandEvent &event)
{
	const TwitterFeed *feed = wxGetApp().GetTwitter().GetFeed(feedResource);
	if (feed) {
		SetItemCount(feed->GetStatuses().size());
	}
	else {
		SetItemCount(0);
	}
}

void FeedPanel::OnImageUpdated(wxCommandEvent &event)
{
}

wxString FeedPanel::OnGetItem(size_t n) const
{
	const TwitterFeed *feed = wxGetApp().GetTwitter().GetFeed(feedResource);
	if (feed == NULL) return _T("");

	size_t len = feed->GetStatuses().size() - 1;
	const TwitterStatus &status = feed->GetStatuses().at(len - n);
	const TwitterUser &user = status.GetUser();

	wxString list;
	list << _T("<table cellpadding='5'><tr><td valign='top'>");
	list << _T("<img id='") << user.GetId() <<_T("' width=48 height=48 src='");
	list << user.GetProfileImageFilename();
	list << _T("' align='left'>");
	list << _T("</td><td valign='top'>");
	list << _T("<b>") + user.GetScreenName() + _T("</b>: ");
	list << status.GetText();

	if (status.GetCreatedAt().IsValid()) {
		list << _T("<p><font color='#aaaaaa'>");
		list << status.GetCreatedAt().FormatISODate() << _T(" ");
		list << status.GetCreatedAt().FormatISOTime();
		list << _T("</font>");
	}
	list << _T("</td></tr></table>");
	return list;
}
