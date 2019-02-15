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

// uEyePixelPeekDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uEyePixelPeek.h"
#include "uEyePixelPeekDlg.h"
#include "DlgProxy.h"
#include "version.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable : 4996)

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
// CUEyePixelPeekDlg dialog

IMPLEMENT_DYNAMIC(CUEyePixelPeekDlg, CDialog);

CUEyePixelPeekDlg::CUEyePixelPeekDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUEyePixelPeekDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CUEyePixelPeekDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pAutoProxy = NULL;
}

CUEyePixelPeekDlg::~CUEyePixelPeekDlg()
{
    // If there is an automation proxy for this dialog, set
    //  its back pointer to this dialog to NULL, so it knows
    //  the dialog has been deleted.
    if (m_pAutoProxy != NULL)
        m_pAutoProxy->m_pDialog = NULL;
}

void CUEyePixelPeekDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUEyePixelPeekDlg)
    DDX_Control(pDX, IDC_BUTTON_ACQUIRE, m_ctrlBottonAcquire);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUEyePixelPeekDlg, CDialog)
    //{{AFX_MSG_MAP(CUEyePixelPeekDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_BN_CLICKED(IDC_BUTTON_ACQUIRE, OnButtonAcquire)
    ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAM, OnButtonLoadParam)
    ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_RADIO_COLOR_Y8, OnRadioColorY8)
    ON_BN_CLICKED(IDC_RADIO_COLOR_RGB24, OnRadioColorRgb24)
    //}}AFX_MSG_MAP
    ON_MESSAGE( IS_UEYE_MESSAGE, OnUEyeMessage )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUEyePixelPeekDlg message handlers

BOOL CUEyePixelPeekDlg::OnInitDialog()
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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TODO: Add extra initialization here

    // init variables
    m_hCam = 0;
    m_pcImgMem = NULL;
    m_lMemId = 0;
    m_pcPPImgMem = NULL;
    m_lPPMemId = 0;
    m_nPPSizeX = 12;
    m_nPPSizeY = 12;

    // display
    m_hWndDisp = GetDlgItem( IDC_DISPLAY )->GetSafeHwnd(); // get display window handle
    m_hWndDispPixPeek = GetDlgItem( IDC_DISPLAY_PIXEL_PEEK )->GetSafeHwnd(); // get pixel peek display window handle

    // init camera setup operation
    CamOpen(); // open a camera handle
    CamAcqDisp(); // acquire and display an image


    return TRUE; // return TRUE  unless you set the focus to a control
}

void CUEyePixelPeekDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
// to draw the icon. For MFC applications using the document/view model,
// this is automatically done for you by the framework.

void CUEyePixelPeekDlg::OnPaint()
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
        CamDisp(); // update camera image
        PixelPeekDisp(); // update pixel peek window
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUEyePixelPeekDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
// if a controller still holds on to one of its objects. These
// message handlers make sure that if the proxy is still in use,
// then the UI is hidden but the dialog remains around if it
// is dismissed.

void CUEyePixelPeekDlg::OnClose()
{
    if (CanExit())
        CDialog::OnClose();
}

void CUEyePixelPeekDlg::OnOK()
{
    // nop
    /*
    if (CanExit())
    CDialog::OnOK();
    */
}

void CUEyePixelPeekDlg::OnCancel()
{
    if (CanExit())
    {
        is_ExitCamera( m_hCam );
        CDialog::OnCancel();
    }
}

BOOL CUEyePixelPeekDlg::CanExit()
{
    // If the proxy object is still around, then the automation
    // controller is still holding on to this application. Leave
    // the dialog around, but hide its UI.
    if (m_pAutoProxy != NULL)
    {
        ShowWindow(SW_HIDE);
        return FALSE;
    }

    return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CamOpen()
//
// DESCRIPTION: - opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyePixelPeekDlg::CamOpen()
{
    if ( m_hCam != 0 )
        is_ExitCamera( m_hCam );

    // init camera
    m_hCam = (HIDS) 0; // open first available camera
    m_Ret = InitCamera( &m_hCam, m_hWndDisp );
    if( m_Ret == IS_SUCCESS )
    {
        // enable the dialog based error report
        //m_Ret = is_SetErrorReport(m_hCam, IS_ENABLE_ERR_REP); // or IS_DISABLE_ERR_REP);
        //if( m_Ret != IS_SUCCESS )
        //{
        //    AfxMessageBox(L"ERROR: Can not enable the automatic uEye error report!" , MB_ICONEXCLAMATION, 0 );
        //    return false;
        //}


        // init image size
        SENSORINFO SensorInfo;
        is_GetSensorInfo(m_hCam, &SensorInfo);

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

        // use color depth according to monochrome or color camera
        if( SensorInfo.nColorMode == IS_COLORMODE_MONOCHROME )
        {
            // monochrome camera
            m_nColorMode = IS_CM_MONO8;
            CamSetColorMode();
        }
        else
        {
            // color camera
            m_nColorMode = IS_CM_BGR8_PACKED;
            CamSetColorMode();

            bool bAutoSoftwareWhitebalance;
            bool bAutoSensorWhitebalance;

            double dblWB;
            is_SetAutoParameter(m_hCam, IS_GET_ENABLE_AUTO_WHITEBALANCE, &dblWB, NULL);
            bAutoSoftwareWhitebalance = (dblWB == 1.0);

            is_SetAutoParameter(m_hCam, IS_GET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &dblWB, NULL);
            bAutoSensorWhitebalance = (dblWB == 1.0);

            dblWB = 0.0;

            // Disable software white balance
            if (bAutoSoftwareWhitebalance)
            {
                is_SetAutoParameter( m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dblWB, &dblWB);
            }

            // Disable sensor white balance
            if (bAutoSensorWhitebalance)
            {
                is_SetAutoParameter( m_hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &dblWB, &dblWB);
            }
        }

        // display initialization
        is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);


        // enable some useful uEye messages
        is_EnableMessage( m_hCam, IS_FRAME, this->GetSafeHwnd() );
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
// METHOD:      OnButtonExit()
//
// DESCRIPTION: - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CUEyePixelPeekDlg::OnButtonExit()
{
    // stop capture
    is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP );

    // memory and messages are automatically released
    is_ExitCamera( m_hCam );

    // terminate
    PostQuitMessage( 0 );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnUEyeMessage
//
// DESCRIPTION: - handles the uEye messages
//
///////////////////////////////////////////////////////////////////////////////
LRESULT CUEyePixelPeekDlg::OnUEyeMessage( WPARAM wParam, LPARAM lParam )
{
    // we should not receive a uEye message without camera
    if ( m_hCam != 0 )
    {

        switch ( wParam )
        {

        case IS_FRAME:
            // great! We got a new frame and everything is fine
            // nop
            break;

        case IS_SEQUENCE:
            // nop
            break;

        case IS_TRIGGER:
            // nop
            break;

        case IS_CAPTURE_STATUS:
            Beep(1000,100);
            AfxMessageBox(L"ERROR: Frame transfer failed!" , MB_ICONEXCLAMATION, 0 );
            break;

        case IS_DEVICE_REMOVED:
            Beep(2000,100);
            AfxMessageBox(L"ERROR: Camera disconnected!" , MB_ICONEXCLAMATION, 0 );
            break;

        case IS_DEVICE_RECONNECTED:
            Beep(4000,100);
            AfxMessageBox(L"INFO: Camera reconnected!" , MB_ICONINFORMATION , 0 );
            break;

        case IS_NEW_DEVICE:
            // nop
            break;

        case IS_DEVICE_REMOVAL:
            // nop
            break;
        }
    }
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnButtonAcquire()
//
// DESCRIPTION: - acquire image
//              - display image
//
///////////////////////////////////////////////////////////////////////////////
void CUEyePixelPeekDlg::OnButtonAcquire()
{
    CamAcqDisp();
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CamAcqDisp()
//
// DESCRIPTION: - acquire image
//              - display image
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyePixelPeekDlg::CamAcqDisp()
{
    bool bRet = true;
    INT nRet;

    nRet = is_FreezeVideo( m_hCam, IS_WAIT );
    if( nRet == IS_SUCCESS )
    {
        bRet = CamDisp();
    }
    else
    {
        bRet = false;
    }

    return bRet;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CamDisp()
//
// DESCRIPTION: - display image
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyePixelPeekDlg::CamDisp()
{
    bool bRet = true;
    INT nRet;

    nRet = is_RenderBitmap( m_hCam, m_lMemId, m_hWndDisp, IS_RENDER_FIT_TO_WINDOW ); // scale image to window
    if( nRet != IS_SUCCESS )
    {
        bRet = false;
    }

    return bRet;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      PixelPeekDisp()
//
// DESCRIPTION: - display image in pixel peek window
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyePixelPeekDlg::PixelPeekDisp()
{
    bool bRet = true;
    INT nRet;

    nRet = is_RenderBitmap( m_hCam, m_lPPMemId, m_hWndDispPixPeek, IS_RENDER_FIT_TO_WINDOW ); // scale image to window
    if( nRet != IS_SUCCESS )
    {
        bRet = false;
    }

    return bRet;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnBnClickedButtonLoadParameter()
//
// DESCRIPTION: - load parameters from an ini file
//              - reallocate the memory
//
///////////////////////////////////////////////////////////////////////////////
void CUEyePixelPeekDlg::OnButtonLoadParam()
{
    if ( m_hCam != 0 )
    {
        if( is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS )
        {
            // ok, the new parameter set is accepted, continue....

            // update some member variables
            IS_SIZE_2D imageSize;
            is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

            m_nSizeX = imageSize.s32Width;
            m_nSizeY = imageSize.s32Height;

            m_nColorMode = is_SetColorMode( m_hCam, IS_GET_COLOR_MODE ) ;

            // set AOI to upper left corner
            IS_POINT_2D imagePoint;

            is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS, (void*)&imagePoint, sizeof(imagePoint));
            is_AOI(m_hCam, IS_AOI_IMAGE_SET_POS, (void*)&imagePoint, sizeof(imagePoint));

            // apply new color mode and alloc memory
            // disable image acquisition when the color format is not supported
            m_ctrlBottonAcquire.EnableWindow(false);
            if( CamSetColorMode() ) 
            {
                m_ctrlBottonAcquire.EnableWindow(true);
            }
        }

        // get a new image
        CamAcqDisp();

    }
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CamAllocMem()
//
// DESCRIPTION:	- free previous allocatd memory
//              - allocate image memory buffer according
//                to image size and color depth
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyePixelPeekDlg::CamAllocMem()
{
    bool bReturn = TRUE;

    // free memory
    if( m_pcImgMem != NULL )
    {
        is_FreeImageMem( m_hCam, m_pcImgMem, m_lMemId);
        m_pcImgMem = NULL;
        m_lMemId = 0;
    }


    // main memory allocate and activate
    m_Ret = is_AllocImageMem(	m_hCam,
        m_nSizeX,
        m_nSizeY,
        m_nBitsPerPixel,
        &m_pcImgMem,
        &m_lMemId);
    if( m_Ret == IS_SUCCESS )
    {
        is_SetImageMem(m_hCam, m_pcImgMem, m_lMemId ); // set memory active
        is_InquireImageMem (m_hCam, m_pcImgMem, m_lMemId, &m_nSizeX, &m_nSizeY, &m_nBitsPerPixel, &m_nPitch ); // read back for calculation
        m_nBytesPerPixel = (m_nBitsPerPixel+1)/8; // calculate bytes per pixel
        m_nImageSize = m_nSizeX * m_nSizeY * m_nBytesPerPixel; // image size [bytes]
    }
    else
    {
        bReturn = FALSE;
    }

    return bReturn;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      PixelPeekAllocMem
//
// DESCRIPTION: - free previous allocated memory
//              - allocate pixel peek memory
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyePixelPeekDlg::PixelPeekAllocMem()
{
    bool bReturn = TRUE;

    // free memory
    if( m_pcPPImgMem != NULL )
    {
        is_FreeImageMem( m_hCam, m_pcPPImgMem, m_lPPMemId);
        m_pcPPImgMem = NULL;
        m_lPPMemId = 0;
    }

    // allocate memory for pixel peek
    m_Ret = is_AllocImageMem(m_hCam,
        m_nPPSizeX,
        m_nPPSizeY,
        m_nBitsPerPixel,
        &m_pcPPImgMem,
        &m_lPPMemId);
    if( m_Ret != IS_SUCCESS )
    {
        bReturn = FALSE;
    }

    return bReturn;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      PixelPeekAllocMem
//
// DESCRIPTION: - copy pixel peek window out of the main image buffer 
//
// VARIABLES:   CenterX, CenterX: center pixel for peek window
//              in source image buffer.
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyePixelPeekDlg::PixelPeekMemCopy(INT CenterX, INT CenterY)
{
    //some variables
    bool bReturn = TRUE;
    static bool bBusy = false;

    // prevent a multiple function call
    if( bBusy == false )
    {
        bBusy = true;

        // source memory: m_pcImgMem
        // target memory: m_pcPPImgMem
        // center pixel:  pcPixelPointer

        // transform the center coordinates of the main image to the upper left corner of the
        //   pixel peek windows
        // limit the borders
        long lStartX,lStartY;
        lStartX = CenterX - (m_nPPSizeX/2);
        if( lStartX < 0 )
        {
            lStartX = 0;
        }
        if( lStartX >= ( m_nSizeX - m_nPPSizeX) )
        {
            lStartX = m_nSizeX - m_nPPSizeX ;
        }
        lStartY = CenterY - (m_nPPSizeY/2);
        if( lStartY < 0 )
        {
            lStartY = 0;
        }
        if( lStartY >= ( m_nSizeY - m_nPPSizeY) )
        {
            lStartY = m_nSizeY - m_nPPSizeY ;
        }

        // number of bytes per line in peek window
        INT nBytesPerLine;
        nBytesPerLine = m_nPPSizeX * m_nBytesPerPixel;


        // no camera - no memory copy
        if( m_hCam )
        {
            // copy memory peek window line wise
            for( INT i = 0 ; i<m_nPPSizeY ; i++ )
            {
                void *dest;
                void *src;
                INT  PixelOffset;

                // calculate pointer offset for source memory
                PixelOffset = ((long)lStartY * m_nPitch)  + (m_nPitch * i ) + ((long)lStartX * m_nBytesPerPixel);
                src = (reinterpret_cast<char*> (m_pcImgMem) + (PixelOffset)) ;

                // calculate pointer offset for target memory
                PixelOffset = m_nPPSizeX * m_nBytesPerPixel * i ;
                dest = (reinterpret_cast<char*> (m_pcPPImgMem) + (PixelOffset)) ;

                // copy memory
                try
                {
                    memcpy( dest, src, (size_t) nBytesPerLine );
                }
                catch( ... )
                {
                    bReturn = FALSE;
                }
            }
        }

        // done!!
        bBusy = false;
    }
    return bReturn;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CamSetColorMode()
//
// DESCRIPTION: Set the color mode according to:
//              - m_nBitsPerPixel
//              - m_nColorMode
//              - reallocate memory
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyePixelPeekDlg::CamSetColorMode()
{
    bool bReturn = TRUE;

    // set new color mode, read back color mode
    m_Ret = is_SetColorMode(m_hCam, m_nColorMode );
    if( m_Ret == IS_SUCCESS )
    {
        // change bits per pixel according color mode
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

        // reallocate image buffers
        bReturn = CamAllocMem();
        if( bReturn == true )
        {
            bReturn = PixelPeekAllocMem();
        }

        // update the GUI
        CamSetColorModeGUI();
    }
    else
    {
        bReturn = FALSE;
    }

    return bReturn;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CamSetColorModeGUI()
//
// DESCRIPTION: Adjust the info at the GUI according the color mode.
//              This is especially important when the color depth has changed
//              because of loading a parameter file
//
///////////////////////////////////////////////////////////////////////////////
void CUEyePixelPeekDlg::CamSetColorModeGUI()
{
    // some locals
    UINT nCheckCOLOR_Y8     = BST_UNCHECKED;
    UINT nCheckCOLOR_RGB24  = BST_UNCHECKED;
    UINT nCheckCOLOR_X      = BST_UNCHECKED;
    char strTextCOLOR_X[64] = "";
    BOOL bShowTextCOLOR_X   = FALSE;

    // change color settings in GUI according the color mode
    switch( is_SetColorMode( m_hCam, IS_GET_COLOR_MODE ) )
    {
    case IS_CM_BGRA8_PACKED:
        nCheckCOLOR_X = BST_CHECKED;
        sprintf(strTextCOLOR_X,"RGB32 color");
        bShowTextCOLOR_X = TRUE;
        break;

    case IS_CM_BGR8_PACKED:
        nCheckCOLOR_RGB24 = BST_CHECKED;
        break;

    case IS_CM_BGR565_PACKED:
        nCheckCOLOR_X = BST_CHECKED;
        sprintf(strTextCOLOR_X,"RGB16 color");
        bShowTextCOLOR_X = TRUE;
        break;

    case IS_CM_BGR5_PACKED:
        nCheckCOLOR_X = BST_CHECKED;
        sprintf(strTextCOLOR_X,"RGB15 color");
        bShowTextCOLOR_X = TRUE;
        break;

    case IS_CM_MONO8:
        nCheckCOLOR_Y8 = BST_CHECKED;
        break;

    case IS_CM_SENSOR_RAW8:
        nCheckCOLOR_X = BST_CHECKED;
        sprintf(strTextCOLOR_X,"Raw Bayer");
        bShowTextCOLOR_X = TRUE;
        break;

    case IS_CM_UYVY_PACKED:
        nCheckCOLOR_X = BST_CHECKED;
        sprintf(strTextCOLOR_X,"UYVY color");
        bShowTextCOLOR_X = TRUE;
        break;

    default:
        // we never should pass this point
        nCheckCOLOR_X = BST_CHECKED;
        sprintf(strTextCOLOR_X,"Invalid color Mode");
        bShowTextCOLOR_X = TRUE;
        AfxMessageBox(L"ERROR in CamSetColorModeGUI().\nInvalid color mode." , MB_ICONINFORMATION , 0 );
        return;
    }


    // GUI update the radio buttons
    CheckDlgButton( IDC_RADIO_COLOR_Y8, nCheckCOLOR_Y8 );
    CheckDlgButton( IDC_RADIO_COLOR_RGB24, nCheckCOLOR_RGB24 );
    CheckDlgButton( IDC_RADIO_COLOR_X, nCheckCOLOR_X );
    SetDlgItemText( IDC_RADIO_COLOR_X, CString(strTextCOLOR_X) );
    GetDlgItem( IDC_RADIO_COLOR_X )->ShowWindow(bShowTextCOLOR_X);


    // GUI image geometry
    SetDlgItemInt(IDC_EDIT_IMG_X, m_nSizeX, FALSE);
    SetDlgItemInt(IDC_EDIT_IMG_Y, m_nSizeY, FALSE);
    SetDlgItemInt(IDC_EDIT_IMG_PITCH, m_nPitch, FALSE);
    SetDlgItemInt(IDC_EDIT_IMG_BPP, m_nBitsPerPixel, FALSE);
    char strTxt[64];
    sprintf( strTxt,"%ld",m_nImageSize/1024 );
    SetDlgItemText( IDC_EDIT_IMG_SIZE, CString(strTxt) );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnRadioColorY8()
//
// DESCRIPTION: Change color mode
//
///////////////////////////////////////////////////////////////////////////////
void CUEyePixelPeekDlg::OnRadioColorY8()
{
    m_nColorMode = IS_CM_MONO8;
    CamSetColorMode();
    CamAcqDisp();
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnRadioColorRgb24()
//
// DESCRIPTION: Change color mode
//
///////////////////////////////////////////////////////////////////////////////
void CUEyePixelPeekDlg::OnRadioColorRgb24()
{
    m_nColorMode = IS_CM_BGR8_PACKED;
    CamSetColorMode();
    CamAcqDisp();
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      OnMouseMove
//
// DESCRIPTION: Display the color values at the mouse position
//
///////////////////////////////////////////////////////////////////////////////
void CUEyePixelPeekDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CDialog::OnMouseMove(nFlags, point);

    // continue when we have a valid image buffer
    if( m_pcImgMem != NULL )
    {
        // get size of display window
        CRect RectClientDisplay;
        GetDlgItem( IDC_DISPLAY )->GetClientRect(&RectClientDisplay);
        int nSizeDisplayX = RectClientDisplay.Width() ;
        int nSizeDisplayY = RectClientDisplay.Height() ;

        // get offset of the display window
        CRect RectClientDisplayMap;
        POINT Offset;
        RectClientDisplayMap = RectClientDisplay;
        GetDlgItem( IDC_DISPLAY )->MapWindowPoints( this, &RectClientDisplayMap );
        Offset.x = RectClientDisplayMap.right - RectClientDisplay.right;
        Offset.y = RectClientDisplayMap.top - RectClientDisplay.top;

        // transform mouse coordinates to display window coordinates
        // the coordinates start at [0,0]
        int nImageMouseCoordianteX = point.x - Offset.x;
        int nImageMouseCoordianteY = point.y - Offset.y;

        // mouse pointer inside the image display window ?
        if( (nImageMouseCoordianteX >= 0)  & (nImageMouseCoordianteX<nSizeDisplayX) &
            (nImageMouseCoordianteY >= 0)  & (nImageMouseCoordianteY<nSizeDisplayY) )
        {
            // mouse is in display window !

            // transform display window coordinates to image coordinates
            nImageMouseCoordianteX = (((nImageMouseCoordianteX)* m_nSizeX) / nSizeDisplayX );
            nImageMouseCoordianteY = (((nImageMouseCoordianteY)* m_nSizeY) / nSizeDisplayY );

            // get pointer to active image buffer
            void *pMemVoid;
            is_GetImageMem( m_hCam, &pMemVoid );

            // get pitch
            INT nPitch;
            is_GetImageMemPitch( m_hCam, &nPitch );

            // calculate pixel offset (pixel location in memory)
            char* pPixelPointer;
            long lMemoryPixelOffset = ((long)nImageMouseCoordianteY * nPitch ) + ((long)nImageMouseCoordianteX * m_nBytesPerPixel);
            pPixelPointer = (reinterpret_cast<char*> (pMemVoid) + (lMemoryPixelOffset)) ;

            // some variables for pixel peek
            unsigned char cPixVal1 = 0;
            unsigned char cPixVal2 = 0;
            unsigned char cPixVal3 = 0;
            unsigned int  nPixVal  = 0;
            char strPixVal1[32] = "";
            char strPixVal2[32] = "";
            char strPixVal3[32] = "";
            BOOL bShowPixVal1	= FALSE;
            BOOL bShowPixVal2	= FALSE;
            BOOL bShowPixVal3	= FALSE;

            // pixel peek according color format
            switch( m_nColorMode )
            {
            case IS_CM_BGRA8_PACKED:
            case IS_CM_BGR8_PACKED:

                cPixVal1 = *(reinterpret_cast<char*>(pPixelPointer+2));
                cPixVal2 = *(reinterpret_cast<char*>(pPixelPointer+1));
                cPixVal3 = *(reinterpret_cast<char*>(pPixelPointer+0));
                sprintf( strPixVal1,"R:%03d",cPixVal1 );
                sprintf( strPixVal2,"G:%03d",cPixVal2 );
                sprintf( strPixVal3,"B:%03d",cPixVal3 );
                bShowPixVal1 = TRUE ;
                bShowPixVal2 = TRUE ;
                bShowPixVal3 = TRUE ;
                break;

            case IS_CM_BGR565_PACKED:
                nPixVal = *(reinterpret_cast<unsigned int*>(pPixelPointer));
                cPixVal1 = (nPixVal & 0xf800) >> 11;
                cPixVal2 = (nPixVal & 0x07e0) >> 5;
                cPixVal3 = (nPixVal & 0x001f) >> 0;
                sprintf( strPixVal1,"R:%03d",cPixVal1 );
                sprintf( strPixVal2,"G:%03d",cPixVal2 );
                sprintf( strPixVal3,"B:%03d",cPixVal3 );
                bShowPixVal1 = TRUE ;
                bShowPixVal2 = TRUE ;
                bShowPixVal3 = TRUE ;
                break;

            case IS_CM_BGR5_PACKED:
                nPixVal = *(reinterpret_cast<unsigned int*>(pPixelPointer));
                cPixVal1 = (nPixVal & 0x7c00) >> 10;
                cPixVal2 = (nPixVal & 0x03e0) >> 5;
                cPixVal3 = (nPixVal & 0x001f) >> 0;
                sprintf( strPixVal1,"R:%03d",cPixVal1 );
                sprintf( strPixVal2,"G:%03d",cPixVal2 );
                sprintf( strPixVal3,"B:%03d",cPixVal3 );
                bShowPixVal1 = TRUE ;
                bShowPixVal2 = TRUE ;
                bShowPixVal3 = TRUE ;
                break;

            case IS_CM_MONO8:
                cPixVal1 = *(reinterpret_cast<char*>(pPixelPointer));
                sprintf( strPixVal1,"%03d",cPixVal1 );
                bShowPixVal1 = TRUE ;
                break;

            case IS_CM_SENSOR_RAW8:
                cPixVal1 = *(reinterpret_cast<char*>(pPixelPointer));
                sprintf( strPixVal1,"%03d",cPixVal1 );
                bShowPixVal1 = TRUE ;
                break;

            case IS_CM_UYVY_PACKED:
                nPixVal = *(reinterpret_cast<unsigned int*>(pPixelPointer));
                cPixVal1 = *(reinterpret_cast<char*>(pPixelPointer+2));
                cPixVal2 = *(reinterpret_cast<char*>(pPixelPointer+1));
                cPixVal3 = *(reinterpret_cast<char*>(pPixelPointer+0));
                sprintf( strPixVal1,"V:%03d",cPixVal1 );
                sprintf( strPixVal2,"Y:%03d",cPixVal2 );
                sprintf( strPixVal3,"U:%03d",cPixVal3 );
                bShowPixVal1 = TRUE ;
                bShowPixVal2 = TRUE ;
                bShowPixVal3 = TRUE ;
                break;

            default:
                // we never should pass this point
                sprintf( strPixVal1,"-" );
                sprintf( strPixVal2,"-" );
                sprintf( strPixVal3,"-" );
                bShowPixVal1 = TRUE ;
                bShowPixVal2 = TRUE ;
                bShowPixVal3 = TRUE ;
                return;
            }

            // display mouse coordinates
            SetDlgItemInt( IDC_EDIT_MOUSE_X, nImageMouseCoordianteX , false );
            SetDlgItemInt( IDC_EDIT_MOUSE_Y, nImageMouseCoordianteY , false );

            // display pixel value(s)
            GetDlgItem( IDC_EDIT_VALUE_1 )->ShowWindow(bShowPixVal1);
            GetDlgItem( IDC_EDIT_VALUE_2 )->ShowWindow(bShowPixVal2);
            GetDlgItem( IDC_EDIT_VALUE_3 )->ShowWindow(bShowPixVal3);
            SetDlgItemText( IDC_EDIT_VALUE_1, CString(strPixVal1) );
            SetDlgItemText( IDC_EDIT_VALUE_2, CString(strPixVal2) );
            SetDlgItemText( IDC_EDIT_VALUE_3, CString(strPixVal3) );

            // display pointers
            char strPointer[32] = "";
            sprintf( strPointer, "0x%p", pMemVoid );
            SetDlgItemText( IDC_EDIT_POINTER_BUF, CString(strPointer) );
            sprintf( strPointer, "0x%p", pPixelPointer );
            SetDlgItemText( IDC_EDIT_POINTER_PIX, CString(strPointer) );

            // display pixel peek window
            PixelPeekMemCopy( nImageMouseCoordianteX, nImageMouseCoordianteY );
            PixelPeekDisp();
        }
    }
}


INT CUEyePixelPeekDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CUEyePixelPeekDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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

#pragma warning (default : 4996)

/*
EOF
*/


