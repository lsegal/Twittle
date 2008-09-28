#pragma once

#include <wx/wx.h>
#include "main_window.h"

class Application : public wxApp
{
	MainWindow *main;
	Twitter twitter;

public:
	inline Twitter& Twitter() { return twitter; }

	bool OnInit();
};

DECLARE_APP(Application)
