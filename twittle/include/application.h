#pragma once

#include <wx/wx.h>
#include "main_window.h"

class Application : public wxApp
{
	MainWindow *main;

public:
	bool OnInit();
};

DECLARE_APP(Application)
