#pragma once

#include <wx/wx.h>
#include "thread_callback.h"

/**
 * Previews an image and allows the user to upload it with
 * the Twitpic API, returning a URL location to the uploaded image.
 *
 * Use this dialog by showing it modal with ShowModal() and then
 * use #GetUrl() to retrieve the URL. For example:
 * @code
 *   ImagePreviewDialog dlg(this);
 *   if (dlg.ShowModal() == wxID_OK) {
 *     wxString url = dlg.GetUrl(); 
 *     // ...use the URL here...
 *   }
 * @endcode
 *
 */
class ImagePreviewDialog : public wxDialog
{
	enum ControlIdentifiers { ID_CHOOSEIMAGE };

	ThreadCallback1<ImagePreviewDialog, wxString> *callback;

	wxString imageUrl;
	wxString imageFilename;
	wxButton chooseImageButton;
	wxStaticBitmap imagePreview;
	wxStaticText progress;

	/** Initializes window components */
	void InitializeComponents();

	/** Shows the image specified by the image filename */
	void ShowImagePreview();

	/** 
	 * Resizes an image to fit inside a width x height.
	 *
	 * @param image the image to resize (modifies this image)
	 * @param w the bounding width
	 * @param h the bounding height
	 * @return a reference to the image passed in (for chaining)
	 */
	wxImage& ResizeToFit(wxImage& image, int w, int h);

public:
	///
	ImagePreviewDialog(wxWindow *parent);
	ImagePreviewDialog(wxWindow *parent, wxString initFilename);
	virtual ~ImagePreviewDialog();

	/**
	 * Helper method to test if an image can be loaded by wxWidgets by
	 * Testing if there is a valid wxImageHandler for the filename extension.
	 *
	 * @pre filename must have an extension
	 * @param filename the filename to test (does not read the contents, just
	 *   the extension)
	 * @return whether or not the image can be previewed (valid image file)
	 */
	static bool IsValidImage(wxString& filename);

	/**
	 * Retrieves the URL location (empty if there was a problem) 
	 */
	inline wxString GetUrl() const { return imageUrl; }

	/**
	 * Called when the user clicks OK, initiates the image upload
	 * via the thread callback method
	 *
	 * @see ImageUpload
	 */
	bool TransferDataFromWindow();

	/**
	 * Thread callback method for image uploading. If this succeeds,
	 * #imageUrl should be filled with the uploaded image URL location,
	 * otherwise it is set to the empty string.
	 *
	 * @warning This method runs in a separate thread and should not
	 *   interact directly with the GUI.
	 */
	void ImageUpload(wxString& filename);

	///
	/// @name Window Events
	///
	//@{

	/**
	 * Activated when a user clicks the Choose Image button
	 */
	void OnChooseImage(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();

	//@}
};
