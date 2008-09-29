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
	Twitter() { }
	Twitter(const wxString& username, const wxString& password);
	void SetAuth(const wxString& user, const wxString& password);

	std::vector<TwitterStatus> GetPublicTimeline();
	std::vector<TwitterStatus> GetStatuses(const wxString& resource, const wxString& format = _T("xml"));

	TwitterStatus* UpdateStatus(const wxString& message);

	bool VerifyCredentials(const wxString& username_, const wxString& password_) const;
	void EndSession() const;

	static wxString TwitterBaseUrl;
	static wxString AccountBaseUrl;
	static wxString StatusesBaseUrl;
	static wxString PublicTimelineUrl;
	static wxString FriendsTimelineUrl;
	static wxString UpdateStatusUrl;
};
