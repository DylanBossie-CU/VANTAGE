
#include "stdafx.h"
#include "uEyeCrosshair.h"
#include "uEyeCrosshairDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(uEyeCrosshairApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


uEyeCrosshairApp::uEyeCrosshairApp()
{
}

uEyeCrosshairApp theApp;


BOOL uEyeCrosshairApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    SetRegistryKey(_T("IDS\\uEye\\uEyeCrosshair"));

    uEyeCrosshairDlg dlg;
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
