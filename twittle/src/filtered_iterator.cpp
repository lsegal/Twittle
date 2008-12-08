#include "filtered_iterator.h"
#include "application.h"
#include "twitter/twitter.h"
#include "twitter/twitter_user.h"

bool FilteredIterator::Valid()
{
	if (iter == iterend) return true;

	if (filter & FILTER_REPLIES) {
		if (iter->GetText()[0] != _T('@')) return false;
		if (wxGetApp().GetTwitter().GetUsername() ==
			iter->GetUser().GetScreenName()) return false;
	}
	return true;
}
