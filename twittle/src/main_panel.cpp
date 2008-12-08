#include "main_panel.h"
#include "filtered_iterator.h"
#include "application.h"
#include "thread_callback.h"
#include "http/http_client.h"
#include "is.gd/isgd.h"
#include "image_preview_dialog.h"
#include <wx/regex.h>

// Icons
#include "resources/atsign.xpm"
#include "resources/twitpic_normal.xpm"
#include "resources/link_icon.xpm"
#include "resources/public_icon.xpm"
#include "resources/home.xpm"

// Events
BEGIN_EVENT_TABLE(MainPanel, wxPanel)
	EVT_TEXT(ID_EDIT, MainPanel::OnEditText)
	EVT_TEXT_ENTER(ID_EDIT, MainPanel::OnEditEnter)
	EVT_BUTTON(ID_TINYURL, MainPanel::OnShortenUrl)
	EVT_BUTTON(ID_TWITPIC, MainPanel::OnImageClick)
	EVT_BUTTON(ID_PUBLIC, MainPanel::OnButtonClick)
	EVT_BUTTON(ID_FRIEND, MainPanel::OnButtonClick)
	EVT_BUTTON(ID_FILTER_AT, MainPanel::OnButtonClick)
	EVT_DROP_FILES(MainPanel::OnDropFiles)
END_EVENT_TABLE()

MainPanel::MainPanel(wxWindow *parent) : wxPanel(parent), buttonSizer(NULL), editSizer(NULL)
{
	wxFocusEvent evt;
	InitializeComponents();
	SetAccelerators();
	editbox.OnFocusLost(evt); // unset focus
	DragAcceptFiles(true); // accept file dragging

	// replace maximize ability
	parent->SetWindowStyle(parent->GetWindowStyle() | wxMAXIMIZE_BOX);

	SetMinSize(wxSize(300, 300));

	content.SetFeed(Twitter::FriendsTimelineUrl);

	// set height if we have one
	long width = wxGetApp().GetSettings().GetLong(_T("window.width"));
	long height = wxGetApp().GetSettings().GetLong(_T("window.height"));
	if (width > 0 && height > 0) {
		SetSize(width, height);
	}
}

MainPanel::~MainPanel()
{
	wxSize s = GetParent()->GetClientSize();
	wxGetApp().GetSettings().Set(_T("window.width"), (long)s.GetWidth());
	wxGetApp().GetSettings().Set(_T("window.height"), (long)s.GetHeight());
}

void MainPanel::SetAccelerators()
{
	wxAcceleratorEntry entries[1];
	entries[0].Set(wxACCEL_CTRL, (int)'L', ID_TINYURL);
	wxAcceleratorTable accel(1, entries);
	SetAcceleratorTable(accel);
}

void MainPanel::InitializeComponents()
{
	font.Create(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Arial"));
	editbox.Create(this, ID_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	charcounter.Create(this, ID_COUNTER, _T("0"), wxDefaultPosition, wxSize(28, 20), wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	content.Create(this, ID_CONTENT);

	// buttons
	publicButton.Create(this, ID_PUBLIC, wxBitmap(public_icon), wxDefaultPosition, wxDefaultSize);
	followButton.Create(this, ID_FRIEND, wxBitmap(home), wxDefaultPosition, wxDefaultSize);
	atButton.Create(this, ID_FILTER_AT, wxBitmap(atsign), wxDefaultPosition, wxDefaultSize);
	tinyurl.Create(this, ID_TINYURL, wxBitmap(link_icon), wxDefaultPosition, wxDefaultSize);
	twitpic.Create(this, ID_TWITPIC, wxBitmap(twitpic_normal), wxDefaultPosition, wxDefaultSize);

	publicButton.SetToolTip(_T("View all public tweets"));
	followButton.SetToolTip(_T("View your friends' tweets"));
	atButton.SetToolTip(_T("View replies directed to you"));
	tinyurl.SetToolTip(_T("Shorten a URL with http://is.gd"));
	twitpic.SetToolTip(_T("Upload an image via http://twitpic.com"));

	editbox.SetFont(font);
	charcounter.SetFont(font);

	wxBoxSizer *buttonSizer2 = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer2->Add(&twitpic, wxSizerFlags().Right());
	buttonSizer2->Add(&tinyurl, wxSizerFlags().Right());

	buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(&publicButton);
	buttonSizer->Add(&followButton);
	buttonSizer->Add(&atButton);

	editSizer = new wxBoxSizer(wxHORIZONTAL);
	editSizer->Add(&editbox, wxSizerFlags(1).Expand());
	editSizer->AddSpacer(5);
	editSizer->Add(buttonSizer2);
	editSizer->AddSpacer(5);
	editSizer->Add(&charcounter, wxSizerFlags(0).Center().Right());

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->SetSizeHints(this);
	SetSizer(panelSizer);

	SetPanelOrder();
}

void MainPanel::SetPanelOrder()
{
	wxSizer *sizer = GetSizer();

	Freeze();

	sizer->Detach(editSizer);
	sizer->Detach(buttonSizer);
	sizer->Detach(&content);

	wxString editPosition = wxGetApp().GetSettings().GetString(_T("feedpanel.editposition"));
	if (editPosition == _T("top")) {
		sizer->Add(editSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));
		sizer->Add(&content, wxSizerFlags(1).Expand().Border(wxALL & ~(wxTOP), 5));
		sizer->Add(buttonSizer, wxSizerFlags(0));
	}
	else { // bottom
		sizer->Add(buttonSizer, wxSizerFlags(0));
		sizer->Add(&content, wxSizerFlags(1).Expand().Border(wxALL & ~(wxTOP), 5));
		sizer->Add(editSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));
	}

	sizer->Layout();

	Thaw();
}

void MainPanel::ForceUpdateUI()
{
	content.RefreshAll();
}

void MainPanel::OnButtonClick(wxCommandEvent& event)
{
	content.ResetFilter();

	switch (event.GetId()) {
		case ID_PUBLIC:
			content.SetFeed(Twitter::PublicTimelineUrl);
			break;
		case ID_FRIEND:
			content.SetFeed(Twitter::FriendsTimelineUrl);
			break;
		case ID_FILTER_AT:
			content.SetFeed(Twitter::FriendsTimelineUrl);
			content.AddFilter(FilteredIterator::FILTER_REPLIES);
			break;
	}
}

void MainPanel::OnEditText(wxCommandEvent &evt)
{
	size_t len = editbox.IsActive() ? editbox.GetValue().Length() : 0;
	charcounter.SetOwnForegroundColour(len > 140 ? wxColour(200, 0, 0) : *wxBLACK);
	charcounter.SetLabel(wxString::Format(_T("%d"), len));
}

void MainPanel::OnEditEnter(wxCommandEvent &evt)
{
	if (wxGetApp().GetTwitter().UpdateStatus(editbox.GetValue())) {
		editbox.SetValue(_T(""));
	}
}

void MainPanel::OnShortenUrl(wxCommandEvent& evt)
{
	wxString result = editbox.GetStringSelection(); // start with any selection
	if (result == wxEmptyString || (!result.StartsWith(_T("http")) && !result.StartsWith(_T("www.")))) {
		// no selection? or maybe it's invalid? ask user for url
		result = wxGetTextFromUser(_T("Shorten URL with is.gd"),
			_T("Enter URL to shorten via http://is.gd"), result, this);
	}

	// start thread with timeout code
	wxString shortUrl = result;
	ThreadCallback1<MainPanel, wxString> cb(*this, &MainPanel::ShortenUrl, shortUrl);

	wxDateTime startTime = wxDateTime::Now();
	wxBeginBusyCursor();
	while (cb.IsAlive()) {
		wxYield();

		if ((wxDateTime::Now() - startTime) > wxTimeSpan::Seconds(10)) {
			cb.Kill();
			wxMessageBox(_T("A connection timeout occurred while trying to connect to http://is.gd"),
				_T("Error while shortening URL"), wxOK|wxICON_ERROR|wxCENTRE);
		}
	}
	wxEndBusyCursor();

	// shortUrl should be filled
	editbox.InsertUrl(shortUrl);
}

void MainPanel::ShortenUrl(wxString& shortUrl)
{
	wxString url = Isgd::ShortenUrl(shortUrl);
	shortUrl.Empty();
	shortUrl.Append(url);
}

void MainPanel::OnImageClick(wxCommandEvent& evt)
{
	ImagePreviewDialog dlg(this);
	if (dlg.ShowModal() == wxID_OK) {
		editbox.InsertUrl(dlg.GetUrl());
	}
}

void MainPanel::OnDropFiles(wxDropFilesEvent& evt)
{
	int numFiles = evt.GetNumberOfFiles();
	wxString *files = evt.GetFiles();

	if (numFiles > 0) {
		// make sure this file is an image file.
		if (!ImagePreviewDialog::IsValidImage(files[0]))
			return;

		// show the preview for it
		ImagePreviewDialog dlg(this, files[0]);
		if (dlg.ShowModal() == wxID_OK) {
			editbox.InsertUrl(dlg.GetUrl());
		}
	}
}
