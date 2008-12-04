#pragma once

#include <wx/wx.h>

/**
 * Implements multipart/form-data for HttpClient
 */
class Multipart
{
	wxString boundary;
	wxString buffer;

public:
	Multipart() : boundary(_T("MaGiCaLBoUnDaRyStRiNg")) { }

	void SetBoundary(const wxString& newboundary) { boundary = newboundary; }
	void End();
	const wxString& GetString() { return buffer; }
	const wxString& GetBoundary() { return boundary; }

	void Add(const wxString& name, const wxString& value);
	void AddFile(const wxString& name, const wxString& value, 
		const wxString& filename = wxEmptyString, 
		const wxString& mimeType = _T("application/octet-stream"));
};