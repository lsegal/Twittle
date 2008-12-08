#pragma once

#include <wx/wx.h>

/**
 * Implements multipart/form-data for HttpClient.
 *
 * Note that this does not set the Content-Type header for the
 * request. This is still only done manually.
 *
 * Also note that this class is not suitable for streaming large
 * files since it passes the entire contents of the file around
 * instead of reading chunks from a stream.
 */
class Multipart
{
	wxString boundary;
	wxString buffer;

public:
	/** Creates a new Multipart object with default bounary string */
	Multipart() : boundary(_T("MaGiCaLBoUnDaRyStRiNg")) { }

	/**
	 * Sets a boundary string. Changing this is usually not required.
	 *
	 * @param newboundary the new boundary string to use 
	 */
	void SetBoundary(const wxString& newboundary) { boundary = newboundary; }

	/** 
	 * Ends a multipart/form-data request by appending the final boundary string
	 */
	void End();

	/** 
	 * Retrieves the full form request contents as a string
	 */
	const wxString& GetString() { return buffer; }

	/** 
	 * Returns the boundary, used for setting the Content-type header.
	 */
	const wxString& GetBoundary() { return boundary; }

	/**
	 * Adds basic string data to the form request.
	 *
	 * @param name the parameter name
	 * @param value the parameter value
	 */
	void Add(const wxString& name, const wxString& value);

	/** 
	 * Adds a file (encoded as binary) to the form request.
	 *
	 * @param name the parameter name
	 * @param contents the file contents as a binary string 
	 * @param filename the filename (required by the HTTP specification)
	 * @param mimeType the mime type representing the file contents
	 */
	void AddFile(const wxString& name, const wxString& contents,
		const wxString& filename = wxEmptyString,
		const wxString& mimeType = _T("application/octet-stream"));
};
