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
	unsigned long GetContentLength();
	wxString Get(const wxURL& url);
	unsigned long GetToFile(const wxURL& url, const wxString& filename);
	wxXmlDocument GetXml(const wxURL& url);
};
