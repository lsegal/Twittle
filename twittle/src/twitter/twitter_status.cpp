#include "twitter/twitter_status.h"

TwitterStatus::TwitterStatus(const wxXmlNode& node)
{
	ParseXmlNode(node);
}

void TwitterStatus::ParseXmlNode(const wxXmlNode& node)
{
	wxXmlNode *child = node.GetChildren();
	while (child) {
		const wxString& name = child->GetName();
		wxString value = child->GetNodeContent();
		if (name == _T("id")) {
			id = value;
		}
		else if (name == _T("created_at")) {
			created_at.ParseFormat(value, _T("%a %b %d %H:%M:%S +0000 %Y"));
		}
		else if (name == _T("text")) {
			text = value;
		}
		else if (name == _T("truncated")) {
			truncated = value == _T("true") ? true : false;
		}
		else if (name == _T("user")) {
			user.ParseXmlNode(*child);
		}


		child = child->GetNext();
	}
}
