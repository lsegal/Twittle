#pragma once

#include <wx/wx.h>
#include <wx/xml/xml.h>

class Application;

/**
 * Represents a map of all application settings in memory which can be loaded from
 * or saved to an XML document.
 *
 * Internally, all data is stored in a live XML document in memory and is flushed
 * to disk when Save() is called.
 *
 * To reset the settings object, Defaults() can be called, or Load() to load the
 * settings from disk.
 *
 * Note that a settings object is not always tied to the SettingsFile. This allows
 * you to create new temporary settings objects and play with their values without
 * modifying settings on the live application. You can then #Merge them back into
 * a main settings object (this is how OptionsDialog works).
 */
class Settings
{
	wxXmlDocument doc; ///< The XML document to store data in.
	bool loaded; ///< Have settings been loaded (from disk)?

	/** 
	 * Returns the file name of the settings file on disk. The path name
	 * should be returned by Application::GetSettingsPath.
	 */
	wxString SettingsFile() const;

public:
	/** 
	 * Initializes the base settings with the default values. Load()
	 * has not been called at this point. To load settings from the SettingsFile
	 * use Load.
	 *
	 * @see Load
	 */
	Settings();

	/**
	 * Returns a long integer represented by the key.
	 */
	long GetLong(const wxString& key) const;

	/**
	 * Returns a boolean value represented by the key.
	 */
	bool GetBool(const wxString& key) const;

	/**
	 * Returns a string represented by the key.
	 */
	wxString GetString(const wxString& key) const;

	/**
	 * Sets a string value for the specified key.
	 */
	void Set(const wxString& key, const wxString& value);

	/**
	 * Sets a long integer value for the specified key.
	 */
	void Set(const wxString& key, long value);

	/**
	 * Sets a boolean value for the specified key.
	 */
	void Set(const wxString& key, bool value);

	/**
	 * Merges the current settings object with another, modifying
	 * the objects contents. This will overwrite any settings that
	 * other has but this does not, and will leave all settings that
	 * this object has intact if they are not defined in other
	 *
	 * Implementation note: this only works at the properties level of
	 * the root node of the XML file. Child elements will be ignored.
	 * This is not by design but by incomplete implementation.
	 */
	void Merge(const Settings& other);

	/**
	 * Sets all of the default setting values on this object.
	 */
	void Defaults();

	/** 
	 * Loads the SettingsFile XML document and replaces all current
	 * settings with those from disk
	 *
	 * @return whether data was successfully loaded from disk.
	 */
	bool Load();

	/**
	 * Saves the current object to disk as an XML file located at SettingsFile
	 *
	 * @return whether the object was saved to disk.
	 */
	bool Save() const;
};
