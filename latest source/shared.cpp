
#include "shared.h"

#define SOCK_BUFFER 2048
#define CMD_DELAY 50

void ScrollToBottom(CDlgMain* dlg)
{
	long l = MAKELONG(SB_BOTTOM,0);
	::SendMessage(dlg->GetDlgItem(IDC_EDIT14)->GetSafeHwnd(), WM_VSCROLL, l ,0);
}

CString Base64Encode(const BYTE *strIn, DWORD len)
{
	CString temp;
      const static CHAR base64ABC[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

      for (DWORD i = 0; i < len; i += 3) {
            LONG l = ( ((LONG)strIn[i]) << 16 ) |
            (((i + 1) < len) ? (((LONG)strIn[i + 1]) << 8) : 0) |
            (((i + 2) < len) ? ((LONG)strIn[i + 2]) : 0);

            temp += base64ABC[(l >> 18) & 0x3F];
            temp += base64ABC[(l >> 12) & 0x3F];
            if (i + 1 < len) temp += base64ABC[(l >> 6) & 0x3F];
            if (i + 2 < len) temp += base64ABC[(l ) & 0x3F];
      }

      switch (len % 3) {
      case 1:            temp += '=';
      case 2:            temp += '=';
      }

  return temp;
}

void ShowError(CSocket* socket,CDlgMain* dlg)
{
	dlg->console+=_T("\r\nError (");
	CString temp;
	temp.Format(_T("%d"),socket->GetLastError());
	dlg->console+=temp;
	dlg->console+=_T(")");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);
}

void Send(CSocket* socket,CDlgMain* dlg,const char* data)
{
	char* ndata = new char[strlen(data)+3];
	if(strlen(data))
		memcpy(ndata,data,strlen(data));
	ndata[strlen(data)+0] = '\r';
	ndata[strlen(data)+1] = '\n';
	ndata[strlen(data)+2] = '\0';
	
	if(socket->Send(ndata,strlen(data)+2,0)==SOCKET_ERROR)
		ShowError(socket,dlg);
	
	CString temp(data);
	dlg->console+=_T("\r\n>>> ");
	if(strlen(data)<1000)
		dlg->console+=data;
	else
		dlg->console+=_T("[DATA TOO LONG TO SHOW]");

	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);
}

int Receive(CSocket* socket,CDlgMain* dlg,char* buffer)
{
	int res = socket->Receive(buffer,SOCK_BUFFER,0);
	if(res==SOCKET_ERROR)
		ShowError(socket,dlg);
	else if(res==0)
	{
		dlg->console+=_T("\r\nRecv:Connection closed.");
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);
	}
	else
	{
		buffer[res] = '\0';
		CString temp(buffer);
		dlg->console+=_T("\r\n<<< ");
		dlg->console+=temp;
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);
	}

	return res;
}

CString GetDomain(CString& src)
{
	CString result;

	char* address = StringToChar(src);
	bool is_ip = true;

	int i=0;
	while(address[i]!='\0')
	{
		if((address[i]!='.')||((address[i]<'0')&&(address[i]>'9')))
		{
			is_ip = false;
			break;
		}
		i++;
	}

	if(is_ip)
		result = src;
	else
	{
		int dots = 0;
		int i=strlen(address)-1;
		while(i>=0)
		{
			if(address[i]=='.')
				dots++;
			if(dots==2)
				break;
			i--;
		}

		char* addr = MidStr(address,i+1,strlen(address));
		CString dom(addr);
		result = dom;
	}

	return result;
}

bool Auth(CSocket* socket,CDlgMain* dlg,char* buffer,bool auth_use,CString cauth_login,CString cauth_pass)
{
	Send(socket,dlg,"EHLO mozilla");
	Receive(socket,dlg,buffer);

	char* auth_login = StringToChar(cauth_login);
	int auth_login_len = strlen(auth_login);
	char* auth_pass = StringToChar(cauth_pass);
	int auth_pass_len = strlen(auth_pass);

	int auth2_len = 2+auth_login_len+auth_pass_len;
	char* auth2 = new char[auth2_len];
	auth2[0] = '\0';
	memcpy(&(auth2[1]),auth_login,auth_login_len);
	auth2[1+auth_login_len] = '\0';
	memcpy(&(auth2[2+auth_login_len]),auth_pass,auth_pass_len);

	CString auth = Base64Encode((unsigned char*)auth2,auth2_len);
	char* auth3 = StringToChar(auth);

	Send(socket,dlg,"AUTH PLAIN");
	Receive(socket,dlg,buffer);

	if(strcmp((const char*)MidStr(buffer,0,3),"334")!=0)
		return false;

	Send(socket,dlg,(const char*)auth3);
	Receive(socket,dlg,buffer);

	return (strcmp((const char*)MidStr(buffer,0,3),"235")==0);
}

bool RelayTest(CSocket* socket,CDlgMain* dlg,char* buffer,CDlgSettings* dlg2)
{
	CString msg,temp;
	dlg->console+=_T("\r\n*Starting Open Relay Test.");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);
	msg = _T("HELO ");
	msg+=dlg2->set_relayfrom2;
	Send(socket,dlg,StringToChar(msg));
	Receive(socket,dlg,buffer);

	bool open_relay = false;
	bool test_open[20];
	bool from_ok, to_ok;
	
	UINT port = 25; // smtp port

	memset(test_open,0,20);

	if(!(socket->GetPeerName(temp,port)))
		ShowError(socket,dlg);

	for(int i=1;i<=20;i++)
	{
		from_ok = false;
		to_ok = false;

		Sleep(CMD_DELAY);
		Send(socket,dlg,"RSET");
		Receive(socket,dlg,buffer);
		msg = _T("MAIL FROM:<");

		if(i!=4)
			msg+=dlg2->set_relayfrom1;
		if((i!=4)&&(i!=3))
			msg+=_T("@");
		if((i>=7)||(i==5))
		{
			msg+=_T("[");
			msg+=temp;
			msg+=_T("]");
		}
		if(i==6)
		{
			msg+=_T("www.");
			msg+=dlg->res_domain;//dlg->settings->server_address; // not always domain
		}
		if((i==1)||(i==2))
			msg+=dlg2->set_relayfrom2;

		msg+=_T(">");
		Send(socket,dlg,StringToChar(msg));
		Receive(socket,dlg,buffer);
		if(strcmp((const char*)MidStr(buffer,0,3),"250")==0)
			from_ok = true;

		msg = _T("RCPT TO:");
		
		if(i!=2)
			msg+=_T("<");
		if((i==9)||(i==10)||(i==12))
			msg+=_T("\"");
		if((i==14)||(i==15))
			msg+=_T("@");
		if(i==15)
		{
			msg+=_T("www.");
			msg+=dlg->res_domain;//dlg->settings->server_address; // not always domain
		}
		if(i==14)
		{
			msg+=_T("[");
			msg+=temp;
			msg+=_T("]");
		}
		if((i==14)||(i==15))
			msg+=_T(":");
		if((i<=15)||(i==19)||(i==20))
			msg+=dlg2->set_relayto1;
		if((i==7)||(i==8)||(i==10)||(i==19))
			msg+=_T("%");
		if((i==11)||(i==12)||(i==13)||(i==20))
			msg+=_T("@");
		if( (i==7) || (i==8) || ((i>=10)&&(i<=13)) || (i>=16) )
			msg+=dlg2->set_relayto2;
		if((i>=16)&&(i<=18))
		{
			msg+=_T("!");
			msg+=dlg2->set_relayto1;
		}
		if(i==12)
			msg+=_T("\"");
		if((i!=10)&&(i!=16))
			msg+=_T("@");
		if((i==7)||(i==11)||(i==12)||(i==17))
		{
			msg+=_T("[");
			msg+=temp;
			msg+=_T("]");
		}
		if((i==8)||(i==13)||(i==18))
		{
			msg+=_T("www.");
			msg+=dlg->res_domain;//dlg->settings->server_address; // not always domain
		}
		if((i<=6)|| (i==9) || (i==14) || (i==15))
			msg+=dlg2->set_relayto2;
		if((i==9)||(i==10))
			msg+=_T("\"");
		if(i!=2)
			msg+=_T(">");

		Send(socket,dlg,StringToChar(msg));
		Receive(socket,dlg,buffer);
		if(strcmp((const char*)MidStr(buffer,0,3),"250")==0)
			to_ok = true;

		if(from_ok&&to_ok)
		{
			open_relay = true;
			test_open[i-1] = true;
		}

		dlg->result_3openrelay.Format(_T("%d"),i*5);
		dlg->result_3openrelay+=_T("%");
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);

		//Sleep(CMD_DELAY);
	}

	//Sleep(CMD_DELAY);
	//Send(socket,dlg,"QUIT");
	//Receive(socket,dlg,buffer);

	// test_open

	if(open_relay)
		dlg->result_3openrelay = _T("YES");
	else
		dlg->result_3openrelay = _T("NO");

	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	return open_relay;
}

bool SpoofingTest(CSocket* socket,CDlgMain* dlg,char* buffer,int which,CDlgSettings* dlg2)
{
	bool spoofing = true;

	dlg->console+=(which==1)?_T("\r\n*Starting Spoofing Test."):_T("\r\n*Starting Spoofing check 2.");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	if((which==2)&&(dlg2->auth_use))
	{
		if(!Auth(socket,dlg,buffer,dlg2->auth_use,dlg2->set_authuser,dlg2->set_authpass))
		{
			dlg->result_spoofing1 = _T("?");
			dlg->MessageBox(_T("Couldn't test spoofing because provided authentication data are incorrect"));
			return false;
		}
	}
	else
	{
		Send(socket,dlg,"HELO mozilla");
		Receive(socket,dlg,buffer);
	}

	CString temp;
	char* msg;

	temp = _T("MAIL FROM:<");
	temp+=dlg2->set_spoofing_mailfrom;
	temp+=_T(">");
	msg = StringToChar(temp);
	Send(socket,dlg,msg);
	Receive(socket,dlg,buffer);

	if(strcmp((const char*)MidStr(buffer,0,3),"250")!=0)
		spoofing = false;
	else
	{
		temp = _T("RCPT TO:<");
		temp+=dlg2->set_spoofing_rpctto;
		temp+=_T(">");
		msg = StringToChar(temp);
		Send(socket,dlg,msg);
		Receive(socket,dlg,buffer);

		if(strcmp((const char*)MidStr(buffer,0,3),"250")!=0)
			spoofing = false;
		else
		{
			Send(socket,dlg,"DATA");
			Receive(socket,dlg,buffer);
			if(strcmp((const char*)MidStr(buffer,0,3),"354")!=0)
				spoofing = false;
			else
			{
				temp = _T("From: ");
				temp+=dlg2->set_spoofing_from;
				msg = StringToChar(temp);
				Send(socket,dlg,msg);

				temp = _T("To: ");
				temp+=dlg2->set_spoofing_to;
				msg = StringToChar(temp);
				Send(socket,dlg,msg);

				temp = _T("Reply-to: ");
				temp+=dlg2->set_spoofing_replyto;
				msg = StringToChar(temp);
				Send(socket,dlg,msg);

				temp = _T("Subject: ");
				temp+=dlg2->set_spoofing_subject;
				msg = StringToChar(temp);
				Send(socket,dlg,msg);

				Send(socket,dlg,"");

				POSITION lpos = dlg2->set_spoofing_message_list.GetHeadPosition();
				CString line;
				for(int k=0;k<dlg2->set_spoofing_message_list.GetCount();k++)
				{
					line = dlg2->set_spoofing_message_list.GetNext(lpos);
					msg = StringToChar(line);
					Send(socket,dlg,msg);
				}

				Send(socket,dlg,".");

				Receive(socket,dlg,buffer);

				spoofing = (strcmp((const char*)MidStr(buffer,0,3),"250")==0);

				Send(socket,dlg,"QUIT");
				Receive(socket,dlg,buffer);
			}
		}
	}

	if(which==1)
		dlg->result_spoofing1 = spoofing?_T("YES"):_T("NO");
	else
		dlg->result_spoofing2 = spoofing?_T("YES"):_T("NO");

	return true;
}

bool VerifyTest(CSocket* socket,CDlgMain* dlg,char* buffer,CDlgSettings* dlg2)
{
	dlg->console+=_T("\r\n*Starting Verify Command Test.");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	CString temp;
	CString domain = dlg->res_domain;//GetDomain(dlg->settings->server_address);
	char* msg;

	Send(socket,dlg,"HELO mozilla");
	Receive(socket,dlg,buffer);

	temp = _T("VRFY ");
	temp+=_T("frankblack");
	temp+=_T("@");
	temp+=domain;
	msg = StringToChar(temp);
	Send(socket,dlg,msg);
	Receive(socket,dlg,buffer);

	bool verify = ((strcmp((const char*)MidStr(buffer,0,3),"502")!=0)&&(strcmp((const char*)MidStr(buffer,0,3),"252")!=0));
	bool from = true;

	if(!verify)
	{
		Send(socket,dlg,"RSET");
		Receive(socket,dlg,buffer);

		temp = _T("MAIL FROM:<");
		temp+=dlg2->set_fromabuseverify;
		temp+=_T(">");
		msg = StringToChar(temp);
		Send(socket,dlg,msg);
		Receive(socket,dlg,buffer);

		if(strcmp((const char*)MidStr(buffer,0,3),"250")!=0)
			from = false;
		else
		{
			temp = _T("RCPT TO:<");
			temp+=_T("userdhdrhanonexistafw");
			temp+=_T("@");
			temp+=domain;
			temp+=_T(">");
			msg = StringToChar(temp);
			Send(socket,dlg,msg);
			Receive(socket,dlg,buffer);

			POSITION lpos = dlg->rcptto_list.GetHeadPosition();

			verify = false;

			CString rcp;

			for(int k=0;k<dlg->rcptto_list.GetCount();k++)
			{
				rcp = dlg->rcptto_list.GetNext(lpos);

				if(FindStr(buffer,StringToChar(rcp),0)!=-1)
				{
					verify = true;
					break;
				}
			}
			//verify = (FindStr(buffer,"5.3.0",0)!=-1);
		}
	}

	if(!from)
	{
		dlg->MessageBox(_T("Couldn't test verify as FROM field is incorrect"));
		dlg->result_veriify = _T("?");
	}
	else
		dlg->result_veriify = verify?_T("YES"):_T("NO");

	return verify;
}

bool PostAbuseTest(CSocket* socket,CDlgMain* dlg,char* buffer,CDlgSettings* dlg2,int mode)
{
	Send(socket,dlg,"HELO mozilla");
	Receive(socket,dlg,buffer);

	CString domain = dlg->res_domain;//GetDomain(dlg->settings->server_address);

	bool from = true;
	bool to_post = true;
	bool to_abuse = true;

	for(int i=mode;i<mode+1;i++)
	{
		/*
		if(i==1)
		{
			Send(socket,dlg,"RSET");
			Receive(socket,dlg,buffer);
		}*/

		CString temp,temp2;
		char* msg;
		UINT port = 25;

		temp = _T("MAIL FROM:<");
		temp+=dlg2->set_fromabuseverify;
		temp+=_T(">");
		msg = StringToChar(temp);
		Send(socket,dlg,msg);
		Receive(socket,dlg,buffer);

		if(strcmp((const char*)MidStr(buffer,0,3),"250")!=0)
			from = false;
		else
		{
			temp = _T("RCPT TO:<");
			temp+=i?_T("abuse"):_T("postmaster");
			temp+=_T("@");
			temp+=domain;
			temp+=_T(">");
			msg = StringToChar(temp);
			Send(socket,dlg,msg);
			Receive(socket,dlg,buffer);

			if(strcmp((const char*)MidStr(buffer,0,3),"250")!=0)
			{
				if(i)
					to_abuse = false;
				else
					to_post = false;
			}
			else
			{
			}
		}
	}

	if(from==true)
	{
		if(!mode)
			dlg->result_postmaster = to_post?_T("YES"):_T("NO");
		if(mode)
			dlg->result_abuse = to_abuse?_T("YES"):_T("NO");
	}
	else
	{
		dlg->MessageBox(_T("Couldn't test postmaster/abuse as FROM field is incorrect"));
		if(!mode)
			dlg->result_postmaster = _T("?");
		if(mode)
			dlg->result_abuse = _T("?");
	}

	return true;
}

bool AttachmentTest(CSocket* socket,CDlgMain* dlg,char* buffer,CDlgSettings* dlg2)
{
	bool filtering = false;

	dlg->console+=_T("\r\n*Starting Attachment filtering Test.");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	if(dlg2->auth_use)
	{
		if(!Auth(socket,dlg,buffer,dlg2->auth_use,dlg2->set_authuser,dlg2->set_authpass))
		{
			dlg->result_attachm = _T("?");
			dlg->MessageBox(_T("Couldn't test attachment filtering because provided authentication data are incorrect"));
			return false;
		}
	}
	else
	{
		Send(socket,dlg,"HELO mozilla");
		Receive(socket,dlg,buffer);
	}

	CString temp, temp2;
	char* msg;

	temp = dlg2->set_attach_path;
	temp+= _T("/*.*");

	char* path = StringToChar(temp);

	CFileFind Finder;
	BOOL found1 = Finder.FindFile(temp);
	DWORD Error1 = GetLastError();
	DWORD Error2 = -1;
	BOOL found2 = TRUE;
	CString sName = _T("(not found)");
	int check = 0;

	CString sEncoded;
	char* encoded;

	if(found1)
	{
		while(found2)
		{
			check++;
			found2 = Finder.FindNextFile();
			Error2 = GetLastError();

			if(!Finder.IsDirectory())
			{
				sName = Finder.GetFileName();

				FILE* pFile;
				long fSize;
				char* fbuffer;
				size_t fresult;
				temp = dlg2->set_attach_path;
				temp+= _T("/");
				temp+=sName;
				msg = StringToChar(temp);
				pFile = fopen((const char*)msg,"rb");
				if(!pFile)
				{
					dlg->result_attachm = _T("?");
					dlg->UpdateData(FALSE);
					ScrollToBottom(dlg);

					dlg->MessageBox(_T("Attachment filtering test error: Unable to open file"));
					break;
				}
				else
				{
					fseek(pFile , 0 , SEEK_END);
					fSize = ftell(pFile);
					rewind(pFile);

					fbuffer = new char[fSize];
					fresult = fread(fbuffer,1,fSize,pFile);
					fclose (pFile);
					sEncoded = Base64Encode((unsigned char*)fbuffer,fSize);
					free (fbuffer);

					Send(socket,dlg,"RSET");
					Receive(socket,dlg,buffer);

					temp = _T("MAIL FROM:<");
					temp+=dlg2->set_attach_from;
					temp+=_T(">");

					msg = StringToChar(temp);
					Send(socket,dlg,msg);
					Receive(socket,dlg,buffer);

					if(strcmp((const char*)MidStr(buffer,0,3),"250")!=0)
					{
						dlg->result_attachm = _T("?");
						dlg->UpdateData(FALSE);

						dlg->MessageBox(_T("Attachment filtering test error: Bad FROM field"));
						break;
					}
					else // [FROM] OK
					{
						temp = _T("RCPT TO:<");
						temp+=dlg2->set_attach_to;
						temp+=_T(">");
						msg = StringToChar(temp);
						Send(socket,dlg,msg);
						Receive(socket,dlg,buffer);

						if(strcmp((const char*)MidStr(buffer,0,3),"250")!=0)
						{
							dlg->result_attachm = _T("?");
							dlg->UpdateData(FALSE);

							dlg->MessageBox(_T("Attachment filtering test error: Bad RCPT TO field"));
							break;
						}
						else // [TO] OK
						{
							Send(socket,dlg,"DATA");
							Receive(socket,dlg,buffer);

							Send(socket,dlg,"MIME-Version: 1.0");

							temp = _T("From: ");
							temp+=dlg2->set_attach_from;
							msg = StringToChar(temp);
							Send(socket,dlg,msg);

							temp = _T("To: ");
							temp+=dlg2->set_attach_to;
							msg = StringToChar(temp);
							Send(socket,dlg,msg);

							temp = _T("Subject: Check Nr ");
							temp2.Format(_T("%d"),check);
							temp+=temp2;
							temp+= _T(" with \"");
							temp+=sName;
							temp+= _T("\"");
							msg = StringToChar(temp);
							Send(socket,dlg,msg);

							Send(socket,dlg,"Content-Type: multipart/mixed; boundary=\"xxxHELLOxWORLDxxx\"");
							Send(socket,dlg,"");
							Send(socket,dlg,"I am MIME message.");
							Send(socket,dlg,"--xxxHELLOxWORLDxxx");
							Send(socket,dlg,"Content-type: text/plain;");
							Send(socket,dlg,"");
							Send(socket,dlg,"SMTP Tool test.");
							Send(socket,dlg,"--xxxHELLOxWORLDxxx");
							Send(socket,dlg,"Content-Type: application/octet-stream");
							Send(socket,dlg,"Content-Transfer-Encoding: base64");

							temp = _T("Content-Disposition: attachment; filename=\"");
							temp+=sName;
							temp+= _T("\"");
							msg = StringToChar(temp);
							Send(socket,dlg,msg);

							Send(socket,dlg,"");

							// data
							encoded = StringToChar(sEncoded);
							Send(socket,dlg,encoded);
							delete encoded;

							Send(socket,dlg,"--xxxHELLOxWORLDxxx--");
							Send(socket,dlg,"");
							Send(socket,dlg,".");

							Receive(socket,dlg,buffer);

							filtering = (strcmp((const char*)MidStr(buffer,0,3),"250")!=0);
							
							if(filtering)
							{
								//break;
							}

							if((dlg2->set_attach_delay!=_T(""))||(dlg2->set_attach_delay!=_T("0")))
							{
								temp = _T("\r\nWaiting ");
								int ag = _wtoi(dlg2->set_attach_delay);
								temp2.Format(_T("%d"),ag);
								temp+=temp2;
								temp+= _T(" seconds...");
								dlg->console+=temp;
								dlg->UpdateData(FALSE);
								ScrollToBottom(dlg);
								
								Sleep(ag*1000);
							}
						}
					}
				}
			}
		}
	}

	if(dlg->result_attachm!=_T("?"))
		dlg->result_attachm = filtering?_T("YES"):_T("NO");

	Send(socket,dlg,"QUIT");
	Receive(socket,dlg,buffer);

	return true;
}

bool SpfTest(CDlgMain* dlg,CURL* curl,thread_status* this_struct)
{
	dlg->console+=_T("\r\n*Retrieving SPF record.");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	CURLcode result; 
	
	CString temp = _T("http://tools.bevhost.com/cgi-bin/dnslookup?data=");
	CString domain = dlg->res_domain;//GetDomain(dlg->settings->server_address);

	temp+=domain;
	temp+=_T("&server=");
	
	SetCurlUrl(curl,temp);

	CleanData(this_struct);
	result = curl_easy_perform(curl);

	if((result!=CURLE_OK)||(this_struct->data==NULL))
	{
		//dlg->MessageBox(_T("Unable to retrieve spf record"));
		dlg->result_spfbox = _T("");
		dlg->result_spf = _T("?");
		dlg->console+=_T("\r\nUnable to get SPF record.");
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);
	}
	else
	{
		int pos = 0;
		int pos_end = 0;
		dlg->result_spfbox = _T("");
		while(true)
		{
			pos = FindStr(this_struct->data,"descriptive text \"",pos);
			pos_end = FindStr(this_struct->data,"\"",pos+19);
			if(pos!=-1)
			{
				dlg->result_spfbox+=MidStr(this_struct->data,pos+18,pos_end);
				dlg->result_spfbox+=_T("\r\n");
				//dlg->result_spf_type = _T("TXT");
				dlg->result_spf_domain = domain;
			}
			else
				break;

			pos+=1;
		}

		if(dlg->result_spfbox==_T(""))
		{
			dlg->console+=_T("\r\nNo SPF records found.");
			dlg->result_spfbox = _T("[No SPF records found]");
			dlg->result_spf = _T("NO");
		}
		else
		{
			dlg->console+=_T("\r\nSuccess.");
			dlg->result_spf = _T("YES");
		}

		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);
	}

	return true;
}

bool BlacklistTest(CDlgMain* dlg,CURL* curl,thread_status* this_struct)
{
	dlg->console+=_T("\r\n*Retrieving Blacklist record.");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	CURLcode result; 

	CString temp = _T("q=");
	CString temp2;
	temp+=dlg->res_domain;//GetDomain(dlg->settings->server_address);
	//temp+=_T("&check=check");

	SetCurlUrl(curl,"http://www.blacklistalert.org/");
	CleanData(this_struct);
	result = curl_easy_perform(curl);
//<input type=hidden name="PHPSESSID" value="1885fc88a84fee781f38ef97da59386f">
	int af = FindStr(this_struct->data,"name=\"PHPSESSID\" value=\"",0);
	int af2 = FindStr(this_struct->data,"\">",af);
	CString sess = CharToString(MidStr(this_struct->data,af+24,af2));
	temp+=_T("&PHPSESSID=")+sess;

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, StringToChar(temp));
	
	SetCurlUrl(curl,"http://www.blacklistalert.org/");

	CleanData(this_struct);
	result = curl_easy_perform(curl);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

	if((this_struct->data==NULL))
	{
		//dlg->MessageBox(_T("Unable to retrieve blacklist records"));
		dlg->result_blacklistednum = _T("?");
		dlg->console+=_T("\r\nUnable to get Blacklist record.");
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);
	}
	else
	{
		int pos1 = FindStr(this_struct->data,"(Alphabetic order):</b><br>",0);
		int pos1end = FindStr(this_struct->data,"<hr>",pos1+10);
		int pos2 = FindStr(this_struct->data,"(Alphabetic order)</b><br>",pos1end);
		int pos2end = FindStr(this_struct->data,"<hr>",pos2+10);

		if((pos1==-1)||(pos1end==-1)||(pos2==-1)||(pos2end==-1))
		{
			dlg->result_blacklistednum = _T("?");
			dlg->result_blacklist = _T("?");
			dlg->console+=_T("\r\nUnable to get Blacklist record.");
			dlg->UpdateData(FALSE);
			ScrollToBottom(dlg);
		}
		else
		{
			
			int pos4 = 0, pos5 = 0, pos6 = pos1+26, pos7 = 0, pos8 = 0, pos9 = 0;

			bool ok;
			bool no_result;

			int c_ok = 0;
			int c_listed = 0;
			int count = 0;

			int index = 0;

			while((pos4!=-1)&&(pos6<pos2end)) // < !!!!!!!!!!!!!!!!!!!
			{
				pos4 = FindStr(this_struct->data,"<br>",pos6);
				if(pos4==pos1end-4)
					pos4 = FindStr(this_struct->data,"<br>",pos2+26);
				else if(pos4==pos2end-4)
					break;
				pos5 = FindStr(this_struct->data,"<font color=",pos4);
				temp = MidStr(this_struct->data,pos4+4,pos5-1);
				index = dlg->blacklist.InsertItem(dlg->blacklist.GetItemCount(),temp);
				pos6 = FindStr(this_struct->data,">",pos5+5);
				pos7 = FindStr(this_struct->data,"</em>",pos6);
				pos8 = FindStr(this_struct->data,"href=\"",pos6);
				pos9 = FindStr(this_struct->data,"\">",pos8);
				// is blacklsited or not
				ok = (strcmp((const char*)MidStr(this_struct->data,pos5+12,pos6),"green")==0);
				no_result = (strcmp((const char*)MidStr(this_struct->data,pos5+12,pos6),"magenta")==0);

				if(ok)
				{
					dlg->blacklist.SetItemText(index,1,_T("OK"));
					c_ok++;
				}
				else if(no_result)
				{
					dlg->blacklist.SetItemText(index,1,_T("NO RESULT"));
				}
				else
				{
					dlg->blacklist.SetItemText(index,1,_T("LISTED"));
					c_listed++;

					if(pos8<pos7)
					{
						CString temp3(MidStr(this_struct->data,pos8+6,pos9));
						dlg->blacklist.SetItemText(index,2,temp3);
					}
				}

				count++;

				temp.Format(_T("%d"),c_listed);
				temp2.Format(_T("%d"),count);
				temp+=_T("/");
				temp+=temp2;

				dlg->result_blacklistednum = temp;
				dlg->UpdateData(FALSE);
			}

			if(c_listed)
				dlg->result_blacklist = _T("YES");
			else
				dlg->result_blacklist = _T("NO");

			dlg->console+=_T("\r\nSuccess.");

			dlg->UpdateData(FALSE);
			ScrollToBottom(dlg);
		}
	}

	return true;
}

bool HelpTest(CSocket* socket,CDlgMain* dlg,char* buffer)
{
	bool result = false;

	dlg->console+=_T("\r\n*Starting Help command Test.");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	Send(socket,dlg,"HELO mozilla");
	Receive(socket,dlg,buffer);

	Send(socket,dlg,"HELP");
	Receive(socket,dlg,buffer);

	result = (strcmp((const char*)MidStr(buffer,0,3),"214")==0);

	dlg->result_help = result?_T("YES"):_T("NO");

	return true;
}


bool EhloTest(CSocket* socket,CDlgMain* dlg,char* buffer,int subtype)
{
	CString name[4];
	name[0] = _T("Encrypted authentication");
	name[1] = _T("Cleartext login");
	name[2] = _T("SASL Login methods");
	name[3] = _T("Authentication");

	dlg->console+=(_T("\r\n*Starting ")+name[subtype-1]+_T(" Test."));
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	Send(socket,dlg,"EHLO mozilla");
	Receive(socket,dlg,buffer);

	bool result = false;

	result = (strcmp((const char*)MidStr(buffer,0,3),"250")==0);

	int pos1, pos2;

	if(result)
	{
		switch(subtype)
		{
			case 1: // ENCR
				result = FindStr(buffer,"STARTTLS",0)!=-1;
				dlg->result_encauth = result?_T("YES"):_T("NO");
				break;
			case 2: // CLEARTEXT
				result = FindStr(buffer,"PLAIN",0)!=-1;
				dlg->result_cleartext = result?_T("YES"):_T("NO");
				break;
			case 3: // SASL
				pos1 = FindStr(buffer,"AUTH",0);
				if(pos1!=-1)
				{
					pos2 = FindStr(buffer,"\r",pos1);
					dlg->result_sasl = CharToString(MidStr(buffer,pos1+5,pos2));
					dlg->result_sasl.Replace(_T("LOGIN"),_T(""));
				}
				else
				{
					dlg->result_sasl = _T("-");
				}
				break;
			case 4: // AUTH
				result = ((FindStr(buffer,"AUTH",0)!=-1)||(FindStr(buffer,"STARTTLS",0)!=-1));
				dlg->result_auth = result?_T("YES"):_T("NO");
				break;
		}
	}
	else
	{
		dlg->console+=_T("\r\n[ERROR] Server extensions not obtained.");
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);
	}

	return true;
}


CSocket* Connect(CSocket* socket,CDlgMain* dlg,char* buffer,bool auth,CDlgSettings* dlg2)
{
	if(socket)
	{
		dlg->console+=_T("\r\nDisconnecting...");
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);

		Send(socket,dlg,"QUIT");
		Receive(socket,dlg,buffer);
		delete socket;
		socket = NULL;
	}

	dlg->console+=_T("\r\nConnecting...");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	socket = new CSocket;
	//dlg->socket = socket;
	socket->Create();
	if(socket->Connect(dlg2->set_serveraddress,25))
	{
		dlg->console+=_T("OK");
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);
		Receive(socket,dlg,buffer);

		int pos1 = FindStr(buffer," ",1);
		int pos2 = FindStr(buffer," ",pos1+1);
		
		if((pos1!=-1)&&(pos2!=-1))
			dlg->res_domain = CharToString(MidStr(buffer,pos1+1,pos2));
		else
			dlg->res_domain = dlg2->set_serveraddress;

		//dlg->MessageBox(dlg->res_domain);
	}
	else
	{
		dlg->console+=_T("ERROR!");
		ShowError(socket,dlg);
		delete socket;
		socket = NULL;
	}

	return socket;
}

bool CheckIfCancelled(CDlgMain* Dlg1)
{
	if(Dlg1->want_cancel)
	{
		Dlg1->console+=_T("\r\nCANCELLED ALL TASKS.");
		
		return true;
	}

	return false;
}

DWORD WINAPI RunTests(LPVOID lpParam)
{
	thread_data* data = (thread_data*)lpParam;
	CDlgMain* dlg = data->Dlgmain;
	CDlgSettings* dlg2 = data->Dlgsettings;
	//CResultsDlg* dlg = data->result_dlg;
	char buffer[SOCK_BUFFER+1];
	CString msg;

	dlg->console+=_T("\r\nTest started.");
	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);
	AfxSocketInit();
	CSocket* socket = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();

	thread_status* this_struct = new thread_status;

	this_struct->bufferlen = 0;
	this_struct->initialized = false;
	this_struct->data = NULL;
	this_struct->writepos = 0;

	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);  
	curl_easy_setopt(curl,
      CURLOPT_WRITEFUNCTION, writefunction);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA, (void *) &this_struct);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT,25);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,30);

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

	for(int GG=0;GG<1;GG++) // to make cancel possible
	{

	if(socket = Connect(socket,dlg,buffer,false,dlg2))
	{
		dlg->result_1ismailserver = _T("YES");
		
		int pos1 = FindStr(buffer,"Version",0);
		int pos2;
		if(pos1!=-1)
		{
			pos2 = FindStr(buffer," ",pos1+7+2);
			if(pos2==-1)
				pos2 = FindStr(buffer,"\r",pos1+7+2);

			dlg->result_2version = CharToString(MidStr(buffer,pos1+7+1,pos2));
		}

		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);

		if(CheckIfCancelled(dlg))
			break;

		// OPEN RELAY
		if(dlg->test_openrelay)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------ Open Relay Test ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			RelayTest(socket,dlg,buffer,dlg2);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// VERIFY COMMAND TEST
		if(dlg->test_verify)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------   Verify Test   ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			VerifyTest(socket,dlg,buffer,dlg2);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// POSTMAN/ABUSE TEST
		if(dlg->test_postmaster)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------   Postman Test  ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			PostAbuseTest(socket,dlg,buffer,dlg2,0);
		}

		if(CheckIfCancelled(dlg))
			break;

		if(dlg->test_abuse)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------   Abuse Test    ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			PostAbuseTest(socket,dlg,buffer,dlg2,1);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// ATTACHMENT TEST // (with server change)
		if(dlg->test_attachments)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------ Attachment Test ------------------");
			CString temp = dlg2->set_serveraddress;
			dlg2->set_serveraddress = dlg2->set_authserver;
			socket = Connect(socket,dlg,buffer,false,dlg2);
			dlg2->set_serveraddress = temp;
			AttachmentTest(socket,dlg,buffer,dlg2);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// SPOOFING TEST
		if(dlg->test_spoofing1)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------ Spoofing 1 Test ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			SpoofingTest(socket,dlg,buffer,1,dlg2);
		}

		if(CheckIfCancelled(dlg))
			break;
		
		/// SPOOFING TEST 2  // (with server change)
		if(dlg->test_spoofing2)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------ Spoofing 2 Test ------------------");
			CString temp = dlg2->set_serveraddress;
			dlg2->set_serveraddress = dlg2->set_authserver;
			socket = Connect(socket,dlg,buffer,false,dlg2);
			dlg2->set_serveraddress = temp;
			SpoofingTest(socket,dlg,buffer,2,dlg2);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// HELP TEST
		if(dlg->test_help)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------    Help Test    ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			HelpTest(socket,dlg,buffer);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// ENCRYPTED AUTH
		if(dlg->test_encauth)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------ Encry.Auth Test ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			EhloTest(socket,dlg,buffer,1);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// CLEARTEXT
		if(dlg->test_cleartext)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------ Cleartext Test  ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			EhloTest(socket,dlg,buffer,2);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// SASL
		if(dlg->test_sasl)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------    SASL Test    ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			EhloTest(socket,dlg,buffer,3);
		}

		if(CheckIfCancelled(dlg))
			break;

		/// AUTH
		if(dlg->test_auth)
		{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------    Auth. Test   ------------------");
			socket = Connect(socket,dlg,buffer,false,dlg2);
			EhloTest(socket,dlg,buffer,4);
		}

		if(CheckIfCancelled(dlg))
			break;

	}
	else
	{
		dlg->result_1ismailserver = _T("NO");
		dlg->UpdateData(FALSE);
		ScrollToBottom(dlg);
	}

	if(dlg->test_spf)
	{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------     SPF Test    ------------------");
		SpfTest(dlg,curl,this_struct);
	}

	if(dlg->test_blacklist)
	{
			dlg->console+=_T("\r\n-----------------------------------------------------");
			dlg->console+=_T("\r\n------------------  Blacklist Test ------------------");
		BlacklistTest(dlg,curl,this_struct);
	}

	} // end GG

	delete socket;

			dlg->console+=_T("\r\n-----------------------------------------------------");
	dlg->console+=_T("\r\nTEST COMPLETED.");

	dlg->GetDlgItem(IDC_BUTTON7)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_BUTTON8)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_BUTTON3)->SetWindowTextW(_T("Run"));
	dlg->GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	dlg->running = FALSE;
	dlg->want_cancel = FALSE;

	dlg->UpdateData(FALSE);
	ScrollToBottom(dlg);

	return 0;
}