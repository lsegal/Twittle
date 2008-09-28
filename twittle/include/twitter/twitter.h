#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include "twitter_status.h"
#include "http_client.h"

class Twitter
{
	wxString username;
	wxString password;

	void SetAuth(HttpClient& http);

protected:
	bool ResourceRequiresAuthentication(const wxString& resource);

public:
	Twitter() : username(_T("twittletest")), password(_T("twittle")) { }
	Twitter(const wxString& username, const wxString& password);
	void SetAuth(const wxString& user, const wxString& password);

	std::vector<TwitterStatus> GetPublicTimeline();
	std::vector<TwitterStatus> GetStatuses(const wxString& resource, const wxString& format = _T("xml"));

	TwitterStatus* UpdateStatus(const wxString& message);

	static wxString Twitter::StatusesBaseUrl;
	static wxString Twitter::PublicTimelineUrl;
	static wxString Twitter::UpdateStatusUrl;
};
