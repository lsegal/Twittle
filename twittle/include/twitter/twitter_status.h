#ifndef TWITTERSTATUS_H
#define TWITTERSTATUS_H 1

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include "twitter_user.h"
#include <vector>

class TwitterStatus
{
public:
	wxString id;
	wxString created_at;
	wxString text;
	bool truncated;
	TwitterUser user;

	TwitterStatus(const wxXmlNode& node);

	void ParseXmlNode(const wxXmlNode& node);
};

typedef std::vector<TwitterStatus> TwitterStatuses;

#endif /* TWITTERSTATUS_H */
