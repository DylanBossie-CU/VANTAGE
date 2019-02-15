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

// uEyeEventDlg.cpp : implementation file

#include "stdafx.h"
#include "uEyeEvent.h"
#include "uEyeEventDlg.h"
#include <process.h>
#include ".\ueyeeventdlg.h"
#include "version.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned WINAPI threadProc(void* pv);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

    virtual BOOL OnInitDialog();

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
    ON_WM_TIMER()
    ON_WM_MOVE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CuEyeEventDlg dialog
CuEyeEventDlg::CuEyeEventDlg(CWnd* pParent /*=NULL*/)
: CDialog(CuEyeEventDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CuEyeEventDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_DISP_DIB, m_nDispModeSel);
}


BEGIN_MESSAGE_MAP(CuEyeEventDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_EXIT, OnExit)
    ON_BN_CLICKED(IDC_BUTTON_RESET_EVENTS, OnButtonResetEvents)
    ON_BN_CLICKED(IDC_BUTTON_FREEZE, OnButtonFreeze)
    ON_BN_CLICKED(IDC_BUTTON_START_LIVE, OnButtonStartLive)
    ON_BN_CLICKED(IDC_BUTTON_STOP_LIVE, OnButtonStopLive)
    ON_BN_CLICKED(IDC_BUTTON_SEQ_START, OnButtonSeqStart)
    ON_BN_CLICKED(IDC_BUTTON_SEQ_STOP, OnButtonSeqStop)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_RADIO_TRG_OFF, OnRadioTrgOff)
    ON_BN_CLICKED(IDC_RADIO_TRG_HW, OnRadioTrgHw)
    ON_BN_CLICKED(IDC_RADIO_TRG_SW, OnRadioTrgSw)
    ON_EN_CHANGE(IDC_EDIT_TIMEOUT, OnChangeEditTimeout)
    ON_WM_MOVE()
    ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAMETER, OnBnClickedButtonLoadParameter)
    ON_BN_CLICKED(IDC_DISP_DIB, OnBnClickedDispModeChange)
    ON_BN_CLICKED(IDC_DISP_DD, OnBnClickedDispModeChange)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CuEyeEventDlg message handlers
BOOL CuEyeEventDlg::OnInitDialog()
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
    // when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE); // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // Display
    m_hWndDisplay = GetDlgItem( IDC_DISPLAY )->GetSafeHwnd(); // set display window handle

    // Init variables
    m_nSizeX = 640; // set video width  to fit into display window
    m_nSizeY = 480; // set video height to fit into display window

    m_hCam = 0; // open next available camera
    m_boRunThread = FALSE; // no event handling active
    m_BeepFreq = 3000; // std beep freq.
    m_nDispModeSel = e_disp_mode_bitmap;
    UpdateData(FALSE);

    m_hThread =  NULL;
    m_lMemoryId = 0;
    m_pcImageMemory = NULL;
    for( int i = 0; i < SEQ_BUFFERS; i++ )
    {
        m_lSeqMemId[i] = 0;
        m_pcSeqImgMem[i] = NULL;
    }

    // GUI
    SetDlgItemInt (IDC_EDIT_TIMEOUT, 5000);

    // open camera and run event handling
    OpenCamera(); // open a camera handle
    EvInitAll(); // enable all events and install the event handler

    // trigger
    OnRadioTrgOff();
    ((CButton*)GetDlgItem(IDC_RADIO_TRG_OFF))->SetCheck(BST_CHECKED);
    TriggerGuiEnable (true);

    // beep and uncheck timer
    m_nTimer = (UINT)SetTimer (1, 250, 0);

    m_bDoInit = FALSE;

    return TRUE; // return TRUE  unless you set the focus to a control
}



void CuEyeEventDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CuEyeEventDlg::OnPaint()
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
HCURSOR CuEyeEventDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


void CuEyeEventDlg::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);
    if(x<0)
    {
        Invalidate( true );
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CSimpleEventDlg::OpenCamera()
//
// DESCRIPTION: Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::OpenCamera()
{
    if ( m_hCam != 0 )
    {
        is_ExitCamera( m_hCam );
    }

    // init camera
    m_hCam = (HIDS) 0;  // open next camera
    m_Ret = InitCamera( &m_hCam, m_hWndDisplay );
    if( m_Ret == IS_SUCCESS )
    {
        // enable the dialog based error report
        //m_Ret = is_SetErrorReport(m_hCam, IS_ENABLE_ERR_REP); // or IS_DISABLE_ERR_REP);
        //if( m_Ret != IS_SUCCESS )
        //{
        //    AfxMessageBox( L"ERROR: Can not enable the automatic uEye error report!" , MB_ICONEXCLAMATION, 0 );
        //    return false;
        //}

        // Retrieve sensor info
        is_GetSensorInfo (m_hCam, &m_sInfo);

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        if( m_sInfo.nColorMode == IS_COLORMODE_BAYER )
        {
            // for color camera models use RGB24 mode
            m_nColorMode = IS_CM_BGR8_PACKED;
            m_nBitsPerPixel = 24;
        }
        else if (m_sInfo.nColorMode == IS_COLORMODE_CBYCRY)
        {
            // for CBYCRY camera models use RGB32 mode
            m_nColorMode = IS_CM_BGRA8_PACKED;
            m_nBitsPerPixel = 32;
        }
        else
        {
            // for monochrome camera models use Y8 mode
            m_nColorMode = IS_CM_MONO8;
            m_nBitsPerPixel = 8;
        }

        // display initialization
        InitDisplayMode(m_nDispModeSel);
    }
    else
    {
        AfxMessageBox( L"ERROR: Cannot open uEye camera!" , MB_ICONEXCLAMATION, 0 );
        PostQuitMessage( 0 );
    }

    // set exposure = 1/framerate
    double dExposure = 0.0;
    is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE,(void*)&dExposure, sizeof(dExposure));

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeEventDlg::OnBnClickedButtonLoadParameter()
//
// DESCRIPTION: - loads parameters from an ini file
//              - reallocates the memory
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnBnClickedButtonLoadParameter()
{
    if ( m_hCam == 0 )
    {
        OpenCamera();
    }

    if ( m_hCam != 0 )
    {
        if( is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS && m_pcImageMemory != NULL )
        {
            m_bDoInit = TRUE;

            // stop image acquisition
            is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);

            // refresh global parameters to have actual size and colormode
            IS_SIZE_2D imageSize;
            is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

            m_nSizeX = imageSize.s32Width;
            m_nSizeY = imageSize.s32Height;

            m_nColorMode = is_SetColorMode( m_hCam, IS_GET_COLOR_MODE );
            switch( m_nColorMode )
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
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeEventDlg::OnButtonExit()
//
// DESCRIPTION: - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnExit()
{
    // delete Timer
    KillTimer (m_nTimer );

    // terminate event handling
    EvTerminateAll();

    // uEye
    is_StopLiveVideo( m_hCam, IS_WAIT );

    // memory and events are automatically released
    is_ExitCamera( m_hCam );

    // terminate
    PostQuitMessage( 0 );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CSimpleEventDlg::ThreadProc()
//
// DESCRIPTION: Event handler for uEye events
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::ThreadProc()
{
    DWORD lReturn;
    int nEvIndex;
    DWORD static Time1,Time2;

    // run the event handler
    do
    {
        // wait to get signaled
        lReturn = WaitForMultipleObjects(m_EvMax, m_hEv, FALSE ,INFINITE );
        if(m_boRunThread)
        {
            nEvIndex = lReturn - WAIT_OBJECT_0;

            // get time stamp
            m_nEvChecked[nEvIndex] = ::GetTickCount();

            // update counter and GUI
            m_nEvCount[nEvIndex] ++;
            SetDlgItemInt( IDC_EDIT_EV_0 + nEvIndex , m_nEvCount[nEvIndex] );

            CheckDlgButton( IDC_RADIO_EVENT_0 + nEvIndex, BST_CHECKED );

            // display the sequence buffer or in bitmap live mode
            if( m_nEvUI[nEvIndex] == IS_SET_EVENT_FRAME)
            {
                // in sequence mode
                if (!GetDlgItem(IDC_BUTTON_SEQ_START)->IsWindowEnabled()
                    & !GetDlgItem(IDC_BUTTON_START_LIVE)->IsWindowEnabled()
                    & GetDlgItem(IDC_BUTTON_SEQ_STOP)->IsWindowEnabled())
                {
                    // find the latest image buffer and display the buffer
                    INT nNum = 0;
                    char *pcMem = NULL;
                    char *pcMemLast = NULL;

                    INT nRet = is_GetActSeqBuf(m_hCam, &nNum, &pcMem, &pcMemLast);

                    if ((IS_SUCCESS == nRet) && (NULL != pcMemLast))
                    {
                        int j;
                        for (j = 0; j < SEQ_BUFFERS; j++)
                        {
                            if (pcMemLast == m_pcSeqImgMem[j])
                            {
                                break;
                            }
                        }

                        nRet = is_LockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, m_pcSeqImgMem[j]);

                        if (IS_SUCCESS == nRet)
                        {
                            is_RenderBitmap(m_hCam, m_lSeqMemId[j], m_hWndDisplay, IS_RENDER_FIT_TO_WINDOW);
                            is_UnlockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, m_pcSeqImgMem[j]);
                        }
                    }
                }
                else if (m_nDispModeSel == e_disp_mode_bitmap)
                {
                    INT nRet = is_LockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, m_pcImageMemory);

                    if (IS_SUCCESS == nRet)
                    {
                        is_RenderBitmap(m_hCam, m_lMemoryId, m_hWndDisplay, IS_RENDER_FIT_TO_WINDOW);
                        is_UnlockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, m_pcImageMemory);
                    }
                }
            }
        }
    }
    while(m_boRunThread);
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
    CuEyeEventDlg* p = (CuEyeEventDlg*)pv;

    p->ThreadProc();

    _endthreadex(0);

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::EvEnumerate
//
// DESCRIPTION: Put all available events into an array
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::EvEnumerate()
{
    m_nEvUI[0] = IS_SET_EVENT_FRAME;
    m_nEvUI[1] = IS_SET_EVENT_EXTTRIG;
    m_nEvUI[2] = IS_SET_EVENT_VSYNC;
    m_nEvUI[3] = IS_SET_EVENT_SEQ;
    m_nEvUI[4] = IS_SET_EVENT_CAPTURE_STATUS;
    m_nEvUI[5] = IS_SET_EVENT_DEVICE_RECONNECTED;
    m_nEvUI[6] = IS_SET_EVENT_MEMORY_MODE_FINISH;
    m_nEvUI[7] = IS_SET_EVENT_REMOVE;
    m_nEvUI[8] = IS_SET_EVENT_REMOVAL;
    m_nEvUI[9] = IS_SET_EVENT_NEW_DEVICE;
    m_nEvUI[10] = IS_SET_EVENT_CONNECTIONSPEED_CHANGED;

    m_EvMax = 11;

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      EvGetIndex
//
// DESCRIPTION: Get the index of an event according to EvEnumerate()
//
// PARAMTER:    nEvent:   the event for the index query
//              pnIndex:  return value is the pointer to the event index
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::EvGetIndex(INT nEvent, int *pnIndex)
{
    // search for index
    for( int i=0 ; i< m_EvMax ; i++)
    {
        if( m_nEvUI[i] == nEvent )
        {
            *pnIndex = i;
            return true;
        }
    }

    // found no match
    AfxMessageBox( L"ERROR in EvGetIndex()" , MB_ICONEXCLAMATION, 0 );
    return false;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::EvInit
//
// DESCRIPTION: Initialize an event
//
// PARAMTER:    nEvent: event to be initialized 
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::EvInit(INT nEvent)
{
    int   nIndex;
    bool  bRet;

    // get event index
    bRet = EvGetIndex( nEvent, &nIndex);
    if( bRet == false )
    {
        AfxMessageBox( L"ERROR in EvInit() index" , MB_ICONEXCLAMATION, 0 );
        return false;
    }

    // init event
    m_Ret = is_InitEvent( m_hCam, m_hEv[nIndex], nEvent );
    if( m_Ret != IS_SUCCESS )
    {
        return false;
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::EvEnable
//
// DESCRIPTION: Enable an event
//
// PARAMTER:    nEvent:   event to be enabled/disabled
//              bEnable:  event state to set: true/false
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::EvEnable(INT nEvent, bool bEnable)
{
    if( bEnable == true )
    {
        // enable event
        m_Ret = is_EnableEvent( m_hCam, nEvent );
        if( m_Ret != IS_SUCCESS )
        {
            return false;
        }
    }
    else
    {
        // enable event
        m_Ret = is_DisableEvent( m_hCam, nEvent );
        if( m_Ret != IS_SUCCESS )
        {
            return false;
        }
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::EvInitAll
//
// DESCRIPTION: Initialize and anable all events.
//              Setup the event handler routine.
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::EvInitAll()
{
    CString str;
    bool bRet;

    // enumerate all events
    EvEnumerate();

    // init and run events
    for( int i=0 ; i< m_EvMax ; i++)
    {
        // create event
        m_hEv[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        if(m_hEv == NULL)
        {
            str.Format(L"ERROR in EvInitAll() - CreateEvent #%d",i);
            AfxMessageBox( str , MB_ICONEXCLAMATION, 0 );
            return false;
        }

        // init event
        bRet = EvInit( m_nEvUI[i] );
        if(bRet == false)
        {
            str.Format(L"ERROR in EvInitAll() - EvInit #%d",i);
            AfxMessageBox( str , MB_ICONEXCLAMATION, 0 );
            return false;
        }

        // enable event
        bRet = EvEnable( m_nEvUI[i], true );
        if(bRet == false)
        {
            str.Format(L"ERROR in EvInitAll() - EvEnable #%d",i);
            AfxMessageBox( str , MB_ICONEXCLAMATION, 0 );
            return false;
        }

        // reset counter
        m_nEvCount[i]=0;
    }

    // create thread for events
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, threadProc, (void*)this, 0, (UINT*)&m_dwThreadID);
    if(m_hThread == NULL)
    {
        AfxMessageBox( L"ERROR: Cannot create event tread!" , MB_ICONEXCLAMATION, 0 );
        return false;
    }

    // we really shouldn't do that !!!!
    SetThreadPriority (m_hThread, THREAD_PRIORITY_TIME_CRITICAL);

    // event handler thread must now be active
    m_boRunThread = TRUE;

    // GUI
    OnButtonResetEvents();

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::EvTerminateAll
//
// DESCRIPTION: Terminate and kill all events and finally the also the
//              eventhandler routine.
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::EvTerminateAll()
{
    // stop events in handler routine
    m_boRunThread = false;

    // launch an event to leave the thread loop
    SetEvent(m_hEv[m_EvMax-1]);
    if (WaitForSingleObject (m_hThread, 1000) != WAIT_OBJECT_0)
    {
        // finally terminate thread if it does not by itself
        TerminateThread (m_hThread, 0);
    }

    CloseHandle (m_hThread);
    m_hThread = NULL;

    // terminate and close events
    for( int i=0 ; i< m_EvMax ; i++)
    {
        CloseHandle(m_hEv[i]);
        m_hEv[i] = NULL;
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnButtonResetEvents()
//
// DESCRIPTION: Reset event counterns and GUI signals
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnButtonResetEvents()
{
    for( int i=0 ; i< m_EvMax ; i++)
    {
        // rester counter
        m_nEvCount[i]=0;

        // update GUI
        CheckDlgButton( IDC_RADIO_EVENT_0 + i, BST_UNCHECKED );
        SetDlgItemInt( IDC_EDIT_EV_0 + i , m_nEvCount[i] );
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnButtonFreeze()
//
// DESCRIPTION: Setup camera into single shot mode
//              Perform a singe grab and do the display
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnButtonFreeze()
{
    // We need a new display initialization (parameters have been loaded)
    if(m_bDoInit)
    {
        m_bDoInit = FALSE;
        InitDisplayMode(m_nDispModeSel);
    }

    is_FreezeVideo( m_hCam, m_nTimeout );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnButtonStartLive()
//
// DESCRIPTION: Setup camera into live mode
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnButtonStartLive()
{
    UpdateData(TRUE);
    if(InitDisplayMode(m_nDispModeSel) == IS_SUCCESS)
    {
        // disable trigger for live display
        TriggerGuiEnable( false );

        ResetEvGuiStaes();

        // GUI
        GetDlgItem(IDC_BUTTON_START_LIVE)->EnableWindow( FALSE );
        GetDlgItem(IDC_BUTTON_STOP_LIVE)->EnableWindow( TRUE );
        GetDlgItem(IDC_BUTTON_FREEZE)->EnableWindow( FALSE );
        GetDlgItem(IDC_BUTTON_SEQ_START)->EnableWindow( FALSE );
        GetDlgItem(IDC_BUTTON_SEQ_STOP)->EnableWindow( FALSE );
        GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow( FALSE );
        GetDlgItem(IDC_DISP_DIB)->EnableWindow( FALSE );
        GetDlgItem(IDC_DISP_DD)->EnableWindow( FALSE );

        is_CaptureVideo( m_hCam, IS_WAIT );
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnButtonStopLive()
//
// DESCRIPTION: Stop live display
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnButtonStopLive()
{
    // uEye
    is_StopLiveVideo( m_hCam, IS_WAIT );

    // GUI
    GetDlgItem(IDC_BUTTON_START_LIVE)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_STOP_LIVE)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_FREEZE)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_SEQ_START)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_SEQ_STOP)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow( TRUE );
    GetDlgItem(IDC_DISP_DIB)->EnableWindow( TRUE );
    GetDlgItem(IDC_DISP_DD)->EnableWindow( TRUE );

    // enable trigger for live display
    TriggerGuiEnable( true );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::SeqBuilt()
//
// DESCRIPTION: Built a sequence for acquisition into the sequence buffer
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::SeqBuilt()
{
    int i;

    SeqKill();

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
        nAllocSizeX = m_sInfo.nMaxWidth;
    }
    if (nAbsPosY)
    {
        nAllocSizeY = m_sInfo.nMaxHeight;
    }

    // alloc seq buffer
    for( i=0; i< SEQ_BUFFERS ; i++ )
    {
        // alloc memory
        m_Ret = is_AllocImageMem( m_hCam,
            nAllocSizeX,
            nAllocSizeY,
            m_nBitsPerPixel,
            &m_pcSeqImgMem[i],
            &m_lSeqMemId[i]);
        if( m_Ret != IS_SUCCESS )
        {
            AfxMessageBox( L"ERROR: cannot allocate sufficient sequence buffer memory" , MB_ICONEXCLAMATION, 0 );
            is_ExitCamera( m_hCam );
            PostQuitMessage( 0 );
        }
    }
    for( i=0; i< SEQ_BUFFERS ; i++ )
    {
        // put memory into seq buffer
        m_Ret = is_AddToSequence(	m_hCam, m_pcSeqImgMem[i], m_lSeqMemId[i]);
        if( m_Ret != IS_SUCCESS )
        {
            AfxMessageBox( L"ERROR: cannot put buffer into sequence" , MB_ICONEXCLAMATION, 0 );
            return false;
        }
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::SeqKill()
//
// DESCRIPTION: Release the sequence for acquisition into the sequence buffer
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeEventDlg::SeqKill()
{
    is_ClearSequence( m_hCam );

    for( int i=0; i< SEQ_BUFFERS ; i++ )
    {
        if( (m_pcSeqImgMem[i] != NULL) && (m_lSeqMemId[i] != 0) )
        {
            do 
            {
                // free memory
                m_Ret = is_FreeImageMem(m_hCam,
                                        m_pcSeqImgMem[i],
                                        m_lSeqMemId[i]);

                if (IS_SEQ_BUFFER_IS_LOCKED == m_Ret)
                {
                    ::Sleep(1);
                    continue;
                }

                break;
            }
            while (true);

            m_pcSeqImgMem[i] = NULL;
            m_lSeqMemId[i] = 0;
        }
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnButtonSeqStart()
//
// DESCRIPTION: Setup camera into sequence mode
//              Perform acquistion to sequence buffer
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnButtonSeqStart()
{
    // disable trigger for live display
    TriggerGuiEnable( false );

    ResetEvGuiStaes();

    // for sequence mode we MUST set to bitmap mode
    InitDisplayMode(e_disp_mode_bitmap);

    // uEye
    SeqBuilt();

    is_CaptureVideo( m_hCam, IS_WAIT );

    // GUI
    GetDlgItem(IDC_BUTTON_START_LIVE)->EnableWindow( FALSE );
    GetDlgItem(IDC_BUTTON_STOP_LIVE)->EnableWindow( FALSE );
    GetDlgItem(IDC_BUTTON_FREEZE)->EnableWindow( FALSE );
    GetDlgItem(IDC_BUTTON_SEQ_START)->EnableWindow( FALSE );
    GetDlgItem(IDC_BUTTON_SEQ_STOP)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow( FALSE );
    GetDlgItem(IDC_DISP_DIB)->EnableWindow( FALSE );
    GetDlgItem(IDC_DISP_DD)->EnableWindow( FALSE );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnButtonSeqStop()
//
// DESCRIPTION: Stop acquistion to sequence buffer and release sequence
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnButtonSeqStop()
{
    // uEye
    is_StopLiveVideo( m_hCam, IS_WAIT );
    SeqKill();

    // GUI
    GetDlgItem(IDC_BUTTON_START_LIVE)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_STOP_LIVE)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_FREEZE)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_SEQ_START)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_SEQ_STOP)->EnableWindow( TRUE );
    GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow( TRUE );
    GetDlgItem(IDC_DISP_DIB)->EnableWindow( TRUE );
    GetDlgItem(IDC_DISP_DD)->EnableWindow( TRUE );

    // enbale trigger for live display
    TriggerGuiEnable( true );

    // restore to selected display mode
    InitDisplayMode(m_nDispModeSel);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnTimer(UINT nIDEvent)
//
// DESCRIPTION:
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnTimer(UINT_PTR nIDEvent)
{
    CDialog::OnTimer(nIDEvent);

    int time = ::GetTickCount();
    for( int i=0 ; i< m_EvMax ; i++)
    {
        // uncheck signals only when the time difference is large enough
        if((m_nEvChecked[i] != 0) && (time - m_nEvChecked[i]) >= 50 )
        {
            CheckDlgButton( IDC_RADIO_EVENT_0 + i, BST_UNCHECKED );
            m_nEvChecked[i] = 0;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnRadioTrgOff()
//
// DESCRIPTION: Turn trigger off
//              Timeout is IS_WAIT
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnRadioTrgOff()
{
    m_nTimeout = IS_WAIT;  // default for untriggered acquisition
    m_Ret = is_SetExternalTrigger( m_hCam, IS_SET_TRIGGER_OFF );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnRadioTrgHw()
//
// DESCRIPTION: Turn trigger hardware trigger in
//              Timeout is read from GUI
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnRadioTrgHw()
{
    m_nTimeout = (GetDlgItemInt( IDC_EDIT_TIMEOUT ))/10;
    m_Ret = is_SetTimeout( m_hCam, IS_TRIGGER_TIMEOUT, m_nTimeout );
    m_Ret = is_SetExternalTrigger( m_hCam, IS_SET_TRIGGER_HI_LO);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnRadioTrgSw()
//
// DESCRIPTION: Turn trigger software trigger in
//              Timeout is read from GUI
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnRadioTrgSw()
{
    m_nTimeout = (GetDlgItemInt( IDC_EDIT_TIMEOUT ))/10;
    m_Ret = is_SetTimeout( m_hCam, IS_TRIGGER_TIMEOUT, m_nTimeout );
    m_Ret = is_SetExternalTrigger( m_hCam, IS_SET_TRIGGER_SOFTWARE);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::OnChangeEditTimeout()
//
// DESCRIPTION: Update timeout variable
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::OnChangeEditTimeout()
{
    m_nTimeout = (GetDlgItemInt( IDC_EDIT_TIMEOUT ))/10;
    if (m_hCam)
    {
        m_Ret = is_SetTimeout( m_hCam, IS_TRIGGER_TIMEOUT, m_nTimeout );
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CuEyeEventDlg::TriggerGuiEnable()
//
// DESCRIPTION: Enable/disable the trigger for camera and GUI
//
// PARAMTER:    bEnable:   when true -> enabled
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeEventDlg::TriggerGuiEnable(bool bEnable)
{   
    // GUI
    GetDlgItem(IDC_RADIO_TRG_OFF)->EnableWindow(bEnable);
    GetDlgItem(IDC_RADIO_TRG_HW)->EnableWindow(bEnable);
    GetDlgItem(IDC_RADIO_TRG_SW)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_TIMEOUT)->EnableWindow(bEnable);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleLiveDlg::InitDisplayMode()
//
// DESCRIPTION: - initializes the display mode
//
///////////////////////////////////////////////////////////////////////////////
int CuEyeEventDlg::InitDisplayMode(int nModeSel)
{
    if(m_hCam == NULL)
    {
        return IS_NO_SUCCESS;
    }

    if( m_pcImageMemory != NULL )
    {  
        is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );
    }
    m_pcImageMemory = NULL;

    switch(nModeSel)
    {
    case e_disp_mode_direct3d:

        // if initializing the Direct3D mode succeeded then
        // leave the switch tree else run through to bitmap mode
        m_Ret = is_SetDisplayMode (m_hCam, IS_SET_DM_DIRECT3D);
        if(m_Ret == IS_SUCCESS )
        {
            is_DirectRenderer (m_hCam, DR_ENABLE_SCALING, NULL, NULL);

            // setup the color depth to the current VGA setting
            is_GetColorDepth( m_hCam, &m_nBitsPerPixel, &m_nColorMode );
            break;
        }
        else
        {
            m_nDispModeSel = e_disp_mode_bitmap;
            UpdateData(FALSE);
        }

    case e_disp_mode_bitmap:
        {
            m_Ret = is_SetDisplayMode( m_hCam, IS_SET_DM_DIB);

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
                nAllocSizeX = m_sInfo.nMaxWidth;
            }
            if (nAbsPosY)
            {
                nAllocSizeY = m_sInfo.nMaxHeight;
            }
            // allocate an image memory.
            if (is_AllocImageMem(m_hCam,
                nAllocSizeX,
                nAllocSizeY,
                m_nBitsPerPixel,
                &m_pcImageMemory,
                &m_lMemoryId ) != IS_SUCCESS )
            {
                AfxMessageBox(L"Memory allocation failed!", MB_ICONWARNING );
            }
            else
            {
                is_SetImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );
                m_nDispModeSel = e_disp_mode_bitmap;
                UpdateData (FALSE);
            }
        }
        break;
    }

    if(m_Ret == IS_SUCCESS )
    {
        // set the desired color mode
        is_SetColorMode( m_hCam, m_nColorMode );

        // set the image size to capture
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
    }

    return m_Ret;
}


void CuEyeEventDlg::OnBnClickedDispModeChange()
{
    UpdateData(TRUE);
    if (InitDisplayMode (m_nDispModeSel) == IS_SUCCESS)
    {
        RedrawWindow();
    }
}


void CuEyeEventDlg::ResetEvGuiStaes()
{
    for( int i=0 ; i< m_EvMax ; i++)
    {
        m_nEvChecked[i] = 0;
    }
}


void CuEyeEventDlg::OnClose()
{
    // delete Timer
    KillTimer (m_nTimer );

    // terminate event handling
    EvTerminateAll();

    // uEye
    is_StopLiveVideo( m_hCam, IS_WAIT );

    // memory and events are automatically released
    is_ExitCamera( m_hCam );

    CDialog::OnClose();
}


INT CuEyeEventDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CuEyeEventDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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
