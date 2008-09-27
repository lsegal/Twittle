#include <string>
#include <fstream>
#include "twitter/twitter_user.h"
#include "http_client.h"

TwitterUser::TwitterUser(const wxXmlNode& node)
{
	TwitterUser();
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

const wxString TwitterUser::GetProfileImageFilename() const
{
	if (GetProfileImageUrl() == _T("")) {
		return _T("imgs/spacer.png");
	}

	wxString filename;
	filename << _T("imgs/") << GetId();
	filename << _T(".") << GetProfileImageUrl().AfterLast('.');
	return filename;
}

void TwitterUser::GetProfileImage()
{
	HttpClient http;
	void *data = http.GetRaw(wxURL(GetProfileImageUrl()));
	int len = http.GetContentLength();

	mkdir("imgs");
	std::ofstream file(GetProfileImageFilename().c_str());
	file.write(static_cast<const char *>(data), len);
	file.close();

	delete data;
}
