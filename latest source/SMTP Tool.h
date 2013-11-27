// SMTP Tool.h : Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CSMTPToolApp:
// Siehe SMTP Tool.cpp für die Implementierung dieser Klasse
//

class CSMTPToolApp : public CWinApp
{
public:
	CSMTPToolApp();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CSMTPToolApp theApp;