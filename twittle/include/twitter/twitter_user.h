#pragma once

class wxString;
class wxXmlNode;

/**
 * Represents a Twitter user. A user will have many TwitterStatus objects, though
 * they are not accessible from this object. All users are registered in the Twitter
 * user registry (Twitter::RegisterUser) and can be retrieved by Twitter::RetrieveUser.
 */
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
	/**
	 * Creates a TwitterUser object from XML.
	 *
	 * @param node the root XML node representing the data
	 */
	TwitterUser(const wxXmlNode& node);

	/** 
	 * Parses the XML node and updates member data. Called by constructor.
	 *
	 * @param node the root XML node representing the data
	 */
	void ParseXmlNode(const wxXmlNode& node);

	/**
	 * Performs an HTTP request to retrieve the profile image and store it locally
	 * on disk.
	 */
	void GetProfileImage();

	/**
	 * Returns the profile image filename where the profile image is (or should be)
	 * stored.
	 */
	const wxString GetProfileImageFilename() const;

	/** Returns the user's ID. */
	inline const unsigned long long GetId() const { return id; }

	/** Returns the user's full name. */
	inline const wxString& GetName() const { return name; }

	/** Returns the user's short name or screen name. */
	inline const wxString& GetScreenName() const { return screen_name; }

	/** Returns the user's "description". */
	inline const wxString& GetDescription() const { return description; }

	/** Returns the user's location. */
	inline const wxString& GetLocation() const { return location; }

	/** 
	 * Returns the user's profile image URL (remote file). 
	 * See GetProfileImageFilename for the local file.
	 */
	inline const wxString& GetProfileImageUrl() const { return profile_image_url; }
	
	/** Returns the user's personal website (if set). */
	inline const wxString& GetUrl() const { return url; }

	/** Returns the user's protected mode (?). */
	inline const bool IsProtected() const { return protected_; }

	/** Returns the number of followers the user has. */
	inline unsigned long GetFollowersCount() const { return followers_count; }
};
