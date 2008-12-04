#pragma once

#include <wx/wx.h>

class StatusTextCtrl : public wxTextCtrl
{
public:
	StatusTextCtrl();
	virtual ~StatusTextCtrl() { }

	void InsertUrl(const wxString& url);

	bool IsActive() const;

	void OnFocusGained(wxFocusEvent& evt);
	void OnFocusLost(wxFocusEvent& evt);

	// @override wxTextCtrl
	bool CanPaste();
	void Paste();

	DECLARE_EVENT_TABLE();
};
