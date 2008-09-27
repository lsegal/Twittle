#include "http_client.h"
#include <wx/app.h>

HttpClient::HttpClient()
{
	SetTimeout(10); // 10 seconds of timeout instead of 10 minutes
	SetHeader(_T("User-Agent"), _T("Tweet!"));
	SetHeader(_T("Accept"), _T("*/*"));
}

unsigned long HttpClient::GetContentLength()
{
	unsigned long val;
	wxString lenstr = this->GetHeader(_T("Content-length"));
	lenstr.ToULong(&val);
	return val;
}

wxInputStream *HttpClient::GetResourceStream(const wxURL& url)
{
	// Connect to server and setup stream on path
	Connect(url.GetServer());
	wxString path = (url.GetPath() == _T("") ? _T("/") : url.GetPath());
	wxInputStream *stream = GetInputStream(path);
	int x = this->GetResponse();
	wxString y = this->GetContentType();

	// Only return the stream if it's valid
	if (GetError() == wxPROTO_NOERR || GetError() == wxPROTO_NOFILE) {
		return stream;
	}
	else {
		return NULL;
	}
}

wxString HttpClient::Get(const wxURL& url)
{
	wxString data;
	wxInputStream *httpStream = GetResourceStream(url);

	if (httpStream) {
		wxStringOutputStream out_stream(&data);
		httpStream->Read(out_stream);
	}

	// Close the stream
	delete httpStream;
	Close();

	return data;
}

void *HttpClient::GetRaw(const wxURL& url)
{
	wxInputStream *httpStream = GetResourceStream(url);

	unsigned long length = GetContentLength();
	char *data = new char[length];
	if (httpStream) {
		httpStream->Read(data, length);
	}

	// Close the stream
	delete httpStream;
	Close();

	return data;
}

wxXmlDocument HttpClient::GetXml(const wxURL& url)
{
	wxXmlDocument doc;
	wxInputStream *httpStream = GetResourceStream(url);

	if (httpStream)
		doc.Load(*httpStream);

	// Close the stream
	delete httpStream;
	Close();

	return doc;
}
