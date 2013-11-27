#pragma once

#include "afxcmn.h"

#include "curl/curl.h"
#include "curl_tools.h"
#include "afxtempl.h"

// CResultsDlg dialog

//char* GetIpFromString(char* str);

DWORD WINAPI RunTests(LPVOID lpParam);

struct test_settings
{
	CString server_address;
	BOOL address_type;
	BOOL test_open_relay;
	BOOL test_spoofing;
	BOOL test_verify;
	BOOL test_postmaster;
	BOOL test_attachments;
	BOOL test_blacklist;
	BOOL test_spf;
	BOOL test_spoofing2;
	CString spoofing_to;
	CString spoofing_from;
	CString spoofing_subject;
	//CString spoofing_message;
	CList<CString> spoofing_message;
	CString relay_from_name;
	CString relay_from_domain;
	CString relay_to_name;
	CString relay_to_domain;
	BOOL auth_use;
	CString auth_login;
	CString auth_pass;
	CString from_mail;
	CString attachment_path;
	CString attachment_to;
	CString attachment_from;
	CString spoofing_from2;
	CString spoofing_to2;
	CString spoofing_replyto;
	CString spoofing_server;

	BOOL test_help;
	BOOL test_encrypted;
	BOOL test_cleartext;
	BOOL check_sasl;
	BOOL check_auth;
};

class CResultsDlg : public CDialog
{
	DECLARE_DYNAMIC(CResultsDlg)

public:
	CResultsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultsDlg();

// Dialog Data
	enum { IDD = IDD_RESULTSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString result_openrelay;
	CString result_spoofing;
	CString result_verify;
	CString result_postmaster;
	CString result_abuse;
	CString result_attachments;
	CString result_spf_type;
	CString result_spf_domain;
	CString result_spf_ttl;
	CString result_spf_record;
	CString result_blacklisted;
	void InitList(void);
	virtual BOOL OnInitDialog();
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	bool columns_init;
//	afx_msg void OnEnterSizeMove();
//	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl blacklist;
	CString console;
	test_settings* settings;
	CString result_mailserver;
	SOCKET sock;
	char* receive;
	void Send(const char* msg);
	void SendBinary(char* data, int len);
	void Receive(void);
	void PrintLastError(void);
	DWORD ThreadId;
	HANDLE Thread;
	CSocket* socket;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CURL* curl;
	thread_status* this_struct;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	CString result_spf;
	CList<CString> rcptto_list;
	CString result_spoofing2;
	CString result_help;
	CString result_encrypted;
	CString result_cleartext;
	CString result_sasl;
	CString result_auth;
	CString sasl_methods;
	CString version;
	afx_msg void OnBnClickedButton6();
	CString res_domain;
};

struct thread_data
{
	CResultsDlg* result_dlg;
};