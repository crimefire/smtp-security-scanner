// ResultsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMTP Tool.h"
#include "ResultsDlg.h"

// CResultsDlg dialog

#define SOCK_BUFFER 2048
#define CMD_DELAY 50






IMPLEMENT_DYNAMIC(CResultsDlg, CDialog)

CResultsDlg::CResultsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultsDlg::IDD, pParent)
	, result_openrelay(_T("..."))
	, result_spoofing(_T("..."))
	, result_verify(_T("..."))
	, result_postmaster(_T("..."))
	, result_abuse(_T("..."))
	, result_attachments(_T("..."))
	//, result_spf_type(_T("..."))
	, result_spf_domain(_T("..."))
	, result_spf_record(_T(""))
	, result_blacklisted(_T("..."))
	, columns_init(false)
	, console(_T(""))
	, result_mailserver(_T("..."))
	, receive(NULL)
	, ThreadId(0)
	, socket(NULL)
	, curl(NULL)
	, this_struct(NULL)
	, result_spf(_T("..."))
	, result_spoofing2(_T("..."))
	, result_help(_T("..."))
	, result_encrypted(_T("..."))
	, result_cleartext(_T("..."))
	, result_sasl(_T("..."))
	, result_auth(_T("..."))
	, sasl_methods(_T("..."))
	, version(_T(""))
	, res_domain(_T(""))
{
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	this_struct = new thread_status;

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
}

CResultsDlg::~CResultsDlg()
{
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}

void CResultsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, result_openrelay);
	DDX_Text(pDX, IDC_EDIT7, result_spoofing);
	DDX_Text(pDX, IDC_EDIT8, result_verify);
	DDX_Text(pDX, IDC_EDIT9, result_postmaster);
	DDX_Text(pDX, IDC_EDIT10, result_abuse);
	DDX_Text(pDX, IDC_EDIT11, result_attachments);
	//DDX_Text(pDX, IDC_EDIT12, result_spf_type);
	DDX_Text(pDX, IDC_EDIT13, result_spf_domain);
	DDX_Text(pDX, IDC_EDIT2, result_spf_record);
	DDX_Text(pDX, IDC_EDIT15, result_blacklisted);
	DDX_Control(pDX, IDC_LIST2, blacklist);
	DDX_Text(pDX, IDC_EDIT3, console);
	DDX_Text(pDX, IDC_EDIT16, result_mailserver);
	DDX_Text(pDX, IDC_EDIT12, result_spf);
	DDX_Text(pDX, IDC_EDIT29, result_spoofing2);
	DDX_Text(pDX, IDC_EDIT31, result_help);
	DDX_Text(pDX, IDC_EDIT32, result_encrypted);
	DDX_Text(pDX, IDC_EDIT33, result_cleartext);
	DDX_Text(pDX, IDC_EDIT30, result_sasl);
	DDX_Text(pDX, IDC_EDIT34, result_auth);
	DDX_Text(pDX, IDC_EDIT14, sasl_methods);
	DDX_Text(pDX, IDC_EDIT35, version);
}


BEGIN_MESSAGE_MAP(CResultsDlg, CDialog)
//	ON_WM_ACTIVATE()
//	ON_WM_ENTERSIZEMOVE()
//	ON_WM_ENTERIDLE()
ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CResultsDlg::OnHdnItemdblclickList1)
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_BUTTON1, &CResultsDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON3, &CResultsDlg::OnBnClickedButton3)
ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CResultsDlg::OnNMDblclkList2)
ON_BN_CLICKED(IDC_BUTTON6, &CResultsDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CResultsDlg message handlers

void CResultsDlg::InitList(void)
{
	UpdateData(FALSE);

	if(columns_init)
		return;

	//CListCtrl& ctlList = blacklist;

	blacklist.ModifyStyle(0,LVS_REPORT);

	blacklist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	int index = 0;
	blacklist.InsertColumn(index++,_T("Blacklist"),LVCFMT_LEFT,115);
	blacklist.InsertColumn(index++,_T("Status"),LVCFMT_LEFT,50);
	blacklist.InsertColumn(index++,_T("Reason"),LVCFMT_LEFT,200);
	columns_init = true;

	//blacklist.InsertItem(0,_T("test"));
	//blacklist.InsertItem(1,_T("test2"));

	UpdateData(FALSE);
	
}

BOOL CResultsDlg::OnInitDialog()
{
	//rcptto_list
	{
		FILE * pFile;
		char* result;
		char line[1024+1];
		pFile = fopen ("data/rcptto.txt" , "r");
		if(pFile != NULL)
		{
			while(true)
			{
				result = fgets(line,1024,pFile);
				if(result[strlen(result)-1]=='\n')
					result[strlen(result)-1] = '\0';
				
				rcptto_list.AddTail(CString(line));

				if (feof (pFile))
					break;
			}
			fclose (pFile);
		}
		else MessageBox(_T("Warning: Can't load rcpto list, verify test may show incorrect results."));
	}

	receive = new char[1024];

	DWORD style = LVS_REPORT;
	CListCtrl* ctrl = (CListCtrl*)GetDlgItem(IDC_LIST2);
	ctrl->ModifyStyle(0,style);//style;
	InitList();

	thread_data* data = (thread_data*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(thread_data));

	data->result_dlg = this;

	Thread = CreateThread(NULL,0,RunTests,data,0,&ThreadId);

	if(!Thread)
		MessageBox(_T("FAILED TO CREATE TEST THREAD"));

	return TRUE;
}

//void CResultsDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialog::OnActivate(nState, pWndOther, bMinimized);
//
//	// TODO: Add your message handler code here
//	//InitList();
//}

//void CResultsDlg::OnEnterSizeMove()
//{
//	// TODO: Add your message handler code here and/or call default
//	//InitList();
//	CDialog::OnEnterSizeMove();
//}

//void CResultsDlg::OnEnterIdle(UINT nWhy, CWnd* pWho)
//{
//	CDialog::OnEnterIdle(nWhy, pWho);
//
//	// TODO: Add your message handler code here
//	InitList();
//}

void CResultsDlg::OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);



	*pResult = 0;
}

void CResultsDlg::Send(const char* msg)
{
	send(sock,msg,strlen(msg),0);
	CString temp(msg);
	console+=_T("\r\n>>> ");
	console+=temp;
	UpdateData(FALSE);
	ScrollToBottom(this);
}

void CResultsDlg::SendBinary(char* data, int len)
{
	send(sock,(const char*)data,len,0);
	console+=_T("\r\n>>> [BINARY]");
}

void CResultsDlg::Receive(void)
{
	recv(sock,receive,1023,0);
	console+=_T("\r\n<<< ");
	CString temp(receive);
	console+=receive;
	UpdateData(FALSE);
	ScrollToBottom(this);
}

void CResultsDlg::PrintLastError(void)
{
	int er = WSAGetLastError();
	CString err;
	err.Format(_T("%d"),er);
	console+=_T("\r\nERROR:WSOCKERR:");
	console+=err;
}

HBRUSH CResultsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	bool reverse = true;
	if( (pWnd->GetDlgCtrlID() == IDC_EDIT16)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT9)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT10)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT11) )
		reverse = true;
	else if( (pWnd->GetDlgCtrlID() == IDC_EDIT1)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT7)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT8)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT33)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT29) )
		reverse = false;

	// TODO:  Change any attributes of the DC here
	//if (pWnd->GetDlgCtrlID() == IDC_EDIT1)
	//CString
	if(nCtlColor==CTLCOLOR_STATIC)
    {
		CEdit* edit = reinterpret_cast<CEdit*>(pWnd);
		CString str;
		edit->GetWindowTextW(str);
		if(str==_T("YES"))
		{
			if(reverse)
				pDC->SetTextColor(RGB(5, 200, 5));
			else
				pDC->SetTextColor(RGB(200, 5, 5));
		}
		else if(str==_T("NO"))
		{
			if(!reverse)
				pDC->SetTextColor(RGB(5, 200, 5));
			else
				pDC->SetTextColor(RGB(200, 5, 5));
		}
    }


	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CResultsDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void wresult(const wchar_t* str,CString& result,FILE* pFile)
{
	char* buff = NULL;
	CString temp(str);
	temp+=result;
	temp+=_T("\r\n");
	buff = StringToChar(temp);
	fwrite(buff,1,strlen(buff),pFile);

	return;
}

void CResultsDlg::OnBnClickedButton3()
{

	CFileDialog FileSaveDialog(FALSE,_T("txt"),0,OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
	_T("Txt files (*.txt)|*.txt||"));

	CString temp;
	char* buff = NULL;

	FileSaveDialog.m_ofn.lpstrTitle = _T("Export to txt");
	
	if(FileSaveDialog.DoModal()==IDOK)
	{
		CString path = FileSaveDialog.GetPathName();

		FILE* pFile = fopen(StringToChar(path), "w");
		if(pFile!=NULL)
		{
			fwrite("SMTP Tool results: \r\n\r\n",1,23,pFile);

			fwrite("-----------[ RESULTS ]----------\r\n\r\n",1,36,pFile);

			wresult(_T("Is mail server:        "),result_mailserver,pFile);
			wresult(_T("Open relay:            "),result_openrelay,pFile);
			wresult(_T("Verify user command:   "),result_verify,pFile);
			wresult(_T("Accept @postmaster:    "),result_postmaster,pFile);
			wresult(_T("Accept @abuse:         "),result_abuse,pFile);
			wresult(_T("Filters attachemnts:   "),result_attachments,pFile);
			wresult(_T("Spoofing check 1:      "),result_spoofing,pFile);
			wresult(_T("Spoofing check 2:      "),result_spoofing2,pFile);
			wresult(_T("Help command:          "),result_help,pFile);
			wresult(_T("Encrypted auth:        "),result_encrypted,pFile);
			wresult(_T("Cleartext login:       "),result_cleartext,pFile);
			wresult(_T("Authentication:        "),result_auth,pFile);
			wresult(_T("SASL methods:          "),sasl_methods,pFile);
			wresult(_T("Version:               "),version,pFile);


			fwrite("-------------[ SPF ]------------\r\n\r\n",1,36,pFile);

			wresult(_T("Domain name:           "),result_spf_domain,pFile);
			fwrite("- SPF records:\r\n",1,16,pFile);
			wresult(_T("\r\n"),result_spf_record,pFile);
			fwrite("\r\n",1,2,pFile);

			fwrite("------------[ BLACKLIST ]-----------\r\n",1,38,pFile);
			wresult(_T("\r\nBlacklisted:           "),result_blacklisted,pFile);

			fwrite("\r\n\r\n-------[ CONSOLE OUTPUT ]-------\r\n",1,38,pFile);

			buff = StringToChar(console);
			fwrite(buff,1,strlen(buff),pFile);
			delete[] buff;

			fclose(pFile);

			MessageBox(_T("Txt report succesfully saved."));
		}
	}
}

void CResultsDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;

	int item = pNMItemActivate->iItem;

	CString listed = blacklist.GetItemText(item,1);
	CString link = blacklist.GetItemText(item,2);

	if(listed==_T("OK"))
		MessageBox(_T("Server is not listed on this list."));
	else if(listed==_T("LISTED"))
	{
		ShellExecute(NULL,_T("open"),link,NULL,NULL,SW_SHOWNORMAL);
	}

	*pResult = 0;
}

void CResultsDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	CFileDialog FileSaveDialog(FALSE,_T("html"),0,OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
	_T("Html files (*.html)|*.html||"));

	//CString temp;
	//char* buff = NULL;

	FileSaveDialog.m_ofn.lpstrTitle = _T("Export to html");

	CString report = _T("");
	
	if(FileSaveDialog.DoModal()==IDOK)
	{
		CString path = FileSaveDialog.GetPathName();

		if(true)
		{
			report = _T("");
			report+= _T("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\"><html><head><title>DNS Tool html report</title>");
			report+=_T("<style type=\"text/css\">	  .table {	clear:both;	border: 1px solid #000000; ");
			report+=_T("margin-top: 5px; margin-bottom: 5px;	background: #cccccc; border-spacing: 1px; ");
			report+=_T("	font-size: 12px;	font-family:\"Verdana\";	} ");
			report+=_T(" .first_tr {	background: #99ccff;	font-weight: bold;	height:25px;	font-size: 13px; } ");
			report+=_T(" td.first_td {	border: 1px solid #6699ff;	padding-left: 5px;	padding-right: 5px; } ");
			report+=_T(" td.left {	text-align: left;	padding-left: 5px;	padding-right: 5px; } ");
			report+=_T(" .tr_1 { background: #ffffff; } .tr_2 {	background: #eeeeee; } ");
			report+=_T(" .main2 {	border: 1px solid #333333;	background: #ddeeff; font-size: 1.25em;	text-align: left;	padding-left: 20px; margin-top: 30px; } ");
			report+=_T("</style></head><body>");

			report+= _T("<a name=\"top\"></a><h1>SMTP Tool html report.</h1>");
			report+= _T("<h3><a href=\"#1\">1. Results</a></h3>");
			report+= _T("<h3><a href=\"#2\">2. SPF</a></h3>");
			report+= _T("<h3><a href=\"#3\">3. Blacklist</a></h3>");
			report+= _T("<h3><a href=\"#4\">4. Console</a></h3>");
			report+= _T("<br><br>");

			report+= _T("<a name=\"1\"></a><div class=\"main2\">1. Results</div><a href=\"#top\">top</a>");
			
			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Test</td><td class=\"first_td\">Result</td></tr> ");
			int i = 0;
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Is mail server")+_T("</td><td class=\"left\">")+result_mailserver+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Open relay")+_T("</td><td class=\"left\">")+result_openrelay+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Verify user command")+_T("</td><td class=\"left\">")+result_verify+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Accept @postmaster")+_T("</td><td class=\"left\">")+result_postmaster+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Accept @abuse")+_T("</td><td class=\"left\">")+result_abuse+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Filters attachemnts")+_T("</td><td class=\"left\">")+result_attachments+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Spoofing check 1")+_T("</td><td class=\"left\">")+result_spoofing+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Spoofing check 2")+_T("</td><td class=\"left\">")+result_spoofing2+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Help command")+_T("</td><td class=\"left\">")+result_help+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Encrypted auth")+_T("</td><td class=\"left\">")+result_encrypted+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Cleartext login")+_T("</td><td class=\"left\">")+result_cleartext+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Authentication")+_T("</td><td class=\"left\">")+result_auth+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("SASL methods")+_T("</td><td class=\"left\">")+sasl_methods+_T("</td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Version")+_T("</td><td class=\"left\">")+version+_T("</td></tr>");

			report+= _T("</table><a name=\"2\"></a><div class=\"main2\">2. SPF</div><a href=\"#top\">top</a>");
			
			report+= _T("<p>Domain name: ")+result_spf_domain;
			report+= _T("</p>");
			
			report+= _T("<p>SPF records: ");
			report+= result_spf_record;
			report+= _T("</p>");

			report+= _T("<a name=\"3\"></a><div class=\"main2\">3. Blacklist</div><a href=\"#top\">top</a>");
			
			report+= _T("<p>Blacklisted: ")+result_blacklisted+_T("</p>");

			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Blacklist</td><td class=\"first_td\">Status</td><td class=\"first_td\">Reason</td></tr> ");

			for( i=0;i<blacklist.GetItemCount();i++)
			{
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+blacklist.GetItemText(i,0)+_T("</td><td class=\"left\">")+blacklist.GetItemText(i,1)+_T("</td><td class=\"left\">")+blacklist.GetItemText(i,2)+_T("</td></tr>");
			}

			report+= _T("</table><a name=\"4\"></a><div class=\"main2\">4. Console</div><a href=\"#top\">top</a>");
			
			CString tem5 = console;
			tem5.Replace(_T("\r\n"),_T("<br>"));
			report+=_T("<br><br>")+tem5+_T("<a href=\"#top\">top</a><h2>End of report</h2></body>");
			
			//convert to cstr
			char* ta = StringToChar(report);

			// save
			FILE * pFile;
			int result;
			pFile = fopen(StringToChar(path), "w");
			if(pFile!=NULL)
			{
				result = fputs((const char*)ta,pFile);
				fclose(pFile);
			}

			MessageBox(_T("Html report succesfully saved."));
		}
	}
}
