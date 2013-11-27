// DlgMain.cpp : implementation file
//

#include "stdafx.h"
#include "SMTP Tool.h"
#include "DlgMain.h"


// CDlgMain dialog

IMPLEMENT_DYNAMIC(CDlgMain, CDialog)

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMain::IDD, pParent)
	, result_1ismailserver(_T("..."))
	, result_2version(_T(""))
	, result_3openrelay(_T("..."))
	, result_veriify(_T("..."))
	, result_postmaster(_T("..."))
	, result_abuse(_T("..."))
	, result_attachm(_T("..."))
	, result_blacklist(_T("..."))
	, result_spf(_T("..."))
	, result_spoofing1(_T("..."))
	, result_spoofing2(_T("..."))
	, result_help(_T("..."))
	, result_encauth(_T("..."))
	, result_cleartext(_T("..."))
	, result_sasl(_T(""))
	, result_auth(_T("..."))
	, result_blacklistednum(_T("..."))
	, result_spfbox(_T("..."))
	, console(_T("SMTP Tool. Ready."))
	, columns_init(false)
	, receive(NULL)
	, test_openrelay(FALSE)
	, test_verify(FALSE)
	, test_postmaster(FALSE)
	, test_abuse(FALSE)
	, test_attachments(FALSE)
	, test_blacklist(FALSE)
	, test_spf(FALSE)
	, test_spoofing1(FALSE)
	, test_spoofing2(FALSE)
	, test_help(FALSE)
	, test_encauth(FALSE)
	, test_cleartext(FALSE)
	, test_sasl(FALSE)
	, test_auth(FALSE)
	, ThreadId(0)
	, result_spf_domain(_T(""))
	, res_domain(_T(""))
	, running(FALSE)
	, want_cancel(FALSE)
{

}

CDlgMain::~CDlgMain()
{
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT16, result_1ismailserver);
	DDX_Text(pDX, IDC_EDIT43, result_2version);
	DDX_Text(pDX, IDC_EDIT30, result_3openrelay);
	DDX_Text(pDX, IDC_EDIT31, result_veriify);
	DDX_Text(pDX, IDC_EDIT32, result_postmaster);
	DDX_Text(pDX, IDC_EDIT33, result_abuse);
	DDX_Text(pDX, IDC_EDIT37, result_attachm);
	DDX_Text(pDX, IDC_EDIT36, result_blacklist);
	DDX_Text(pDX, IDC_EDIT38, result_spf);
	DDX_Text(pDX, IDC_EDIT39, result_spoofing1);
	DDX_Text(pDX, IDC_EDIT40, result_spoofing2);
	DDX_Text(pDX, IDC_EDIT41, result_help);
	DDX_Text(pDX, IDC_EDIT42, result_encauth);
	DDX_Text(pDX, IDC_EDIT44, result_cleartext);
	DDX_Text(pDX, IDC_EDIT45, result_sasl);
	DDX_Text(pDX, IDC_EDIT46, result_auth);
	DDX_Text(pDX, IDC_EDIT26, result_blacklistednum);
	DDX_Text(pDX, IDC_EDIT2, result_spfbox);
	DDX_Text(pDX, IDC_EDIT14, console);
	DDX_Control(pDX, IDC_LIST3, blacklist);
	DDX_Check(pDX, IDC_CHECK1, test_openrelay);
	DDX_Check(pDX, IDC_CHECK3, test_verify);
	DDX_Check(pDX, IDC_CHECK4, test_postmaster);
	DDX_Check(pDX, IDC_CHECK15, test_abuse);
	DDX_Check(pDX, IDC_CHECK5, test_attachments);
	DDX_Check(pDX, IDC_CHECK6, test_blacklist);
	DDX_Check(pDX, IDC_CHECK7, test_spf);
	DDX_Check(pDX, IDC_CHECK2, test_spoofing1);
	DDX_Check(pDX, IDC_CHECK9, test_spoofing2);
	DDX_Check(pDX, IDC_CHECK11, test_help);
	DDX_Check(pDX, IDC_CHECK12, test_encauth);
	DDX_Check(pDX, IDC_CHECK13, test_cleartext);
	DDX_Check(pDX, IDC_CHECK10, test_sasl);
	DDX_Check(pDX, IDC_CHECK14, test_auth);
	DDX_Text(pDX, IDC_EDIT25, result_spf_domain);
}


BEGIN_MESSAGE_MAP(CDlgMain, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMain::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgMain::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgMain::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgMain::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgMain::OnBnClickedButton3)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgMain::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlgMain::OnBnClickedButton8)
END_MESSAGE_MAP()


// CDlgMain message handlers

BOOL CDlgMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	//shared->Dlgmain = this;

	int a = 4;

	//rcptto_list
	
		int b = 4;
		FILE * pFile = NULL;
		FILE * pFile2 = NULL;
		char* result;
		char line[1024+1];
		//pFile2 = (FILE*)result;
		pFile2 = fopen ("data/rcptto.txt" , "r");
		if(pFile2 != NULL)
		{
			while(true)
			{
				result = fgets(line,1024,pFile2);
				if(result[strlen(result)-1]=='\n')
					result[strlen(result)-1] = '\0';
				
				rcptto_list.AddTail(CString(line));

				if (feof (pFile2))
					break;
			}
			fclose (pFile2);
		}
		else MessageBox(_T("Warning: Can't load rcpto list, verify test may show incorrect results."));
	

	receive = new char[1024];

	DWORD style = LVS_REPORT;
	CListCtrl* ctrl = (CListCtrl*)GetDlgItem(IDC_LIST2);
	ctrl->ModifyStyle(0,style);//style;
	InitList();

	return TRUE;
}

void CDlgMain::InitList(void)
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
void CDlgMain::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	test_openrelay = TRUE;
	test_verify = TRUE;
	test_postmaster = TRUE;
	test_abuse = TRUE;
	test_attachments = TRUE;
	test_blacklist = TRUE;
	test_spf = TRUE;
	test_spoofing1 = TRUE;
	test_spoofing2 = TRUE;
	test_help = TRUE;
	test_encauth = TRUE;
	test_cleartext = TRUE;
	test_sasl = TRUE;
	test_auth = TRUE;

	UpdateData(FALSE);
}

void CDlgMain::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	test_openrelay = FALSE;
	test_verify = FALSE;
	test_postmaster = FALSE;
	test_abuse = FALSE;
	test_attachments = FALSE;
	test_blacklist = FALSE;
	test_spf = FALSE;
	test_spoofing1 = FALSE;
	test_spoofing2 = FALSE;
	test_help = FALSE;
	test_encauth = FALSE;
	test_cleartext = FALSE;
	test_sasl = FALSE;
	test_auth = FALSE;

	UpdateData(FALSE);
}

void CDlgMain::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;// = new CAboutDlg;
	dlg.DoModal();
	//delete dlg;
}

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

void CDlgMain::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	exit(0);
}

void CDlgMain::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	DlgSettings->UpdateData(TRUE);
	UpdateData(TRUE);

	if(!running)
	{

		if(DlgSettings->set_serveraddress==_T(""))
		{
			MessageBox(_T("Please enter server address"));
			return;
		}

		if(test_openrelay&&( (DlgSettings->set_relayfrom1==_T(""))||(DlgSettings->set_relayfrom2==_T(""))||(DlgSettings->set_relayto1==_T(""))||(DlgSettings->set_relayto2==_T("")) ))
		{
			MessageBox(_T("To test for Open Relay you need to fill Relay Test Configuration"));
			return;
		}

		if((test_spoofing1||test_spoofing2)&&( (DlgSettings->set_spoofing_from==_T(""))||(DlgSettings->set_spoofing_message==_T(""))||(DlgSettings->set_spoofing_subject==_T(""))||(DlgSettings->set_spoofing_to==_T("")) ))
		{
			MessageBox(_T("To test for spoofing you need to fill Configuration"));
			return;
		}
	/*
		if(test_spoofing2&&(spoofing_server==_T("")))
		{
			MessageBox(_T("You need to fill server for Spoofing check 2"));
			return;
		}
	*/
		if(test_spoofing2&&((!(DlgSettings->auth_use))||(DlgSettings->set_authserver==_T(""))))
		{
			MessageBox(_T("Spoofing check 2 requires authentication"));
			return;
		}

		if((test_postmaster||test_abuse)&&(DlgSettings->set_fromabuseverify==_T("")))
		{
			MessageBox(_T("To test for Postmaster/Abuse you need to fill MAIL field"));
			return;
		}

		if(test_attachments&&((DlgSettings->set_attach_from==_T(""))||(DlgSettings->set_attach_path==_T(""))||(DlgSettings->set_attach_to==_T(""))))
		{
			MessageBox(_T("To test for Attachment Filtering you need to fill Attachment configuration"));
			return;
		}

		CEdit* pmyEdit = ((CEdit*)(DlgSettings->GetDlgItem(IDC_EDIT6)));

		int i, nLineCount = pmyEdit->GetLineCount();
		CString strText, strLine;

		for (i=0;i < nLineCount;i++)
		{
		   // length of line i:
		   int len = pmyEdit->LineLength(pmyEdit->LineIndex(i));
		   pmyEdit->GetLine(i, strText.GetBuffer(len), len);
		   strText.ReleaseBuffer(len);
		   DlgSettings->set_spoofing_message_list.AddTail(strText);
		}

		thread_data* data = (thread_data*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
					sizeof(thread_data));

		data->test_id = 1;
		data->Dlgmain = this;
		data->Dlgsettings = DlgSettings;

		Thread = CreateThread(NULL,0,RunTests,data,0,&ThreadId);

		if(!Thread)
			MessageBox(_T("FAILED TO CREATE TEST THREAD"));

		running = TRUE;
		GetDlgItem(IDC_BUTTON3)->SetWindowTextW(_T("Cancel"));
	}
	else if(running)
	{
		want_cancel = TRUE;
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	}
}

HBRUSH CDlgMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	bool reverse = true; // if reverse then YES=GREEN
	/*
	if( (pWnd->GetDlgCtrlID() == IDC_EDIT16)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT9)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT10)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT11) )
		reverse = true;*/
	 if( (pWnd->GetDlgCtrlID() == IDC_EDIT30)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT31)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT36)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT39)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT44)||
		(pWnd->GetDlgCtrlID() == IDC_EDIT40) )
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

void CDlgMain::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
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

			wresult(_T("Is mail server:        "),result_1ismailserver,pFile);
			wresult(_T("Version:               "),result_2version,pFile);
			wresult(_T("Open relay:            "),result_3openrelay,pFile);
			wresult(_T("Verify user command:   "),result_veriify,pFile);
			wresult(_T("Accept @postmaster:    "),result_postmaster,pFile);
			wresult(_T("Accept @abuse:         "),result_abuse,pFile);
			wresult(_T("Filters attachemnts:   "),result_attachm,pFile);
			wresult(_T("Blacklist occurence:   "),result_blacklist,pFile);
			wresult(_T("SPF:                   "),result_spf,pFile);
			wresult(_T("Spoofing check 1:      "),result_spoofing1,pFile);
			wresult(_T("Spoofing check 2:      "),result_spoofing2,pFile);
			wresult(_T("Help command:          "),result_help,pFile);
			wresult(_T("Encrypted auth:        "),result_encauth,pFile);
			wresult(_T("Cleartext login:       "),result_cleartext,pFile);
			wresult(_T("SASL methods:          "),result_sasl,pFile);
			wresult(_T("Authentication:        "),result_auth,pFile);


			fwrite("-------------[ SPF ]------------\r\n\r\n",1,36,pFile);

			//wresult(_T("Domain name:           "),result_spf_domain,pFile);
			fwrite("- SPF records:\r\n",1,16,pFile);
			wresult(_T("\r\n"),result_spfbox,pFile);
			fwrite("\r\n",1,2,pFile);

			fwrite("------------[ BLACKLIST ]-----------\r\n",1,38,pFile);
			wresult(_T("\r\nBlacklisted:           "),result_blacklistednum,pFile);

			fwrite("\r\n\r\n-------[ CONSOLE OUTPUT ]-------\r\n",1,38,pFile);

			buff = StringToChar(console);
			fwrite(buff,1,strlen(buff),pFile);
			delete[] buff;

			fclose(pFile);

			MessageBox(_T("Txt report succesfully saved."));
		}
	}
}

void CDlgMain::OnBnClickedButton8()
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
			report+=_T(" td.center {	text-align: center;	padding-left: 5px;	padding-right: 5px; } ");
			report+=_T(" .tr_1 { background: #ffffff; } .tr_2 {	background: #eeeeee; } ");
			report+=_T(" .main2 {	border: 1px solid #333333;	background: #ddeeff; font-size: 1.25em;	text-align: left;	padding-left: 20px; margin-top: 30px; } ");
			report+=_T(" .code_block { display: block; font-family: \"dejavu sans mono\", \"monaco\", \"lucida console\", \"courier new\", monospace; ");
			report+=_T("    font-size: x-small;	background: #eef;	border-top: 2px solid #999;	border-bottom: 2px solid #999; ");
			report+=_T("	line-height: 1.5em;	padding: 3px 1em;	overflow: auto;	white-space: nowrap; } ");
			report+=_T(" .code_short { 	max-height: 24em; padding-left: 30px;	}	 .code_full { 	max-height: 64em;	} ");
			report+=_T(" .some_font { 	text-align: left;	padding-left: 5px;	padding-right: 5px;	font-family:\"Verdana\";	font-size: 13px;	} ");
			report+=_T("</style></head><body>");

			/// TABLE OF CONTENTS

			report+= _T("<a name=\"top\"></a><h1>SMTP Tool html report.</h1>");
			report+= _T("<h3><a href=\"#1\">1. Results</a></h3>");
			report+= _T("<h3><a href=\"#2\">2. SPF</a></h3>");
			report+= _T("<h3><a href=\"#3\">3. Blacklist</a></h3>");
			report+= _T("<h3><a href=\"#4\">4. Console</a></h3>");
			report+= _T("<br><br>");

			/// 1.RESULTS

			report+= _T("<a name=\"1\"></a><div class=\"main2\">1. Results</div><a href=\"#top\">top</a>");
			
			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Test</td><td class=\"first_td\">Result</td><td class=\"first_td\">Output</td></tr> ");
			int i = 0;
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Is mail server")		+_T("</td><td class=\"left\">")+result_1ismailserver	+_T("</td><td class=\"center\"></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Version")				+_T("</td><td class=\"left\">")+result_2version			+_T("</td><td class=\"center\"></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Open relay")			+_T("</td><td class=\"left\">")+result_3openrelay		+_T("</td><td class=\"center\"><a href=\"#out0\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Verify user command")	+_T("</td><td class=\"left\">")+result_veriify			+_T("</td><td class=\"center\"><a href=\"#out1\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Accept @postmaster")	+_T("</td><td class=\"left\">")+result_postmaster		+_T("</td><td class=\"center\"><a href=\"#out2\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Accept @abuse")			+_T("</td><td class=\"left\">")+result_abuse			+_T("</td><td class=\"center\"><a href=\"#out3\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Filters attachemnts")	+_T("</td><td class=\"left\">")+result_attachm			+_T("</td><td class=\"center\"><a href=\"#out4\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Blacklist occurence")	+_T("</td><td class=\"left\">")+result_blacklist		+_T("</td><td class=\"center\"><a href=\"#out13\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("SPF")					+_T("</td><td class=\"left\">")+result_spf				+_T("</td><td class=\"center\"><a href=\"#out12\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Spoofing check 1")		+_T("</td><td class=\"left\">")+result_spoofing1		+_T("</td><td class=\"center\"><a href=\"#out5\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Spoofing check 2")		+_T("</td><td class=\"left\">")+result_spoofing2		+_T("</td><td class=\"center\"><a href=\"#out6\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Help command")			+_T("</td><td class=\"left\">")+result_help				+_T("</td><td class=\"center\"><a href=\"#out7\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Encrypted auth")		+_T("</td><td class=\"left\">")+result_encauth			+_T("</td><td class=\"center\"><a href=\"#out8\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Cleartext login")		+_T("</td><td class=\"left\">")+result_cleartext		+_T("</td><td class=\"center\"><a href=\"#out9\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("SASL methods")			+_T("</td><td class=\"left\">")+result_sasl				+_T("</td><td class=\"center\"><a href=\"#out10\">click</a></td></tr>");
			report+= _T("<tr class=\"tr_")+IntToStr((i++)%2+1)+_T("\"><td class=\"left\">")+_T("Authentication")		+_T("</td><td class=\"left\">")+result_auth				+_T("</td><td class=\"center\"><a href=\"#out11\">click</a></td></tr>");
			report+= _T("</table>");

			const char* jj[14] = {
				"------------------ Open Relay Test ------------------",
				"------------------   Verify Test   ------------------",
				"------------------   Postman Test  ------------------",
				"------------------   Abuse Test    ------------------",
				"------------------ Attachment Test ------------------",
				"------------------ Spoofing 1 Test ------------------",
				"------------------ Spoofing 2 Test ------------------",
				"------------------    Help Test    ------------------",
				"------------------ Encry.Auth Test ------------------",
				"------------------ Cleartext Test  ------------------",
				"------------------    SASL Test    ------------------",
				"------------------    Auth. Test   ------------------",
				"------------------     SPF Test    ------------------",
				"------------------  Blacklist Test ------------------"
			};

			const char* jq[14] = {
				"Open relay",
				"Verify user command",
				"Accept @postmaster",
				"Accept @abuse",
				"Filters attachemnts",
				"Spoofing check 1",
				"Spoofing check 2",
				"Help command",
				"Encrypted auth",
				"Cleartext login",
				"SASL methods",
				"Authentication",
				"SPF",
				"Blacklist occurence"
			};


			int poz1 = 0, poz2 = 0;
			char* consoles = StringToChar(console);
			char* tempc;

			CString mtem, mblack, mspf;
			mblack = _T("");
			mspf = _T("");

			for(int q=0;q<14;q++)
			{
				poz1 = FindStr(consoles,jj[q],poz2);
				if(poz1==-1)
					continue;
				poz2 = FindStr(consoles,"-----------------------------------------------------",poz1);
				if(poz2==-1)
					break; //error!

				mtem = _T("");

				tempc = MidStr(consoles,poz1,poz2+53);
				if(!tempc)
					tempc = " ";
				CString ga3;
				ga3 = tempc;
				ga3.Replace(_T("\r\n"),_T("<br>"));
				mtem+= _T("<a name=\"out");
				CString hg;
				hg.Format(_T("%d"),q);
				mtem+=hg;
				mtem+= _T("\"><a href=\"#1\">back</a><p class=\"some_font\"><b>");
				mtem+= jq[q];
				mtem+= _T("</b> output</p>");
				mtem+= _T("<div class=\"code_block code_short\"> ");
				mtem+= ga3;
				mtem+= _T("</div><br>");
				delete[] tempc;

				if(q==12) // SPF
					mspf = mtem;
				else if(q==13) // Blacklist
					mblack = mtem;
				else
					report+= mtem;
			}

			delete[] consoles;

			/// 2.SPF
				
			report+= _T("<a name=\"2\"></a><div class=\"main2\">2. SPF</div><a href=\"#top\">top</a>");
			
			//report+= _T("<p>Domain name: ")+result_spf_domain;
			//report+= _T("</p>");
			
			report+= _T("<p>SPF records: ");
			report+= result_spfbox;
			report+= _T("</p><br>");
			report+=mspf;

			report+= _T("<a name=\"3\"></a><div class=\"main2\">3. Blacklist</div><a href=\"#top\">top</a>");
			
			report+= _T("<p>Blacklisted: ")+result_blacklistednum+_T("</p>");

			report+= _T("<table class=\"table\"><tr class=\"first_tr\" ><td class=\"first_td\">Blacklist</td><td class=\"first_td\">Status</td><td class=\"first_td\">Reason</td></tr> ");

			for( i=0;i<blacklist.GetItemCount();i++)
			{
				report+= _T("<tr class=\"tr_")+IntToStr(i%2+1)+_T("\"><td class=\"left\">")+blacklist.GetItemText(i,0)+_T("</td><td class=\"left\">")+blacklist.GetItemText(i,1)+_T("</td><td class=\"left\"><a href=\"")+blacklist.GetItemText(i,2)+_T("\">")+blacklist.GetItemText(i,2)+_T("</a></td></tr>");
			}

			report+= _T("</table>");
			report+=mblack;

			report+= _T("<a name=\"4\"></a><div class=\"main2\">4. Console</div><a href=\"#top\">top</a>");
			
			CString tem5 = console;
			tem5.Replace(_T("\r\n"),_T("<br>"));

			report+=_T("<br><br><div class=\"code_block code_full\">")+tem5+_T("</div> <a href=\"#top\">top</a><h2>End of report</h2></body>");
			
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
