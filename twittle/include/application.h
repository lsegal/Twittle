#ifndef TWEETAPP_H
#define TWEETAPP_H 1

#include <wx/wx.h>
#include <wx/image.h>
#include "main_window.h"

class Application : public wxApp
{
	MainWindow *main;

public:
	bool OnInit();
};

DECLARE_APP(Application)

#endif /* TWEETAPP_H */
