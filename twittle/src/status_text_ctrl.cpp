#include "status_text_ctrl.h"

BEGIN_EVENT_TABLE(StatusTextCtrl, wxTextCtrl)
	EVT_SET_FOCUS(StatusTextCtrl::OnFocusGained)
	EVT_KILL_FOCUS(StatusTextCtrl::OnFocusLost)
END_EVENT_TABLE()

static wxColour INACTIVE_COLOUR(160, 160, 160);

StatusTextCtrl::StatusTextCtrl() : wxTextCtrl()
{
	OnFocusLost(wxFocusEvent());
}

bool StatusTextCtrl::IsActive() const
{
	return GetForegroundColour() != INACTIVE_COLOUR;
}

void StatusTextCtrl::OnFocusGained(wxFocusEvent& evt)
{
	if (!IsActive()) {
		SetOwnForegroundColour(*wxBLACK);
		SetValue(_T(""));
	}
}

void StatusTextCtrl::OnFocusLost(wxFocusEvent& evt)
{
	if (GetValue() == _T("")) {
		SetOwnForegroundColour(INACTIVE_COLOUR);
		SetValue(_T("What are you doing right now?"));
		SetSelection(0, 0);
		SetInsertionPoint(0);
	}
}
