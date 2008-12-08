#pragma once

#include <vector>
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/htmllbox.h>
#include "twitter/twitter.h"
#include "twitter/twitter_update_listener.h"
#include "filtered_iterator.h"

DECLARE_EVENT_TYPE(EVT_FEED_UPDATED, -1)
DECLARE_EVENT_TYPE(EVT_IMAGE_UPDATED, -2)

/**
 * Represents the listbox where TwitterStatus items are displayed of 
 * a specific TwitterFeed object (one at a time). The feed can be changed
 * with #SetFeed, which registers with the feed for updates (using the
 * observer pattern).
 *
 * @see #SetFeed 
 * @see TwitterUpdateListener
 */
class FeedPanel : public wxHtmlListBox, public TwitterUpdateListener
{
private:
	/** The control window IDs */
	enum ControlIdentifiers {
		ID_UPDATEUI, ID_COPYTEXT, ID_COPYHTML
	};

	wxTimer updateUITimer;
	wxString feedResource;
	unsigned int filter;
	wxMenu itemMenu;

	/** Creates the item popup menu when a user right clicks on a status item */
	void CreateItemMenu();

	/** Creates the keyboard accelerators for accessing menu items via the keyboard */
	void CreateAccelerators();

protected:
	/**
	 * Decorates the Twitter status text with HTML, linking all URLs and
	 * reply or hashtag words.
	 *
	 * @param text the status text to decorate
	 * @return a decorated version (in HTML) of a Twitter status text.
	 */
	wxString DecorateStatusText(wxString text) const;

	/**
	 * Decorates the Twitter source text. Used to change the link
	 * colour since the source already has an HTML link tag in it.
	 *
	 * @param text the source text to decorate
	 * @return the source text decorated with HTML (link colours changed)
	 */
	wxString DecorateSource(wxString text) const;

	/**
	 * Gets the size of the feed with filtering applied using
	 * the FilteredIterator. This is used when you only want
	 * to show "@reply" tweets, for instance (filter is set to FilteredIterator::FILTER_REPLIES).
	 *
	 * @warning Use this function instead of getting the feed size directly,
	 *   as they may not match.
	 *
	 * @return the size of the feed with filters applied 
	 * @see AddFilter
	 * @see FilteredIterator
	 */
	unsigned int GetStatusSize() const;

	/**
	 * Gets a status item at index n of the filtered results. 
	 *
	 * @warning Use this instead of accessing the feed directly.
	 *
	 * @return the nth TwitterStatus item after filtering the feed results
	 * @see FilteredIterator
	 */
	const TwitterStatus GetStatusItem(unsigned int n) const;

	/**
	 * Creates a tray notification (MainWindow::TrayNotification) with
	 * the proper information from a TwitterStatus.
	 *
	 * @post a tray notification should be displayed
	 * @param status the TwitterStatus object to show a tray notification for
	 * @see MainWindow::TrayNotification
	 */
	void StatusNotification(const TwitterStatus& status);

public:
	/** 
	 * Default constructor, use this when using "two step constructon". Call #Create
	 * after constructing with this constructor.
	 *
	 * @see #Create
	 */
	FeedPanel();

	/** 
	 * Creates a feed panel with a parent and any panel settings and calls #Create for you
	 */
	FeedPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);

	/**
	 * Destructor. Unregisters the panel as a listener for Twitter updates 
	 */
	virtual ~FeedPanel();

	/**
	 * Performs window initialization for the object
	 */
	void Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxHtmlListBoxNameStr);


	///
	/// @name Feed Filtering
	///
	// @{

	/**
	 * Sets the feed to follow by a resource name (see Twitter constants).
	 * This method will try to load the feed object from disk (via Twitter::LoadFeed)
	 * if one exists.
	 *
	 * @param resource the feed resource name to follow
	 * @param delay delay between feed updates
	 */
	void SetFeed(const wxString &resource, int delay = Twitter::DefaultFeedDelay);

	/** 
	 * Resets the filtering done on the feed contents 
	 *
	 * @see FilteredIterator
	 */
	void ResetFilter() { filter = FilteredIterator::FILTER_NONE; SetItemCount(GetStatusSize()); }

	/**
	 * Adds a filter to sort contents by.
	 *
	 * @param ftype the FilteredIterator::FilterTypes type to add 
	 * @see FilteredIterator
	 */
	void AddFilter(unsigned int ftype) { 
		filter |= ftype; SetItemCount(GetStatusSize()); }

	/**
	 * Removes a filter for content sorting.
	 *
	 * @param ftype the FilteredIterator::FilterTypes type to remove
	 * @see FilteredIterator
	 */
	void RemoveFilter(unsigned int ftype) { 
		filter &= ~ftype; SetItemCount(GetStatusSize()); }

	// @}

	///
	/// @name Implements wxHtmlListBox
	/// 
	// @{

	/**
	 * Overrides the wxHtmlListBox functionality to retrieve
	 * the list item at index n. Should return an HTML string
	 * representing the list item.
	 * 
	 * @param n the item index to retrieve
	 * @return the HTML string contents of the item
	 */
	wxString OnGetItem(size_t n) const;

	/**
	 * Overrides the basic selected list item text colour.
	 *
	 * @return currently returns wxBLACK
	 */
	wxColour GetSelectedTextColour(const wxColour& colFg) const;

	// @}

	///
	/// @name Implements TwitterUpdateListener
	///
	// @{

	/**
	 * Implements the TwitterUpdateListener observer interface and handles a
	 * status update when registered with a specific feed. 
	 *
	 * @warning this call will likely come from another thread and should not
	 *   try to directly access the wx UI controls (not thread-safe) and should
	 *   post an event instead.
	 *
	 * @pre resource == #feedResource && twitter == Application::GetTwitter()
	 * @post refreshes the listbox contents
	 */
	void TwitterUpdateReceived(const Twitter& twitter, const wxString& resource);

	// @}

	///
	/// @name Window Events
	///
	// @{

	/**
	 * Called by #TwitterUpdateReceived when it receives an update
	 */
	void OnFeedUpdated(wxCommandEvent &event);

	/**
	 * Currently unused.
	 */
	void OnImageUpdated(wxCommandEvent &event);

	/**
	 * Activated when a link cell is clicked in a list item
	 * Opens the link in the default browser
	 *
	 * @see Application::OpenUrl
	 */
	void OnLinkClicked(wxHtmlLinkEvent &event);


	// Popup menu events

	/** Activated when a list item is right clicked. Shows popup menu */
	void OnRightClick(wxMouseEvent &event);

	/** 
	 * Activated when user clicks the Copy as Text popup menu item. 
	 * Copies status text to clipboard.
	 *
	 * @pre clipboard is accessible
	 * @post status text is added to clipboard contents
	 */
	void CopyItemAsText(wxCommandEvent& evt);

	/** 
	 * Activated when user clicks the Copy as HTML popup menu item. 
	 * Copies status text to clipboard as it is seen in the list box (HTML decorated).
	 *
	 * @pre clipboard is accessible
	 * @post status text (as HTML) is added to clipboard contents
	 */
	void CopyItemAsHtml(wxCommandEvent& evt);

	/**
	 * Handles a periodic timer event to refresh the listbox contents
	 */
	void OnUpdateUI(wxTimerEvent& evt);

	DECLARE_EVENT_TABLE();

	// @}
};
