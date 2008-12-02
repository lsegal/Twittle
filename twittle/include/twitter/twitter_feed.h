#pragma once

#include <vector>
#include "twitter_feed_serializer.h"

class wxThread;
class Twitter;
class TwitterStatus;

class TwitterFeed
{
	wxThread *thread;
	Twitter& client;
	unsigned int delay;
	wxString resource;
	std::vector<TwitterStatus> statuses;
	wxCriticalSection addSec;

protected:
	void Refresh();

public:
	TwitterFeed(Twitter& client, const wxString& res);
	~TwitterFeed();

	inline void SetDelay(unsigned int delay_) { delay = delay_; }
	inline const std::vector<TwitterStatus>& GetStatuses() const { return statuses; }

	void Start();
	void Pause();

	bool AddStatus(TwitterStatus& status);

	// serialization
	void Save();

	friend class Serializer<wxString, TwitterFeed>;
};
