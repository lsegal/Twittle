#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>

class Application;

class Settings
{
	wxXmlDocument doc;
	bool loaded;

	wxString SettingsFile() const;

public:
	Settings();

	long GetLong(const wxString& key) const;
	bool GetBool(const wxString& key) const;
	wxString GetString(const wxString& key) const;

	void Set(const wxString& key, const wxString& value);
	void Set(const wxString& key, long value);
	void Set(const wxString& key, bool value);

	void Merge(const Settings& other);
	void Defaults();

	bool Load();
	bool Save() const;
};
