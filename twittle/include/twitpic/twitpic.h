#pragma once

#include <wx/wx.h>

class wxMemoryOutputStream;

/**
 * Implements the TwitPic image uploading service API
 */
class TwitPic
{
	/**
	 * Helper function to get mime type from file extension.
	 * Returns generic 'application/octet-stream' if no mime type
	 * is found.
	 */
	static wxString GetMimeType(const wxString& extension);

public:

	/**
	 * Uploads an image by binary data to the TwitPic service and returns the
	 * URL for the uploaded image
	 */
	static wxString UploadImage(const wxString& user, const wxString& pass,
		const wxString& buffer, const wxString& filename);

	/**
	 * Uploads an image file from the filesystem to TwicPic and returns the
	 * URL for the image
	 */
	static wxString UploadImage(const wxString& user, const wxString& pass, const wxString& filename);
};
