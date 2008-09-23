#include "main_window.h"

// Events
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_TEXT(MainWindow::ID_EDIT, MainWindow::OnEditText)
	EVT_TEXT_ENTER(MainWindow::ID_EDIT, MainWindow::OnEditEnter)
END_EVENT_TABLE()

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, _T("Tweet!"))
{
	wxPanel *panel = new wxPanel(this, wxID_ANY);

	font.Create(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _T("Arial"));
	editbox.Create(panel, ID_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	charcounter.Create(panel, ID_COUNTER, _T("0"), wxDefaultPosition, wxSize(28, 20), wxALIGN_RIGHT | wxST_NO_AUTORESIZE);
	submit.Create(panel, ID_SUBMIT, _T("Go"), wxDefaultPosition, wxSize(30,30));
	content.Create(panel, ID_CONTENT);

	editbox.SetFont(font);
	charcounter.SetFont(font);
	submit.SetFont(font);

	wxBoxSizer *editSizer = new wxBoxSizer(wxHORIZONTAL);
	editSizer->Add(&editbox, wxSizerFlags(1).Expand());
	editSizer->Add(&charcounter, wxSizerFlags(0).Center().Right().Border(wxLEFT, 5));
	editSizer->Add(&submit, wxSizerFlags(0).Right().Border(wxLEFT, 5));

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	panelSizer->Add(editSizer, wxSizerFlags(0).Expand().Border(wxALL, 5));
	panelSizer->Add(&content, wxSizerFlags(1).Expand().Border(wxALL & ~(wxTOP), 5));

	panel->SetSizer(panelSizer);
}

void MainWindow::OnEditText(wxCommandEvent &evt)
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
}

void MainWindow::OnEditEnter(wxCommandEvent &evt)
{
}
