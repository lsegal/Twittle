#ifndef TWITTER_H
#define TWITTER_H 1

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include "http_client.h"
#include "twitter_status.h"

class Twitter
{
	wxString username;
	wxString password;
	HttpClient http;

public:
	Twitter() : username(_T("")), password(_T("")) { }
	Twitter(wxString username, wxString password) : 
		username(username), password(password) { }

	std::vector<TwitterStatus> GetPublicTimeline();
	std::vector<TwitterStatus> GetStatuses(const wxString& resource, const wxString& format = _T("xml"));

	static wxString Twitter::StatusesBaseUrl;
	static wxString Twitter::PublicTimelineUrl;
};

#endif /* TWITTER_H */
