#pragma once

#include <wx/wx.h>

/**
 * Implementation of http://is.gd API
 */
class Isgd
{
public:
	/**
	 * Returns the shortened url from is.gd or wxEmptyString
	 * if an error occurred.
	 */
	static wxString ShortenUrl(const wxString& url);
};
