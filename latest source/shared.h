
#pragma once

//class CShared;
struct thread_data;

#include "stdafx.h"
#include "SMTP Tool.h"
#include "curl/curl.h"
#include "curl_tools.h"

//#include "SMTP ToolDlg.h"
#include "DlgMain.h"
#include "DlgSettings.h"
//#include "Windns.h"
/*
class CShared
{
	public:
		CSMTPToolDlg* Dlg;
		CDlgMain* Dlgmain;
		CDlgSettings* Dlgsettings;
};
*/
struct thread_data
{
	//CShared* shared;
	int test_id;
	CDlgMain* Dlgmain;
	CDlgSettings* Dlgsettings;
};

DWORD WINAPI RunTests(LPVOID lpParam);