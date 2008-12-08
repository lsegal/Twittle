#pragma once

#include <wx/wx.h>
#include "thread_callback.h"

class ImagePreviewDialog : public wxDialog
{
	enum ControlIdentifiers { ID_CHOOSEIMAGE };

	ThreadCallback1<ImagePreviewDialog, wxString> *callback;

	wxString imageUrl;
	wxString imageFilename;
	wxButton chooseImageButton;
	wxStaticBitmap imagePreview;
	wxStaticText progress;

	void InitializeComponents();

	// Shows the image specified by the image filename
	void ShowImagePreview();

	// Resizes an image to fit inside a width x height
	wxImage& ResizeToFit(wxImage& image, int w, int h);

public:
	ImagePreviewDialog(wxWindow *parent);
	ImagePreviewDialog(wxWindow *parent, wxString initFilename);
	virtual ~ImagePreviewDialog();

	static bool IsValidImage(wxString& filename);

	inline wxString GetUrl() const { return imageUrl; }

	// @override wxWindow
	bool TransferDataFromWindow();

	// thread callback
	void ImageUpload(wxString& filename);

	// events
	void OnChooseImage(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};
