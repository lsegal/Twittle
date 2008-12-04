#include "main_panel.h"
#include "feed_panel.h"
#include "application.h"
#include "thread_callback.h"
#include "http_client.h"
#include "is.gd/isgd.h"
#include "image_preview_dialog.h"
#include <wx/regex.h>

// Events
BEGIN_EVENT_TABLE(MainPanel, wxPanel)
	EVT_TEXT(ID_EDIT, MainPanel::OnEditText)
	EVT_TEXT_ENTER(ID_EDIT, MainPanel::OnEditEnter)
	EVT_BUTTON(ID_TINYURL, MainPanel::OnShortenUrl)
	EVT_BUTTON(ID_TWITPIC, MainPanel::OnImageClick)
	EVT_BUTTON(ID_PUBLIC, MainPanel::OnButtonClick)
	EVT_BUTTON(ID_FRIEND, MainPanel::OnButtonClick)
	EVT_BUTTON(ID_FILTER_AT, MainPanel::OnButtonClick)
END_EVENT_TABLE()

MainPanel::MainPanel(wxWindow *parent) : wxPanel(parent)
{
	InitializeComponents();
	SetAccelerators();
	editbox.OnFocusLost(wxFocusEvent()); // unset focus

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
	tinyurl.Create(this, ID_TINYURL, _T("L"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	twitpic.Create(this, ID_TWITPIC, _T("Q"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	content.Create(this, ID_CONTENT);

	// buttons
	publicButton.Create(this, ID_PUBLIC, _T("P"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	followButton.Create(this, ID_FRIEND, _T("F"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	atButton.Create(this, ID_FILTER_AT, _T("@"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	editbox.SetFont(font);
	charcounter.SetFont(font);

	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(&publicButton);
	buttonSizer->Add(&followButton);
	buttonSizer->Add(&atButton);

	wxBoxSizer *editSizer = new wxBoxSizer(wxHORIZONTAL);
	editSizer->Add(&editbox, wxSizerFlags(1).Expand());
	editSizer->AddSpacer(5);
	editSizer->Add(&twitpic);
	editSizer->Add(&tinyurl);
	editSizer->AddSpacer(5);
	editSizer->Add(&charcounter, wxSizerFlags(0).Center().Right());

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->Add(buttonSizer, wxSizerFlags(0));
	panelSizer->Add(&content, wxSizerFlags(1).Expand().Border(wxALL & ~(wxTOP), 5));
	panelSizer->Add(editSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));

	panelSizer->SetSizeHints(this);
	SetSizer(panelSizer);
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
			content.AddFilter(FeedPanel::FILTER_REPLIES);
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
