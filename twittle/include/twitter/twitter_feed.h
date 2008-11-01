#pragma once

#include <vector>
#include <wx/wx.h>
#include "twitter.h"
#include "twitter_status.h"

class Twitter;
class TwitterStatus;

class TwitterFeed
{
	const Twitter& client;
	bool enabled;
	unsigned int delay;
	std::vector<TwitterStatus> statuses;

protected:
	bool AddStatus(TwitterStatus& status);

public:
	TwitterFeed(const Twitter& client) : enabled(false), client(client) { }
	inline void SetDelay(unsigned int delay_) { delay = delay_; }
	inline unsigned int Delay() { return delay; }
	inline void Enable() { enabled = true; }
	inline void Disable() { enabled = false; }
	inline bool IsEnabled() const { return enabled; }
	inline const std::vector<TwitterStatus>& GetStatuses() const { return statuses; }

	friend class Twitter;
};