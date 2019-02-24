// uEyeIO.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "uEyeIO.h"
#include "uEyeIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUEyeIOApp

BEGIN_MESSAGE_MAP(CUEyeIOApp, CWinApp)
	//{{AFX_MSG_MAP(CUEyeIOApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUEyeIOApp construction

CUEyeIOApp::CUEyeIOApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUEyeIOApp object

CUEyeIOApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUEyeIOApp initialization

BOOL CUEyeIOApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	InitCommonControls();

#if _MSC_VER < 1300					// if Visual Studio 6.0
	#ifdef _AFXDLL
		Enable3dControls();			// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
	#endif
#endif

	CUEyeIODlg dlg;
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
