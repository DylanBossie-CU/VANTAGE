// uEyeEvent.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "uEyeEvent.h"
#include "uEyeEventDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CuEyeEventApp
BEGIN_MESSAGE_MAP(CuEyeEventApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CuEyeEventApp construction
CuEyeEventApp::CuEyeEventApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CuEyeEventApp object
CuEyeEventApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CuEyeEventApp initialization
BOOL CuEyeEventApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CuEyeEventDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = (int)dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	// application, rather than start the application's message pump.
	return FALSE;
}
