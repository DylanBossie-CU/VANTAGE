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

// uEyeSequenceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uEyeSequence.h"
#include "uEyeSequenceDlg.h"
#include <process.h>
#include "version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


unsigned WINAPI threadProcEvent(void* pv);


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
// CUEyeSequenceDlg dialog

CUEyeSequenceDlg::CUEyeSequenceDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUEyeSequenceDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CUEyeSequenceDlg)
    m_nBuffersInUse = 10;
    m_nBuffersNew = 10;
    m_strIdsWeb = _T("www.uEye.com");
    m_nCntCapturedBuffers = 0;
    m_nCntCapturedSeq = 0;
    m_nCntTransferErrors = 0;
    m_nBufferIndex = 0;
    m_nBufferStart = -1;
    m_nBufferStop = -1;
    m_nBufferReplay = -1;
    m_dwSingleBufferSize = 0;
    m_dwTotalBufferSize = 0;
    m_dwSysMemTotal = 0;
    m_strCamFps = _T("");
    m_strCamType = _T("");
    m_strCamSN = _T("");
    m_strCamHwRev = _T("");
    m_strCamDrvVer = _T("");
    m_strCamSensorColor = _T("");
    m_nCamX = 0;
    m_nCamY = 0;
    m_nCamID = 0;
    m_nCamPclk = 0;
    m_strCurrentFps = _T("");
    m_bLoopDisplay = FALSE;
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUEyeSequenceDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUEyeSequenceDlg)
    DDX_Control(pDX, IDC_EDIT_BUFFER_SIZE_SYSMEM_TOTAL, m_ctrlSysMemTotal);
    DDX_Control(pDX, IDC_BUTTON_LOAD_PARAMETERS, m_ctrlButtonLoadParam);
    DDX_Control(pDX, IDC_BUTTON_REPLAY_STOP, m_ctrlButtonReplayStop);
    DDX_Control(pDX, IDC_BUTTON_REPLAY, m_ctrlButtonReplay);
    DDX_Control(pDX, IDC_SPIN_REPLAY, m_ctrlSpinReplay);
    DDX_Control(pDX, IDC_SLIDER_REPLAY, m_ctrlSliderReplay);
    DDX_Control(pDX, IDC_PROGRESS_REPLAY, m_ctrlProgressReplay);
    DDX_Control(pDX, IDC_EDIT_BUFFER_REPLAY, m_ctrlBufferReplay);
    DDX_Control(pDX, IDC_PROGRESS_RECORD, m_ctrlProgressRecord);
    DDX_Control(pDX, IDC_EDIT_BUF_INDEX, m_ctrlBufferIndex);
    DDX_Control(pDX, IDC_EDIT_CURRENT_FPS, m_ctrlCurrentFps);
    DDX_Control(pDX, IDC_EDIT_CAM_FPS, m_ctrlCamFps);
    DDX_Control(pDX, IDC_BUTTON_CAPTURE_STOP, m_ctrlButtonCaptureStop);
    DDX_Control(pDX, IDC_BUTTON_CAPTURE, m_ctrlButtonCapture);
    DDX_Control(pDX, IDC_EDIT_BUFFER_SIZE_TOTAL, m_ctrlTotalBufferSize);
    DDX_Control(pDX, IDC_EDIT_BUFFERS_IN_USE, m_ctrlBuffersInUse);
    DDX_Control(pDX, IDC_EDIT_BUFFER_SIZE_SYSMEM_FREE, m_ctrlSysMemFree);
    DDX_Control(pDX, IDC_EDIT_BUFFERS_STATUS, m_ctrlBufferStatus);
    DDX_Control(pDX, IDC_BUTTON_REALLOCTE_BUFFERS, m_ctrlButtonReallocBuf);
    DDX_Control(pDX, IDC_PROGRESS_MEM_ALLOC, m_ctrlProgressMemAlloc);
    DDX_Control(pDX, IDC_EDIT_CNT_BUFFERS_NEW, m_ctrlBuffersNew);
    DDX_Text(pDX, IDC_EDIT_BUFFERS_IN_USE, m_nBuffersInUse);
    DDX_Text(pDX, IDC_EDIT_CNT_BUFFERS_NEW, m_nBuffersNew);
    DDV_MinMaxUInt(pDX, m_nBuffersNew, MIN_SEQ_BUFFERS, MAX_SEQ_BUFFERS);
    DDX_Text(pDX, IDC_EDIT_IDS_WEB, m_strIdsWeb);
    DDX_Text(pDX, IDC_EDIT_CNT_BUF, m_nCntCapturedBuffers);
    DDX_Text(pDX, IDC_EDIT_CNT_SEQ, m_nCntCapturedSeq);
    DDX_Text(pDX, IDC_EDIT_CNT_TRANS_ERR, m_nCntTransferErrors);
    DDX_Text(pDX, IDC_EDIT_BUF_INDEX, m_nBufferIndex);
    DDX_Text(pDX, IDC_EDIT_BUFFER_START, m_nBufferStart);
    DDX_Text(pDX, IDC_EDIT_BUFFER_STOP, m_nBufferStop);
    DDX_Text(pDX, IDC_EDIT_BUFFER_REPLAY, m_nBufferReplay);
    DDX_Text(pDX, IDC_EDIT_BUFFER_SIZE_SINGLE, m_dwSingleBufferSize);
    DDX_Text(pDX, IDC_EDIT_BUFFER_SIZE_TOTAL, m_dwTotalBufferSize);
    DDX_Text(pDX, IDC_EDIT_BUFFER_SIZE_SYSMEM_TOTAL, m_dwSysMemTotal);
    DDX_Text(pDX, IDC_EDIT_CAM_FPS, m_strCamFps);
    DDX_Text(pDX, IDC_EDIT_CAM_TYPE, m_strCamType);
    DDX_Text(pDX, IDC_EDIT_CAM_SN, m_strCamSN);
    DDX_Text(pDX, IDC_EDIT_CAM_REV, m_strCamHwRev);
    DDX_Text(pDX, IDC_EDIT_CAM_DRV, m_strCamDrvVer);
    DDX_Text(pDX, IDC_EDIT_CAM_COL, m_strCamSensorColor);
    DDX_Text(pDX, IDC_EDIT_CAM_X, m_nCamX);
    DDX_Text(pDX, IDC_EDIT_CAM_Y, m_nCamY);
    DDX_Text(pDX, IDC_EDIT_CAM_ID, m_nCamID);
    DDX_Text(pDX, IDC_EDIT_CAM_PCLK, m_nCamPclk);
    DDX_Text(pDX, IDC_EDIT_CURRENT_FPS, m_strCurrentFps);
    DDX_Check(pDX, IDC_CHECK_REPLAY_LOOP, m_bLoopDisplay);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUEyeSequenceDlg, CDialog)
    //{{AFX_MSG_MAP(CUEyeSequenceDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_BN_CLICKED(IDC_RADIO_BUFFER_RING, OnRadioBufferRing)
    ON_BN_CLICKED(IDC_RADIO_BUFFER_LIN, OnRadioBufferLin)
    ON_BN_CLICKED(IDC_BUTTON_CAPTURE_STOP, OnButtonCaptureStop)
    ON_BN_CLICKED(IDC_BUTTON_CAPTURE, OnButtonCapture)
    ON_BN_CLICKED(IDC_BUTTON_REALLOCTE_BUFFERS, OnButtonReallocteBuffers)
    ON_BN_CLICKED(IDC_BUTTON_REPLAY, OnButtonReplay)
    ON_WM_CLOSE()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_REPLAY_STOP, OnButtonReplayStop)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_REPLAY, OnDeltaposSpinReplay)
    ON_WM_VSCROLL()
    ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAMETERS, OnButtonLoadParameters)
    ON_BN_CLICKED(IDC_RADIO_CM_Y8, OnRadioCmY8)
    ON_BN_CLICKED(IDC_RADIO_CM_RGB24, OnRadioCmRgb24)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUEyeSequenceDlg message handlers

BOOL CUEyeSequenceDlg::OnInitDialog()
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

    SetIcon(m_hIcon, TRUE); // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // TODO: Add extra initialization here


    // init variables
    m_hCam = 0;             // open next available camera
    m_boRunThread = FALSE;  // no event handling active
    m_SeqMode = SEQ_RING;   // ring buffer mode

    // open camera and run event handling
    CamOpen(); // open a camera handle


    // GUI
    CheckDlgButton( IDC_RADIO_BUFFER_RING, BST_CHECKED );
    CheckDlgButton( IDC_CHECK_SHOW_BUFFERS, BST_CHECKED );

    return TRUE; // return TRUE  unless you set the focus to a control
}


void CUEyeSequenceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUEyeSequenceDlg::OnPaint()
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
HCURSOR CUEyeSequenceDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CUEyeSequenceDlg::OnClose()
{
    OnButtonExit() ;
    if (CanExit())
        CDialog::OnClose();
}

void CUEyeSequenceDlg::OnOK()
{
    this->UpdateData( TRUE );
}

void CUEyeSequenceDlg::OnCancel()
{
    // nop
}

BOOL CUEyeSequenceDlg::CanExit()
{
    // If the proxy object is still around, then the automation
    //  controller is still holding on to this application. Leave
    //  the dialog around, but hide its UI.
    if (m_pAutoProxy != NULL)
    {
        ShowWindow(SW_HIDE);
        return FALSE;
    }

    return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeSequenceDlg::OpenCamera()
//
// DESCRIPTION: Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::CamOpen()
{
    if ( m_hCam != 0 )
        is_ExitCamera( m_hCam );

    // init camera
    m_hCam = (HIDS) 0;  // open next camera
    m_Ret = InitCamera( &m_hCam, m_hWndDisplay );
    if( m_Ret == IS_SUCCESS )
    {
        // Get sensor info
        is_GetSensorInfo(m_hCam, &m_sInfo);

        // enable the dialog based error report
        //m_Ret = is_SetErrorReport(m_hCam, IS_ENABLE_ERR_REP); // or IS_DISABLE_ERR_REP);
        //if( m_Ret != IS_SUCCESS )
        //{
        //    AfxMessageBox(L"ERROR: Can not enable the automatic uEye error report!" , MB_ICONEXCLAMATION, 0 );
        //    return false;
        //}

        // query camera information
        SENSORINFO SensorInfo;
        is_GetSensorInfo(m_hCam, &SensorInfo );


        CAMINFO CamInfo;
        is_GetCameraInfo(m_hCam, &CamInfo );

        // setup image size
        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

        // use color depth according to monochrome or color camera
        if( SensorInfo.nColorMode == IS_COLORMODE_MONOCHROME )
        {
            // monochrome camera
            m_nBitsPerPixel =  8;
            is_SetColorMode(m_hCam, IS_CM_MONO8 );
            CheckDlgButton( IDC_RADIO_CM_Y8, BST_CHECKED );
        }
        else
        {
            // color camera
            m_nBitsPerPixel =  24;
            is_SetColorMode( m_hCam, IS_CM_BGR8_PACKED );
            CheckDlgButton( IDC_RADIO_CM_RGB24, BST_CHECKED );

            double dblVal = 1.0;

            UEYE_AUTO_INFO autoInfo;
            if (is_GetAutoInfo(m_hCam, &autoInfo) == IS_SUCCESS)
            {
                // Sensor white balance is supported
                if (autoInfo.AutoAbility & AC_SENSOR_WB)
                {
                    is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &dblVal, NULL);
                }
                // Sensor white balance is not supported
                else
                {
                    if (autoInfo.AutoAbility & AC_WHITEBAL)
                    {
                        // Try to activate software white balance
                        is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dblVal, NULL);
                    }
                }
            }
        }


        // calculate single buffer size
        m_dwSingleBufferSize = m_nSizeX * m_nSizeY * m_nBitsPerPixel / 8;
        UpdateData( FALSE );

        // display initialization
        m_hWndDisplay = GetDlgItem( IDC_DISPLAY )->GetSafeHwnd(); // get display window handle
        is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);

        // get handles to replay windows
        for( int i=0; i<NUM_REPLAY_WINDOWS ; i++)
        {
            m_hWndDispReplay[i] = GetDlgItem( IDC_BUF_0+i )->GetSafeHwnd(); // get replay display window handle
        }

        // query the system RAM size
        MEMORYSTATUS stat;
        GlobalMemoryStatus (&stat);
        m_dwSysMemTotal = (DWORD)stat.dwTotalPhys;
        m_ctrlSysMemTotal.RedrawWindow();

        // allocate memory and built the sequence
        SeqBuilt();

        // enable all events and install the event handler
        EvInitAll();

        // set exposure = 1/framerate
        double dExposure = 0.0;
        is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE,(void*)&dExposure, sizeof(dExposure));

        // output camera information
        m_strCamType = SensorInfo.strSensorName;
        m_nCamID = CamInfo.Select;
        m_strCamHwRev = CamInfo.Version;
        m_strCamSN = CamInfo.SerNo;
        m_nCamX = m_nSizeX;
        m_nCamY = m_nSizeY;

        // Get current pixel clock
        is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_GET, (void*)&m_nCamPclk, sizeof(m_nCamPclk));

        // get framerate
        double newFps;
        is_SetFrameRate(m_hCam, IS_GET_FRAMERATE, &newFps );
        m_strCamFps.Format(L"%2.1f", newFps );

        // get the camera's sensor color mode
        switch( SensorInfo.nColorMode )
        {
        case IS_COLORMODE_MONOCHROME:
            m_strCamSensorColor = _T("Monochrome");
            break;

        case IS_COLORMODE_BAYER:
            m_strCamSensorColor = _T("Bayer (color)");
            break;

        case IS_COLORMODE_INVALID:
            m_strCamSensorColor = _T("Invalid");
            break;

        default:
            m_strCamSensorColor = _T("unknown");
        }


        // get driver version
        // bits 31-24: major version number
        // bits 16-23: minor version number
        // bits 15-0: build version number 
        int nDllVer = is_GetDLLVersion();
        char strDllVer[32];
        sprintf( strDllVer, "%d.%02d.%04d", ((nDllVer>>24)&0xff), ((nDllVer>>16)&0xff),(nDllVer&0xff) );
        m_strCamDrvVer = strDllVer;

        // setup replay spin control
        UDACCEL Accel;
        Accel.nSec=1;
        Accel.nInc =1;
        m_ctrlSpinReplay.SetAccel( 1, &Accel );

        // update GUI
        m_ctrlButtonCapture.EnableWindow( TRUE );
        m_ctrlButtonLoadParam.EnableWindow( TRUE );
        GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( TRUE );
        GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( TRUE );
        UpdateData( FALSE );
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
// METHOD CUEyeSequenceDlg::OnButtonExit()
//
// DESCRIPTION: - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnButtonExit()
{
    // stop capture
    OnButtonCaptureStop();

    // terminate event handling
    EvTerminateAll();

    // release memory and the sequence
    SeqKill();

    // memory and events are automatically released
    is_ExitCamera( m_hCam );

    // terminate
    PostQuitMessage( 0 );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnButtonReallocteBuffers()
//
// DESCRIPTION: Kill previous sequence and free image buffers.
//              Allocate new buffers for the sequence according the number of
//              desired buffers in the GUI
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnButtonReallocteBuffers() 
{
    // prevent the OnButtonReallocteBuffers() button to be pressed twice => disable
    m_ctrlButtonReallocBuf.EnableWindow( FALSE );
    m_ctrlButtonCapture.EnableWindow( FALSE );
    m_ctrlButtonCaptureStop.EnableWindow( FALSE );

    // GUI
    m_ctrlButtonReplay.EnableWindow( FALSE );
    m_ctrlButtonReplayStop.EnableWindow( FALSE );
    m_ctrlSliderReplay.EnableWindow( FALSE );
    m_ctrlSpinReplay.EnableWindow( FALSE );
    m_ctrlButtonLoadParam.EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( FALSE );


    // rebuilt new sequence based of the number of buffers in the GUI
    if( UpdateData( TRUE)==0 )
    {
        m_nBuffersNew = MIN_SEQ_BUFFERS ;
        UpdateData( FALSE );
    }

    SeqKill();  // delete currently used sequence
    SeqBuilt(); // built new sequence

    // prevent the OnButtonReallocteBuffers() button to be pressed twice => enable
    m_ctrlButtonReallocBuf.EnableWindow( TRUE );

    // GUI
    m_ctrlButtonCapture.EnableWindow( TRUE );
    m_ctrlButtonLoadParam.EnableWindow( TRUE );
    GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( TRUE );
    GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( TRUE );
    DisplayReplayBuffer();
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::SeqBuilt()
//
// DESCRIPTION: Built a sequence for acquisition into the sequence buffer
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::SeqBuilt()
{
    // variables
    unsigned int i,imax;

    // GUI - what are going to do
    m_ctrlBufferStatus.SetWindowText(L"Allocate Buffers and built sequence");
    m_ctrlBufferStatus.RedrawWindow();


    // read m_BuffersInUse
    imax=m_nBuffersNew;

    // setup the progress bar
    m_ctrlProgressMemAlloc.SetRange(0,imax-1);
    m_ctrlProgressMemAlloc.SetStep( 1 );
    m_ctrlProgressMemAlloc.SetPos(0);

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

    // calculate single buffer size
    m_dwSingleBufferSize = nAllocSizeX * nAllocSizeY * m_nBitsPerPixel / 8;

    // alloc seq buffers in a loop
    for( i=0; i< imax  ; i++ )
    {

        // allocate buffer memory
        m_Ret = is_AllocImageMem(m_hCam,
            nAllocSizeX,
            nAllocSizeY,
            m_nBitsPerPixel,
            &m_pcSeqImgMem[i],
            &m_lSeqMemId[i]);
        if( m_Ret != IS_SUCCESS )
        {
            break;  // it makes no sense to continue
        }

        // put memory into seq buffer
        m_Ret = is_AddToSequence(	m_hCam, m_pcSeqImgMem[i], m_lSeqMemId[i] );
        m_nSeqNumId[i] = i+1; // store sequence buffer number Id
        if( m_Ret != IS_SUCCESS )
        {
            // free latest buffer
            is_FreeImageMem( m_hCam, m_pcSeqImgMem[i], m_lSeqMemId[i] );
            break; // it makes no sense to continue
        }

        // update GUI
        SeqShowStateInGUI(i+1, imax, true );
    }

    // store the current buffers parameters and update GUI
    m_nBuffersInUse = i;
    m_nBufferStart = 0;
    m_nBufferStop = m_nBuffersInUse -1;
    m_nBufferReplay = -1;
    m_ctrlProgressReplay.SetRange(0,m_nBuffersInUse -1);
    m_ctrlProgressReplay.SetStep( 1 );
    m_ctrlProgressReplay.SetPos(0);
    m_ctrlSliderReplay.SetRange(-((int)m_nBuffersInUse -1),0,true);
    m_ctrlSliderReplay.SetPos(0);
    m_nBufferReplay = 0;
    UpdateData( FALSE );


    // GUI - what we did
    m_ctrlBufferStatus.SetWindowText(L"Buffers are ready to use");
    m_ctrlBufferStatus.RedrawWindow();

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::SeqKill()
//
// DESCRIPTION: Release the sequence and memory of the image buffers
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::SeqKill()
{
    // GUI - tell the user what are we going to do
    m_ctrlBufferStatus.SetWindowText(L"Free Buffers and kill sequence");
    m_ctrlBufferStatus.RedrawWindow();

    // remove the buffers from the sequence
    INT nRet = IS_SUCCESS;

    is_ClearSequence(m_hCam);

    // free seq buffers
    int i;
    for( i=(m_nBuffersInUse-1); i>=0   ; i-- )
    {   
        // free buffers
        do
        {
            nRet = is_FreeImageMem(m_hCam, m_pcSeqImgMem[i], m_lSeqMemId[i]);

            if (IS_SEQ_BUFFER_IS_LOCKED == nRet)
            {
                ::Sleep(1);
                continue;
            }

            break;
        }
        while (true);

        // update GUI
        SeqShowStateInGUI(i, m_nBuffersInUse, false );
    }

    // store the number of buffers
    m_nBuffersInUse = i;
    UpdateData( FALSE );

    // GUI - what we did
    m_ctrlBufferStatus.SetWindowText(L"Buffers cleaned up");
    m_ctrlBufferStatus.RedrawWindow();

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::SeqShowStateInGUI
//
// DESCRIPTION: Show the current state of the sequence built or kill state
//              in the GUI.
//              Update buffer counter, progress bar and available system
//              memory.
//
// PARAMTER:    nBufferIndex:   latest successfully handled buffer
//              nBuffersMax:    maximum number of buffers
//                              bUp:  SeqBulit = true
//                              bUp:  SeqKill  = false
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::SeqShowStateInGUI(unsigned int nBufferIndex, unsigned int nBuffersMax, bool bUp )
{
    // variables
    MEMORYSTATUS stat;
    CString str;
    static DWORD dwOldAvailPhys;

    // query the system RAM size
    GlobalMemoryStatus (&stat);

    // warning - Windows starts swapping
    if( bUp==true )
    {
        if( nBufferIndex <= 1 )
        {
            dwOldAvailPhys = (DWORD)stat.dwTotalPhys;
        }

        if( stat.dwAvailPhys > dwOldAvailPhys )
        {
            m_ctrlBufferStatus.SetWindowText(L"WARNING: Memory Swapping");
        }

        dwOldAvailPhys = (DWORD)stat.dwAvailPhys;
    }

    // for a high amount of buffers only display the progress of every 10th buffer.
    if( (nBuffersMax<=100) | ((nBuffersMax>100) & (nBufferIndex%10==0)) | (nBufferIndex==nBuffersMax))
    {
        // set progress bar
        m_ctrlProgressMemAlloc.SetPos( nBufferIndex );

        // display the system RAM size
        str.Format(L"%ld",stat.dwAvailPhys);
        m_ctrlSysMemFree.SetWindowText( str );
        m_ctrlSysMemFree.RedrawWindow();

        // warning - Windows starts swapping
        if( bUp==true )
        {
            m_ctrlBufferStatus.RedrawWindow();
        }

        // buffer count
        str.Format(L"%d",nBufferIndex);
        m_ctrlBuffersInUse.SetWindowText( str );
        m_ctrlBuffersInUse.RedrawWindow();

        // buffer size
        m_dwTotalBufferSize = m_dwSingleBufferSize * nBufferIndex ;
        str.Format(L"%lu",m_dwTotalBufferSize);
        m_ctrlTotalBufferSize.SetWindowText( str );
        m_ctrlTotalBufferSize.RedrawWindow();
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::EvInitAll
//
// DESCRIPTION: Initialize and enable all events.
//              Setup the event handler routine.
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::EvInitAll()
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

        // rest counter
        m_nEvCount[i]=0;
    }

    // create threads 
    m_hThreadEvent = (HANDLE)_beginthreadex(NULL, 0, threadProcEvent, (void*)this, 0, (UINT*)&m_dwThreadIDEvent);
    if(m_hThreadEvent == NULL)
    {
        AfxMessageBox(L"ERROR: Cannot create event tread!" , MB_ICONEXCLAMATION, 0 );
        return false;
    }

    // event handler thread must now be active
    m_boRunThread = TRUE;

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::EvEnumerate
//
// DESCRIPTION: Put all available events into an array
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::EvEnumerate()
{
    m_nEvUI[0] = IS_SET_EVENT_FRAME;
    m_nEvUI[1] = IS_SET_EVENT_SEQ;
    m_nEvUI[2] = IS_SET_EVENT_CAPTURE_STATUS;
    m_nEvUI[3] = IS_SET_EVENT_DEVICE_RECONNECTED;
    m_nEvUI[4] = IS_SET_EVENT_REMOVE;

    m_EvMax = 5;

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::EvGetIndex
//
// DESCRIPTION: Get the index of an event according to EvEnumerate()
//
// PARAMTER:    nEvent:   the event for the index query
//              pnIndex:  return value is the pointer to the event index
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::EvGetIndex(INT nEvent, int *pnIndex)
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
    AfxMessageBox(L"ERROR in EvGetIndex()" , MB_ICONEXCLAMATION, 0 );
    return false;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::EvInit
//
// DESCRIPTION: Initialize an event
//
// PARAMTER:    nEvent: event to be initialized 
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::EvInit(INT nEvent)
{
    int   nIndex;
    bool  bRet;

    // get event index
    bRet = EvGetIndex( nEvent, &nIndex);
    if( bRet == false )
    {
        AfxMessageBox(L"ERROR in EvInit() index" , MB_ICONEXCLAMATION, 0 );
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
// METHOD:      CUEyeSequenceDlg::EvEnable
//
// DESCRIPTION: Enable an event
//
// PARAMTER:    nEvent:   event to be enabled/disabled
//              bEnable:  event state to set: true/false
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::EvEnable(INT nEvent, bool bEnable)
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
// METHOD:      CUEyeSequenceDlg::EvTerminateAll
//
// DESCRIPTION: Terminate and kill all events and finally the also the
//              event handler routine.
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeSequenceDlg::EvTerminateAll()
{
    // stop events in handler routine
    m_boRunThread = false;

    // terminate and close events
    for( int i=0 ; i< m_EvMax ; i++)
    {
        // only handle valid events
        if( m_hEv[i] !=NULL )
        {
            // launch, catch and close event
            SetEvent(m_hEv[i]);
            if (WaitForSingleObject (m_hThreadEvent, 100) != WAIT_OBJECT_0)
            {
                // too bad - however we continue
            }

            CloseHandle(m_hEv[i]);
            m_hEv[i] = NULL;
        }
    }

    // finally terminate thread
    if(WaitForSingleObject(m_hThreadEvent, 1000) == WAIT_TIMEOUT)
    {
        TerminateThread (m_hThreadEvent, 0);
    }

    CloseHandle (m_hThreadEvent);
    m_hThreadEvent = NULL;

    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD threadProc(void* pv)
//
// DESCRIPTION: Init threads for event handling and buffer save
//
///////////////////////////////////////////////////////////////////////////////
unsigned WINAPI threadProcEvent(void* pv)
{
    CUEyeSequenceDlg* p = (CUEyeSequenceDlg*)pv;

    p->ThreadProcEvent();

    _endthreadex(0);

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeSequenceDlg::ThreadProc()
//
// DESCRIPTION: Event handler for uEye events
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::ThreadProcEvent()
{
    DWORD lReturn;
    int nEvIndex;
    unsigned int i;

    // run the event handler
    do
    {
        // wait to get signaled
        lReturn = WaitForMultipleObjects(m_EvMax,m_hEv, FALSE ,INFINITE );
        if(m_boRunThread)
        {
            nEvIndex = lReturn - WAIT_OBJECT_0;

            // TRACE( "ThreadProcEvent()");
            switch( m_nEvUI[nEvIndex] )
            {
            case IS_SET_EVENT_FRAME:
                // increment buffer counter
                m_nCntCapturedBuffers++;

                // in linear buffer mode we must stop when all buffers are filled
                if( (m_SeqMode == SEQ_LIN) &  (m_nCntCapturedBuffers == m_nBuffersInUse) )
                {
                    m_nBufferReplay = 0;
                    is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP );

                    // GUI
                    m_ctrlButtonCapture.EnableWindow( TRUE );
                    m_ctrlButtonCaptureStop.EnableWindow( FALSE );
                    m_ctrlButtonReallocBuf.EnableWindow( TRUE );
                    m_ctrlButtonReplay.EnableWindow( TRUE );
                    m_ctrlButtonReplayStop.EnableWindow( FALSE );
                    m_ctrlSliderReplay.EnableWindow( TRUE );
                    m_ctrlSpinReplay.EnableWindow( TRUE );
                    m_ctrlButtonLoadParam.EnableWindow( TRUE );
                    GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( TRUE );
                    GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( TRUE );
                    GetDlgItem( IDC_RADIO_BUFFER_RING )->EnableWindow( TRUE );
                    GetDlgItem( IDC_RADIO_BUFFER_LIN )->EnableWindow( TRUE );
                    Beep(7000,100);
                }

                // find the latest image buffer
                INT nNum;
                char *pcMem, *pcMemLast;
                is_GetActSeqBuf(m_hCam, &nNum, &pcMem, &pcMemLast);
                for( i=0 ; i<m_nBuffersInUse ; i++)
                {
                    if( pcMemLast == m_pcSeqImgMem[i] )
                        break;
                }

                // lock buffer for processing
                m_Ret = is_LockSeqBuf( m_hCam,m_nSeqNumId[i], m_pcSeqImgMem[i] );

                if (IS_SUCCESS == m_Ret)
                {
                    // start processing

                    // display buffer
                    is_RenderBitmap(m_hCam, m_lSeqMemId[i], m_hWndDisplay, IS_RENDER_FIT_TO_WINDOW);

                    // processing completed

                    // unlock buffer
                    is_UnlockSeqBuf(m_hCam, m_nSeqNumId[i], m_pcSeqImgMem[i]);
                }

                // update GUI
                m_ctrlProgressRecord.SetPos(i);
                SetDlgItemInt( IDC_EDIT_BUF_INDEX, i );
                SetDlgItemInt( IDC_EDIT_CNT_BUF, m_nCntCapturedBuffers );

                // framerate
                double newFps;
                is_GetFramesPerSecond(m_hCam, &newFps );
                m_strCurrentFps.Format(L"%2.1f", newFps );
                SetDlgItemText(IDC_EDIT_CURRENT_FPS, m_strCurrentFps);
                break;

            case IS_SET_EVENT_SEQ:
                // all buffers of the have been filled
                SetDlgItemInt( IDC_EDIT_CNT_SEQ, m_nCntCapturedSeq++ );
                break;

            case IS_SET_EVENT_CAPTURE_STATUS:
                // we lost a buffer
                SetDlgItemInt( IDC_EDIT_CNT_TRANS_ERR, m_nCntTransferErrors++ );
                break;

            case IS_SET_EVENT_DEVICE_RECONNECTED:
                // ok - we can continue!
                AfxMessageBox(L"uEye is reconnected!" , MB_ICONINFORMATION , 0 );
                break;

            case IS_SET_EVENT_REMOVE:
                // that is too bad!
                AfxMessageBox(L"CRITICAL ERROR: uEye is disconnected!" , MB_ICONEXCLAMATION, 0 );
                break;

            default:;
            }
        }
    }
    while(m_boRunThread);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::OnRadioBufferRing()
//
// DESCRIPTION: use sequence capture in ring buffer mode
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnRadioBufferRing()
{
    m_SeqMode = SEQ_RING; // ring buffer mode
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::OnRadioBufferRing()
//
// DESCRIPTION: use sequence capture in linear buffer mode
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnRadioBufferLin()
{
    m_SeqMode = SEQ_LIN;  // linear buffer mode
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::OnButtonCaptureStop() 
//
// DESCRIPTION: stop capture
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnButtonCaptureStop()
{
    // GUI
    m_ctrlButtonCapture.EnableWindow( TRUE );
    m_ctrlButtonCaptureStop.EnableWindow( FALSE );
    m_ctrlButtonReallocBuf.EnableWindow( TRUE );

    // stop the image acquisition
    is_StopLiveVideo( m_hCam, IS_WAIT );

    // GUI again
    m_strCurrentFps = _T("");
    m_nBufferReplay = 0;
    m_ctrlButtonReplay.EnableWindow( TRUE );
    m_ctrlButtonReplayStop.EnableWindow( FALSE );
    m_ctrlSliderReplay.EnableWindow( TRUE );
    m_ctrlSpinReplay.EnableWindow( TRUE );
    m_ctrlButtonLoadParam.EnableWindow( TRUE );
    GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( TRUE );
    GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( TRUE );
    GetDlgItem( IDC_RADIO_BUFFER_RING )->EnableWindow( TRUE );
    GetDlgItem( IDC_RADIO_BUFFER_LIN )->EnableWindow( TRUE );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::OnButtonCapture()
//
// DESCRIPTION: start capture
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnButtonCapture()
{
    // GUI
    m_ctrlButtonCapture.EnableWindow( FALSE );
    m_ctrlButtonCaptureStop.EnableWindow( TRUE );
    m_ctrlButtonReallocBuf.EnableWindow( FALSE );

    // setup the progress bar
    m_ctrlProgressRecord.SetRange(0,m_nBuffersInUse-1 );
    m_ctrlProgressRecord.SetStep( 1 );
    m_ctrlProgressRecord.SetPos(0);

    // GUI again
    m_strCurrentFps = _T("");
    m_nCntCapturedBuffers = 0;
    m_nCntCapturedSeq = 0 ;
    m_nCntTransferErrors = 0;
    m_nBufferReplay = -1;
    m_ctrlButtonReplay.EnableWindow( FALSE );
    m_ctrlButtonReplayStop.EnableWindow( FALSE );
    m_ctrlSliderReplay.EnableWindow( FALSE );
    m_ctrlSpinReplay.EnableWindow( FALSE );
    m_ctrlButtonLoadParam.EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO_BUFFER_RING )->EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO_BUFFER_LIN )->EnableWindow( FALSE );

    // run the image acquisition
    is_CaptureVideo( m_hCam, IS_DONT_WAIT);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnButtonReplay()
//
// DESCRIPTION: replay the captured buffers
//              The image buffers are displayed in a timer routine
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnButtonReplay() 
{
    // variables
    UINT nElapse; // timer interval

    // calculate timer interval and run timer
    double newFps;
    is_SetFrameRate(m_hCam, IS_GET_FRAMERATE, &newFps );
    nElapse = UINT ((1/newFps)*1000);
    m_nTimer = 1;
    SetTimer( m_nTimer, nElapse, NULL );

    // GUI
    m_ctrlButtonCapture.EnableWindow( FALSE );
    m_ctrlButtonReplay.EnableWindow( FALSE );
    m_ctrlButtonReplayStop.EnableWindow( TRUE );
    m_ctrlSliderReplay.EnableWindow( FALSE );
    m_ctrlSpinReplay.EnableWindow( FALSE );
    m_ctrlButtonLoadParam.EnableWindow( FALSE );
    m_ctrlButtonReallocBuf.EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( FALSE );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnButtonReplayStop()
//
// DESCRIPTION: Stop the replay immediately.
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnButtonReplayStop()
{
    KillTimer( m_nTimer );

    // GUI
    m_ctrlButtonCapture.EnableWindow( TRUE );
    m_ctrlButtonReplay.EnableWindow( TRUE );
    m_ctrlButtonReplayStop.EnableWindow( FALSE );
    m_ctrlSliderReplay.EnableWindow( TRUE );
    m_ctrlSpinReplay.EnableWindow( TRUE );
    m_ctrlButtonLoadParam.EnableWindow( TRUE );
    m_ctrlButtonReallocBuf.EnableWindow( TRUE );
    GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( TRUE );
    GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( TRUE );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnTimer(UINT nIDEvent)
//
// DESCRIPTION: Do the display in replay mode in real time
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnTimer(UINT_PTR nIDEvent)
{
    CDialog::OnTimer(nIDEvent);

    // variables
    static unsigned int nDisplayBufferCounter = 0 ; // lock buffer counter

    // only hook an our display timer
    if( nIDEvent == m_nTimer)
    {
        // display!
        DisplayReplayBuffer();

        // did the buffer change in the GUI?
        if( nDisplayBufferCounter!= (unsigned int) m_nBufferReplay )
            nDisplayBufferCounter = m_nBufferReplay;

        // increment local buffer counter
        nDisplayBufferCounter++;

        // now kill the timer and stop the replay?
        if( nDisplayBufferCounter >= m_nBuffersInUse )
        {
            // only single loop?
            UpdateData( TRUE );
            if( m_bLoopDisplay==false )
            {
                KillTimer( m_nTimer );

                // GUI
                m_nBufferReplay = m_nBuffersInUse-1;
                m_ctrlButtonCapture.EnableWindow( TRUE );
                m_ctrlButtonReplay.EnableWindow( TRUE );
                m_ctrlButtonReplayStop.EnableWindow( FALSE );
                m_ctrlSliderReplay.EnableWindow( TRUE );
                m_ctrlSpinReplay.EnableWindow( TRUE );
                m_ctrlButtonLoadParam.EnableWindow( TRUE );
                m_ctrlButtonReallocBuf.EnableWindow( TRUE );
                GetDlgItem( IDC_RADIO_CM_Y8 )->EnableWindow( TRUE );
                GetDlgItem( IDC_RADIO_CM_RGB24 )->EnableWindow( TRUE );
            }
            // reset counters
            nDisplayBufferCounter = 0;
            m_nBufferReplay = 0;
        }
        else
        {
            // update member counter
            m_nBufferReplay = nDisplayBufferCounter;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      DisplayReplayBuffer(UINT nIndex)
//
// DESCRIPTION: Display a replay buffer at index m_nBufferReplay in the GUI.
//              If desired also display the buffer history in the small display
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::DisplayReplayBuffer( void )
{
    // variables
    int nRepBufIndex[NUM_REPLAY_WINDOWS];

    // show progress in GUI
    m_ctrlProgressReplay.SetPos( m_nBufferReplay );
    m_ctrlSliderReplay.SetPos(-m_nBufferReplay);
    SetDlgItemInt( IDC_EDIT_BUFFER_REPLAY, m_nBufferReplay );

    // display major buffer in big display
    is_RenderBitmap( m_hCam, m_lSeqMemId[m_nBufferReplay], m_hWndDisplay, IS_RENDER_FIT_TO_WINDOW);

    // display the buffer history in the small display, the mid window holds the current buffer
    if( m_nBufferReplay <0 )
        m_nBufferReplay = 0;
    // mid buffer
    nRepBufIndex[2] = m_nBufferReplay;
    // lower index
    nRepBufIndex[1] = nRepBufIndex[2]-1;
    if( nRepBufIndex[1] < 0 )
        nRepBufIndex[1]=(m_nBuffersInUse-1) ;
    nRepBufIndex[0] = nRepBufIndex[1]-1;
    if( nRepBufIndex[0] < 0 )
        nRepBufIndex[0]=(m_nBuffersInUse-1) ;
    // upper index
    nRepBufIndex[3] = nRepBufIndex[2]+1;
    if( nRepBufIndex[3] == (int) m_nBuffersInUse )
        nRepBufIndex[3]=0 ;
    nRepBufIndex[4] = nRepBufIndex[3]+1;
    if( nRepBufIndex[4] == (int) m_nBuffersInUse )
        nRepBufIndex[4]=0 ;

    // now display the buffers in sample display
    for( int i=0 ; i<NUM_REPLAY_WINDOWS ; i++ )
    {
        SetDlgItemInt( IDC_EDIT_BUF_0+i,nRepBufIndex[i] ,false );
        is_RenderBitmap( m_hCam, m_lSeqMemId[nRepBufIndex[i]], m_hWndDispReplay[i], IS_RENDER_FIT_TO_WINDOW);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnDeltaposSpinReplay(NMHDR* pNMHDR, LRESULT* pResult)
//
// DESCRIPTION: Single step in Replay mode. The steps can go up and down
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnDeltaposSpinReplay(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    *pResult = 0;

    switch( pNMUpDown->iDelta )
    {
        // down
    case 1:
        if( m_nBufferReplay-- <= 0 ) m_nBufferReplay = (m_nBuffersInUse-1) ;
        DisplayReplayBuffer();  // display new buffers
        break;
        // up
    case -1:
        if( m_nBufferReplay++ >= ((int)m_nBuffersInUse-1) ) m_nBufferReplay = 0 ;
        DisplayReplayBuffer();  // display new buffers
        break;
    default:;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//
// DESCRIPTION: Check for any VScroll action
//              IDC_SLIDER_REPLAY: Used for replay control
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
    CSliderCtrl* pSc = (CSliderCtrl*) pScrollBar;

    switch (pScrollBar->GetDlgCtrlID ())
    {
    case IDC_SLIDER_REPLAY:
        m_nBufferReplay = -(pSc->GetPos ());  // get position
        DisplayReplayBuffer();                // display new buffers
        break;

    default:;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnButtonLoadParameters()
//
// DESCRIPTION: Load camera parameters from file.
//              Rebuilt the sequence based on the loaded parameters
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnButtonLoadParameters()
{
    // GUI - disable to open the file dialog more than once
    m_ctrlButtonLoadParam.EnableWindow( FALSE );

    // load parameters from file
    if (is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS)
    {

        // realloc memory
        IS_SIZE_2D imageSize;
        is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

        m_nSizeX = imageSize.s32Width;
        m_nSizeY = imageSize.s32Height;

        m_nColorMode = is_SetColorMode (m_hCam, IS_GET_COLOR_MODE);
        switch (m_nColorMode)
        {
        case IS_CM_RGBA12_UNPACKED:
        case IS_CM_BGRA12_UNPACKED:
            m_nBitsPerPixel = 64;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"12-Bit color");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_RGB12_UNPACKED:
        case IS_CM_BGR12_UNPACKED:
            m_nBitsPerPixel = 48;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"12-Bit color");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_RGB10_UNPACKED:
        case IS_CM_BGR10_UNPACKED:
            m_nBitsPerPixel = 48;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"10-Bit color");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_RGB10_PACKED:
        case IS_CM_BGR10_PACKED:
            m_nBitsPerPixel = 32;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"10-Bit packed color");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_RGBA8_PACKED:
        case IS_CM_BGRA8_PACKED:
            m_nBitsPerPixel = 32;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"8-Bit color");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_RGB8_PACKED:
        case IS_CM_BGR8_PACKED:
            m_nBitsPerPixel = 24;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_CHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_HIDE);
            break;

        case IS_CM_BGR565_PACKED:
            m_nBitsPerPixel = 16;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER )->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"BGR565 color");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_UYVY_PACKED:
        case IS_CM_CBYCRY_PACKED:
            m_nBitsPerPixel = 16;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"YUV/UYVY");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break; 

        case IS_CM_BGR5_PACKED:
            m_nBitsPerPixel = 15;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"5-Bit color");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;


        case IS_CM_MONO16:
            m_nBitsPerPixel = 16;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"16-Bit Mono");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_MONO12:
            m_nBitsPerPixel = 16;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"12-Bit Mono");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_MONO10:
            m_nBitsPerPixel = 16;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"10-Bit Mono");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_SENSOR_RAW16:
            m_nBitsPerPixel = 16;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"16-Bit RAW");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_SENSOR_RAW12:
            m_nBitsPerPixel = 16;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"12-Bit RAW");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_SENSOR_RAW10:
            m_nBitsPerPixel = 16;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"10-Bit RAW");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_RGB8_PLANAR:
            m_nBitsPerPixel = 24;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"8-Bit Planar");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        case IS_CM_MONO8:
            m_nBitsPerPixel = 8;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_CHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_HIDE);
            break;

        case IS_CM_SENSOR_RAW8:
            m_nBitsPerPixel = 8;
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER)->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"8-Bit RAW");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            break;

        default:
            CheckDlgButton(IDC_RADIO_CM_Y8, BST_UNCHECKED);
            CheckDlgButton(IDC_RADIO_CM_RGB24, BST_UNCHECKED);
            GetDlgItem(IDC_RADIO_CM_OTHER )->ShowWindow(SW_SHOW);
            SetDlgItemText(IDC_RADIO_CM_OTHER, L"Invalid Color Mode");
            CheckDlgButton(IDC_RADIO_CM_OTHER, BST_CHECKED);
            AfxMessageBox(L"ERROR: Invalid Color Mode!", MB_ICONEXCLAMATION, 0);
            return;
        }

        // rebuilt the sequence
        OnButtonReallocteBuffers();

        // GUI - update new settings
        m_nCamX = m_nSizeX;
        m_nCamY = m_nSizeY;

        // Get current pixel clock
        is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_GET, (void*)&m_nCamPclk, sizeof(m_nCamPclk));

        double newFps;
        is_SetFrameRate(m_hCam, IS_GET_FRAMERATE, &newFps );
        m_strCamFps.Format(L"%2.1f", newFps );
        UpdateData( FALSE );
    }

    // GUI - enable the button again
    m_ctrlButtonLoadParam.EnableWindow( TRUE );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::OnRadioCmY8()
//
// DESCRIPTION: Change the color mode
//              Rebuilt the sequence based on the new color mode
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnRadioCmY8()
{
    // go Y8
    m_nBitsPerPixel =  8;
    is_SetColorMode(m_hCam, IS_CM_MONO8 );

    // rebuilt the sequence
    OnButtonReallocteBuffers();

    // GUI
    GetDlgItem( IDC_RADIO_CM_OTHER )->ShowWindow(SW_HIDE);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeSequenceDlg::OnRadioCmRgb24()
//
// DESCRIPTION: Change the color mode
//              Rebuilt the sequence based on the new color mode
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeSequenceDlg::OnRadioCmRgb24()
{
    // go RGB24
    m_nBitsPerPixel =  24;
    is_SetColorMode(m_hCam, IS_CM_BGR8_PACKED);

    // rebuilt the sequence
    OnButtonReallocteBuffers();

    // GUI
    GetDlgItem( IDC_RADIO_CM_OTHER )->ShowWindow(SW_HIDE);
}


INT CUEyeSequenceDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CUEyeSequenceDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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

// EOF

