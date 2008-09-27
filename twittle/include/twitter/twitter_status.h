#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include "twitter_user.h"
#include <vector>

class TwitterStatus
{
	wxString id;
	wxDateTime created_at;
	wxString text;
	bool truncated;
	TwitterUser user;

public:
	TwitterStatus(const wxXmlNode& node);

	void ParseXmlNode(const wxXmlNode& node);

	inline const wxString& GetId() const { return id; }
	inline const wxDateTime& GetCreatedAt() const { return created_at; }
	inline const wxString& GetText() const { return text; }
	inline const TwitterUser& GetUser() const { return user; }
	inline const bool isTruncated() const { return truncated; }
};

typedef std::vector<TwitterStatus> TwitterStatuses;
