#include "twitter/twitter_feed.h"

wxCriticalSection addSec;

bool TwitterFeed::AddStatus(TwitterStatus& status)
{
	bool result = false;

	addSec.Enter();
	if (statuses.size() == 0 ||	statuses.at(statuses.size() - 1).GetId() < status.GetId()) {
		statuses.push_back(status);
		result = true;
	}
	addSec.Leave();

	return result;
}