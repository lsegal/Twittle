#pragma once

#include <wx/sstream.h>
#include <wx/url.h>
#include <wx/xml/xml.h>
#include <wx/protocol/http.h>

class HttpClient : public wxHTTP
{
	wxInputStream *GetResourceStream(const wxURL& url);

public:
	HttpClient();

	static wxString UrlEncode(const wxString& input);

	unsigned long GetContentLength();
	wxString Get(const wxURL& url);
	wxXmlDocument GetXml(const wxURL& url);
	unsigned long GetToFile(const wxURL& url, const wxString& filename);
};
