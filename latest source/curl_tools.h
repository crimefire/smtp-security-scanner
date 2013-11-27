
#ifndef CURL_TOOLS_H
#define CURL_TOOLS_H

#include "stdafx.h"
#include "curl/curl.h"

CString IntToStr(int a);

CString UrlEncode(CString url);

void NewCookie(CURL* curl);

char* ClearUnicode(char* src);

char* StringToChar(CString& src);

CString CharToString(char* src);

struct thread_status
{
    char *data;
    size_t bufferlen;
    size_t writepos;
    int initialized;
};

thread_status* CopyThreadStatus(thread_status* arg);

void CleanData(thread_status* thread);

size_t static
writefunction( void *ptr, size_t size,
               size_t nmemb, void *userdata)
{
    size_t nbytes = size*nmemb;
      struct thread_status **this_status;
      this_status = (struct thread_status **) userdata;
 
      if (!(*this_status)->initialized){
            (*this_status)->data = new char[nbytes+1];//(void *)malloc(nbytes);
            (*this_status)->bufferlen = nbytes;
            (*this_status)->writepos = 0;
            (*this_status)->initialized = true;
      }
 
      if ((*this_status)->bufferlen < ((*this_status)->writepos + nbytes))
	  {
		  //exit(33);
		  char* temp = new char[(size_t) ((*this_status)->writepos + nbytes+1)];
		 memcpy(temp,(*this_status)->data,(*this_status)->bufferlen);
		 temp[((*this_status)->writepos + nbytes)] = '\0';
		 if((*this_status)->data)
			 delete[] (*this_status)->data;
		 (*this_status)->data = temp;
		 (*this_status)->bufferlen = ((*this_status)->writepos + nbytes);
      }
 
      if((*this_status)->data == NULL)
			exit(4);
      memcpy((char*)((*this_status)->data + ((*this_status)->writepos)),(char*)ptr, nbytes);
      ((*this_status)->data)[(*this_status)->writepos+nbytes]='\0';
	  (*this_status)->writepos += nbytes;
		
    return nbytes;
}
/*
struct curl_out
{
	CString data2;
	char* dataf;
	size_t size;*/
/*
	curl_out() : dataf(NULL), size(0), changed(false) { }
	~curl_out() { if(dataf) delete[] dataf; dataf = NULL; }
	void Clear() {  if(dataf) delete[] dataf; dataf = NULL; size = 0; data2=_T(""); changed = false; }
*/
//};

//void ClearCurlData(curl_out& data);

/*
struct curl_out
{
	char* data;
	size_t size;

	curl_out() : data(NULL), size(0) { }
	~curl_out() { if(data) delete[] data; data = NULL; }
	void Clear() { if(data) delete[] data; data = NULL; size = 0; }
};
*/

// Write all expected data in here  
static char buffer2[65536+1];
/*
static int writer(char *data, size_t size, size_t nmemb,  
                  void *buffer)
{
// What we will return  
	int result = 0;  
	
	//MessageBoxW(NULL,_T("writer"),_T("error"),MB_OK);

	// Is there anything in the buffer?  
	if (buffer != NULL)  
	{
		for(int t=0;t<size*nmemb;t++)
			((curl_out*)buffer)->data2+=CharToString(data)//.AppendChar(data[t]);
		((curl_out*)buffer)->size = ((curl_out*)buffer)->data2.GetLength();
		if(((curl_out*)buffer)->dataf)
			delete[] ((curl_out*)buffer)->dataf;
		((curl_out*)buffer)->dataf = StringToChar(((curl_out*)buffer)->data2);
*/
		/*
		size_t new_size = ((curl_out*)buffer)->size+size*nmemb;
		char* new_data = new char[new_size+1];
		if(!new_data)
			MessageBoxW(NULL,_T("curl buffer out of memory"),_T("error"),MB_OK);
		memcpy(new_data,((curl_out*)buffer)->data,((curl_out*)buffer)->size);
		memcpy(&(new_data[((curl_out*)buffer)->size+1]),data,size*nmemb);
		new_data[new_size] = '\0';
		((curl_out*)buffer)->Clear();
		((curl_out*)buffer)->data = new_data;
		((curl_out*)buffer)->size = new_size;
		*/
/*
	  // Append the data to the buffer  
	  memcpy(buffer2,data,size*nmemb);
	  buffer2[size*nmemb] = '\0';

	  FILE* fil = fopen("out.txt","wb");
		fwrite (new_data,new_size+1,1,fil);

	  fclose(fil);
	*/
	  // How much did we write?  
/*	  result = size * nmemb;  
	}  
	else
		MessageBoxW(NULL,_T("empty curl buffer"),_T("error"),MB_OK);
	
	return result;  
}
*/
// Write any errors in here  
static char errorBuffer[CURL_ERROR_SIZE];  


static CString curl_buff;

void SetCurlUrl(CURL* curl,CString& url);

int FindStr(char* src,const char* to_find, int start = 0);

char* MidStr(char* src,int start,int end);

void SetCurlUrl(CURL* curl,char* url);

CString& EscapeSpaces(CString& src);

#endif