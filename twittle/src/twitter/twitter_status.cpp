#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <stdlib.h>
#include "twitter/twitter.h"
#include "twitter/twitter_user.h"
#include "twitter/twitter_status.h"
#include "win32/compat.h"

TwitterStatus::TwitterStatus(Twitter& twitter, const wxXmlNode& node) : source(_T("web"))
{
	ParseXmlNode(twitter, node);
}

void TwitterStatus::ParseXmlNode(Twitter& twitter, const wxXmlNode& node)
{
	wxXmlNode *child = node.GetChildren();
	while (child) {
		const wxString& name = child->GetName();
		wxString value = child->GetNodeContent();
		if (name == _T("id")) {
			id = strtoull(value.mb_str(), NULL, 10);
			// value.ToULongLong(&id); (GCC implementations don't like this!)
		}
		else if (name == _T("created_at")) {
			try {
				created_at.ParseFormat(value, _T("%a %b %d %H:%M:%S +0000 %Y"));
			}
			catch (...) {
				// generally should not happen
			}
		}
		else if (name == _T("text")) {
			text = value;
		}
		else if (name == _T("source")) {
			source = value;
		}
		else if (name == _T("truncated")) {
			truncated = value == _T("true") ? true : false;
		}
		else if (name == _T("user")) {
			// Try to look up the user in the registry by the ID
			wxXmlNode *sub = child->GetChildren();
			user = NULL;
			while (sub) {
				if (sub->GetName() == _T("id")) {
					unsigned long long tid;
					tid = strtoull(sub->GetNodeContent().mb_str(), NULL, 10);
					// sub->GetNodeContent().ToULongLong(&tid); (GCC implementations don't like this!)
					TwitterUser *tmp = twitter.RetrieveUser(tid);
					if (tmp) {
						user = tmp;
						user->ParseXmlNode(*child);
						break;
					}
				}
				sub = sub->GetNext();
			}

			// No user is registered, register a new user in the registry
			if (user == NULL) {
				user = new TwitterUser(*child);
				twitter.RegisterUser(user->GetId(), user);
			}
		}


		child = child->GetNext();
	}
}

wxString TwitterStatus::GetTimeSincePost() const
{
	long long tMin = 60, tHour = 3600, tDay = 86400;
	wxDateTime now = wxDateTime::Now().ToUTC();
	wxTimeSpan diff = now - created_at;
	long long diffSecs = diff.GetSeconds().ToLong(); // diff in seconds

	if (diffSecs < tMin) { // show in seconds
		return wxString::Format(_T("%ds ago"), diffSecs);
	}
	else if (diffSecs < tHour) { // show in minutes
		return wxString::Format(_T("about %dm ago"), diffSecs / tMin);
	}
	else if (diffSecs < tDay) { // show in hours
		return wxString::Format(_T("about %dh ago"), diffSecs / tHour);
	}
	else { // show in days
		return wxString::Format(_T("about %dd ago"), diffSecs / tDay);
	}
}

