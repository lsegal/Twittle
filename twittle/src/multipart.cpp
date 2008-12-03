#include "multipart.h"
#include <wx/sstream.h>

void Multipart::Add(const wxString& name, const wxString& value)
{
	wxString crlf = _T("\r\n");
	buffer << _T("--") << boundary << crlf;
	buffer << _T("Content-Disposition: form-data; name=\"") << name << _T("\"") << crlf << crlf;
	buffer << value << crlf;
}

void Multipart::AddFile(const wxString& name, const wxString& value, 
						const wxString& filename, const wxString& mimeType)
{
	wxString crlf = _T("\r\n");
	buffer << _T("--") << boundary << crlf;
	buffer << _T("Content-Disposition: form-data; name=\"") << name << _T("\"; ");
	buffer << _T("filename=\"") << filename << _T("\"") << crlf;
	buffer << _T("Content-Type: ") << mimeType << crlf;
	buffer << _T("Content-Transfer-Encoding: binary") << crlf << crlf;
	int len = value.length();
	buffer.Append(value);
	buffer << crlf;
}

void Multipart::End()
{
	buffer << _T("--") << boundary << _T("--\r\n");
}