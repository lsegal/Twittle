#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>

class TwitterUser
{
	unsigned long long id;
	wxString name;
	wxString screen_name;
	wxString description;
	wxString location;
	wxString profile_image_url;
	wxString url;
	bool protected_;
	unsigned long followers_count;

public:
	TwitterUser(const wxXmlNode& node);

	void ParseXmlNode(const wxXmlNode& node);

	void GetProfileImage();
	const wxString GetProfileImageFilename() const;

	inline const unsigned long long GetId() const { return id; }
	inline const wxString& GetName() const { return name; }
	inline const wxString& GetScreenName() const { return screen_name; }
	inline const wxString& GetDescription() const { return description; }
	inline const wxString& GetLocation() const { return location; }
	inline const wxString& GetProfileImageUrl() const { return profile_image_url; }
	inline const wxString& GetUrl() const { return url; }
	inline const bool IsProtected() const { return protected_; }
	inline unsigned long GetFollowersCount() const { return followers_count; }
};
