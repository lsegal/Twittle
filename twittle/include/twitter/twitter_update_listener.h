#pragma once

#include <wx/wx.h>
#include "twitter.h"

class Twitter;

class TwitterUpdateListener
{
public:
	virtual void TwitterUpdateReceived(const Twitter& twitter, const wxString& resource) = 0;
};