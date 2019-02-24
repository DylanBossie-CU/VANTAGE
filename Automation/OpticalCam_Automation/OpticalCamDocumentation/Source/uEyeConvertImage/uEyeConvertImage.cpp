#include "stdafx.h"
#include "uEyeConvertImage.h"
#include "uEyeConvertImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(ConvertImageApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


ConvertImageApp::ConvertImageApp()
{
}

ConvertImageApp theApp;


BOOL ConvertImageApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();

	SetRegistryKey(_T("IDS\\uEye"));
    WriteProfileString(_T("MRU"), _T(""), theApp.m_pszAppName);

	ConvertImageDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
