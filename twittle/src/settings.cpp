#include <wx/file.h>
#include "settings.h"
#include "application.h"

Settings::Settings() : loaded(false)
{
	if (!Load()) {
		wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, _T("settings"));
		doc.SetRoot(root);
	}
}

wxString Settings::SettingsFile() const
{
	return wxGetApp().GetSettingsPath() + _T("settings.xml");
}

long Settings::GetLong(const wxString& key) const
{
	long val;
	doc.GetRoot()->GetPropVal(key, _T("0")).ToLong(&val);
	return val;
}

bool Settings::GetBool(const wxString& key) const
{
	return doc.GetRoot()->GetPropVal(key, _T("false")) == _T("true") ? true : false;
}

wxString Settings::GetString(const wxString& key) const
{
	return doc.GetRoot()->GetPropVal(key, _T(""));
}

void Settings::Set(const wxString& key, const wxString& value)
{
	if (doc.GetRoot()->HasProp(key)) {
		doc.GetRoot()->DeleteProperty(key);
	}
	doc.GetRoot()->AddProperty(key, value);
}

void Settings::Set(const wxString& key, long value)
{
	if (doc.GetRoot()->HasProp(key)) {
		doc.GetRoot()->DeleteProperty(key);
	}
	doc.GetRoot()->AddProperty(key, wxString::Format(_T("%d"), value));
}

void Settings::Set(const wxString& key, bool value)
{
	if (doc.GetRoot()->HasProp(key)) {
		doc.GetRoot()->DeleteProperty(key);
	}
	doc.GetRoot()->AddProperty(key, value ? _T("true") : _T("false"));
}

bool Settings::Load()
{
	if (!wxFile::Exists(SettingsFile())) {
		return false;
	}
	return doc.Load(SettingsFile());
}

bool Settings::Save() const
{
	return doc.Save(SettingsFile());
}