#pragma once

#include <wx/wx.h>

class StaticUrlText : public wxStaticText
{
public:
	StaticUrlText(wxWindow *parent, wxWindowID id, const wxString& label);

	// Events
	void OnMouseMove(wxMouseEvent& evt);
	void OnMouseLeave(wxMouseEvent& evt);
	void OnMouseClick(wxMouseEvent& evt);

	DECLARE_EVENT_TABLE()
};
