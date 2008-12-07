#include "static_url_text.h"
#include "application.h"

BEGIN_EVENT_TABLE(StaticUrlText, wxStaticText)
	EVT_MOTION(StaticUrlText::OnMouseMove)
	EVT_LEAVE_WINDOW(StaticUrlText::OnMouseLeave)
	EVT_LEFT_UP(StaticUrlText::OnMouseClick)
END_EVENT_TABLE()

StaticUrlText::StaticUrlText(wxWindow *parent, wxWindowID id, const wxString& label) : 
  wxStaticText(parent, id, label)
{
	SetOwnForegroundColour(*wxBLUE);
	wxFont font = GetFont();
	font.SetUnderlined(true);
	SetOwnFont(font);
}

void StaticUrlText::OnMouseMove(wxMouseEvent& evt)
{
	wxSetCursor(wxCURSOR_HAND);
}

void StaticUrlText::OnMouseLeave(wxMouseEvent& evt)
{
	wxSetCursor(wxCURSOR_ARROW);
}

void StaticUrlText::OnMouseClick(wxMouseEvent& evt)
{
	wxGetApp().OpenUrl(GetLabel());
}
