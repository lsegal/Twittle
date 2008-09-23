#include "twitter/twitter_user.h"

TwitterUser::TwitterUser(const wxXmlNode& node)
{
	ParseXmlNode(node);
}

void TwitterUser::ParseXmlNode(const wxXmlNode& node)
{
	wxXmlNode *child = node.GetChildren();
	while (child) {
		const wxString& tagName = child->GetName();
		wxString value = child->GetNodeContent();
		if (tagName == _T("id")) {
			id = value;
		}
		else if (tagName == _T("name")) {
			name = value;
		}
		else if (tagName == _T("screen_name")) {
			screen_name = value;
		}
		else if (tagName == _T("description")) {
			description = value;
		}
		else if (tagName == _T("location")) {
			location = value;
		}
		else if (tagName == _T("profile_image_url")) {
			profile_image_url = value;
		}
		else if (tagName == _T("url")) {
			url = value;
		}
		else if (tagName == _T("protected")) {
			protected_ = value == _T("true") ? true : false;
		}
		else if (tagName == _T("followers_count")) {
			value.ToULong(&followers_count);
		}

		child = child->GetNext();
	}
}

