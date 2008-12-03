#include "main_panel.h"
#include "feed_panel.h"
#include "application.h"
#include "thread_callback.h"
#include "http_client.h"
#include <wx/regex.h>

// Events
BEGIN_EVENT_TABLE(MainPanel, wxPanel)
	EVT_TEXT(ID_EDIT, MainPanel::OnEditText)
	EVT_TEXT_ENTER(ID_EDIT, MainPanel::OnEditEnter)
	EVT_BUTTON(ID_TINYURL, MainPanel::OnShortenUrl)
	EVT_BUTTON(ID_PUBLIC, MainPanel::OnButtonClick)
	EVT_BUTTON(ID_FRIEND, MainPanel::OnButtonClick)
	EVT_BUTTON(ID_FILTER_AT, MainPanel::OnButtonClick)
END_EVENT_TABLE()

MainPanel::MainPanel(wxWindow *parent) : wxPanel(parent)
{
	InitializeComponents();
	editbox.OnFocusLost(wxFocusEvent()); // unset focus

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

void MainPanel::InitializeComponents()
{
	font.Create(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Arial"));
	editbox.Create(this, ID_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	charcounter.Create(this, ID_COUNTER, _T("0"), wxDefaultPosition, wxSize(28, 20), wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	tinyurl.Create(this, ID_TINYURL, _T("L"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	content.Create(this, ID_CONTENT);

	// buttons
	publicButton.Create(this, ID_PUBLIC, _T("P"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	followButton.Create(this, ID_FRIEND, _T("F"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	atButton.Create(this, ID_FILTER_AT, _T("@"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	editbox.SetFont(font);
	charcounter.SetFont(font);
	content.SetSelectionBackground(*wxBLACK);

	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(&publicButton);
	buttonSizer->Add(&followButton);
	buttonSizer->Add(&atButton);

	wxBoxSizer *editSizer = new wxBoxSizer(wxHORIZONTAL);
	editSizer->Add(&editbox, wxSizerFlags(1).Expand());
	editSizer->Add(&tinyurl, wxSizerFlags(0).Right().Border(wxLEFT, 5));
	editSizer->Add(&charcounter, wxSizerFlags(0).Center().Right().Border(wxLEFT, 5));

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->Add(buttonSizer, wxSizerFlags(0));
	panelSizer->Add(&content, wxSizerFlags(1).Expand().Border(wxALL & ~(wxTOP), 5));
	panelSizer->Add(editSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));

	panelSizer->SetSizeHints(this);
	SetSizer(panelSizer);
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
	unsigned int len;
	if (editbox.IsActive()) {
		len = static_cast<unsigned int>(editbox.GetValue().length());
	}
	else {
		len = 0;
	}
	charcounter.SetLabel(wxString::Format(_T("%d"), len));

	if (len > 140) {
		charcounter.SetOwnForegroundColour(wxColour(200, 0, 0));
	}
	else {
		charcounter.SetOwnForegroundColour(*wxBLACK);
	}
}

void MainPanel::OnEditEnter(wxCommandEvent &evt)
{
	if (wxGetApp().GetTwitter().UpdateStatus(editbox.GetValue())) {
		editbox.SetValue(_T(""));
	}
}

void MainPanel::OnShortenUrl(wxCommandEvent& evt)
{
	wxString defaultStr = editbox.GetStringSelection();
	wxString result = wxGetTextFromUser(_T("Shorten URL with is.gd"), 
		_T("Enter URL to shorten via http://is.gd"), defaultStr, this);

	// start thread with timeout code
	wxString shortUrl = result;
	ThreadCallback1<MainPanel, wxString> cb(*this, &MainPanel::ShortenUrl, shortUrl);

	wxDateTime startTime = wxDateTime::Now();
	wxBeginBusyCursor();
	while (cb.IsAlive()) {
		wxYield();

		if ((wxDateTime::Now() - startTime) > wxTimeSpan::Seconds(10)) {
			cb.Kill();
			wxMessageBox(_T("Error while shortening URL"), 
				_T("A connection timeout occurred while trying to connect to http://is.gd"), 
				wxOK|wxICON_ERROR|wxCENTRE);
		}
	}
	wxEndBusyCursor();
	cb.Wait();

	// shortUrl should be filled
	if (shortUrl.StartsWith(_T("http://"))) {
		long from, to;
		editbox.GetSelection(&from, &to);
		wxString val = (editbox.IsActive() ? editbox.GetValue() : _T(""));
		val.replace(from, to-from, shortUrl);

		editbox.SetFocus();
		editbox.SetValue(val);

		if (from != to) {
			// reset selection
			editbox.SetSelection(from, from + shortUrl.Length());
		}
		else {
			// set normal cursor position
			editbox.SetSelection(from + shortUrl.Length(), from + shortUrl.Length());
		}
	}
}

void MainPanel::ShortenUrl(wxString& shortUrl)
{
	HttpClient http;
	http.SetHeader(_T("Referer"), _T("http://is.gd/"));
	http.SetHeader(_T("Content-Type"), _T("application/x-www-form-urlencoded"));
	http.SetPostBuffer(_T("URL=") + shortUrl);
	wxString resp = http.Get(wxURL(_T("http://is.gd/create.php")));

	// find url in result
	wxRegEx re(_T("value=\"([^\"]+)\""), wxRE_ICASE | wxRE_ADVANCED);
	if (re.Matches(resp)) {
		shortUrl.Empty();
		shortUrl.Append(re.GetMatch(resp, 1));
	}
}
