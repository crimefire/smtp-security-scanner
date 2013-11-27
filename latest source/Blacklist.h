#pragma once



// CBlacklist form view

class CBlacklist : public CFormView
{
	DECLARE_DYNCREATE(CBlacklist)

protected:
	CBlacklist();           // protected constructor used by dynamic creation
	virtual ~CBlacklist();

public:
	enum { IDD = IDD_RESULTSDLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


