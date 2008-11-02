#pragma once

#include <vector>
#include <wx/wx.h>
#include "twitter.h"
#include "twitter_status.h"

class Twitter;
class TwitterStatus;

class TwitterFeed
{
	Twitter& client;
	unsigned int delay;
	wxString resource;
	std::vector<TwitterStatus> statuses;
	wxThread *thread;

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
};