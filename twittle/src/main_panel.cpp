#include "main_panel.h"
#include "application.h"

// Events
BEGIN_EVENT_TABLE(MainPanel, wxPanel)
	EVT_TEXT(ID_EDIT, OnEditText)
	EVT_TEXT_ENTER(ID_EDIT, OnEditEnter)
	EVT_BUTTON(ID_PUBLIC, OnButtonClick)
	EVT_BUTTON(ID_FRIEND, OnButtonClick)
END_EVENT_TABLE()

MainPanel::MainPanel(wxWindow *parent) : wxPanel(parent)
{
	InitializeComponents();
	CheckEditboxEmpty();

	SetMinSize(wxSize(300, 300));

	content.SetFeed(Twitter::PublicTimelineUrl, 10);
}

void MainPanel::InitializeComponents()
{
	font.Create(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Arial"));
	editbox.Create(this, ID_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	charcounter.Create(this, ID_COUNTER, _T("0"), wxDefaultPosition, wxSize(28, 20), wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	//submit.Create(this, ID_SUBMIT, _T("Go"), wxDefaultPosition, wxSize(30,30));
	content.Create(this, ID_CONTENT);

	// buttons
	publicButton.Create(this, ID_PUBLIC, _T("P"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	followButton.Create(this, ID_FRIEND, _T("F"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

	editbox.SetFont(font);
	charcounter.SetFont(font);
	//submit.SetFont(font);
	content.SetSelectionBackground(*wxBLACK);

	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(&publicButton);
	buttonSizer->Add(&followButton);

	wxBoxSizer *editSizer = new wxBoxSizer(wxHORIZONTAL);
	editSizer->Add(&editbox, wxSizerFlags(1).Expand());
	editSizer->Add(&charcounter, wxSizerFlags(0).Center().Right().Border(wxLEFT, 5));
	//editSizer->Add(&submit, wxSizerFlags(0).Right().Border(wxLEFT, 5));

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->Add(buttonSizer, wxSizerFlags(0));
	panelSizer->Add(&content, wxSizerFlags(1).Expand().Border(wxALL & ~(wxTOP), 5));
	panelSizer->Add(editSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));

	panelSizer->SetSizeHints(this);
	SetSizerAndFit(panelSizer);
}

void MainPanel::OnButtonClick(wxCommandEvent& event)
{
	switch (event.GetId()) {
		case ID_PUBLIC:
			content.SetFeed(Twitter::PublicTimelineUrl);
			break;
		case ID_FRIEND:
			content.SetFeed(Twitter::FriendsTimelineUrl);
			break;
	}
}

void MainPanel::OnEditText(wxCommandEvent &evt)
{
	wxString label;
	unsigned int len = static_cast<unsigned int>(editbox.GetValue().length());
	label << len;

	charcounter.SetLabel(label);

	if (len > 140) {
		charcounter.SetOwnForegroundColour(wxColour(200, 0, 0));
	}
	else {
		charcounter.SetOwnForegroundColour(*wxBLACK);
	}

	CheckEditboxEmpty();
}

void MainPanel::OnEditEnter(wxCommandEvent &evt)
{
	if (wxGetApp().GetTwitter().UpdateStatus(editbox.GetValue())) {
		editbox.SetValue(_T(""));
	}
}

void MainPanel::CheckEditboxEmpty()
{
	if (editbox.GetValue().length() != 0) return;

	//editbox.SetOwnForegroundColour(wxColour(160, 160, 160));
	//editbox.SetValue(_T("What are you doing right now?"));
	//editbox.SetSelection(0, 0);
	//editbox.SetInsertionPoint(0);
}
