#pragma once

#include <wx/wx.h>
#include "serializer.h"

class TwitterFeed;
class wxXmlNode;
class TwitterStatus;
class TwitterUser;

/**
 * Serializes a TwitterFeed object using XML documents 
 */
template<>
class Serializer<wxString, TwitterFeed>
{
private:
	/**
	 * Returns the TwitterStatus object from a TwitterFeed status collection as an XML node
	 */
	static wxXmlNode *StatusAsXml(const TwitterStatus& status);

	/**
	 * Returns the TwitterUser from a TwitterStatus object as an XML node
	 */
	static wxXmlNode *UserAsXml(const TwitterUser& user);

public:
	/** 
	 * Deserializes a TwitterFeed from an XML file specified at path
	 * 
	 * @param path the path of the XML file to deserialize the object from
	 * @return the deserialized feed object
	 */
	static TwitterFeed *Read(const wxString& path);

	/**
	 * Writes a TwitterFeed to an XML file on disk
	 *
	 * @param path the location of the XML file
	 * @param feed the feed object toe serialize
	 */
	static void Write(const wxString& path, const TwitterFeed& feed);
};
