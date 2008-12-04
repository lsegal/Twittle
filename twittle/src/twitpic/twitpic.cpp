#include "twitpic/twitpic.h"
#include "http/http_client.h"
#include "http/multipart.h"
#include <wx/xml/xml.h>
#include <wx/wfstream.h>
#include <wx/mimetype.h> 
#include <wx/mstream.h>
#include <fstream>

wxString TwitPic::GetMimeType(const wxString& extension)
{
	wxFileType *type = wxTheMimeTypesManager->GetFileTypeFromExtension(extension);
	if (type != NULL) {
		wxString str;
		type->GetMimeType(&str);
		return str;
	}
	else {
		return _T("application/octet-stream");
	}
}

wxString TwitPic::UploadImage(const wxString& user, const wxString& pass,
							  const wxString& buffer, const wxString& filename)
{
	HttpClient http;
	Multipart part;
	
	part.AddFile(_T("media"), buffer, filename, GetMimeType(filename.AfterLast(_T('.'))));
	part.Add(_T("username"), user);
	part.Add(_T("password"), pass);
	part.End();

	http.SetTimeout(240); // longer timeout for files
	http.SetHeader(_T("Content-Type"), _T("multipart/form-data; boundary=\"") + part.GetBoundary() + _T("\""));
	http.SetPostBuffer(part.GetString());
	wxXmlDocument doc = http.GetXml(wxURL(_T("http://twitpic.com/api/upload")));
	wxXmlNode *root = doc.GetRoot();

	if (root) {
		if (root->GetPropVal(_T("stat"), _T("fail")) == _T("ok")) {
			wxXmlNode *child = root->GetChildren();
			while (child) {
				if (child->GetName() == _T("mediaurl")) {
					return child->GetNodeContent();
				}
				child = child->GetNext();
			}
		}
		else {
			wxXmlNode *child = root->GetChildren();
			if (child && child->GetName() == _T("err")) {
				throw child->GetPropVal(_T("msg"), _T("Unknown error"));
			}
		}
	}

	throw wxString(_T("Unknown error"));
}

wxString TwitPic::UploadImage(const wxString& user, const wxString& pass, const wxString& filename)
{
	wxMemoryOutputStream out;
	wxFileInputStream file(filename);
	if (file.IsOk()) {
		file.Read(out);
		wxString data = wxString::From8BitData((const char *)
			out.GetOutputStreamBuffer()->GetBufferStart(), (size_t)out.GetLength());
		return UploadImage(user, pass, data, filename.AfterLast(_T('\\')).AfterLast('/'));
	}
	throw _T("Missing or invalid file: ") + filename;
}
