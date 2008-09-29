/**
 * Implements the Twitter API
 *
 * @author Loren Segal
 */

#include "twitter/twitter.h"
#include <stdexcept>

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

bool Twitter::ResourceRequiresAuthentication(const wxString& resource) {
	if (resource == PublicTimelineUrl) {
		return false;
	}
	return true;
}

std::vector<TwitterStatus> Twitter::GetPublicTimeline()
{
	return GetStatuses(PublicTimelineUrl);
}

std::vector<TwitterStatus> Twitter::GetStatuses(const wxString& resource, const wxString& format)
{
	std::vector<TwitterStatus> statuses;
	HttpClient http;

	if (ResourceRequiresAuthentication(resource)) {
		SetAuth(http);
	}

	wxXmlDocument doc = http.GetXml(wxURL(StatusesBaseUrl + resource + _T(".") + format));
	wxXmlNode *root = doc.GetRoot();

	if (root == NULL) {
		throw std::logic_error("invalid XML document");
	}

	wxXmlNode *node = root->GetChildren();
	while (node) {
		if (node->GetName() == _T("status")) {
			statuses.push_back(TwitterStatus(*node));
		}

		node = node->GetNext();
	}

	return statuses;
}

TwitterStatus* Twitter::UpdateStatus(const wxString& message)
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
		return NULL;
	}
	else if (root.GetName() == _T("hash")) {
		// TODO generate twitterexception
		return NULL;
	}
	else if (root.GetName() == _T("status")) {
		return new TwitterStatus(root);
	}
	else {
		// TODO generate unknownexception
		return NULL;
	}
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
