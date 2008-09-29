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

	static wxString Twitter::TwitterBaseUrl;
	static wxString Twitter::AccountBaseUrl;
	static wxString Twitter::StatusesBaseUrl;
	static wxString Twitter::PublicTimelineUrl;
	static wxString Twitter::UpdateStatusUrl;
};
