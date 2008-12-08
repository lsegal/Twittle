#pragma once

#include <wx/wx.h>

/**
 * Represents a specialized editbox control that shows a
 * label when the control is not focused (and there is no data
 * in the editbox). This can be used to give UI hints to the user
 * about what to enter into the textbox. 
 *
 * The control also has the ability to add text either by
 * appending it, inserting it or replacing an active selection.
 */
class StatusTextCtrl : public wxTextCtrl
{
public:
	/** 
	 * Default constructor which creates a standard editbox control.
	 * Use this when performing "two-step" construction with wxWidgets
	 * Create() methods.
	 */
	StatusTextCtrl();

	/** Do nothing. */
	virtual ~StatusTextCtrl() { }

	/**
	 * Inserts a URL (if it is a valid URL) to the editbox
	 * at the current insertion point, replacing any existing
	 * selections if there are any.
	 *
	 * Note: This method does nothing if the URL is not a valid URL.
	 *
	 * @param url the URL to insert
	 */
	void InsertUrl(const wxString& url);

	/**
	 * Returns whether or not this control has focus 
	 */
	bool IsActive() const;

	///
	/// @name Window Events
	///
	//@{

	/**
	 * Gains focus and removes label if it is set.
	 */
	void OnFocusGained(wxFocusEvent& evt);

	/**
	 * Loses focus and adds label if the editbox is empty.
	 */
	void OnFocusLost(wxFocusEvent& evt);

	DECLARE_EVENT_TABLE();

	//@}
};
