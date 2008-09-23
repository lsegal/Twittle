#ifndef FEEDPANEL_H
#define FEEDPANEL_H 1

#include <vector>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/htmllbox.h>
#include "twitter/twitter.h"

class FeedPanel : public wxHtmlListBox
{
	std::vector<TwitterStatus> items;

public:
	FeedPanel() : wxHtmlListBox() { }
	FeedPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);
	void Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);

	wxString OnGetItem(size_t n) const;
	//wxCoord OnMeasureItem(size_t n) const;

	void Update(const wxString& resource);
};

#endif /* FEEDPANEL_H */
