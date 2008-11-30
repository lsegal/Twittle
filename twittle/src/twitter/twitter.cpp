/**
 * Implements the Twitter API
 *
 * @author Loren Segal
 */

#include <stdexcept>
#include <utility>
#include <wx/xml/xml.h>
#include "twitter/twitter_status.h"
#include "twitter/twitter_update_listener.h"
#include "twitter/twitter_feed.h"
#include "twitter/twitter_user.h"
#include "twitter/twitter.h"
#include "thread_callback.h"
#include "http_client.h"


using namespace std;

wxString Twitter::TwitterBaseUrl = _T("http://twitter.com/");
wxString Twitter::AccountBaseUrl = Twitter::TwitterBaseUrl + _T("account/");
wxString Twitter::StatusesBaseUrl = Twitter::TwitterBaseUrl + _T("statuses/");
wxString Twitter::PublicTimelineUrl = _T("public_timeline");
wxString Twitter::FriendsTimelineUrl = _T("friends_timeline");
wxString Twitter::UpdateStatusUrl = _T("update");

Twitter::Twitter(const wxString& username, const wxString& password)
{
	SetAuth(username, password);
}

Twitter::~Twitter()
{
	// clear listeners
	listeners.clear();

	// end session
	if (username != _T("")) {
		EndSession();
	}

	// delete feeds
	map<wxString, TwitterFeed*>::iterator it;
	for (it = feeds.begin(); it != feeds.end(); ++it) {
		delete it->second;
	}

	// delete users
	map<unsigned long long, TwitterUser*>::iterator it2;
	for (it2 = users.begin(); it2 != users.end(); ++it2) {
		delete it2->second;
	}
}

void Twitter::SetAuth(const wxString& username_, const wxString& password_)
{
	username = username_;
	password = password_;
}

void Twitter::SetAuth(HttpClient& http)
{
	http.SetUser(username);
	http.SetPassword(password);
}

bool Twitter::ResourceRequiresAuthentication(const wxString& resource) const {
	if (resource == PublicTimelineUrl) {
		return false;
	}
	return true;
}

void Twitter::RegisterUser(unsigned long long userid, TwitterUser *user)
{
	users.insert(make_pair(userid, user));
}

TwitterUser* Twitter::RetrieveUser(unsigned long long userid) const
{
	map<unsigned long long, TwitterUser*>::const_iterator it = users.find(userid);
	return it != users.end() ? it->second : NULL;
}

void Twitter::BeginFeed(const wxString& resource, int delay)
{
	TwitterFeed *feed;

	map<wxString, TwitterFeed*>::iterator it;
	it = feeds.find(resource);
	if (it == feeds.end()) {
		feed = new TwitterFeed(*this, resource);
		LoadFeed(resource, feed);
	}
	else {
		feed = it->second;
	}

	feed->SetDelay(delay);
	feed->Start();
}

void Twitter::LoadFeed(const wxString& resource, TwitterFeed *feed) 
{
	feeds.insert(make_pair(resource, feed));
}

TwitterFeed* Twitter::GetFeed(const wxString& resource) const
{
	std::map<wxString,TwitterFeed*>::const_iterator it = feeds.find(resource);
	return it != feeds.end() ? it->second : NULL;
}

bool Twitter::UpdateStatus(const wxString& message)
{
	HttpClient http;
	wxString resource = UpdateStatusUrl;

	if (ResourceRequiresAuthentication(resource)) {
		SetAuth(http);
	}

	http.SetPostBuffer(_T("status=") + HttpClient::UrlEncode(message));
	wxXmlDocument doc = http.GetXml(wxURL(StatusesBaseUrl + resource + _T(".xml")));
	wxXmlNode root = *doc.GetRoot();

	// check for errors
	if (http.GetResponse() != 200) {
		// TODO generate exception, use xml result
		return false;
	}
	else if (root.GetName() == _T("hash")) {
		// TODO generate twitterexception
		return false;
	}
	else if (root.GetName() == _T("status")) {
		// Add to follow feed if it exists
		TwitterFeed *feed = GetFeed(FriendsTimelineUrl);
		TwitterStatus status(*this, root);
		if (feed && feed->AddStatus(status)) {
			// notify listeners
			NotifyListeners(FriendsTimelineUrl);
		}
		return true;
	}

	// TODO generate unknownexception
	return false;
}

bool Twitter::VerifyCredentials(const wxString& username_, const wxString& password_) const
{
	HttpClient http(username_, password_);
	wxString result = http.Get(wxURL(AccountBaseUrl + _T("verify_credentials.xml")));
	return (http.GetResponse() == 200) ? true : false;
}

void Twitter::EndSession() const
{
	HttpClient(username, password).Get(wxURL(AccountBaseUrl + _T("end_session.xml")));
}

void Twitter::RegisterListener(TwitterUpdateListener& listener, const wxString& resource)
{
	listeners.insert( make_pair(resource, &listener) );
}

void Twitter::UnregisterListener(TwitterUpdateListener& listener, const wxString& resource)
{
	multimap<wxString, TwitterUpdateListener*>::iterator it;
	pair<multimap<wxString, TwitterUpdateListener*>::iterator,
		multimap<wxString, TwitterUpdateListener*>::iterator> range;
	range = listeners.equal_range(resource);

	for (it = range.first; it != range.second; ++it) {
		if (it->second == &listener) {
			listeners.erase(it);
			break;
		}
	}
}

void Twitter::NotifyListeners(const wxString& resource) const
{
	multimap<wxString, TwitterUpdateListener*>::const_iterator it;
	pair<multimap<wxString, TwitterUpdateListener*>::const_iterator,
		multimap<wxString, TwitterUpdateListener*>::const_iterator> range;
	range = listeners.equal_range(resource);

	for (it = range.first; it != range.second; ++it) {
		it->second->TwitterUpdateReceived(*this, resource);
	}
}

void Twitter::NotifyAllListeners() const
{
	multimap<wxString, TwitterUpdateListener*>::const_iterator it;
	for (it = listeners.begin(); it != listeners.end(); ++it) {
		it->second->TwitterUpdateReceived(*this, it->first);
	}
}
