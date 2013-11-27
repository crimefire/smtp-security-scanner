
#include "curl_tools.h"

CString IntToStr(int a)
{
	CString t;
	t.Format(_T("%d"),a);
	return t;
}

CString UrlEncode(CString url)
{
	url.Replace(_T("%"),_T("%25"));
	url.Replace(_T("!"),_T("%21"));
	url.Replace(_T("*"),_T("%2A"));
	url.Replace(_T("'"),_T("%27"));
	url.Replace(_T("("),_T("%28"));
	url.Replace(_T(")"),_T("%29"));
	url.Replace(_T(";"),_T("%3B"));
	url.Replace(_T(":"),_T("%3A"));
	url.Replace(_T("@"),_T("%40"));
	url.Replace(_T("&"),_T("%26"));
	url.Replace(_T("="),_T("%3D"));
	url.Replace(_T("+"),_T("%2B"));
	url.Replace(_T("$"),_T("%24"));
	url.Replace(_T(","),_T("%2C"));
	url.Replace(_T("/"),_T("%2F"));
	url.Replace(_T("?"),_T("%3F"));
	url.Replace(_T("#"),_T("%23"));
	url.Replace(_T("["),_T("%5B"));
	url.Replace(_T("]"),_T("%5D"));
	url.Replace(_T(" "),_T("%20"));
	url.Replace(_T("\r\n"),_T("%2C")); 
	url.Replace(_T("\n"),_T("%2C")); 
	url.Replace(_T("\""),_T("%22"));

	return url;
}

void NewCookie(CURL* curl)
{
	CString cookie = _T("cookie/cookie");
	CString cookie2;
	CString cookie3 = _T(".txt");
	int cookie4 = int(time(NULL));
	cookie2.Format(_T("%d"),cookie4);
	cookie = cookie+cookie2+cookie3;
	char* cookie_file = StringToChar(cookie);
	//MessageBox(cookie);

	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookie_file);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookie_file);
	curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
}

char* ClearUnicode(char* src)
{
	if(!src)
	{
		src = new char[1];
		src[0] = '\0';
	}
	else
	{
		int i = 0;
		char a;
		while(src[i]!='\0')
		{
			a = src[i];
			if(!(((a>=32)&&(a<=126))||(a==13)||(a==10)))
				src[i] = ' ';

			i++;
		}
	}

	return src;
}

thread_status* CopyThreadStatus(thread_status* arg)
{
	thread_status* copy;
	if(arg)
	{
		copy = new thread_status;
		copy->bufferlen = arg->bufferlen;
		copy->data = new char[copy->bufferlen+1];
		memcpy(copy->data,arg->data,arg->bufferlen+1);
		copy->writepos = arg->writepos;
		copy->initialized = arg->initialized;
	}
	else
		copy = NULL;

	return copy;
}

/*
void ClearCurlData(curl_out& data)
{
	data.data2 = _T("");
	data.size = 0;
	if(data.dataf)
		delete[] data.dataf;
	data.dataf = NULL;
}
*/

void CleanData(thread_status* thread)
{
	if(thread->data)
		delete[] thread->data;
	thread->data = NULL;
	thread->bufferlen = 0;
	thread->writepos = 0;
	thread->initialized = false;
}

char* StringToChar(CString& src)
{
	long len = src.GetLength();//wcslen (src);
	char* Resultt = new char[len+1];
	if(len==0)
	{
		Resultt[0] = '\0';
		return Resultt;
	}
	//wcstombs(Resultt,src,len);
	size_t chars;
	errno_t err;
	//wchar_t* aa = src.GetBuffer();
	err = wcstombs_s(&chars,Resultt,len+1,src,len+1);
	//src.ReleaseBuffer();
	//Resultt[len] = '\0';

	if(err)
		exit(err);

	return Resultt;
}

CString CharToString(char* src)
{
	//CString temp(src);
	CString temp;

	LPWSTR a = temp.GetBuffer(2048);

	size_t chars;
	errno_t err;
	err = mbstowcs_s(&chars,a,2048,src,2048);

	if(err)
		exit(err);

	temp.ReleaseBuffer();

	return temp;
}

void SetCurlUrl(CURL* curl,CString& url)
{
	char* urlc = StringToChar(url);
	curl_easy_setopt(curl, CURLOPT_URL,urlc); 
	delete[] urlc;
}

int FindStr(char* src,const char* to_find, int start)
{
	bool found;
	int src_char = start;
	int to_len = strlen(to_find);
	while(src[src_char]!='\0')
	{
		int to_find_char = 0;
		found = true;
		while(to_find[to_find_char]!='\0')
		{
			if(src[src_char+to_find_char]=='\0')
				return -1;

			if(src[src_char+to_find_char]!=to_find[to_find_char])
			{
				found = false;
				break;
			}
			to_find_char++;
		}

		if(found)
			return src_char;

		src_char++;
	}

	return -1;
}

char* MidStr(char* src,int start,int end)
{
	char* data = NULL;

	if(src==NULL)
		return data;

	if(strlen(src)<end||start<0||end<=start)
		return data;

	data = new char[end-start+1];
	memcpy(data,&(src[start]),end-start);
	data[end-start] = '\0';

	return data;
}

void SetCurlUrl(CURL* curl,char* url)
{
	curl_easy_setopt(curl, CURLOPT_URL,url); 
}
