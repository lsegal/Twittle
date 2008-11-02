#include "twitter/twitter_feed.h"
#include "thread_callback.h"

using namespace std;

static wxCriticalSection addSec;

TwitterFeed::TwitterFeed(Twitter& cli, const wxString& res) :
	thread(NULL), client(cli), resource(res)
{
}

TwitterFeed::~TwitterFeed()
{
	thread->Delete();
	thread->Wait();
	delete thread;

	statuses.clear();
}

void TwitterFeed::Start()
{
	if (thread) {
		thread->Resume();
	}
	else {
		// callback to thread
		thread = new ThreadCallback<TwitterFeed>(*this, &TwitterFeed::Refresh);
	}
}

void TwitterFeed::Pause() 
{
	if (thread) thread->Pause();
}

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

void TwitterFeed::Refresh()
{
	HttpClient http;

	if (client.ResourceRequiresAuthentication(resource)) {
		client.SetAuth(http);
	}

	while (true) {
		thread->TestDestroy();

		wxXmlDocument doc = http.GetXml(wxURL(Twitter::StatusesBaseUrl + resource + _T(".xml")));
		wxXmlNode *root = doc.GetRoot();

		if (root != NULL) {
			wxXmlNode *node = root->GetChildren();

			// create a list of the status nodes
			vector<wxXmlNode*> nodes;
			while (node) { 
				if (node->GetName() == _T("status")) {
					nodes.push_back(node);
				}
				node = node->GetNext();
			}

			// read nodes backwards
			vector<wxXmlNode*>::reverse_iterator it;
			for (it = nodes.rbegin(); it != nodes.rend(); ++it) {
				thread->TestDestroy();

				if (AddStatus(TwitterStatus(client, **it))) {
					// notify listeners if the status was added
					client.NotifyListeners(resource);
				}
			}

			// notify listeners if the status was added
			//	NotifyListeners(resource);
		}

		// Delay but make sure we can leave the thread easily
		for (unsigned int i = 0; i < delay; i++) {
			thread->TestDestroy();
			wxSleep(1);
		}
	}
}