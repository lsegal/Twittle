#include "is.gd/isgd.h"
#include "http_client.h"
#include <wx/regex.h>

wxString Isgd::ShortenUrl(const wxString& url)
{
	HttpClient http;
	http.SetHeader(_T("Referer"), _T("http://is.gd/"));
	http.SetHeader(_T("Content-Type"), _T("application/x-www-form-urlencoded"));
	http.SetPostBuffer(_T("URL=") + url);
	wxString resp = http.Get(wxURL(_T("http://is.gd/create.php")));

	// find url in result
	wxRegEx re(_T("value=\"([^\"]+)\""), wxRE_ICASE | wxRE_ADVANCED);
	if (re.Matches(resp)) {
		return re.GetMatch(resp, 1);
	}
	return wxEmptyString;
}