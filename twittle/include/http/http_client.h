#pragma once

#include <wx/sstream.h>
#include <wx/url.h>
#include <wx/xml/xml.h>
#include <wx/protocol/http.h>

/**
 * Subclasses the wxHTTP HTTP library to make a slightly more 
 * programmer friendly HTTP client. Adds the Get(), GetXml()
 * and GetToFile() methods which simplify basic GET requests.
 *
 * To use this class for a POST request, simply call SetPostData()
 * before doing the Get and the method will automatically be selected.
 * This is done by wxHTTP.
 *
 * Note that the default timeout of this class is 10 seconds.
 */
class HttpClient : public wxHTTP
{
	/**
	 * Gets the input stream after setting up the connection
	 *
	 * @return on success, returns a wxInputStream, otherwise NULL
	 *   is returned.
	 */
	wxInputStream *GetResourceStream(const wxURL& url);

public:
	/** Default constructor for simple HTTP requests. */
	HttpClient();

	/** Construct an HTTP client with user/password auth information */
	HttpClient(const wxString& user, const wxString& pass);

	/** 
	 * Helper method to perform URLEncoding on an input string
	 *
	 * @param input the text to URLEncode
	 * @return the URLEncoded string
	 */
	static wxString UrlEncode(const wxString& input);

	/**
	 * Returns the Content-Length header (omitted from wxHTTP)
	 *
	 * @pre a Content-Length header exists
	 */
	unsigned long GetContentLength();

	/**
	 * Returns the response from a GET/POST request as a string.
	 *
	 * Note: To perform a POST request, call SetPostData before 
	 *       calling this method.
	 *
	 * @param url the URL to request
	 * @return the response contents as a string
	 */
	wxString Get(const wxURL& url);

	/**
	 * Same as Get but parses the output as an XML document and 
	 * returns that instead.
	 *
	 * Note: GetXml may fail if the XML document is invalid.
	 *
	 * @param url the URL to request
	 * @return the response contents as an XML document
	 */
	wxXmlDocument GetXml(const wxURL& url);

	/**
	 * Gets the url as a file and stores it in filename, returning the 
	 * content length of the response.
	 *
	 * @param url the URL of the file to retrieve
	 * @param filename the filename to store the output contents
	 * @return the content length (file length) of the response data
	 */
	unsigned long GetToFile(const wxURL& url, const wxString& filename);
};
