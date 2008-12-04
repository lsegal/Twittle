#include "image_preview_dialog.h"
#include "twitpic/twitpic.h"
#include "application.h"
#include "twitter/twitter.h"

BEGIN_EVENT_TABLE(ImagePreviewDialog, wxDialog)
	EVT_BUTTON(ID_CHOOSEIMAGE, ImagePreviewDialog::OnChooseImage)
END_EVENT_TABLE()

ImagePreviewDialog::ImagePreviewDialog(wxWindow *parent) :
	wxDialog(parent, wxID_ANY, _T("Select an image to upload"), wxDefaultPosition,
		wxSize(400, 400)), callback(NULL)
{
	InitializeComponents();
	wxPostEvent(this, wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_CHOOSEIMAGE));
}

ImagePreviewDialog::~ImagePreviewDialog()
{
	if (wxIsBusy()) wxEndBusyCursor();
	if (callback && callback->IsAlive()) callback->Kill();
	delete callback;
}

void ImagePreviewDialog::InitializeComponents()
{
	chooseImageButton.Create(this, ID_CHOOSEIMAGE, _T("Choose Image..."));
	imagePreview.Create(this, wxID_ANY, wxBitmap(), wxDefaultPosition, wxSize(200, 200));
	progress.Create(this, wxID_ANY, _T(""));

	wxSizer *buttonSizer = CreateButtonSizer(wxOK|wxCANCEL);
	wxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	buttonSizer->AddSpacer(15);
	buttonSizer->Add(&chooseImageButton);

	wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _T("Preview"));
	wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxVERTICAL);
	boxSizer->Add(&imagePreview);

	panelSizer->Add(boxSizer, wxSizerFlags().Center());
	panelSizer->AddSpacer(5);
	panelSizer->Add(&progress, wxSizerFlags().Left());
	panelSizer->AddSpacer(5);
	panelSizer->AddStretchSpacer();
	panelSizer->Add(buttonSizer, wxSizerFlags().Right());

	wxSizer *borderedSizer = new wxBoxSizer(wxVERTICAL);
	borderedSizer->Add(panelSizer, wxSizerFlags(1).Expand().Border(wxALL, 20));

	SetSizerAndFit(borderedSizer);
	Centre();
}

bool ImagePreviewDialog::TransferDataFromWindow()
{
	// disable all buttons
	wxWindow *okButton = FindWindowById(wxID_OK, this);
	wxWindow *cancelButton = FindWindowById(wxID_CANCEL, this);
	chooseImageButton.Disable();
	if (okButton) okButton->Disable();
	progress.SetOwnForegroundColour(*wxBLACK);
	progress.SetLabel(_T("Uploading ..."));

	callback = new ThreadCallback1<
		ImagePreviewDialog, wxString>(*this, &ImagePreviewDialog::ImageUpload, imageFilename);

	wxDateTime startTime = wxDateTime::Now();
	wxBeginBusyCursor();
	while (callback->IsAlive()) {
		wxYield();

		if ((wxDateTime::Now() - startTime) > wxTimeSpan::Seconds(10000)) {
			callback->Kill();
			progress.SetOwnForegroundColour(*wxRED);
			progress.SetLabel(_T("Connection timeout."));
//			wxMessageBox(_T("A connection timeout occurred while trying to connect to http://twitpic.com"), 
//				_T("Error while uploading image"), wxOK|wxICON_ERROR|wxCENTRE);
		}
	}
	wxEndBusyCursor();

	delete callback;
	callback = NULL;

	// reenable buttons
	chooseImageButton.Enable();
	if (okButton) okButton->Enable();
	
	return !imageUrl.IsEmpty();
}

void ImagePreviewDialog::ImageUpload(wxString& filename)
{
	try { 
		const Twitter& twitter = wxGetApp().GetTwitter();
		imageUrl = TwitPic::UploadImage(twitter.GetUsername(), twitter.GetPassword(), filename);
	}
	catch (wxString msg) {
//		wxMessageBox(msg, _T("Error while uploading image"), wxOK|wxICON_ERROR|wxCENTRE);
		progress.SetOwnForegroundColour(*wxRED);
		progress.SetLabel(msg);
		imageUrl = wxEmptyString;
	}
}

void ImagePreviewDialog::OnChooseImage(wxCommandEvent& evt)
{
	wxString tmpFilename = wxFileSelector(_T("Select an image to upload"));
	if (!imageFilename && !tmpFilename) {
		Close();
		return;
	}
	else if (!tmpFilename) {
		return;
	}
	
	imageFilename = tmpFilename;
	
	// set preview
	wxImage image(imageFilename);
	ResizeToFit(image, 250, 250);

	// center image
	imagePreview.GetParent()->GetSizer()->RecalcSizes();

	imagePreview.SetBitmap(wxBitmap(image));
}

wxImage& ImagePreviewDialog::ResizeToFit(wxImage& image, int w, int h)
{
	int iw = image.GetWidth(), ih = image.GetHeight();
	if (ih > iw && ih > h) {
		double aspect = (double)iw / (double)ih;
		int newWidth = (int)(200.0 * aspect);
		image.Rescale(newWidth, 200, wxIMAGE_QUALITY_HIGH);
	}
	else if (iw > ih && iw > w) {
		double aspect = (double)ih / (double)iw;
		int newHeight = (int)(200.0 * aspect);
		image.Rescale(200, newHeight, wxIMAGE_QUALITY_HIGH);
	}
	return image;
}
