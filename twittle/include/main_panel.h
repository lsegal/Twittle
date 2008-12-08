#pragma once

#include <wx/wx.h>
#include <wx/toolbar.h>
#include "status_text_ctrl.h"
#include "feed_panel.h"

class MainWindow;

/**
 * The main panel after the user logs into the application
 */
class MainPanel : public wxPanel
{
	/// Control identifiers
	enum ControlIdentifiers {
		ID_EDIT,		///< The editbox
		ID_CONTENT,		///< The FeedPanel listbox
		ID_TINYURL,		///< Link button
		ID_TWITPIC,		///< Twitpic image upload button
		ID_COUNTER,		///< Character counter 
		ID_PUBLIC,		///< View public feeds button
		ID_FRIEND,		///< View friend feeds button
		ID_FILTER_AT	///< Filter by replies button
	};

	StatusTextCtrl editbox;
	FeedPanel content;
	wxBitmapButton tinyurl;
	wxBitmapButton twitpic;
	wxStaticText charcounter;
	wxFont font;
	wxBitmapButton publicButton;
	wxBitmapButton followButton;
	wxBitmapButton atButton;
	wxBoxSizer *buttonSizer;
	wxBoxSizer *editSizer;

	/** Initializes all components */
	void InitializeComponents();

	/** Orders panel controls by user settings (editbox on top / editbox on bottom) */
	void SetPanelOrder();

	/** Sets up all the keyboard accelerators (Ctrl+L for link shortening) */
	void SetAccelerators();

public:
	/** Attaches panel to parent window (MainWindow) */
	MainPanel(wxWindow *parent);

	/** Cleans up any twitter objects */
	virtual ~MainPanel();

	/** 
	 * Shortens a URL (thread callback) and replaces the string shortUrl
	 * with the shortened URL.
	 *
	 * @warning this code runs from another thread and should not attempt
	 *   to directly manipulate the GUI.
	 *
	 * @param shortUrl the URL to be shortened. This will be replaced on
	 *   return with the shortened URL, or the empty string if an error
	 *   occurred.
	 */
	void ShortenUrl(wxString& shortUrl);

	/** 
	 * Called after options have been changed and the feed panel UI needs to be updated.
	 */
	void ForceUpdateUI();

	///
	/// @name Window Events
	///
	//@{

	/** 
	 * Handles filter/feed button clicks and changes the
	 * FeedPanel's feed contents with FeedPanel::SetFeed
	 */
	void OnButtonClick(wxCommandEvent& evt);

	/**
	 * Handles the clicking of the image upload button
	 * and shows the ImagePreviewDialog
	 *
	 * @post If the image is uploaded successfully, the URL to the image
	 *   is appended (or inserted) to the editbox.
	 */
	void OnImageClick(wxCommandEvent& evt);

	/**
	 * Handles the shorten URL button click (or Ctrl+L accelerator).
	 * If no text is selected in the editbox or if the text is not a valid
	 * URL, an input dialog will be shown to request the URL to shorten,
	 * otherwise the selected text is used as the URL.
	 */
	void OnShortenUrl(wxCommandEvent& evt);

	/**
	 * Handles typing in the editbox, updates the character counter
	 * to show how many characters have been typed. Twitter has a 140
	 * character limit on its messages, so this is important for usability.
	 */
	void OnEditText(wxCommandEvent& evt);

	/**
	 * Handles the enter key being pressed in the editbox, sending
	 * the existing editbox text to Twitter::UpdateStatus and clearing
	 * the contents.
	 */
	void OnEditEnter(wxCommandEvent& evt);

	/**
	 * Handles image files being dropped on the main window and
	 * shows the ImagePreviewDialog for image uploading.
	 *
	 * @pre the file dropped on the window is a valid image file
	 */
	void OnDropFiles(wxDropFilesEvent& evt);

	DECLARE_EVENT_TABLE()

	//@}

	friend class MainWindow;
};
