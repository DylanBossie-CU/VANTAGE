// uEyeSteal.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "uEyeSteal.h"
#include "uEyeStealDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUEyeStealApp
BEGIN_MESSAGE_MAP (CUEyeStealApp, CWinApp)
	ON_COMMAND (ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CUEyeStealApp construction
CUEyeStealApp::CUEyeStealApp ()
{
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CUEyeStealApp object
CUEyeStealApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CUEyeStealApp initialization
BOOL CUEyeStealApp::InitInstance ()
{
	InitCommonControls ();
	
#if _MSC_VER < 1300					// if Visual Studio 6
	#ifdef _AFXDLL
		Enable3dControls ();		// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic ();	// Call this when linking to MFC statically
	#endif
#endif

	CUEyeStealDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal ();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
