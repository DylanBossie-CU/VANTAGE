// IdsRotationDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "IdsRotationDemo.h"
#include "IdsRotationDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIdsRotationDemoApp

BEGIN_MESSAGE_MAP(CIdsRotationDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CIdsRotationDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdsRotationDemoApp construction

CIdsRotationDemoApp::CIdsRotationDemoApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIdsRotationDemoApp object

CIdsRotationDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIdsRotationDemoApp initialization

BOOL CIdsRotationDemoApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();

	SetRegistryKey(_T("IDS\\uEye"));
    WriteProfileString(_T("MRU"), _T(""), theApp.m_pszAppName);

	CIdsRotationDemoDlg dlg;
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
