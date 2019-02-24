
#include "stdafx.h"
#include "uEyeMeasureSharpness.h"
#include "uEyeMeasureSharpnessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CuEyeMeasureSharpnessApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


CuEyeMeasureSharpnessApp::CuEyeMeasureSharpnessApp()
{
}


CuEyeMeasureSharpnessApp theApp;


BOOL CuEyeMeasureSharpnessApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();

	SetRegistryKey(_T("IDS\\uEye"));
    WriteProfileString(_T("MRU"), _T(""), theApp.m_pszAppName);

	CuEyeMeasureSharpnessDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
