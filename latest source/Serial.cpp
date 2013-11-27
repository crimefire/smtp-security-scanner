// Serial.cpp : implementation file
//

#include "stdafx.h"
#include "SMTP Tool.h"
#include "Serial.h"


// CSerial dialog

IMPLEMENT_DYNAMIC(CSerial, CDialog)

CSerial::CSerial(CWnd* pParent /*=NULL*/)
	: CDialog(CSerial::IDD, pParent)
	, canceled(false)
	, ser1(_T(""))
	, ser2(_T(""))
	, ser3(_T(""))
{

}

CSerial::~CSerial()
{
}

void CSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ser1);
	DDX_Text(pDX, IDC_EDIT2, ser2);
	DDX_Text(pDX, IDC_EDIT3, ser3);
}


BEGIN_MESSAGE_MAP(CSerial, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CSerial::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSerial message handlers

void CSerial::OnBnClickedCancel()
{
	canceled = true;
	// TODO: Add your control notification handler code here
	OnCancel();
}
