#include "feed_panel.h"
#include "application.h"

BEGIN_EVENT_TABLE(FeedPanel, wxHtmlListBox)
	EVT_COMMAND(wxID_ANY, wxEVT_FEED_UPDATED, FeedPanel::OnFeedUpdated)
	EVT_COMMAND(wxID_ANY, wxEVT_IMAGE_UPDATED, FeedPanel::OnImageUpdated)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxEVT_FEED_UPDATED)
DEFINE_EVENT_TYPE(wxEVT_IMAGE_UPDATED)

FeedImageUpdater::FeedImageUpdater(TwitterStatus &item, unsigned int index, FeedPanel &panel) :
	wxThread(wxTHREAD_DETACHED), items(*new std::vector<TwitterStatus>),
	panel(panel), index(index), singleitem(true)
{
	items.push_back(item);
}

FeedImageUpdater::~FeedImageUpdater()
{
	//if (singleitem) {
		// hack to delete a reference type that was dynamically allocated
		std::vector<TwitterStatus> *i = &items;
		delete i;
	//}
}

void* FeedImageUpdater::Entry()
{
	for (unsigned int i = 0; i < items.size(); i++) {
		// Cast to non-const TwitterUser because GetProfileImage modified
		// TwitterUser object. We generally don't want modifications, but
		// in this case we explicitly allow it.
		static_cast<TwitterUser>(items.at(i).GetUser()).GetProfileImage();

		wxCommandEvent evt(wxEVT_IMAGE_UPDATED, wxID_ANY);
		wxPostEvent(&panel, evt);
	}

	return 0;
}

void FeedImageUpdater::Update(std::vector<TwitterStatus>& items, FeedPanel& panel)
{
	FeedImageUpdater *updater = new FeedImageUpdater(items, panel);
	updater->Create();
	updater->Run();
}

void FeedImageUpdater::Update(TwitterStatus& item, unsigned int index, FeedPanel& panel)
{
	FeedImageUpdater *updater = new FeedImageUpdater(item, index, panel);
	updater->Create();
	updater->Run();
}

void* FeedPanelUpdater::Entry()
{
	std::vector<TwitterStatus> *items =
		new std::vector<TwitterStatus>(wxGetApp().GetTwitter().GetStatuses(url));

	wxCommandEvent evt(wxEVT_FEED_UPDATED, wxID_ANY);
	evt.SetClientData(static_cast<void*>(items));
	wxPostEvent(&panel, evt);

	Sleep(180000);
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
	FeedPanelUpdater::Update(Twitter::FriendsTimelineUrl, *this);
}

void FeedPanel::AddItem(TwitterStatus& item)
{
	items.insert(items.begin(), item);
}

void FeedPanel::AddItems(std::vector<TwitterStatus>& items_)
{
	std::vector<TwitterStatus>::reverse_iterator item;
	for (item = items_.rbegin(); item != items_.rend(); item++) {
		std::vector<TwitterStatus>::iterator item2;
		bool match = false;
		for (item2 = items.begin(); item2 != items.end(); item2++) {
			if ((*item2).GetId() == (*item).GetId()) {
				match = true;
				break;
			}
		}
		if (match) break;
		AddItem(*item);
	}
	SetItemCount(items.size());

	FeedImageUpdater::Update(items_, *this);
}

void FeedPanel::OnFeedUpdated(wxCommandEvent &event)
{
	if (event.GetClientData()) {
		AddItems(*static_cast<std::vector<TwitterStatus>*>(event.GetClientData()));
	}

	RefreshAll();
}

void FeedPanel::OnImageUpdated(wxCommandEvent &event)
{
	RefreshAll();
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
