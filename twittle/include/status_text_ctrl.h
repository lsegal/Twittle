#pragma once

#include <wx/wx.h>

class StatusTextCtrl : public wxTextCtrl
{
public:
	StatusTextCtrl();
	virtual ~StatusTextCtrl() { }

	bool IsActive() const;

	void OnFocusGained(wxFocusEvent& evt);
	void OnFocusLost(wxFocusEvent& evt);

	DECLARE_EVENT_TABLE();
};
