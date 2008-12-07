#include "about_dialog.h"
#include "static_url_text.h"
#include "application.h"
#include <wx/statline.h>

#include "resources/aboutlogo.xpm"

AboutDialog::AboutDialog(wxWindow *parent) : wxDialog(parent, wxID_ANY,
		_T("About ") + wxGetApp().APPNAME, wxDefaultPosition, wxSize(305, 370))
{
	InitializeComponents();
}

static void RecursiveBackground(wxSizer& sizer, const wxColour& colour)
{
	wxSizerItemList::iterator it;
	for (it = sizer.GetChildren().begin(); it != sizer.GetChildren().end(); ++it) {
		wxWindow *win = (*it)->GetWindow();
		if (win) win->SetBackgroundColour(colour);
		else {
			wxSizer *nextsizer = (*it)->GetSizer();
			if (nextsizer) RecursiveBackground(*nextsizer, colour);
		}
	}
}

void AboutDialog::InitializeComponents()
{
	wxButton *okButton = new wxButton(this, wxID_OK, _T("Ok"));

	wxStaticText *twittle = new wxStaticText(this, wxID_ANY, wxGetApp().APPNAME);
	wxFont font = twittle->GetFont();
	font.SetWeight(wxFONTWEIGHT_BOLD);
	twittle->SetOwnFont(font);

	wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *topImageSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer *topTextSizer = new wxBoxSizer(wxVERTICAL);
	topImageSizer->Add(new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxIcon(aboutlogo))));
	topImageSizer->AddSpacer(10);
	topTextSizer->Add(twittle, wxSizerFlags().Top());
	topTextSizer->AddSpacer(3);
	topTextSizer->Add(new wxStaticText(this, wxID_ANY, _T("A Really Friendly Twitter App")));
	topTextSizer->Add(new wxStaticText(this, wxID_ANY, _T("By Loren Segal")), wxSizerFlags().Left());
	topImageSizer->Add(topTextSizer, wxSizerFlags().Center());
	topSizer->Add(topImageSizer, wxSizerFlags().Border(wxALL, 10));
	topSizer->Add(new wxStaticText(this, wxID_ANY, wxGetApp().APPNAME));
	topSizer->Add(new wxStaticText(this, wxID_ANY, _T("Version: ") + wxGetApp().APPVERSION));
	topSizer->Add(new wxStaticText(this, wxID_ANY, _T("Copyright \xA9 2008")));
	topSizer->Add(new wxStaticText(this, wxID_ANY, _T("Loren Segal")));
	topSizer->Add(new wxStaticText(this, wxID_ANY, _T("All Rights Reserved.")));
	topSizer->AddSpacer(17);
	topSizer->Add(new StaticUrlText(this, wxID_ANY, _T("http://www.kthx.net/twittle")));
	topSizer->AddSpacer(40);
	wxSizer *topSizerBorder = new wxBoxSizer(wxVERTICAL);
	topSizerBorder->Add(topSizer, wxSizerFlags(1).Expand().Border(wxALL, 15));
	wxPanel *topPanel = new wxPanel(this);

	RecursiveBackground(*topSizer, *wxWHITE);
	topPanel->SetBackgroundColour(*wxWHITE);
	topPanel->SetSizer(topSizerBorder);

	wxSizer *bottomSizer = new wxBoxSizer(wxVERTICAL);
	bottomSizer->Add(okButton, wxSizerFlags().Right().Bottom());

	wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(topPanel, wxSizerFlags(1).Expand());
	sizer->Add(new wxStaticLine(this), wxSizerFlags().Expand());
	sizer->Add(bottomSizer, wxSizerFlags(0).Border(wxALL, 15).Right());
	SetSizer(sizer);
	Center();
}
