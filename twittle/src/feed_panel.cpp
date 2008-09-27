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

	for (int i = items.size() - 1; i >= 0; i--) {
		items.at(i).GetUser().GetProfileImage();
	}
	evt.SetClientData(NULL);
	wxPostEvent(&panel, evt);

	wxSleep(10);
	FeedPanelUpdater::Update(url, panel);

	return 0;
}

void FeedPanelUpdater::Update(const wxString& url, FeedPanel& panel)
{
	FeedPanelUpdater *updater = new FeedPanelUpdater(url, panel);
	updater->Create();
	updater->Run();
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
	BeginUpdate(Twitter::PublicTimelineUrl);
}

void FeedPanel::SetItems(std::vector<TwitterStatus> items_)
{
	items = items_; 
 	SetItemCount(items.size());
}

void FeedPanel::BeginUpdate(const wxString& url)
{
	FeedPanelUpdater *updater = new FeedPanelUpdater(url, *this);
	updater->Create();
	updater->Run();
}

void FeedPanel::OnFeedUpdated(wxCommandEvent &event)
{
	if (event.GetClientData()) {
		SetItems(*static_cast<std::vector<TwitterStatus>*>(event.GetClientData()));
	}
	RefreshAll();
}

wxString FeedPanel::OnGetItem(size_t n) const
{
	const TwitterStatus &status = items.at(n);
	const TwitterUser &user = status.GetUser();

	wxString list;
	list << _T("<table cellpadding='5'><tr><td valign='top'>");
	list << _T("<img width=48 height=48 src='imgs/") + user.GetId() + _T(".png' align='left'>");
	list << _T("</td><td valign='top'>");
	list << _T("<b>") + user.GetScreenName() + _T("</b>: ");
	list << status.GetText();

	if (status.GetCreatedAt().IsValid()) {
		list << _T("<p>");
		list << status.GetCreatedAt().FormatISODate() << _T(" ");
		list << status.GetCreatedAt().FormatISOTime();
	}
	list << _T("</td></tr></table>");
	return list;
}
