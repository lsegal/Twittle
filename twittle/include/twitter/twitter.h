#pragma once

#include <map>
#include <vector>
#include <wx/wx.h>

class TwitterStatus;
class TwitterUser;
class TwitterFeed;
class TwitterUpdateListener;
class HttpClient;

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
	bool ResourceRequiresAuthentication(const wxString& resource) const;
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

	void SetPublicDelay(unsigned int delay);
	void SetFollowDelay(unsigned int delay);

	TwitterFeed* GetFeed(const wxString& resource) const;
	void LoadFeed(const wxString& resource, TwitterFeed *feed);

	TwitterUser *RetrieveUser(unsigned long long userid) const;
	inline const std::map<unsigned long long, TwitterUser *>& GetUsers() { return users; }

	// update status
	bool UpdateStatus(const wxString& message);

	// authentication
	void SetAuth(const wxString& user, const wxString& password);
	bool VerifyCredentials(const wxString& username_, const wxString& password_) const;
	void EndSession() const;

	// listener pattern
	void RegisterListener(TwitterUpdateListener& listener, const wxString& resource);
	void UnregisterListener(TwitterUpdateListener& listener, const wxString& resource);

	inline const wxString& GetUsername() const { return username; }
	inline const wxString& GetPassword() const { return password; }

	// constants
	static wxString TwitterBaseUrl;
	static wxString AccountBaseUrl;
	static wxString StatusesBaseUrl;
	static wxString PublicTimelineUrl;
	static wxString FriendsTimelineUrl;
	static wxString UpdateStatusUrl;

	friend class TwitterStatus;
	friend class TwitterFeed;
};
