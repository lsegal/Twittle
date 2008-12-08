#pragma once

#include <wx/wx.h>
#include "twitter.h"

class Twitter;

/** 
 * An interface for the observer pattern used by Twitter to notify listeners of Twitter updates.
 * This is mainly used by FeedPanel to receive its updates
 */
class TwitterUpdateListener
{
public:
	/** Allow overriding */
	virtual ~TwitterUpdateListener() { }

	/** 
	 * Implement this method to handle an update notification from Twitter on the string 
	 * resource identifier.
	 *
	 * @param twitter the twitter object which initiated the notification
	 * @param resource the resource name representing the feed that was updated. Use Twitter::GetFeed
	 *   to retrieve the feed associated with the resource.
	 */
	virtual void TwitterUpdateReceived(const Twitter& twitter, const wxString& resource) = 0;
};
