// SMTP ToolDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "SMTP Tool.h"
#include "SMTP ToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSMTPToolDlg::CSMTPToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMTPToolDlg::IDD, pParent)
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	DlgSettings.DlgMain = &DlgMain;
	DlgMain.DlgSettings = &DlgSettings;
	//DlgSettings = new CDlgSettings();
	//DlgMain = new CDlgMain();
	/*
	CTime theTime;
	theTime = CTime::GetCurrentTime();
	if(theTime.GetMonth()>8 || ((theTime.GetMonth()==8)&&(theTime.GetDay()>12)) )
	{
		MessageBox(_T("BETA VERSION EXPIRED"));
		exit(46);
		int* a;
		delete[] a;
	}*/
}

void CSMTPToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_TAB1, m_cTab);
}

BEGIN_MESSAGE_MAP(CSMTPToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSMTPToolDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CSMTPToolDlg-Meldungshandler

BOOL CSMTPToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//shared = new CShared;
	//shared->Dlg = this;

	CRect tabRect;

     m_cTab.GetWindowRect(tabRect);

     // Set the size and location of the child windows based on the tab control
     m_rSettingsRect.left = 13-5;
     m_rSettingsRect.top = 44-15;
     m_rSettingsRect.right = tabRect.Width() - 7+2;
     m_rSettingsRect.bottom = tabRect.Height() - 38+7;

     // Create the child windows for the main window class
	 DlgMain.Create(IDD_MAIN, this);
	 DlgSettings.Create(IDD_SETTINGS, this);

     // This is redundant with the default value, considering what OnShowWindow does
     ShowWindowNumber(0);

     // Set the titles for each tab
     TCITEM tabItem;
     tabItem.mask = TCIF_TEXT;
     tabItem.pszText = _T("&Test");
     m_cTab.InsertItem(0, &tabItem);
     tabItem.pszText = _T("&Settings");
     m_cTab.InsertItem(1, &tabItem);

	code_ok = true;
	bool code_ok2 = true;
	/*
	code_ok = false;
	bool code_ok2 = false;
	int* ble = (int*)353633;
	CSerial serial;

	for(;;)
	{
		serial.DoModal();

		//serial.UpdateData(TRUE);
		
		CString temp = _T("DNS Tool");
		temp = _T("")+serial.ser1;
		if(temp.Mid(0,3)==_T("XHD"))
			if(temp.Mid(5,1)==_T("H"))
			{
				int aa = _wtoi(temp.Mid(3,1));
				int ab = _wtoi(temp.Mid(4,1));

				if(aa+ab==13)
				{
					temp = _T("")+serial.ser2;

					aa = _wtoi(temp.Mid(0,1));
					ab = _wtoi(temp.Mid(1,1));

					if(aa+ab==15)
					{
						if(temp.Mid(2,2)==_T("GJ"))
							if(temp.Mid(4,2)==_T("RH"))
							{
								temp = _T("")+serial.ser3;
								if(temp.Mid(0,2)==_T("BH"))
									if(temp.Mid(2,2)==_T("DD"))
										if(temp.Mid(4,2)==_T("87"))
										{
											code_ok = true;
										}
							}
					}
				}
			}

		if(serial.canceled)
		{
			exit(0);
		}

		if(!code_ok)
		{
			Sleep(200);
			Sleep(100);
			MessageBox(_T("Wrong serial code. Please try again."));
		}
		else break;
	}

	if(!code_ok)
	{
		*ble = 455;
		ble+=3531;
		*ble = 3153531;
	}
	else
		code_ok2 = true;
*/

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	int* ble2;

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	if(!code_ok2)
	{
		*ble2 = 8855;
		ble2+=4531;
		*ble2 = 483531;
	}

	// TODO: Hier zusätzliche Initialisierung einfügen

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CSMTPToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CSMTPToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CSMTPToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*


void CSMTPToolDlg::OnBnClickedBrun()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(server_address==_T(""))
	{
		MessageBox(_T("Please enter server address"));
		return;
	}

	if(test_relay&&( (relay_from_domain==_T(""))||(relay_from_name==_T(""))||(relay_to_domain==_T(""))||(relay_to_name==_T("")) ))
	{
		MessageBox(_T("To test for Open Relay you need to fill Relay Test Configuration"));
		return;
	}

	if((test_spoofing||test_spoofing2)&&( (spoofing_from==_T(""))||(spoofing_msg==_T(""))||(spoofing_subject==_T(""))||(spoofing_to==_T("")) ))
	{
		MessageBox(_T("To test for spoofing you need to fill Configuration"));
		return;
	}

	if(test_spoofing2&&(spoofing_server==_T("")))
	{
		MessageBox(_T("You need to fill server for Spoofing check 2"));
		return;
	}

	if(test_spoofing2&&(!auth_use))
	{
		MessageBox(_T("Spoofing check 2 requires authentication"));
		return;
	}

	if(test_postabuse&&(from_mail==_T("")))
	{
		MessageBox(_T("To test for Postmaster/Abuse you need to fill MAIL field"));
		return;
	}

	if(test_attachment&&((attachment_from==_T(""))||(attachment_path==_T(""))||(attachment_to==_T(""))))
	{
		MessageBox(_T("To test for Attachment Filtering you need to fill Attachment configuration"));
		return;
	}

	set.address_type		= type_name;
	set.server_address		= server_address;
	set.spoofing_from		= spoofing_from;
	set.spoofing_subject	= spoofing_subject;
	set.spoofing_to			= spoofing_to;
	set.test_attachments	= test_attachment;
	set.test_blacklist		= test_blacklist;
	set.test_open_relay		= test_relay;
	set.test_postmaster		= test_postabuse;
	set.test_spf			= test_spf;
	set.test_verify			= test_verify;
	set.test_spoofing		= test_spoofing;
	set.test_spoofing2		= test_spoofing2;
	set.relay_from_domain	= relay_from_domain;
	set.relay_from_name		= relay_from_name;
	set.relay_to_domain		= relay_to_domain;
	set.relay_to_name		= relay_to_name;
	set.auth_use			= auth_use;
	set.auth_login			= auth_user;
	set.auth_pass			= auth_pass;
	set.from_mail			= from_mail;
	set.attachment_path		= attachment_path;
	set.attachment_to		= attachment_to;
	set.attachment_from		= attachment_from;
	set.spoofing_from2		= spoofing_from2;
	set.spoofing_to2		= spoofing_to2;
	set.spoofing_replyto	= spoofing_replyto;
	set.spoofing_server		= spoofing_server;

	set.spoofing_message.RemoveAll();
	
	CEdit* pmyEdit = ((CEdit*)GetDlgItem(IDC_EDIT6));

	int i, nLineCount = pmyEdit->GetLineCount();
	CString strText, strLine;

    for (i=0;i < nLineCount;i++)
    {
	   // length of line i:
	   int len = pmyEdit->LineLength(pmyEdit->LineIndex(i));
	   pmyEdit->GetLine(i, strText.GetBuffer(len), len);
	   strText.ReleaseBuffer(len);
	   set.spoofing_message.AddTail(strText);
    }

	set.test_help = test_help;
	set.test_encrypted = test_encrypted;
	set.test_cleartext = test_cleartext;
	set.check_sasl = check_sasl;
	set.check_auth = check_auth;


	CResultsDlg dlg;
	dlg.settings = &set;
	//dlg.Create(IDD_RESULTSDLG,this);
	//dlg.ShowWindow(SW_SHOW);
	//dlg.CenterWindow();
	//dlg.InitList();
	dlg.DoModal();
}

void CSMTPToolDlg::OnBnClickedButton3()
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
		attachment_path = temp;

		UpdateData(FALSE);
    }
}

void CSMTPToolDlg::OnBnClickedCheck8()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT21)->EnableWindow(auth_use);
	GetDlgItem(IDC_EDIT22)->EnableWindow(auth_use);
	GetDlgItem(IDC_EDIT28)->EnableWindow(auth_use);
}
*/
void CSMTPToolDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if (bShow)
	 {
		  ShowWindowNumber(0);
	 }
}

void CSMTPToolDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Get the number of the currently selected tab, and show its window
     ShowWindowNumber(m_cTab.GetCurFocus());

     pNMHDR = NULL;
	 *pResult = 0;
     pResult = NULL;
}

void CSMTPToolDlg::ShowWindowNumber(int number)
{
     // This example uses four windows
     int windowCount = 2;

     // Validate the parameter
     if ((number >= 0) && (number < windowCount))
     {
          // Create and assign pointers to each window
          CDialog *m_dPointer[2];
          m_dPointer[0] = &DlgMain;
          m_dPointer[1] = &DlgSettings;

          // Hide every window except for the chosen one
          for (int count = 0; count < windowCount; count++)
          {
               if (count != number)
               {
                    m_dPointer[count]->ShowWindow(SW_HIDE);
               }
               else if (count == number)
               {
                    // Show the chosen window and set it's location
                    m_dPointer[count]->SetWindowPos(&wndTop, m_rSettingsRect.left,
                         m_rSettingsRect.top, m_rSettingsRect.right,
                         m_rSettingsRect.bottom, SWP_SHOWWINDOW);

                    m_cTab.SetCurSel(count);
               }
          }
     }
}