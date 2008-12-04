#include <vector>
#include <wx/wx.h>
#include "twitter/twitter.h"
#include "twitter/twitter_status.h"
#include "twitter/twitter_feed.h"
#include "twitter_feed_serializer.h"
#include "thread_callback.h"
#include "http/http_client.h"

using namespace std;

TwitterFeed::TwitterFeed(Twitter& cli, const wxString& res) :
	thread(NULL), client(cli), resource(res)
{
}

TwitterFeed::~TwitterFeed()
{
	Save();

	// wxWidgets HANGS on socket operations
	// if you try to Delete() the thread...
	// let's not worry about it at all, then.
	//	thread->Delete();
	//	thread->Wait();
	// Note: Kill() seems to work fine... for now
	thread->Kill(); 

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
	bool exists = false, result = false;

	addSec.Enter();
	
	// check for existing status in the list
	std::vector<TwitterStatus>::const_reverse_iterator it;
	for (it = statuses.rbegin(); it != statuses.rend(); ++it) {
		if (it->GetId() == status.GetId()) exists = true;
	}

	if (!exists) {
		// add status since it does not yet exist
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

				try {
					TwitterStatus status(client, **it);
					if (AddStatus(status)) {
						// notify listeners if the status was added
						client.NotifyListeners(resource);
					}
				}
				catch (...) {
					// parsing the node might have failed.
				}
			}

			// notify listeners if the status was added
			//	NotifyListeners(resource);
		}

		// What a perfect opportunity to save this resource
		Save();

		// Delay but make sure we can leave the thread easily
		for (unsigned int i = 0; i < delay; i++) {
			thread->TestDestroy();
			wxSleep(1);
		}
	}
}

void TwitterFeed::Save()
{
	addSec.Enter();
	
	Serializer<wxString, TwitterFeed>::Write(resource, *this);
	
	addSec.Leave();
}