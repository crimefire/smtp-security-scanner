#pragma once
#include "afxwin.h"

class CDlgSettings;

//#include "shared.h"
//#include "SMTP ToolDlg.h"
#include "DlgMain.h"
#include "afxtempl.h"

// CDlgSettings dialog

class CDlgSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettings)

public:
	CDlgSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettings();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton but_saveprofile;
	//CShared* shared;
	CDlgMain* DlgMain;
	CString loadprofile_id;
	CComboBox loadprofile_combo;
	CString saveprofileas;
	afx_msg void OnBnClickedCheck8();
	BOOL auth_use;
	afx_msg void OnBnClickedButton3();
	CString set_serveraddress;
	CString set_fromabuseverify;
	CString set_authuser;
	CString set_authpass;
	CString set_authserver;
	CString set_spoofing_mailfrom;
	CString set_spoofing_rpctto;
	CString set_spoofing_from;
	CString set_spoofing_to;
	CString set_spoofing_replyto;
	CString set_spoofing_subject;
	CString set_spoofing_message;
	CString set_relayfrom1;
	CString set_relayfrom2;
	CString set_relayto1;
	CString set_relayto2;
	CString set_attach_path;
	CString set_attach_from;
	CString set_attach_to;
	CList<CString> set_spoofing_message_list;
	CString set_attach_delay;
	CButton but_del_profile;
	afx_msg void OnBnClickedButton1();
	void ReloadProfiles(void);
	afx_msg void OnCbnSelchangeCombo1();
	void PushSetting(CString name, CString value);
	afx_msg void OnBnClickedButton2();
};
