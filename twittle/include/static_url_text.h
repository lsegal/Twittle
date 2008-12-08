#pragma once

#include <wx/wx.h>

/**
 * Represents an static text control that is clickable as a URL.
 * When clicked, the default browser is loaded to the URL specified
 * by the label, using Application::OpenUrl.
 *
 * Currently this class is used only by the AboutDialog.
 *
 * @see Application::OpenUrl
 */
class StaticUrlText : public wxStaticText
{
public:
	/** 
	 * Creates a new control with a parent window, id and label representing the URL
	 *
	 * @param parent the parent window
	 * @param id the window identifier
	 * @param label the URL this control represents. The browser will be opened
	 *   to the value set by the label.
	 */
	StaticUrlText(wxWindow *parent, wxWindowID id, const wxString& label);

	///
	/// Window Events
	///
	//@{ 

	/** Sets the hand cursor when the mouse is moved over this control */
	void OnMouseMove(wxMouseEvent& evt);

	/** Turns off the hand cursor when the mouse leaves the boundaries of this control */
	void OnMouseLeave(wxMouseEvent& evt);

	/** 
	 * Calls Application::OpenUrl with the label contents when the user
	 * clicks on the label.
	 */
	void OnMouseClick(wxMouseEvent& evt);

	DECLARE_EVENT_TABLE()

	//@}
};
