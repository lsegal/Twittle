#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include "twitter_status.h"

class Twitter
{
	wxString username;
	wxString password;

public:
	Twitter() : username(_T("")), password(_T("")) { }
	Twitter(wxString username, wxString password) : 
		username(username), password(password) { }

	std::vector<TwitterStatus> GetPublicTimeline();
	std::vector<TwitterStatus> GetStatuses(const wxString& resource, const wxString& format = _T("xml"));

	static wxString Twitter::StatusesBaseUrl;
	static wxString Twitter::PublicTimelineUrl;

	inline const wxString& GetUserName() const { return username; }
	inline const wxString& GetPassword() const { return password; }
	inline void SetPassword(const wxString& password_) { password = password_; }
	inline void SetUserName(const wxString& username_) { username = username_; }
};
