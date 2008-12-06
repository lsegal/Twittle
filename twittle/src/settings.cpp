#include <wx/file.h>
#include "settings.h"
#include "application.h"

Settings::Settings() : loaded(false)
{
	wxXmlNode *root = new wxXmlNode(wxXML_ELEMENT_NODE, _T("settings"));
	doc.SetRoot(root);
	Defaults();
}

void Settings::Defaults()
{
	Set(_T("window.transparency"), 255L);
	Set(_T("window.width"), 300L);
	Set(_T("window.height"), 400L);
	Set(_T("window.showintaskbar"), true);
	Set(_T("window.showintray"), true);
	Set(_T("window.minimizetotray"), true);
	Set(_T("window.shownotifications"), true);
	Set(_T("feedpanel.showscreenname"), true);
	Set(_T("feedpanel.editposition"), _T("bottom"));
	Set(_T("account.autologin"), false);
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

	Settings newSettings;
	if (!newSettings.doc.Load(SettingsFile())) {
		return false;
	}

	Merge(newSettings); // merge settings from xml file on disk

	return true;
}

bool Settings::Save() const
{
	return doc.Save(SettingsFile());
}

/** 
 * Merges a settings object with another. 
 *
 * Implementation note: this only works at the properties level of
 * the root node of the xml file. Child elements will be ignored.
 * This is not by design but by incomplete implementation.
 */
void Settings::Merge(const Settings& other)
{
	wxXmlProperty *prop = other.doc.GetRoot()->GetProperties();
	while (prop) {
		if (doc.GetRoot()->HasProp(prop->GetName())) {
			doc.GetRoot()->DeleteProperty(prop->GetName());
		}
		doc.GetRoot()->AddProperty(prop->GetName(), prop->GetValue());

		prop = prop->GetNext();
	}

	loaded = true;
}
