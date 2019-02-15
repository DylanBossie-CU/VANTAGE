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

// uEyeFlashStrobeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uEyeFlashStrobe.h"
#include "uEyeFlashStrobeDlg.h"
#include <process.h>
#include ".\ueyeflashstrobedlg.h"
#include "version.h"


unsigned WINAPI threadProc(void* pv);


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
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
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUEyeFlashStrobeDlg dialog

CUEyeFlashStrobeDlg::CUEyeFlashStrobeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUEyeFlashStrobeDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CUEyeFlashStrobeDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUEyeFlashStrobeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUEyeFlashStrobeDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUEyeFlashStrobeDlg, CDialog)
    //{{AFX_MSG_MAP(CUEyeFlashStrobeDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_BN_CLICKED(IDC_BUTTON_CAPTURE, OnButtonCapture)
    ON_BN_CLICKED(IDC_BUTTON_CAPTURE_STOP, OnButtonCaptureStop)
    ON_BN_CLICKED(IDC_CHECK_CONT, OnCheckCont)
    ON_BN_CLICKED(IDC_RADIO_FLASH_OFF, OnRadioFlashOff)
    ON_BN_CLICKED(IDC_RADIO_FLASH_ACT_LO, OnRadioFlashActLo)
    ON_BN_CLICKED(IDC_RADIO_FLASH_ACT_HI, OnRadioFlashActHi)
    ON_BN_CLICKED(IDC_RADIO_FLASH_DOUT_LO, OnRadioFlashDoutLo)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_RADIO_FLASH_DOUT_HI, OnRadioFlashDoutHi)
    //}}AFX_MSG_MAP
    ON_MESSAGE( IS_UEYE_MESSAGE, OnUEyeMessage )
    ON_BN_CLICKED(IDC_BUTTON_GLOBAL_FLASH, OnBnClickedButtonGlobalFlash)
    ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAMETER, OnBnClickedButtonLoadParameter)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUEyeFlashStrobeDlg message handlers

BOOL CUEyeFlashStrobeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
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

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE); // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // init variables
    m_hCam = 0;
    m_ContAcq = false;
    m_RunAcq = false;
    m_nCntFail = 0;
    ZeroMemory( &m_SensorInfo, sizeof( m_SensorInfo) );

    // display
    m_hWndDisplay = GetDlgItem( IDC_DISPLAY )->GetSafeHwnd(); // set display window handle

    // init camera setup operation
    OpenCamera(); // open a camera handle
    OnRadioFlashOff();

    // GUI
    CheckDlgButton( IDC_RADIO_FLASH_OFF, BST_CHECKED );
    GetDlgItem(IDC_BUTTON_GLOBAL_FLASH)->EnableWindow( FALSE );
    CheckDlgButton( IDC_CHECK_TRIGGER_SW, BST_CHECKED );
    SetDlgItemInt( IDC_EDIT_TRANSFER_FAIL, m_nCntFail );

    return TRUE; // return TRUE  unless you set the focus to a control
}

void CUEyeFlashStrobeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUEyeFlashStrobeDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUEyeFlashStrobeDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OpenCamera()
//
// DESCRIPTION: - Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeFlashStrobeDlg::OpenCamera()
{
    if ( m_hCam != 0 )
    {
        is_ExitCamera( m_hCam );
    }

    // init camera
    m_hCam = (HIDS) 0; // open next camera
    m_Ret = InitCamera( &m_hCam, m_hWndDisplay );
    if( m_Ret == IS_SUCCESS )
    {
        if (is_SetExternalTrigger(m_hCam, IS_GET_SUPPORTED_TRIGGER_MODE) == 0)
        {
            AfxMessageBox(L"This camera does not support the trigger function!", MB_ICONWARNING);
            m_hThread = NULL;
            m_hEv = NULL;
            return false;
        }

        // enable the dialog based error report
        //m_Ret = is_SetErrorReport(m_hCam, IS_ENABLE_ERR_REP); // or IS_DISABLE_ERR_REP);
        //if( m_Ret != IS_SUCCESS )
        //{
        //    AfxMessageBox(L"ERROR: Can not enable the automatic uEye error report!" , MB_ICONEXCLAMATION, 0 );
        //    return false;
        //}

        // setup the color depth to the current windows setting
        is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
        is_SetColorMode(m_hCam, m_nColorMode);

        // init image size
        is_GetSensorInfo(m_hCam, &m_SensorInfo);

        //m_nSizeX = m_SensorInfo.nMaxWidth;
        //m_nSizeY = m_SensorInfo.nMaxHeight;
        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

        // display initialization
        is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);

        // alloc buffer
        m_Ret = is_AllocImageMem(m_hCam,
            m_nSizeX,
            m_nSizeY,
            m_nBitsPerPixel,
            &m_pcImageMemory,
            &m_lMemoryId);
        if( m_Ret != IS_SUCCESS )
        {
            AfxMessageBox(L"ERROR: cannot allocate buffer memory for single freeze" , MB_ICONEXCLAMATION, 0 );
            is_ExitCamera( m_hCam );
            PostQuitMessage( 0 );
        }

        // set active buffer for next capture
        is_SetImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );

        // camera exposure
        double newEXP = 0.0, dExposure = 0.0;
        is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE,(void*)&dExposure, sizeof(dExposure));

        // calculate timeout
        is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE,(void*)&newEXP, sizeof(newEXP));
        m_nTimeout = (int) (4* newEXP) ; // add factor of safety

        // setup trigger mode
        m_nTriggerMode = IS_SET_TRIGGER_SOFTWARE ;
        is_SetExternalTrigger( m_hCam, m_nTriggerMode );

        // create an event to get signaled when a frame was acquired
        m_hEv = CreateEvent(NULL, FALSE, FALSE, NULL);
        if(m_hEv != NULL)
        {
            m_hThread = (HANDLE)_beginthreadex(NULL, 0, threadProc, (void*)this, 0, (UINT*)&m_dwThreadID);
            if(m_hThread == NULL)
            {

                CloseHandle(m_hEv);
                m_hEv = NULL;
                AfxMessageBox(L"ERROR: Cannot create event!" , MB_ICONEXCLAMATION, 0 );
            }
            else
            {
                // init the frame event for uEye
                is_InitEvent(m_hCam, m_hEv, IS_SET_EVENT_FRAME);
                is_EnableEvent(m_hCam, IS_SET_EVENT_FRAME);
                m_boRunThread = true;
            }
        }

        // enable some useful uEye messages
        is_EnableMessage( m_hCam, IS_CAPTURE_STATUS, this->GetSafeHwnd() );
        is_EnableMessage( m_hCam, IS_DEVICE_REMOVED, this->GetSafeHwnd() );
        is_EnableMessage( m_hCam, IS_DEVICE_RECONNECTED, this->GetSafeHwnd() );

    }
    else
    {
        AfxMessageBox(L"ERROR: Cannot open uEye camera!" , MB_ICONEXCLAMATION, 0 );
        PostQuitMessage( 0 );
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnBnClickedButtonLoadParameter()
//
// DESCRIPTION: - loads parameters from an ini file
//              - reallocates the memory
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnBnClickedButtonLoadParameter()
{ 
    if ( m_hCam == 0 )
    {
        OpenCamera();
    }

    if ( m_hCam != 0 )
    {
        if( is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS )
        {
            // realloc image mem with actual sizes and depth.
            is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );

            IS_SIZE_2D imageSize;
            is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

            INT nAllocSizeX = 0;
            INT nAllocSizeY = 0;

            m_nSizeX = nAllocSizeX = imageSize.s32Width;
            m_nSizeY = nAllocSizeY = imageSize.s32Height;

            UINT nAbsPosX = 0;
            UINT nAbsPosY = 0;

            // absolute pos?
            is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_X_ABS, (void*)&nAbsPosX , sizeof(nAbsPosX));
            is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_Y_ABS, (void*)&nAbsPosY , sizeof(nAbsPosY));

            if (nAbsPosX)
            {
                nAllocSizeX = m_SensorInfo.nMaxWidth;
            }
            if (nAbsPosY)
            {
                nAllocSizeY = m_SensorInfo.nMaxHeight;
            }

            switch( is_SetColorMode( m_hCam, IS_GET_COLOR_MODE ) )
            {
            case IS_CM_RGBA12_UNPACKED:
            case IS_CM_BGRA12_UNPACKED:
                m_nBitsPerPixel = 64;
                break;

            case IS_CM_RGB12_UNPACKED:
            case IS_CM_BGR12_UNPACKED:
            case IS_CM_RGB10_UNPACKED:
            case IS_CM_BGR10_UNPACKED:
                m_nBitsPerPixel = 48;
                break;

            case IS_CM_RGBA8_PACKED:
            case IS_CM_BGRA8_PACKED:
            case IS_CM_RGB10_PACKED:
            case IS_CM_BGR10_PACKED:
            case IS_CM_RGBY8_PACKED:
            case IS_CM_BGRY8_PACKED:
                m_nBitsPerPixel = 32;
                break;

            case IS_CM_RGB8_PACKED:
            case IS_CM_BGR8_PACKED:
                m_nBitsPerPixel = 24;
                break;

            case IS_CM_BGR565_PACKED:
            case IS_CM_UYVY_PACKED:
            case IS_CM_CBYCRY_PACKED:
                m_nBitsPerPixel = 16;
                break;

            case IS_CM_BGR5_PACKED:
                m_nBitsPerPixel = 15;
                break;

            case IS_CM_MONO16:
            case IS_CM_SENSOR_RAW16:
            case IS_CM_MONO12:
            case IS_CM_SENSOR_RAW12:
            case IS_CM_MONO10:
            case IS_CM_SENSOR_RAW10:
                m_nBitsPerPixel = 16;
                break;

            case IS_CM_RGB8_PLANAR:
                m_nBitsPerPixel = 24;
                break;

            case IS_CM_MONO8:
            case IS_CM_SENSOR_RAW8:
            default:
                m_nBitsPerPixel = 8;
                break;
            }

            // memory initialization
            is_AllocImageMem( m_hCam, nAllocSizeX, nAllocSizeY, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId);

            // set memory active
            is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId );

            // display initialization
            imageSize.s32Width = m_nSizeX;
            imageSize.s32Height = m_nSizeY;

            is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
        }
    }
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnButtonExit()
//
// DESCRIPTION: - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnButtonExit()
{
    ExitCamera();

    // terminate
    PostQuitMessage( 0 );
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD threadProc(void* pv)
//
// DESCRIPTION: Thread for event handling
//
///////////////////////////////////////////////////////////////////////////////
unsigned WINAPI threadProc(void* pv)
{
    CUEyeFlashStrobeDlg* p = (CUEyeFlashStrobeDlg*)pv;

    p->ThreadProc();

    _endthreadex(0);

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeTriggerDlg::ThreadProc()
//
// DESCRIPTION: - Event handler for uEye the frame event
//              - controls the double buffer
//              - image display
//              - timeout handling
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::ThreadProc()
{
    DWORD lEv;

    // wait to get an event signal
    do
    {    
        if( m_RunAcq )
        {
            lEv = WaitForSingleObject(m_hEv,m_nTimeout);
            ResetEvent( m_hEv );

            switch( lEv )
            {
            case WAIT_OBJECT_0:
                // continue grabbing ?
                if( m_ContAcq==true )
                {
                    // Wait until the camera is ready to capture again
                    BOOL bFinish = FALSE;
                    DWORD dwStart = GetTickCount();
                    while ( !bFinish )
                    {
                        is_IsVideoFinish( m_hCam, &bFinish );
                        if( (GetTickCount()-dwStart)>1000 )
                        {
                            // timeout!!!
                            is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP);
                            break;
                        }
                    }

                    // capture!!
                    is_FreezeVideo( m_hCam, IS_DONT_WAIT );
                }
                if( m_ContAcq==false )
                {
                    // stop acquisition
                    OnButtonCaptureStop();
                }
                // ... then do some processing

                // for example display the image...
                is_RenderBitmap( m_hCam, m_lMemoryId, m_hWndDisplay, IS_RENDER_FIT_TO_WINDOW );
                break;

            case WAIT_TIMEOUT:
                // cancel acquisition
                is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP);

                // continue grabbing ?
                if(  m_ContAcq==true )
                {
                    is_FreezeVideo( m_hCam, IS_DONT_WAIT );
                }
                if( m_ContAcq==false )
                {
                    // stop acquisition
                    OnButtonCaptureStop();
                }
                break;

            default:
                // unknown error => terminate

                // cancel acquisition
                OnButtonCaptureStop();
            } //end switch
        }
        else
        {
            // Prevent from running at high cpu-load
            Sleep( 100 );
        } //end if( m_RunAcq )
    }
    while(m_boRunThread);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnUEyeMessage
//
// DESCRIPTION: handles the uEye messages
//
///////////////////////////////////////////////////////////////////////////////
LRESULT CUEyeFlashStrobeDlg::OnUEyeMessage( WPARAM wParam, LPARAM lParam )
{

    switch ( wParam )
    {
    case IS_FRAME:
        break;

    case IS_SEQUENCE:
        break;

    case IS_TRIGGER:
        break;

    case IS_CAPTURE_STATUS:
        m_nCntFail++;
        SetDlgItemInt( IDC_EDIT_TRANSFER_FAIL,  m_nCntFail );
        break;

    case IS_DEVICE_REMOVED:
        Beep(5000,100);
        AfxMessageBox(L"ERROR: Camera disconnect!" , MB_ICONEXCLAMATION, 0 );
        break;

    case IS_DEVICE_RECONNECTED:
        Beep(100,100);
        AfxMessageBox(L"INFO: Camera reconnect!" , MB_ICONINFORMATION , 0 );
        break;

    case IS_NEW_DEVICE:
        break;

    case IS_DEVICE_REMOVAL:
        break;

    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnButtonCapture()
//
// DESCRIPTION: lauch the image capture
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnButtonCapture()
{
    // prevent the capture button to be pressed twice
    GetDlgItem( IDC_BUTTON_CAPTURE)->EnableWindow(FALSE);

    // run capture
    m_RunAcq = true; // enable processing in the frame thread
    is_FreezeVideo( m_hCam, IS_WAIT );

    // GUI
    if( m_ContAcq == true )
    {
        GetDlgItem( IDC_BUTTON_CAPTURE_STOP)->EnableWindow(TRUE);
        GetDlgItem( IDC_BUTTON_LOAD_PARAMETER)->EnableWindow(FALSE);
    }
    m_nCntFail = 0;
    SetDlgItemInt( IDC_EDIT_TRANSFER_FAIL,  m_nCntFail );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnButtonCaptureStop()
//
// DESCRIPTION: stop capture in continuous mode
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnButtonCaptureStop()
{
    // stop capture
    m_RunAcq = false;
    is_StopLiveVideo( m_hCam , IS_FORCE_VIDEO_STOP);

    // GUI
    GetDlgItem( IDC_BUTTON_CAPTURE_STOP)->EnableWindow(FALSE);
    GetDlgItem( IDC_BUTTON_CAPTURE)->EnableWindow(TRUE);
    GetDlgItem( IDC_BUTTON_LOAD_PARAMETER)->EnableWindow(TRUE);
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnCheckCont()
//
// DESCRIPTION: stop capture in continuous mode
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnCheckCont()
{
    if( m_ContAcq == false )
    {
        m_ContAcq = true;
    }
    else
    {
        m_ContAcq = false;
        OnButtonCaptureStop();
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnRadioFlashOff()
//
// DESCRIPTION: disable the Flash/Strobe
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnRadioFlashOff()
{
    UINT nMode = IO_FLASH_MODE_OFF;
    is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));

    GetDlgItem(IDC_BUTTON_GLOBAL_FLASH)->EnableWindow( FALSE );
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnRadioFlashActLo()
//
// DESCRIPTION: enable the Flash/Strobe - low active
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnRadioFlashActLo()
{
    UINT nMode = IO_FLASH_MODE_TRIGGER_LO_ACTIVE;
    is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));

    // you cant simulate a global shutter on a global shutter ;)
    GetDlgItem(IDC_BUTTON_GLOBAL_FLASH)->EnableWindow( !m_SensorInfo.bGlobShutter );
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnRadioFlashActHi()
//
// DESCRIPTION: enable the Flash/Strobe - high active
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnRadioFlashActHi()
{
    UINT nMode = IO_FLASH_MODE_TRIGGER_HI_ACTIVE;
    is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));

    // you cant simulate a global shutter on a global shutter ;)
    GetDlgItem(IDC_BUTTON_GLOBAL_FLASH)->EnableWindow( !m_SensorInfo.bGlobShutter );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnRadioFlashDoutLo()
//
// DESCRIPTION: use Flash/Strobe as static output - set to high
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnRadioFlashDoutLo()
{
    UINT nMode = IO_FLASH_MODE_CONSTANT_LOW;
    is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));

    GetDlgItem(IDC_BUTTON_GLOBAL_FLASH)->EnableWindow( FALSE );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnRadioFlashDoutHi()
//
// DESCRIPTION: use Flash/Strobe as static output - set to low
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnRadioFlashDoutHi()
{
    UINT nMode = IO_FLASH_MODE_CONSTANT_HIGH;
    is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&nMode, sizeof(nMode));

    GetDlgItem(IDC_BUTTON_GLOBAL_FLASH)->EnableWindow( FALSE );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::OnBnClickedButtonGlobalFlash()
//
// DESCRIPTION: sets the delay and duration for flash output to become global
//              shutter like behavior
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::OnBnClickedButtonGlobalFlash()
{
    IO_FLASH_PARAMS flashParams;
    if (is_IO(m_hCam, IS_IO_CMD_FLASH_GET_GLOBAL_PARAMS, (void*)&flashParams, sizeof(flashParams)) == IS_SUCCESS)
    {
        is_IO(m_hCam, IS_IO_CMD_FLASH_SET_PARAMS, (void*)&flashParams, sizeof(flashParams));
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeFlashStrobeDlg::ExitCamera()
//
// DESCRIPTION: cleans up and exits the camera
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeFlashStrobeDlg::ExitCamera()
{
    // wait until thread terminated
    m_boRunThread = false;
    SetEvent(m_hEv);
    if (WaitForSingleObject (m_hThread, 2000) != WAIT_OBJECT_0)
    {
        TerminateThread (m_hThread, 0);
    }

    // close thread handle
    CloseHandle (m_hThread);

    CloseHandle(m_hEv);
    m_hEv = NULL;
    m_hThread = NULL;


    // stop the acquisition
    OnButtonCaptureStop();

    // memory and events are automatically released
    is_ExitCamera( m_hCam );
}


void CUEyeFlashStrobeDlg::OnClose()
{
    ExitCamera();

    CDialog::OnClose();
}


INT CUEyeFlashStrobeDlg::InitCamera (HIDS *hCam, HWND hWnd)
{
    INT nRet = is_InitCamera (hCam, hWnd);
    /************************************************************************************************/
    /*                                                                                              */
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*                                                                                              */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */
    /*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
    /*                                                                                              */
    /************************************************************************************************/
    if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
    {
        // Time for the firmware upload = 25 seconds by default
        INT nUploadTime = 25000;
        is_GetDuration (*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);

        CString Str1, Str2, Str3;
        Str1 = "This camera requires a new firmware. The upload will take about";
        Str2 = "seconds. Please wait ...";
        Str3.Format (L"%s %d %s", Str1, nUploadTime / 1000, Str2);
        AfxMessageBox (Str3, MB_ICONWARNING);

        // Set mouse to hourglass
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS) (((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
        nRet = is_InitCamera (hCam, hWnd);
    }

    return nRet;
}


void CUEyeFlashStrobeDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
    // Check if the camera supports an arbitrary AOI
    // Only the ueye xs does not support an arbitrary AOI
    INT nAOISupported = 0;
    BOOL bAOISupported = TRUE;
    if (is_ImageFormat(m_hCam,
        IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
        (void*)&nAOISupported,
        sizeof(nAOISupported)) == IS_SUCCESS)
    {
        bAOISupported = (nAOISupported != 0);
    }

    if (bAOISupported)
    {  
        // All other sensors
        // Get maximum image size
        SENSORINFO sInfo;
        is_GetSensorInfo (m_hCam, &sInfo);
        *pnSizeX = sInfo.nMaxWidth;
        *pnSizeY = sInfo.nMaxHeight;
    }
    else
    {
        // Only ueye xs
        // Get image size of the current format
        IS_SIZE_2D imageSize;
        is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

        *pnSizeX = imageSize.s32Width;
        *pnSizeY = imageSize.s32Height;
    }
}


/*
EOF
*/

