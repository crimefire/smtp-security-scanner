#pragma once


// CSerial dialog

class CSerial : public CDialog
{
	DECLARE_DYNAMIC(CSerial)

public:
	CSerial(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSerial();

// Dialog Data
	enum { IDD = IDD_SERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool canceled;
	CString ser1;
	CString ser2;
	CString ser3;
	afx_msg void OnBnClickedCancel();
};
