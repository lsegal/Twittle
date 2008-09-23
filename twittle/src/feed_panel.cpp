#include "feed_panel.h"

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
	Update(Twitter::PublicTimelineUrl);
}

void FeedPanel::Update(const wxString& resource)
{
	items = Twitter().GetStatuses(resource);
	SetItemCount(items.size());
}

wxString FeedPanel::OnGetItem(size_t n) const
{
	const TwitterStatus &status = items.at(n);
	const TwitterUser &user = status.user;

	wxString list;
	list << _T("<table cellpadding='5'><tr><td valign='top'>");
	list << _T("<img src='") + items.at(n).user.profile_image_url + _T("' align='left'>");
	list << _T("</td><td valign='top'>");
	list << _T("<b>") + user.screen_name + _T("</b>: ");
	list << status.text;
	list << _T("</td></tr></table>");
	return list;
}
