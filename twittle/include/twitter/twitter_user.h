#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include "twitter_user.h"

class TwitterUser
{
	wxString id;
	wxString name;
	wxString screen_name;
	wxString description;
	wxString location;
	wxString profile_image_url;
	wxString url;
	bool protected_;
	unsigned long followers_count;

public:
	TwitterUser() { } //: profile_image_filename(_T("imgs/spacer.png")) { }
	TwitterUser(const wxXmlNode& node);

	void ParseXmlNode(const wxXmlNode& node);

	void GetProfileImage();
	const wxString GetProfileImageFilename() const;

	inline const wxString& GetId() const { return id; }
	inline const wxString& GetName() const { return name; }
	inline const wxString& GetScreenName() const { return screen_name; }
	inline const wxString& GetDescription() const { return description; }
	inline const wxString& GetLocation() const { return location; }
	inline const wxString& GetProfileImageUrl() const { return profile_image_url; }
	inline const wxString& GetUrl() const { return url; }
	inline const bool IsProtected() const { return protected_; }
	inline unsigned long GetFollowersCount() const { return followers_count; }
};
