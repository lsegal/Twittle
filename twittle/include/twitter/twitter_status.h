#pragma once

#include <wx/wx.h>

class wxXmlNode;
class Twitter;
class TwitterUser;

class TwitterStatus
{
	unsigned long long id;
	wxDateTime created_at;
	wxString text;
	bool truncated;
	TwitterUser *user;

public:
	TwitterStatus(Twitter& twitter, const wxXmlNode& node);

	void ParseXmlNode(Twitter& twitter, const wxXmlNode& node);

	inline const unsigned long long GetId() const { return id; }
	inline const wxDateTime& GetCreatedAt() const { return created_at; }
	inline const wxString& GetText() const { return text; }
	inline const TwitterUser& GetUser() const { return *user; }
	inline const bool IsTruncated() const { return truncated; }
};

