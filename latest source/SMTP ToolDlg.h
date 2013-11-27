// SMTP ToolDlg.h : Headerdatei
//

#pragma once

class CSMTPToolDlg;
class CAboutDlg;

//#include "ResultsDlg.h"
#include "DlgSettings.h"
#include "DlgMain.h"
#include "Serial.h"
#include "afxcmn.h"
#include "shared.h"

// CSMTPToolDlg-Dialogfeld
class CSMTPToolDlg : public CDialog
{
// Konstruktion
public:
	CSMTPToolDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_SMTPTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CDlgMain DlgMain;
	CDlgSettings DlgSettings;
	void ShowWindowNumber(int number);
	CRect m_rSettingsRect;
	/*
	BOOL test_relay;
	BOOL test_spoofing;
	BOOL test_verify;
	BOOL test_postabuse;
	BOOL test_attachment;
	BOOL test_blacklist;
	BOOL test_spf;
	CString server_address;
	CString recepient_address;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedBrun();
	BOOL type_name;
	bool code_ok;
	CString spoofing_to;
	CString spoofing_from;
	CString spoofing_subject;
	CString spoofing_msg;
	CString relay_from_name;
	CString relay_from_domain;
	CString relay_to_domain;
	CString relay_to_name;
	CString auth_user;
	CString auth_pass;
	BOOL auth_use;
	test_settings set;
	CString from_mail;
	CString attachment_path;
	afx_msg void OnBnClickedButton3();
	CString attachment_to;
	CString attachment_from;
	CString spoofing_from2;
	CString spoofing_to2;
	CString spoofing_replyto;
	BOOL test_spoofing2;
	CString spoofing_server;
	afx_msg void OnBnClickedCheck8();
	BOOL test_help;
	BOOL test_encrypted;
	BOOL test_cleartext;
	BOOL check_sasl;
	BOOL check_auth;*/
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_cTab;
	//CShared* shared;
	bool code_ok;
};
