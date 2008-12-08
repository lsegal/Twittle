#pragma once

#include <map>
#include <vector>
#include <wx/wx.h>

class TwitterStatus;
class TwitterUser;
class TwitterFeed;
class TwitterUpdateListener;
class HttpClient;

/**
 * The heart and soul of Twittle, implementing the Twitter API.
 *
 * There are three components to the implementation of this class:
 * feeds, users and notifications.
 *
 * This class implements a collection of feeds which can be added to the
 * list. Feeds are defined by a "resource" which is a partial URL identifier.
 * They each represent a set of statuses from the URL much like an RSS feed.
 * Each added feed is periodically updated in an independent thread and 
 * status items are added to them. The feeds can be retrieved by perfoming
 * a lookup in the feed collection by its resource name (see Twitter::GetFeed).
 * 
 * Whenever a status item is added to a feed, a user must be attached. A user
 * might own many status items in a feed, or many feeds. To conserve memory 
 * space as well as improve synchronization of user information, this class 
 * implements a centralized registry of TwitterUser objects. Therefore when a 
 * status item is added, it will perform a lookup (Twitter::RetrieveUser) to see if 
 * the user exists in the registry before creating a new one. In the case where
 * a user is not found, it will be registered with #RegisterUser.
 *
 * Finally, a Twitter implementation is not efficient if the client must
 * poll the data generated by the feeds (after all, the purpose of the feeds 
 * themselves are to solve the problem of "polling"). Therefore, the class
 * implements an observer pattern (through TwitterUpdateListener) to allow
 * clients to register for updates with the class. Registration for updates
 * is allowed only for clients implementing the TwitterUpdateListener interface
 * and is done on a resource (feed) by resource (feed) basis. That is, a 
 * client must register itself for each feed it wishes to follow to avoid 
 * unwanted notifications.
 */
class Twitter
{
private:
	// auth info
	wxString username;
	wxString password;

	// feeds
	std::map<wxString, TwitterFeed*> feeds;
	std::map<unsigned long long, TwitterUser*> users;

	// listeners
	std::multimap<wxString, TwitterUpdateListener*> listeners;

protected:
	/**
	 * Whether or not the specific resource URL is public or if it requires
	 * user authentication to be retrieved. In all cases but #PublicTimelineUrl,
	 * this should return true.
	 */
	bool ResourceRequiresAuthentication(const wxString& resource) const;

	/**
	 * Helper method to set the authentication information on an HttpClient
	 * object.
	 */
	void SetAuth(HttpClient& http);

	/** 
	 * Registers a user to the registry
	 *
	 * @param userid the user's id to add
	 * @param user the twitter user to associate with the id
	 */
	void RegisterUser(unsigned long long userid, TwitterUser* user);

	/** 
	 * Notifies all registered listeners for a specific resource 
	 */
	void NotifyListeners(const wxString& resource) const;

	/**
	 * Notifies all listeners for all resources (usually called when an image is obtained)
	 */
	void NotifyAllListeners() const;

public:
	///
	/// @name Constants
	///
	//@{ 

	static wxString TwitterBaseUrl;			///< The base URL for the Twitter API calls
	static wxString AccountBaseUrl;			///< The base URL for account methods (session calls)
	static wxString StatusesBaseUrl;		///< The base URL for feed resources
	static wxString UpdateStatusUrl;		///< The base URL to send status updates to
	static wxString PublicTimelineUrl;		///< The feed resource name representing the public feeds
	static wxString FriendsTimelineUrl;		///< The feed resource name representing feeds you are following
	static int DefaultFeedDelay;			///< The default feed delay

	//@}

	///
	/// @name Constructor / Destructors
	///
	//@{ 

	/** Default constructor does nothing */
	Twitter() { }

	/**
	 * Constructs a new Twitter object with user credentials. 
	 *
	 * Note that the auth data is never used until a feed is requested.
	 *
	 * @param username the Twitter username 
	 * @param password the Twitter password
	 */
	Twitter(const wxString& username, const wxString& password);

	/** 
	 * Unregister all listeners, attempt to logout (see Twitter::EndSession) and clear
	 * all feeds and user data.
	 */
	virtual ~Twitter();

	//@}

	///
	/// @name Authentication
	///
	//@{

	/**
	 * Sets the authentication information for a user/password
	 *
	 * @param user the username
	 * @param pass the password
	 */
	void SetAuth(const wxString& user, const wxString& pass);

	/**
	 * Verifies the validity of a username and password against the Twitter servers
	 *
	 * @param username the username
	 * @param password the password
	 * @return true if the username / password combination are valid
	 */
	bool VerifyCredentials(const wxString& username, const wxString& password) const;

	/**
	 * Performs a form of logging out handshake with the remote server.
	 */
	void EndSession() const;

	/** Returns the current authentication username */
	inline const wxString& GetUsername() const { return username; }

	/** Returns the current authentication password */
	inline const wxString& GetPassword() const { return password; }

	//@}

	///
	/// @name Feed Manipulation
	///
	//@{

	/**
	 * Adds a feed to the feed update pool by resource name (which creates a new TwitterFeed object)
	 *
	 * @param resource the resource locator of the feed to update
	 * @param delay the time to wait between updates of this feed
	 */
	void BeginFeed(const wxString& resource, int delay = DefaultFeedDelay);

	/**
	 * Returns a feed by the resource name
	 *
	 * @param resource the resource name to look up a TwitterFeed object for.
	 * @return a TwitterFeed object on success or NULL if one could not be found
	 *   for the given resource name.
	 */
	TwitterFeed* GetFeed(const wxString& resource) const;

	/**
	 * Adds a feed to the feed update pool. This can be done from an external source
	 * (via an XML file deserialized from the local disk, for instance).
	 *
	 * @param resource the resource name to use for the feed
	 * @param feed the feed to insert into the pool
	 */
	void LoadFeed(const wxString& resource, TwitterFeed *feed);

	//@}

	///
	/// @name User Registry
	///
	//@{

	/**
	 * Performs a lookup on a user by their id and returns the respective user or NULL.
	 *
	 * @param userid the user id to look up
	 * @return the TwitterUser with the id or NULL if no user was found
	 */
	TwitterUser *RetrieveUser(unsigned long long userid) const;

	/**
	 * Returns a the map of all the users (for iteration).
	 */
	inline const std::map<unsigned long long, TwitterUser *>& GetUsers() { return users; }

	//@}

	/// @name Observer Notifications
	///
	//@{

	/**
	 * Registers a listener to receive updates whenever the feed identified by the resource
	 * is updated.
	 *
	 * @param listener the listener to register for updates
	 * @param resource the feed resource name to receive updates for
	 */
	void RegisterListener(TwitterUpdateListener& listener, const wxString& resource);

	/**
	 * Unregisters a listener from receiving updates for a given feed (by resource name)
	 *
	 * @param listener the listener to unregister
	 * @param resource the feed resource name
	 */
	void UnregisterListener(TwitterUpdateListener& listener, const wxString& resource);

	//@}

	/// @name Miscellaneous Functionality
	//@{

	/**
	 * Sends a status update from this user with a given status message. The length
	 * limit is 140 characters on a message. Anything longer will be truncated by the server.
	 *
	 * @param message the status text to send as an update
	 * @return whether the update was successful
	 */
	bool UpdateStatus(const wxString& message);
	
	//@}

	friend class TwitterStatus;
	friend class TwitterFeed;
};
