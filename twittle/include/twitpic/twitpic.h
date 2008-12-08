#pragma once

#include <wx/string.h>

/**
 * Implements the TwitPic image uploading service API
 */
class TwitPic
{
	/**
	 * Helper function to get mime type from file extension.
	 * Returns generic 'application/octet-stream' if no mime type
	 * is found.
	 *
	 * @param extension the file extension to find a mime type for
	 * @return the mime type or application/octet-stream if none is found
	 */
	static wxString GetMimeType(const wxString& extension);

public:

	/**
	 * Uploads an image by binary data to the TwitPic service and returns the
	 * URL for the uploaded image.
	 *
	 * @param user the Twitter account username
	 * @param pass the Twitter account password
	 * @param buffer the contents of the image
	 * @param filename the image filename 
	 */
	static wxString UploadImage(const wxString& user, const wxString& pass,
		const wxString& buffer, const wxString& filename);

	/**
	 * Uploads an image file from the filesystem to TwicPic and returns the
	 * URL for the image. 
	 *
	 * @param user the Twitter account username
	 * @param pass the Twitter account password
	 * @param filename the image filename 
	 */
	static wxString UploadImage(const wxString& user, const wxString& pass, const wxString& filename);
};
