#pragma once

#include <wx/wx.h>

class wxXmlNode;
class Twitter;
class TwitterUser;

/**
 * Represents a status update from a Twitter user. The most important components
 * of this object are the GetText() and GetUser() methods.
 */
class TwitterStatus
{
	unsigned long long id;
	wxDateTime created_at;
	wxString text;
	wxString source;
	bool truncated;
	TwitterUser *user;

public:
	/** 
	 * Creates a new status object by reading data from an XML node.
	 *
	 * @param twitter the twitter object that holds the user registry
	 * @param node the XML root node representing the status item
	 */
	TwitterStatus(Twitter& twitter, const wxXmlNode& node);

	/** 
	 * Parses an XML node (called from constructor) and fills the
	 * member data.
	 *
	 * @param twitter the twitter object that holds the user registry
	 * @param node the XML root node representing the status item
	 */
	void ParseXmlNode(Twitter& twitter, const wxXmlNode& node);

	/**
 	 * Returns the time difference from the posting time to now in a 
	 * natural English format.
	 */
	wxString GetTimeSincePost() const;

	/** Returns the unique identifier of the status item */
	inline const unsigned long long GetId() const { return id; }

	/** Returns the date the item was posted */
	inline const wxDateTime& GetCreatedAt() const { return created_at; }

	/** Returns the text in the status update */
	inline const wxString& GetText() const { return text; }

	/** 
	 * Returns the source (where it was posted from). This usually contains
	 * 'web' (for tweets from the website), 'txt' (for text message tweets)
	 * or a third party application name (like 'twittle') with an embedded
	 * link to the application website.
	 */
	inline const wxString& GetSource() const { return source; }

	/** Returns the user associated with this posting */
	inline const TwitterUser& GetUser() const { return *user; }

	/** Returns whether or not this posting was truncated by the system */
	inline const bool IsTruncated() const { return truncated; }
};

