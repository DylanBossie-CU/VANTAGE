#include "stdafx.h"
#include "uEyeSimpleLive_DynamicDll.h"
#include "uEyeSimpleLive_DynamicDllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CuEyeSimpleLive_DynamicDllApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CuEyeSimpleLive_DynamicDllApp::CuEyeSimpleLive_DynamicDllApp()
{
}


CuEyeSimpleLive_DynamicDllApp theApp;


BOOL CuEyeSimpleLive_DynamicDllApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	SetRegistryKey(_T("IDS\\uEye"));

	CuEyeSimpleLive_DynamicDllDlg dlg;
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
