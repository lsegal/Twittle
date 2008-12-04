#include "http/multipart.h"
#include <iostream>

static wxString crlf = _T("\r\n");

void Multipart::Add(const wxString& name, const wxString& value)
{
	buffer << _T("--") << boundary << crlf;
	buffer << _T("Content-Disposition: form-data; name=\"") << name << _T("\"") << crlf << crlf;
	buffer << value << crlf;
}

void Multipart::AddFile(const wxString& name, const wxString& value, 
						const wxString& filename, const wxString& mimeType)
{
	buffer << _T("--") << boundary << crlf;
	buffer << _T("Content-Disposition: form-data; name=\"") << name << _T("\"; ");
	buffer << _T("filename=\"") << filename << _T("\"") << crlf;
	buffer << _T("Content-Type: ") << mimeType << crlf;
	buffer << _T("Content-Transfer-Encoding: binary") << crlf << crlf;
	buffer << value << crlf;
}

void Multipart::End()
{
	buffer << _T("--") << boundary << _T("--") << crlf;
}