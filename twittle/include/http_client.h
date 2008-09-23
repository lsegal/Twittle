#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H 1

#include <wx/sstream.h>
#include <wx/url.h>
#include <wx/xml/xml.h>
#include <wx/protocol/http.h>

class HttpClient : public wxHTTP
{
	wxInputStream *GetResourceStream(const wxURL& url);

public:
	wxString Get(const wxURL& url);
	wxXmlDocument GetXml(const wxURL& url);
};

#endif /* HTTPCLIENT_H */
