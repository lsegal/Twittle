#pragma once

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
