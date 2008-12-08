#pragma once

#include <vector>
#include "twitter_feed_serializer.h"

class wxThread;
class Twitter;
class TwitterStatus;

/**
 * Represents a collection of TwitterStatus updates from a resource name (part of a URL).
 *
 * This class will periodically update the collection in a thread which is updated at a specified delay.
 */
class TwitterFeed
{
	wxThread *thread;					 ///< The thread running the periodic udpates.
	Twitter& client;					 ///< The Twitter owner object.
	unsigned int delay;					 ///< The time delay to perform updates.
	wxString resource;					 ///< The feed resource (partial) URL.
	std::vector<TwitterStatus> statuses; ///< The collection of status items.
	wxCriticalSection addSec;

protected:
	/**
	 * The thread callback to periodically refresh the feed.
	 */
	void Refresh();

public:
	/**
	 * Initializes a feed for a specified resource URL but does not start reading the feed
	 * until Start() is called.
	 *
	 * @param client the owner of this feed
	 * @param res the (partial) URL resource locator name
	 */
	TwitterFeed(Twitter& client, const wxString& res);

	/** 
	 * Cleans up all status objects (saving them to disk first).
	 */
	virtual ~TwitterFeed();

	/**
	 * Sets the delay time.
	 *
	 * @param delay_ the new delay time
	 */
	inline void SetDelay(unsigned int delay_) { delay = delay_; }

	/**
	 * Returns the collection of TwitterStatus objects that this feed represents.
	 */
	inline const std::vector<TwitterStatus>& GetStatuses() const { return statuses; }

	/** Starts the thread and continually updates the feed at a delay. */
	void Start();

	/** Pauses the feed (update thread) temporarily. */
	void Pause();

	/** 
	 * Adds a status item to the collection. 
	 * This method is public because not all status items are necessarily
	 * available from the resource URL provided by Twitter, so it is possible
	 * that status items can (and should) be added manually from disk or 
	 * other means.
	 *
	 * @param status the stats item to add to the collection
	 */
	bool AddStatus(TwitterStatus& status);

	/** Flushes the feed to disk using Serializer<wxString,TwitterFeed> */
	void Save();

	friend class Serializer<wxString, TwitterFeed>;
};
