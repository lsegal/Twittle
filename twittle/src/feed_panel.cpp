#include "feed_panel.h"

BEGIN_EVENT_TABLE(FeedPanel, wxHtmlListBox)
	EVT_COMMAND(wxID_ANY, wxEVT_FEED_UPDATED, FeedPanel::OnFeedUpdated)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_FEED_UPDATED)

void* FeedPanelUpdater::Entry()
{
	std::vector<TwitterStatus> items = Twitter().GetStatuses(url);

	wxCommandEvent evt(wxEVT_FEED_UPDATED, wxID_ANY);
	evt.SetClientData(static_cast<void*>(&items));
	wxPostEvent(&panel, evt);

	evt.SetClientData(NULL);
	for (unsigned int i = 0; i < items.size(); i++) {
		// Cast to non-const TwitterUser because GetProfileImage modified
		// TwitterUser object. We generally don't want modifications, but
		// in this case we explicitly allow it.
		static_cast<TwitterUser>(items.at(i).GetUser()).GetProfileImage();

		evt.SetExtraLong(i);
		wxPostEvent(&panel, evt);
	}


	return 0;
}

void FeedPanelUpdater::Update(const wxString& url, FeedPanel& panel)
{
	FeedPanelUpdater *updater = new FeedPanelUpdater(url, panel);
	updater->Create();
	updater->Run();
}

void FeedPanelUpdater::OnExit()
{
	wxSleep(10);
	FeedPanelUpdater::Update(url, panel);
}

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
	SetItemCount(0);
	FeedPanelUpdater::Update(Twitter::PublicTimelineUrl, *this);
}

void FeedPanel::SetItems(std::vector<TwitterStatus> items_)
{
	items = items_; 
 	SetItemCount(items.size());
}

void FeedPanel::OnFeedUpdated(wxCommandEvent &event)
{
	if (event.GetClientData()) {
		SetItems(*static_cast<std::vector<TwitterStatus>*>(event.GetClientData()));
	}

	RefreshAll();
}

void FeedPanel::OnImageUpdated(wxCommandEvent &event)
{
	RefreshLine(event.GetExtraLong());
}

wxString FeedPanel::OnGetItem(size_t n) const
{
	const TwitterStatus &status = items.at(n);
	const TwitterUser &user = status.GetUser();

	wxString list;
	list << _T("<table cellpadding='5'><tr><td valign='top'>");
	list << _T("<img width=48 height=48 src='");
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
