#include "multipart.h"
#include <iostream>
static wxString crlf = _T("\r\n");

const wxChar fillchar = '=';

                        // 00000000001111111111222222
                        // 01234567890123456789012345
static wxString     cvt = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");

wxString Encode(const wxString& pData)
{
    size_t c;
	size_t len = pData.Length();
    wxString ret;
    ret.Alloc(len * 4 / 3 + len * 2 / 50);
    size_t resultLen = 0;
    wxString cr(_T("\r\n"));

    for (size_t i = 0; i < len; ++i)
    {
        c = (pData[i] >> 2) & 0x3f;
        ret.Append(cvt[c], 1);
        if (++resultLen == 72) { ret += cr; resultLen = 0; }
        c = (pData[i] << 4) & 0x3f;
        if (++i < len)
            c |= (pData[i] >> 4) & 0x0f;

        ret.Append(cvt[c], 1);
        if (++resultLen == 72) { ret += cr; resultLen = 0; }
        if (i < len)
        {
            c = (pData[i] << 2) & 0x3f;
            if (++i < len)
                c |= (pData[i] >> 6) & 0x03;

            ret.Append(cvt[c], 1);
        }
        else
        {
            ++i;
            ret.Append(fillchar, 1);
        }
        if (++resultLen == 72) { ret += cr; resultLen = 0; }

        if (i < len)
        {
            c = pData[i] & 0x3f;
            ret.Append(cvt[c], 1);
        }
        else
        {
            ret.Append(fillchar, 1);
        }
        if (++resultLen == 72) { ret += cr; resultLen = 0; }
    }

    return ret;
}

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
//	buffer << Encode(value) << crlf;
	buffer << value << crlf;
}

void Multipart::End()
{
	buffer << _T("--") << boundary << _T("--") << crlf;
}