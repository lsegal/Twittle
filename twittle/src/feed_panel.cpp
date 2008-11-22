#include <wx/wx.h>
#include <wx/mimetype.h>
#include <wx/regex.h>
#include "feed_panel.h"
#include "application.h"
#include "twitter/twitter_feed.h"
#include "twitter/twitter_user.h"
#include "twitter/twitter_status.h"

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
: wxHtmlListBox(parent, id, pos, size, style, name)
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

void FeedPanel::SetFeed(const wxString& resource, int delay) 
{
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
	cmd.Replace(_T("file://"), _T(""));
	wxExecute(cmd);
	delete ft;
}