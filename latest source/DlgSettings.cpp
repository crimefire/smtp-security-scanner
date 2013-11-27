// DlgSettings.cpp : implementation file
//

#include "stdafx.h"
#include "SMTP Tool.h"
#include "DlgSettings.h"


// CDlgSettings dialog

IMPLEMENT_DYNAMIC(CDlgSettings, CDialog)

CDlgSettings::CDlgSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettings::IDD, pParent)
	, loadprofile_id(_T(""))
	, saveprofileas(_T(""))
	, auth_use(FALSE)
	, set_serveraddress(_T(""))
	, set_fromabuseverify(_T(""))
	, set_authuser(_T(""))
	, set_authpass(_T(""))
	, set_authserver(_T(""))
	, set_spoofing_mailfrom(_T(""))
	, set_spoofing_rpctto(_T(""))
	, set_spoofing_from(_T(""))
	, set_spoofing_to(_T(""))
	, set_spoofing_replyto(_T(""))
	, set_spoofing_subject(_T(""))
	, set_spoofing_message(_T(""))
	, set_relayfrom1(_T(""))
	, set_relayfrom2(_T(""))
	, set_relayto1(_T(""))
	, set_relayto2(_T(""))
	, set_attach_path(_T(""))
	, set_attach_from(_T(""))
	, set_attach_to(_T(""))
	, set_attach_delay(_T(""))
{
	
}

CDlgSettings::~CDlgSettings()
{
}

void CDlgSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, but_saveprofile);
	DDX_CBString(pDX, IDC_COMBO1, loadprofile_id);
	DDX_Control(pDX, IDC_COMBO1, loadprofile_combo);
	DDX_Text(pDX, IDC_EDIT7, saveprofileas);
	DDX_Check(pDX, IDC_CHECK8, auth_use);
	DDX_Text(pDX, IDC_EDIT1, set_serveraddress);
	DDX_Text(pDX, IDC_EDIT24, set_fromabuseverify);
	DDX_Text(pDX, IDC_EDIT21, set_authuser);
	DDX_Text(pDX, IDC_EDIT22, set_authpass);
	DDX_Text(pDX, IDC_EDIT28, set_authserver);
	DDX_Text(pDX, IDC_EDIT4, set_spoofing_mailfrom);
	DDX_Text(pDX, IDC_EDIT3, set_spoofing_rpctto);
	DDX_Text(pDX, IDC_EDIT25, set_spoofing_from);
	DDX_Text(pDX, IDC_EDIT26, set_spoofing_to);
	DDX_Text(pDX, IDC_EDIT27, set_spoofing_replyto);
	DDX_Text(pDX, IDC_EDIT5, set_spoofing_subject);
	DDX_Text(pDX, IDC_EDIT6, set_spoofing_message);
	DDX_Text(pDX, IDC_EDIT17, set_relayfrom1);
	DDX_Text(pDX, IDC_EDIT18, set_relayfrom2);
	DDX_Text(pDX, IDC_EDIT19, set_relayto1);
	DDX_Text(pDX, IDC_EDIT20, set_relayto2);
	DDX_Text(pDX, IDC_EDIT2, set_attach_path);
	DDX_Text(pDX, IDC_EDIT14, set_attach_from);
	DDX_Text(pDX, IDC_EDIT23, set_attach_to);
	DDX_Text(pDX, IDC_EDIT8, set_attach_delay);
	DDX_Control(pDX, IDC_BUTTON2, but_del_profile);
}


BEGIN_MESSAGE_MAP(CDlgSettings, CDialog)
	ON_BN_CLICKED(IDC_CHECK8, &CDlgSettings::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgSettings::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSettings::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgSettings::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgSettings::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDlgSettings message handlers

BOOL CDlgSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	//shared->Dlgsettings = this;

	but_saveprofile.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, NULL));
	but_del_profile.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, NULL));

	ReloadProfiles();

	return TRUE;
}
void CDlgSettings::OnBnClickedCheck8()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT21)->EnableWindow(auth_use);
	GetDlgItem(IDC_EDIT22)->EnableWindow(auth_use);
	GetDlgItem(IDC_EDIT28)->EnableWindow(auth_use);
}

void CDlgSettings::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	BROWSEINFO   bi; 
    ZeroMemory(&bi,sizeof(bi)); 
    TCHAR   szDisplayName[MAX_PATH]; 
    szDisplayName[0]    =   '\0';

    bi.hwndOwner        =   NULL; 
    bi.pidlRoot         =   NULL; 
    bi.pszDisplayName   =   szDisplayName; 
    bi.lpszTitle        =   _T("Please select a folder with attachments to send:"); 
    bi.ulFlags          =   BIF_RETURNONLYFSDIRS;
    bi.lParam           =   NULL; 
    bi.iImage           =   0;

    LPITEMIDLIST   pidl   =   SHBrowseForFolder(&bi);
    TCHAR   szPathName[MAX_PATH]; 
    if(pidl)
    {
         BOOL bRet = SHGetPathFromIDList(pidl,szPathName);
         if(FALSE == bRet)
              return;

		UpdateData(TRUE);

		CString temp(szPathName);
		set_attach_path = temp;

		UpdateData(FALSE);
    }
}

int AddStr(char* _where, const char* str)
{
	int l = strlen(str);
	memcpy(_where,str,l);
	return l;
}

int AddStr(char* _where, char* str)
{
	int l = strlen(str);
	memcpy(_where,str,l);
	return l;
}

void CDlgSettings::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DlgMain->UpdateData(TRUE);

	if(saveprofileas==_T(""))
	{
		MessageBox(_T("You must enter profile name"));
	}
	else
	{
		char buffer[2048];
		int cur = 0;
		CString ab = _T("");
		cur+= AddStr(&(buffer[cur]),"smtp_profile_version:1\r\n");
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("smtp_profile_name")+_T(":")+saveprofileas+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_serveraddress")+_T(":")+set_serveraddress+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_fromabuseverify")+_T(":")+set_fromabuseverify+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_authuser")+_T(":")+set_authuser+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_authpass")+_T(":")+set_authpass+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_authserver")+_T(":")+set_authserver+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_spoofing_mailfrom")+_T(":")+set_spoofing_mailfrom+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_spoofing_rpctto")+_T(":")+set_spoofing_rpctto+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_spoofing_from")+_T(":")+set_spoofing_from+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_spoofing_to")+_T(":")+set_spoofing_to+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_spoofing_replyto")+_T(":")+set_spoofing_replyto+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_spoofing_subject")+_T(":")+set_spoofing_subject+_T("\r\n")));
		CString gh;
		int ag = strlen(StringToChar(set_spoofing_message));
		gh.Format(_T("%d"),ag);
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_spoofing_message;")+gh+_T(":")+set_spoofing_message+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_relayfrom1")+_T(":")+set_relayfrom1+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_relayfrom2")+_T(":")+set_relayfrom2+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_relayto1")+_T(":")+set_relayto1+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_relayto2")+_T(":")+set_relayto2+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_attach_path")+_T(":")+set_attach_path+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_attach_from")+_T(":")+set_attach_from+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_attach_to")+_T(":")+set_attach_to+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("set_attach_delay")+_T(":")+set_attach_delay+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("auth_use")+_T(":")+(auth_use?_T("1"):_T("0"))+_T("\r\n")));
		//cur+= AddStr(&(buffer[cur]),StringToChar(_T("")+_T(":")++_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_openrelay")+_T(":")+(DlgMain->test_openrelay?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_verify")+_T(":")+(DlgMain->test_verify?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_postmaster")+_T(":")+(DlgMain->test_postmaster?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_abuse")+_T(":")+(DlgMain->test_abuse?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_attachments")+_T(":")+(DlgMain->test_attachments?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_blacklist")+_T(":")+(DlgMain->test_blacklist?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_spf")+_T(":")+(DlgMain->test_spf?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_spoofing1")+_T(":")+(DlgMain->test_spoofing1?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_spoofing2")+_T(":")+(DlgMain->test_spoofing2?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_help")+_T(":")+(DlgMain->test_help?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_encauth")+_T(":")+(DlgMain->test_encauth?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_cleartext")+_T(":")+(DlgMain->test_cleartext?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_sasl")+_T(":")+(DlgMain->test_sasl?_T("1"):_T("0"))+_T("\r\n")));
		cur+= AddStr(&(buffer[cur]),StringToChar(ab+_T("test_auth")+_T(":")+(DlgMain->test_auth?_T("1"):_T("0"))+_T("\r\n")));

		CString path;
		path = _T("Profiles\\");
		path+=saveprofileas+_T(".smp");
		FILE* pFile;
	    pFile = fopen(StringToChar(path),"wb");
		fwrite(buffer,1,cur,pFile);
	    //fputs(this_struct->data,pFile);
	    fclose(pFile);
			// auth use
			// tests
		// ...

		ReloadProfiles();

		MessageBox(_T("Profile saved"));
	}

	// load with get line + findstr etc
}

void CDlgSettings::ReloadProfiles(void)
{
	loadprofile_combo.ResetContent();
	loadprofile_combo.AddString(_T(" "));

	CString temp = _T("Profiles/*.smp");
	CFileFind Finder;
	BOOL found1 = Finder.FindFile(temp);
	DWORD Error1 = GetLastError();
	DWORD Error2 = -1;
	BOOL found2 = TRUE;
	CString sName = _T("(not found)");
	CString sTitle = _T("(not found)");
	int check = 0;

	if(found1)
	{
		while(found2)
		{
			check++;
			found2 = Finder.FindNextFile();
			Error2 = GetLastError();

			if(!Finder.IsDirectory())
			{
				sTitle = Finder.GetFileTitle();
				loadprofile_combo.AddString(sTitle);
			}
		}
	}
				//sName = Finder.GetFileName();
				
/*
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
					free (fbuffer);*/

}

void CDlgSettings::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString itam;
	loadprofile_combo.GetLBText(loadprofile_combo.GetCurSel(),itam);
	//if(itam==_T(" "))
	if(true)
	{
		auth_use = FALSE;
		set_serveraddress = _T("");
		set_fromabuseverify = _T("");
		set_authuser = _T("");
		set_authpass = _T("");
		set_authserver = _T("");
		set_spoofing_mailfrom = _T("");
		set_spoofing_rpctto = _T("");
		set_spoofing_from = _T("");
		set_spoofing_to = _T("");
		set_spoofing_replyto = _T("");
		set_spoofing_subject = _T("");
		set_spoofing_message = _T("");
		set_relayfrom1 = _T("");
		set_relayfrom2 = _T("");
		set_relayto1 = _T("");
		set_relayto2 = _T("");
		set_attach_path = _T("");
		set_attach_from = _T("");
		set_attach_to = _T("");
		set_attach_delay = _T("");
		DlgMain->test_openrelay = FALSE;
		DlgMain->test_verify = FALSE;
		DlgMain->test_postmaster = FALSE;
		DlgMain->test_abuse = FALSE;
		DlgMain->test_attachments = FALSE;
		DlgMain->test_blacklist = FALSE;
		DlgMain->test_spf = FALSE;
		DlgMain->test_spoofing1 = FALSE;
		DlgMain->test_spoofing2 = FALSE;
		DlgMain->test_help = FALSE;
		DlgMain->test_encauth = FALSE;
		DlgMain->test_cleartext = FALSE;
		DlgMain->test_sasl = FALSE;
		DlgMain->test_auth = FALSE;
		saveprofileas = _T("");

		UpdateData(FALSE);
		DlgMain->UpdateData(FALSE);
	}
	//else
	if(itam!=_T(" "))
	{
		saveprofileas = itam;

		CString temp;
		char* msg;
		// load profile here
		FILE* pFile;
		long fSize;
		char* fbuffer;
		size_t fresult;
		temp = _T("Profiles\\");
		temp+= itam;
		temp+=_T(".smp");
		msg = StringToChar(temp);
		pFile = fopen((const char*)msg,"rb");
		if(!pFile)
		{
			// err
		}
		else
		{
			fseek(pFile , 0 , SEEK_END);
			fSize = ftell(pFile);
			rewind(pFile);

			fbuffer = new char[fSize];
			fresult = fread(fbuffer,1,fSize,pFile);
			fclose (pFile);

			int i=0;
			int pos1 = 0, pos2 = 0;
			bool in_vname = true;
			bool in_vsize = false;
			bool in_vvalue = false;
			bool was_sized = false;
			char* vname;
			char* vsize;
			char* vvalue;
			int bsize;

			while(true)
			{
				if(in_vname)
				{
					if(fbuffer[i]==';')
					{
						pos2 = i;
						vname = MidStr(fbuffer,pos1,pos2);

						in_vname = false;
						in_vsize = true;
						was_sized = true;
						i++;
						pos1 = i;
					}
					else if(fbuffer[i]==':')
					{
						pos2 = i;
						vname = MidStr(fbuffer,pos1,pos2);

						in_vname = false;
						in_vvalue = true;
						was_sized = false;
						//i++;
						pos1 = i+1;
					}
				}
				else if(in_vsize)
				{
					if(fbuffer[i]==':')
					{
						pos2 = i;
						vsize = MidStr(fbuffer,pos1,pos2);
						bsize = atoi(vsize);

						in_vsize = false;
						in_vvalue = true;
						i++;
						pos1 = i;
					}
				}
				else if(in_vvalue)
				{
					if(was_sized)
					{
						// could skip, instead of iteration
						if(i-pos1>=bsize-0)
						{
							if(i-pos1>bsize-0) // co by bugu nie bylo jak was_sized na 0
								pos2 = i-((i-pos1)-bsize);
							else
								pos2 = i;
							vvalue = MidStr(fbuffer,pos1,pos2);
							if(!vvalue)
								vvalue = "";

							in_vname = true;
							in_vvalue = false;
							was_sized = false;
							i+=2-((i-pos1)-bsize);
							pos1 = i;//-((i-pos1)-bsize);

						
							// if end

							//CString aag;
							//aag = CharToString(vname)+_T("=")+CharToString(vvalue);
							//MessageBox(aag);
							PushSetting(CharToString(vname),CharToString(vvalue));

							if(i>=fSize-1)
								break;
						}
					}
					else
					{
						if(fbuffer[i]=='\r')
						{
							pos2 = i;
							vvalue = MidStr(fbuffer,pos1,pos2);
							if(!vvalue)
								vvalue = "";

							in_vname = true;
							in_vvalue = false;
							was_sized = false;
							i+=2;
							pos1 = i;

							
							//if end

							//CString aag;
							//aag = CharToString(vname)+_T("=")+CharToString(vvalue);
							//MessageBox(aag);

							PushSetting(CharToString(vname),CharToString(vvalue));

							if(i>=fSize-1)
								break;
						}
					}
				}

				i++;
			}


			delete[] fbuffer;
			//sEncoded = Base64Encode((unsigned char*)fbuffer,fSize);
			//free (fbuffer);
		}

		
	}

	UpdateData(FALSE);
	DlgMain->UpdateData(FALSE);

	OnBnClickedCheck8();
}

void CDlgSettings::PushSetting(CString name, CString value)
{
	if(name==_T("set_serveraddress"))
		set_serveraddress = value;
	else if(name==_T("set_fromabuseverify"))
		set_fromabuseverify = value;
	else if(name==_T("set_authuser"))
		set_authuser = value;
	else if(name==_T("set_authpass"))
		set_authpass = value;
	else if(name==_T("set_authserver"))
		set_authserver = value;
	else if(name==_T("set_spoofing_mailfrom"))
		set_spoofing_mailfrom = value;
	else if(name==_T("set_spoofing_rpctto"))
		set_spoofing_rpctto = value;
	else if(name==_T("set_spoofing_from"))
		set_spoofing_from = value;
	else if(name==_T("set_spoofing_to"))
		set_spoofing_to = value;
	else if(name==_T("set_spoofing_replyto"))
		set_spoofing_replyto = value;
	else if(name==_T("set_spoofing_subject"))
		set_spoofing_subject = value;
	else if(name==_T("set_spoofing_message"))
		set_spoofing_message = value;
	else if(name==_T("set_relayfrom1"))
		set_relayfrom1 = value;
	else if(name==_T("set_relayfrom2"))
		set_relayfrom2 = value;
	else if(name==_T("set_relayto1"))
		set_relayto1 = value;
	else if(name==_T("set_relayto2"))
		set_relayto2 = value;
	else if(name==_T("set_attach_path"))
		set_attach_path = value;
	else if(name==_T("set_attach_from"))
		set_attach_from = value;
	else if(name==_T("set_attach_to"))
		set_attach_to = value;
	else if(name==_T("set_attach_delay"))
		set_attach_delay = value;

	else if(name==_T("auth_use"))
		auth_use = (value==_T("1"))?TRUE:FALSE;

	else if(name==_T("test_openrelay"))
		DlgMain->test_openrelay = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_verify"))
		DlgMain->test_verify = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_postmaster"))
		DlgMain->test_postmaster = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_abuse"))
		DlgMain->test_abuse = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_attachments"))
		DlgMain->test_attachments = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_blacklist"))
		DlgMain->test_blacklist = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_spf"))
		DlgMain->test_spf = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_spoofing1"))
		DlgMain->test_spoofing1 = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_spoofing2"))
		DlgMain->test_spoofing2 = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_help"))
		DlgMain->test_help = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_encauth"))
		DlgMain->test_encauth = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_cleartext"))
		DlgMain->test_cleartext = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_sasl"))
		DlgMain->test_sasl = (value==_T("1"))?TRUE:FALSE;
	else if(name==_T("test_auth"))
		DlgMain->test_auth = (value==_T("1"))?TRUE:FALSE;


}

void CDlgSettings::OnBnClickedButton2()
{
	UpdateData(TRUE);

	CString itam;
	loadprofile_combo.GetLBText(loadprofile_combo.GetCurSel(),itam);

	if(itam!=_T(" "))
	{
		int res = MessageBox(_T("Are you sure to delete selected profile?"),_T("Delete profile"),MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);

		if(res==IDYES)
		{
			CString temp;
			temp = _T("Profiles\\");
			temp+=itam;
			temp+=_T(".smp");
			DeleteFile(temp);
			ReloadProfiles();
		}
	}
}
