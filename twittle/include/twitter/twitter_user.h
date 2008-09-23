#ifndef TWITTERUSER_H
#define TWITTERUSER_H 1

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include "twitter_user.h"

class TwitterUser
{
public:
	wxString id;
	wxString name;
	wxString screen_name;
	wxString description;
	wxString location;
	wxString profile_image_url;
	wxString url;
	bool protected_;
	unsigned long followers_count;

	TwitterUser() { }
	TwitterUser(const wxXmlNode& node);

	void ParseXmlNode(const wxXmlNode& node);
};

#endif /* TWITTERUSER_H */
