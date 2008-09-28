#include <sstream>
#include <fstream>
#include <iomanip>
#include "http_client.h"
#include <wx/app.h>
#include <wx/filesys.h>

HttpClient::HttpClient()
{
	SetTimeout(10); // 10 seconds of timeout instead of 10 minutes
	SetHeader(_T("User-Agent"), _T("Tweet!"));
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
	unsigned short port;
	url.GetPort().ToULong(reinterpret_cast<unsigned long *>(&port));
	Connect(url.GetServer(), port);
	wxString path = (url.GetPath() == _T("") ? _T("/") : url.GetPath());
	wxInputStream *stream = GetInputStream(path);

	// Only return the stream if it's valid
	if (GetError() != wxPROTO_CONNERR) {
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

unsigned long HttpClient::GetToFile(const wxURL& url, const wxString& filename)
{
	wxInputStream *httpStream = GetResourceStream(url);

	if (httpStream) {
		unsigned int length = GetContentLength();
		char *data = new char[length];

		httpStream->Read(data, length);

		std::ofstream file(filename.c_str(), std::ios_base::binary);
		file.write(data, length);
		file.close();

		delete data;
	}

	// Close the stream
	delete httpStream;
	Close();

	return GetContentLength();
}

wxString HttpClient::UrlEncode(const wxString& input)
{
	std::stringstream out;
	const wxCharBuffer data = input.utf8_str();
	for (unsigned int i = 0; i < input.length(); i++) {
		char c = data[i];
		char cminus = tolower((int)c);
		if (cminus < 'a' || cminus > 'z') {
			out << '%' << std::hex << std::setw(2) << (int)c;
		}
		else {
			out << c;
		}
	} 
	return wxString::From8BitData(out.rdbuf()->str().c_str());
}