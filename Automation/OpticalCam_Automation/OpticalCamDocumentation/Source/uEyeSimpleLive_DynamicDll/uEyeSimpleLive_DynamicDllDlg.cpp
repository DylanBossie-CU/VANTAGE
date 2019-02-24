//==========================================================================//
//                                                                          //
//  Copyright (C) 2004 - 2018                                               //
//  IDS - Imaging Development Systems GmbH                                  //
//  Dimbacherstr. 6-8                                                       //
//  D-74182 Obersulm-Willsbach                                              //
//                                                                          //
//  The information in this document is subject to change without           //
//  notice and should not be construed as a commitment by IDS Imaging       //
//  Development Systems GmbH.                                               //
//  IDS Imaging Development Systems GmbH does not assume any responsibility //
//  for any errors that may appear in this document.                        //
//                                                                          //
//  This document, or source code, is provided solely as an example         //
//  of how to utilize IDS software libraries in a sample application.       //
//  IDS Imaging Development Systems GmbH does not assume any responsibility //
//  for the use or reliability of any portion of this document or the       //
//  described software.                                                     //
//                                                                          //
//  General permission to copy or modify, but not for profit, is hereby     //
//  granted,  provided that the above copyright notice is included and      //
//  included and reference made to the fact that reproduction privileges    //
//  were granted by IDS Imaging Development Systems GmbH.                   //
//                                                                          //
//  IDS cannot assume any responsibility for the use, or misuse, of any     //
//  portion or misuse, of any portion of this software for other than its   //
//  intended diagnostic purpose in calibrating and testing IDS manufactured //
//  image processing boards and software.                                   //
//                                                                          //
//==========================================================================//

#include "stdafx.h"
#include "uEyeSimpleLive_DynamicDll.h"
#include "uEyeSimpleLive_DynamicDllDlg.h"
#include "version.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialog
{

public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

protected:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange(CDataExchange* pDX); 

private:
    DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}


BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString Str;
    GetDlgItem(IDC_STATIC_COPYRIGHT)->GetWindowText(Str);
    Str.Append(_T(CURRENT_YEAR));
    GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(Str);

    return TRUE;
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CuEyeSimpleLive_DynamicDllDlg::CuEyeSimpleLive_DynamicDllDlg(CWnd* pParent /*=NULL*/)
: CDialog(CuEyeSimpleLive_DynamicDllDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CuEyeSimpleLive_DynamicDllDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CuEyeSimpleLive_DynamicDllDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_INIT, &CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonInit)
    ON_BN_CLICKED(IDC_BUTTON_EXIT, &CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonExit)
    ON_BN_CLICKED(IDC_BUTTON_LIVE, &CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonLive)
    ON_BN_CLICKED(IDC_BUTTON_SNAP, &CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonSnap)
    ON_BN_CLICKED(IDC_BUTTON_PARAMETERS, &CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonParameters)
END_MESSAGE_MAP()


BOOL CuEyeSimpleLive_DynamicDllDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_hCam = 0;
    b_Live = false;

    return TRUE;
}

void CuEyeSimpleLive_DynamicDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}


void CuEyeSimpleLive_DynamicDllDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}


HCURSOR CuEyeSimpleLive_DynamicDllDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CuEyeSimpleLive_DynamicDllDlg::OnClose()
{
    OnBnClickedButtonExit() ;
    CDialog::OnClose();
}


void CuEyeSimpleLive_DynamicDllDlg::OnOK()
{
    this->UpdateData(TRUE);
}


void CuEyeSimpleLive_DynamicDllDlg::OnCancel()
{
    OnBnClickedButtonExit() ;
    CDialog::OnClose();
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonInit()
//
// DESCRIPTION: Connects (loads) the uEye API DLL.
//              Open first available uEye camera.
//              Switch to the Direct 3D display mode. In case the graphics
//              board does nit Direct 3D there is no display.
//              In Direct 3D no image memory buffer is required.
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonInit()
{
    // let's start by loading the uEye API DLL
    if (!m_ueye.IsLoaded() && m_ueye.Connect(_T(DRIVER_DLL_NAME)))
    {
        int ret;

        // enable the dialog based error report
        m_ueye.is_SetErrorReport(NULL, IS_ENABLE_ERR_REP);

        // get handle to display window and init first available camera
        HWND hWndDisplay = GetDlgItem( IDC_DISPLAY )->m_hWnd;
        m_hCam = 0;
        ret = m_ueye.is_InitCamera(&m_hCam, hWndDisplay);
        if (ret == IS_SUCCESS)
        {	
            //Check if DirectX is available on the local machine
            m_ueye.is_SetErrorReport(NULL, IS_DISABLE_ERR_REP);
            if(m_ueye.is_DirectRenderer(m_hCam, DR_CHECK_COMPATIBILITY, NULL , NULL) != IS_SUCCESS)
            {
                MessageBox(_T("This Demo requires support of Direct3D.\nThis usually means that either DirectX or the graphics card driver is not installed properly"),
                    _T("Error"), MB_OK | MB_ICONSTOP);
                return;
            }
            m_ueye.is_SetErrorReport(NULL, IS_ENABLE_ERR_REP);

            ret = m_ueye.is_SetColorMode(m_hCam, IS_CM_BGRA8_PACKED);
            if (ret == IS_SUCCESS)
            {
                // got the camera, so switch to the Direct 3D display mode
                ret = m_ueye.is_SetDisplayMode(m_hCam, IS_SET_DM_DIRECT3D);
                if (ret == IS_SUCCESS)
                {
                    // we scale the image to fit into the display window
                    ret = m_ueye.is_DirectRenderer(m_hCam, DR_ENABLE_SCALING, NULL, NULL);
                    if (ret == IS_SUCCESS)
                    {
                        // ok - everthing is fine so process the GUI stuff
                        GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(FALSE);
                        GetDlgItem(IDC_BUTTON_LIVE)->EnableWindow(TRUE);
                        GetDlgItem(IDC_BUTTON_SNAP)->EnableWindow(TRUE);
                        GetDlgItem(IDC_BUTTON_PARAMETERS)->EnableWindow(TRUE);
                    }
                }
            }
        }
    }
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonLive()
//
// DESCRIPTION: Toggle live display on/off.
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonLive()
{
    // switch from live to stop live or vice versa
    if (b_Live == false)
    {
        b_Live = true;
        m_ueye.is_CaptureVideo(m_hCam, IS_DONT_WAIT);
        SetDlgItemText(IDC_BUTTON_LIVE,_T("Stop live"));
    }
    else
    {
        b_Live = false;
        m_ueye.is_StopLiveVideo(m_hCam, IS_WAIT);
        SetDlgItemText(IDC_BUTTON_LIVE,_T("Start live"));
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonSnap()
//
// DESCRIPTION: Perform a single snap and thus stop the live display.
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonSnap()
{
    b_Live = false;
    SetDlgItemText(IDC_BUTTON_LIVE,_T("Start live"));
    m_ueye.is_FreezeVideo(m_hCam, IS_WAIT);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonParameters()
//
// DESCRIPTION: Load camera parameters from ini file.
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonParameters()
{
    m_ueye.is_LoadParameters(m_hCam, NULL);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD void CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonExit()
//
// DESCRIPTION: Cleanup the application.
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleLive_DynamicDllDlg::OnBnClickedButtonExit()
{
    // close camera
    if (m_hCam != 0)
    {
        m_ueye.is_ExitCamera(m_hCam);
    }

    // disconnect (unload) uEye API DLL
    if (m_ueye.IsLoaded())
    {
        m_ueye.Disconnect();
    }

    // suicide
    PostQuitMessage(0);
}
