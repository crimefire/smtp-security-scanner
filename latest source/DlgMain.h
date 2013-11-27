#pragma once
#include "afxtempl.h"
#include "afxcmn.h"

class CDlgMain;
class CAboutDlg;

//#include "shared.h"
//#include "SMTP ToolDlg.h"
#include "DlgSettings.h"
#include "shared.h"

// CDlgMain dialog

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

class CDlgMain : public CDialog
{
	DECLARE_DYNAMIC(CDlgMain)

public:
	CDlgMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMain();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString result_1ismailserver;
	CString result_2version;
	CString result_3openrelay;
	CString result_veriify;
	CString result_postmaster;
	CString result_abuse;
	CString result_attachm;
	CString result_blacklist;
	CString result_spf;
	CString result_spoofing1;
	CString result_spoofing2;
	CString result_help;
	CString result_encauth;
	CString result_cleartext;
	CString result_sasl;
	CString result_auth;
	CString result_blacklistednum;
	CString result_spfbox;
	CString console;
	void InitList(void);
	CList<CString> rcptto_list;
	bool columns_init;
	CListCtrl blacklist;
	char* receive;
	afx_msg void OnBnClickedButton1();
	BOOL test_openrelay;
	BOOL test_verify;
	BOOL test_postmaster;
	BOOL test_abuse;
	BOOL test_attachments;
	BOOL test_blacklist;
	BOOL test_spf;
	BOOL test_spoofing1;
	BOOL test_spoofing2;
	BOOL test_help;
	BOOL test_encauth;
	BOOL test_cleartext;
	BOOL test_sasl;
	BOOL test_auth;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton9();
	//CShared* shared;
	CDlgSettings* DlgSettings;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton3();
	HANDLE Thread;
	DWORD ThreadId;
	CString result_spf_domain;
	CString res_domain;
	BOOL running;
	BOOL want_cancel;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
};
