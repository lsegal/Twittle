#pragma once

#include <wx/wx.h>
#include "serializer.h"

class TwitterFeed;
class wxXmlNode;
class TwitterStatus;
class TwitterUser;

template<>
class Serializer<wxString, TwitterFeed>
{
private:
	static wxXmlNode *StatusAsXml(const TwitterStatus& status);
	static wxXmlNode *UserAsXml(const TwitterUser& user);

public:
	static TwitterFeed *Read(const wxString& path);
	static void Write(const wxString& path, const TwitterFeed& feed);
};