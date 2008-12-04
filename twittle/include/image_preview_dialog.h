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

	// Resizes an image to fit inside a width x height
	wxImage& ResizeToFit(wxImage& image, int w, int h);

public:
	ImagePreviewDialog(wxWindow *parent);
	virtual ~ImagePreviewDialog();

	inline wxString GetUrl() { return imageUrl; }

	// @override wxWindow
	bool TransferDataFromWindow();

	// thread callback
	void ImageUpload(wxString& filename);

	// events
	void OnChooseImage(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE();
};
