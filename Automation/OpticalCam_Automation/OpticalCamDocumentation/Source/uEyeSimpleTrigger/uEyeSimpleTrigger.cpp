
#include "stdafx.h"
#include "uEyeSimpleTrigger.h"
#include "uEyeSimpleTriggerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CuEyeSimpleTriggerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CuEyeSimpleTriggerApp::CuEyeSimpleTriggerApp()
{
}

/////////////////////////////////////////////////////////////////////////////
CuEyeSimpleTriggerApp theApp;

/////////////////////////////////////////////////////////////////////////////
BOOL CuEyeSimpleTriggerApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();

	SetRegistryKey(_T("IDS\\uEye"));
    WriteProfileString(_T("MRU"), _T(""), theApp.m_pszAppName);

	CuEyeSimpleTriggerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
