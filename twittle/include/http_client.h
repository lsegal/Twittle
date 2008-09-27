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
	wxString Get(const wxURL& url);
	wxXmlDocument GetXml(const wxURL& url);
};
