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

// IdsSimpleAcquireDlg.cpp : implementation file


#include "stdafx.h"
#include "IdsSimpleAcquire.h"
#include "IdsSimpleAcquireDlg.h"
#include ".\idssimpleacquiredlg.h"
#include "version.h"

extern CIdsSimpleAcquireApp theApp;

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
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
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


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdsSimpleAcquireDlg dialog

CIdsSimpleAcquireDlg::CIdsSimpleAcquireDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIdsSimpleAcquireDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CIdsSimpleAcquireDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIdsSimpleAcquireDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIdsSimpleAcquireDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIdsSimpleAcquireDlg, CDialog)
    //{{AFX_MSG_MAP(CIdsSimpleAcquireDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_BN_CLICKED(IDC_BUTTON_ACQUIRE, OnButtonAcquire)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAMETER, OnBnClickedButtonLoadParameter)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdsSimpleAcquireDlg message handlers

BOOL CIdsSimpleAcquireDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

#ifndef _WIN32_WCE
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
#endif
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE); // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    m_hCam = 0;	

    m_hWndDisplay = GetDlgItem( IDC_DISPLAY )->m_hWnd; // set display window handle

    m_nSizeX = 640; //rc.right - rc.left; // set video width  to fit into display window
    m_nSizeY = 480; //rc.bottom - rc.top; // set video height to fit into display window
    m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;

    OpenCamera(); // open a camera handle

#ifdef _WIN32_WCE
    LoadParameters();
#endif

    return true;

}



void CIdsSimpleAcquireDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIdsSimpleAcquireDlg::OnPaint()
{
    if (IsIconic())
    {
#ifndef _WIN32_WCE
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
#endif
    }
    else
    {
        CDialog::OnPaint();
    }
}



// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIdsSimpleAcquireDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleAcquireDlg::OnButtonAcquire()
//
// DESCRIPTION: - acquire a single frame
//              - display frame
//
///////////////////////////////////////////////////////////////////////////////
void CIdsSimpleAcquireDlg::OnButtonAcquire()
{
    if ( m_hCam == 0 )
        OpenCamera();

    if ( m_hCam != 0 )
    {
        if( is_FreezeVideo( m_hCam, IS_WAIT ) == IS_SUCCESS )
            is_RenderBitmap( m_hCam, m_lMemoryId, m_hWndDisplay, m_nRenderMode );
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleAcquireDlg::OnBnClickedButtonLoadParameter()
//
// DESCRIPTION: - loads parameters from an ini file
//              - reallocates the memory
//
///////////////////////////////////////////////////////////////////////////////
void CIdsSimpleAcquireDlg::OnBnClickedButtonLoadParameter()
{
#ifdef _WIN32_WCE
    LoadParameters();
#else
    if ( m_hCam == 0 )
        OpenCamera();

    if ( m_hCam != 0 )
    {
        if( is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS )
        {
            // realloc image memory with actual sizes and depth.
            is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );

            IS_SIZE_2D imageSize;
            is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

            INT nAllocSizeX = 0;
            INT nAllocSizeY = 0;

            m_nSizeX = nAllocSizeX = imageSize.s32Width;
            m_nSizeY = nAllocSizeY = imageSize.s32Height;

            UINT nAbsPosX = 0;
            UINT nAbsPosY = 0;

            // absolute position?
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

            is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId ); // set memory active

            // display initialization
            imageSize.s32Width = m_nSizeX;
            imageSize.s32Height = m_nSizeY;

            is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
        }
    }
#endif
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleAcquireDlg::OnButtonExit()
//
// DESCRIPTION: - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CIdsSimpleAcquireDlg::OnButtonExit()
{
    if( m_hCam != 0 )
    {
        //free old image memory.
        is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );
        is_ExitCamera( m_hCam );
    }
    PostQuitMessage( 0 );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleAcquireDlg::OpenCamera()
//
// DESCRIPTION: - Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CIdsSimpleAcquireDlg::OpenCamera()
{
    if ( m_hCam != 0 )
    {
        //free old image mem.
        is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );
        is_ExitCamera( m_hCam );
    }

    // init camera
    m_hCam = (HIDS) 0; // open next camera
    m_Ret = InitCamera (&m_hCam, NULL); // init camera - no window handle required
    if( m_Ret == IS_SUCCESS )
    {
        // Get sensor info
        is_GetSensorInfo(m_hCam, &m_sInfo);

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        // setup the color depth to the current windows setting
        is_GetColorDepth (m_hCam, &m_nBitsPerPixel, &m_nColorMode);
        is_SetColorMode (m_hCam, m_nColorMode);

        // memory initialization
        is_AllocImageMem (	m_hCam,
            m_nSizeX,
            m_nSizeY,
            m_nBitsPerPixel,
            &m_pcImageMemory,
            &m_lMemoryId);
        is_SetImageMem (m_hCam, m_pcImageMemory, m_lMemoryId); // set memory active

        // display initialization
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

        is_SetDisplayMode (m_hCam, IS_SET_DM_DIB);

        // enable the dialog based error report
        //m_Ret = is_SetErrorReport (m_hCam, IS_ENABLE_ERR_REP); //IS_DISABLE_ERR_REP);
        //if (m_Ret != IS_SUCCESS)
        //{
            //AfxMessageBox( TEXT("ERROR: Can not enable the automatic uEye error report!") , MB_ICONEXCLAMATION, 0 );
            //return false;
        //}

#ifdef _WIN32_WCE
        LoadParameters();
#endif

    }
    else
    {
        AfxMessageBox( TEXT("ERROR: Can not open uEye camera!") , MB_ICONEXCLAMATION, 0 );
        return false;
    }

    return true;
}


void CIdsSimpleAcquireDlg::OnClose()
{
    if ( m_hCam != 0 )
    {
        //free old image memory.
        is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId );
        is_ExitCamera( m_hCam );
    }

    CDialog::OnClose();
}

#define NOT_FOUND_VALUE 999999

#define READ_PROFILE_INT(_s_, _n_, _v_, _d_) \
{   \
    int _data_; \
    _data_ = (UINT)theApp.GetProfileInt(_T(_s_), _T(_n_), NOT_FOUND_VALUE); \
    if(_data_ == NOT_FOUND_VALUE)  \
{   \
    theApp.WriteProfileInt(_T(_s_), _T(_n_), _d_); \
    _v_ = _d_; \
}   \
        else \
        _v_ = _data_; \
}


void CIdsSimpleAcquireDlg::LoadParameters()
{
    if(m_hCam == NULL)
        return;

    UINT nPixelClock;
    UINT nFrameRate;
    UINT nExposureTime;
    UINT nGainR, nGainG, nGainB, nGainM;
    UINT nColorCorrection;
    UINT nBayerMode;

    nGainM  = is_SetHardwareGain(m_hCam, (int)IS_GET_DEFAULT_MASTER, -1, -1, -1);
    nGainR  = is_SetHardwareGain(m_hCam, (int)IS_GET_DEFAULT_RED,    -1, -1, -1);
    nGainG  = is_SetHardwareGain(m_hCam, (int)IS_GET_DEFAULT_GREEN,  -1, -1, -1);
    nGainB  = is_SetHardwareGain(m_hCam, (int)IS_GET_DEFAULT_BLUE,   -1, -1, -1);

    READ_PROFILE_INT("MRU", "PixelClock",       nPixelClock,        10);
    READ_PROFILE_INT("MRU", "FrameRate",        nFrameRate,         5);
    READ_PROFILE_INT("MRU", "ExposureTime",     nExposureTime,      100);
    READ_PROFILE_INT("MRU", "GainMaster",       nGainM,             nGainM);
    READ_PROFILE_INT("MRU", "GainRed",          nGainR,             nGainR);
    READ_PROFILE_INT("MRU", "GainGreen",        nGainG,             nGainG);
    READ_PROFILE_INT("MRU", "GainBlue",         nGainB,             nGainB);
    READ_PROFILE_INT("MRU", "ColorCorrection",  nColorCorrection,   IS_CCOR_DISABLE);
    READ_PROFILE_INT("MRU", "BayerMode",        nBayerMode,         IS_SET_BAYER_CV_NORMAL);
    READ_PROFILE_INT("MRU", "RenderMode",       m_nRenderMode,      IS_RENDER_NORMAL);

    m_Ret = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_SET, (void*)&nPixelClock, sizeof(nPixelClock));
    m_Ret = is_SetFrameRate(m_hCam, (double)nFrameRate, NULL);

    double dTemp = 0.0;
    dTemp = (double)nExposureTime;
    m_Ret = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dTemp, sizeof(dTemp));

    m_Ret = is_SetHardwareGain(m_hCam, nGainM, nGainR, nGainG, nGainB);
    m_Ret = is_SetColorCorrection(m_hCam, nColorCorrection, NULL);
    m_Ret = is_SetBayerConversion(m_hCam, nBayerMode);
}


INT CIdsSimpleAcquireDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CIdsSimpleAcquireDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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