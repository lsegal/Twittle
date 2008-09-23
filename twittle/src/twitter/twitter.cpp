/**
 * Implements the Twitter API
 *
 * @author Loren Segal
 */

#include "twitter/twitter.h"
#include <stdexcept>

wxString Twitter::StatusesBaseUrl = _T("http://twitter.com/statuses/");
wxString Twitter::PublicTimelineUrl = _T("public_timeline");

std::vector<TwitterStatus> Twitter::GetPublicTimeline()
{
	return GetStatuses(PublicTimelineUrl);
}

std::vector<TwitterStatus> Twitter::GetStatuses(const wxString& resource, const wxString& format)
{
	std::vector<TwitterStatus> statuses;

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
