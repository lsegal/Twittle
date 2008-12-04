#include <fstream>
#include <wx/wx.h>
#include <wx/file.h>
#include <wx/xml/xml.h>
#include "thread_callback.h"
#include "twitter/twitter_user.h"
#include "http/http_client.h"
#include "application.h"

TwitterUser::TwitterUser(const wxXmlNode& node)
{
	ParseXmlNode(node);

	if (!wxFile::Exists(GetProfileImageFilename())) {
		GetProfileImage();
	}
}

void TwitterUser::ParseXmlNode(const wxXmlNode& node)
{
	wxXmlNode *child = node.GetChildren();
	while (child) {
		const wxString& tagName = child->GetName();
		wxString value = child->GetNodeContent();
		if (tagName == _T("id")) {
			value.ToULongLong(&id);
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
	wxString basePath = wxGetApp().GetImagesPath();
	if (GetProfileImageUrl() == _T("")) {
		return basePath + _T("spacer.png");
	}

	wxString filename;
	filename << basePath << GetId();
	filename << _T("-") << GetProfileImageUrl().AfterLast(_T('/'));
	return filename;
}

void TwitterUser::GetProfileImage()
{
	if (GetProfileImageUrl() == _T("")) return;
	HttpClient().GetToFile(wxURL(GetProfileImageUrl()), GetProfileImageFilename());
}
