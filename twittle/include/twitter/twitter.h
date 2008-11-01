#pragma once

#include <map>
#include <vector>
#include <wx/wx.h>
#include <wx/xml/xml.h>
#include "twitter_status.h"
#include "twitter_update_listener.h"
#include "twitter_feed.h"
#include "twitter_user.h"
#include "http_client.h"

class TwitterStatus;
class TwitterUser;
class TwitterFeed;

class Twitter
{
private:
	// auth info
	wxString username;
	wxString password;

	// feeds
	std::map<wxString, TwitterFeed*> feeds;
	std::map<unsigned long long, TwitterUser*> users;

	// listeners
	std::multimap<wxString, TwitterUpdateListener*> listeners;

protected:
	bool ResourceRequiresAuthentication(const wxString& resource);
	void SetAuth(HttpClient& http);

	void RegisterUser(unsigned long long userid, TwitterUser* user);
	void RefreshFeed(const wxString &resource);

	void NotifyListeners(const wxString& resource) const;
	void NotifyAllListeners() const;

public:
	Twitter() { }
	Twitter(const wxString& username, const wxString& password);
	~Twitter();

	void BeginFeed(const wxString& resource, int delay = 300);
	void EndFeed(const wxString& resource);

	void SetPublicDelay(unsigned int delay);
	void SetFollowDelay(unsigned int delay);

	TwitterFeed* GetFeed(const wxString& resource) const;

	TwitterUser *RetrieveUser(unsigned long long userid) const;

	std::vector<TwitterStatus> GetPublicTimeline();
	std::vector<TwitterStatus> GetStatuses(const wxString& resource, const wxString& format = _T("xml"));

	// update status
	bool UpdateStatus(const wxString& message);

	// authentication
	void SetAuth(const wxString& user, const wxString& password);
	bool VerifyCredentials(const wxString& username_, const wxString& password_) const;
	void EndSession() const;

	// listener pattern
	void RegisterListener(TwitterUpdateListener& listener, const wxString& resource);
	void UnregisterListener(TwitterUpdateListener& listener, const wxString& resource);

	// constants
	static wxString TwitterBaseUrl;
	static wxString AccountBaseUrl;
	static wxString StatusesBaseUrl;
	static wxString PublicTimelineUrl;
	static wxString FriendsTimelineUrl;
	static wxString UpdateStatusUrl;

	friend class TwitterStatus;
};
