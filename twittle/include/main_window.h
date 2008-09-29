#pragma once

#include <wx/wx.h>
#include "feed_panel.h"

class MainWindow : public wxFrame
{
	void ClearPanels();

public:
	MainWindow();

	void ShowFollowingPanel();
	void ShowLogin();
};
