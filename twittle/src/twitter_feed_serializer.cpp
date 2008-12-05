#include <iostream>
#include <wx/wx.h>
#include <wx/file.h>
#include <wx/xml/xml.h>
#include "application.h"
#include "twitter/twitter_feed.h"
#include "twitter/twitter_status.h"
#include "twitter/twitter_user.h"
#include "twitter_feed_serializer.h"

TwitterFeed *Serializer<wxString, TwitterFeed>::Read(const wxString& path)
{
	wxXmlDocument doc;
	wxString fullpath = wxGetApp().GetFeedsPath() + path + _T(".xml");

	// file does not exist, don't even try to load it
	if (!wxFile::Exists(fullpath)) {
		return NULL;
	}

	bool loadable = doc.Load(fullpath, _T("UTF-8"));
	if (!loadable) {
		return NULL;
	}

	wxXmlNode *root = doc.GetRoot();

	// create the feed
	TwitterFeed *feed = new TwitterFeed(wxGetApp().GetTwitter(), root->GetPropVal(_T("resource"), _T("")));
	root->GetPropVal(_T("delay"), _T("300")).ToULong((unsigned long *)&feed->delay);

	wxXmlNode *schild = root->GetChildren();
	while (schild) {
		if (schild->GetName() == _T("statuses")) {
			wxXmlNode *inner = schild->GetChildren();
			while (inner) {
				if (inner->GetName() == _T("status")) {
					TwitterStatus s(feed->client, *inner);
					feed->AddStatus(s);
				}

				inner = inner->GetNext();
			}
		}

		schild = schild->GetNext();
	}


	return feed;
}

void Serializer<wxString, TwitterFeed>::Write(const wxString& path, const TwitterFeed& feed)
{
	wxXmlDocument doc;
	wxString fullpath = wxGetApp().GetFeedsPath() + path + _T(".xml");

	wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, _T("feed"));
	doc.SetRoot(root);

	root->AddProperty(_T("delay"), wxString::Format(_T("%d"), feed.delay));
	root->AddProperty(_T("resource"), feed.resource);

	// statuses
	wxXmlNode *statuses = new wxXmlNode(wxXML_ELEMENT_NODE, _T("statuses"));
	std::vector<TwitterStatus>::const_iterator it;
	for (it = feed.statuses.begin(); it != feed.statuses.end(); ++it) {
		statuses->AddChild(StatusAsXml(*it));
	}

	root->AddChild(statuses);

	doc.Save(fullpath);
}

wxXmlNode *Serializer<wxString, TwitterFeed>::StatusAsXml(const TwitterStatus &status)
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("status"));
	wxXmlNode *id = new wxXmlNode(wxXML_ELEMENT_NODE, _T("id"));
	wxXmlNode *truncated = new wxXmlNode(wxXML_ELEMENT_NODE, _T("truncated"));
	wxXmlNode *text = new wxXmlNode(wxXML_ELEMENT_NODE, _T("text"));
	wxXmlNode *created_at = new wxXmlNode(wxXML_ELEMENT_NODE, _T("created_at"));
	wxXmlNode *source = new wxXmlNode(wxXML_ELEMENT_NODE, _T("source"));

	id->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), wxString::Format(_T("%d"), status.GetId())));
	truncated->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), status.IsTruncated() ? _T("true") : _T("false")));
	text->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), status.GetText()));
	source->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), status.GetSource()));
	created_at->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), status.GetCreatedAt().Format(_T("%a %b %d %H:%M:%S +0000 %Y"))));

	node->AddChild(id);
	node->AddChild(truncated);
	node->AddChild(created_at);
	node->AddChild(text);
	node->AddChild(source);
	node->AddChild(UserAsXml(status.GetUser()));

	return node;
}

wxXmlNode *Serializer<wxString, TwitterFeed>::UserAsXml(const TwitterUser &user)
{
	wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, _T("user"));
	wxXmlNode *id = new wxXmlNode(wxXML_ELEMENT_NODE, _T("id"));
	wxXmlNode *name = new wxXmlNode(wxXML_ELEMENT_NODE, _T("name"));
	wxXmlNode *screen_name = new wxXmlNode(wxXML_ELEMENT_NODE, _T("screen_name"));
	wxXmlNode *description = new wxXmlNode(wxXML_ELEMENT_NODE, _T("description"));
	wxXmlNode *location = new wxXmlNode(wxXML_ELEMENT_NODE, _T("location"));
	wxXmlNode *profile_image_url = new wxXmlNode(wxXML_ELEMENT_NODE, _T("profile_image_url"));
	wxXmlNode *url = new wxXmlNode(wxXML_ELEMENT_NODE, _T("url"));
	wxXmlNode *protected_ = new wxXmlNode(wxXML_ELEMENT_NODE, _T("protected"));
	wxXmlNode *followers_count = new wxXmlNode(wxXML_ELEMENT_NODE, _T("followers_count"));

	id->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), wxString::Format(_T("%d"), user.GetId())));
	name->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), user.GetName()));
	screen_name->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), user.GetScreenName()));
	description->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), user.GetDescription()));
	location->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), user.GetLocation()));
	profile_image_url->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), user.GetProfileImageUrl()));
	url->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), user.GetUrl()));
	protected_->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), user.IsProtected() ? _T("true") : _T("false")));
	followers_count->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), wxString::Format(_T("%d"), user.GetFollowersCount())));

	node->AddChild(id);
	node->AddChild(name);
	node->AddChild(screen_name);
	node->AddChild(description);
	node->AddChild(location);
	node->AddChild(profile_image_url);
	node->AddChild(url);
	node->AddChild(protected_);
	node->AddChild(followers_count);

	return node;
}
