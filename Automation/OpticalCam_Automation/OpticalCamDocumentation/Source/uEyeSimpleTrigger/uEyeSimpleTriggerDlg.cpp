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
#include "uEyeSimpleTrigger.h"
#include "uEyeSimpleTriggerDlg.h"
#include ".\ueyesimpletriggerdlg.h"
#include "version.h"


extern CuEyeSimpleTriggerApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

private:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange(CDataExchange* pDX);

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


/////////////////////////////////////////////////////////////////////////////
CuEyeSimpleTriggerDlg::CuEyeSimpleTriggerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CuEyeSimpleTriggerDlg::IDD, pParent)
, m_cnNumberOfSeqMemory(3)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CuEyeSimpleTriggerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RADIO_OFF,           m_nTriggerMode);
    DDX_Text(pDX, IDC_EDIT_FRAME_EVENTS,    m_nFrameEvents);
    DDX_Text(pDX, IDC_EDIT_TRIGGER_EVENTS,  m_nTriggerEvents);
}


BEGIN_MESSAGE_MAP(CuEyeSimpleTriggerDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_START,             OnButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_STOP,              OnButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_EXIT,              OnButtonExit)
    ON_MESSAGE(IS_UEYE_MESSAGE,                 OnUEyeMessage)
    ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAMETER,    OnBnClickedButtonLoadParameter)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_RADIO_OFF,                OnBnClickedRadioTrigger)
    ON_BN_CLICKED(IDC_RADIO_SOFTWARE,           OnBnClickedRadioTrigger)
    ON_BN_CLICKED(IDC_RADIO_LO_HI,              OnBnClickedRadioTrigger)
    ON_BN_CLICKED(IDC_RADIO_HI_LO,              OnBnClickedRadioTrigger)
    ON_BN_CLICKED(IDC_BUTTON_RESET_COUNTERS, OnBnClickedButtonResetCounters)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
BOOL CuEyeSimpleTriggerDlg::OnInitDialog()
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

    // Will only be enabled if in stop mode
    GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow(FALSE);

    // Set the icon for this dialog.  The framework does this automatically
    // when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE); // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // Get handle to display window
    m_hWndDisplay = GetDlgItem( IDC_DISPLAY )->m_hWnd;

    m_vecMemory.reserve(m_cnNumberOfSeqMemory);
    m_vecMemory.resize(m_cnNumberOfSeqMemory);

    m_hCam = 0;
    m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;
    m_nPosX = 0;
    m_nPosY = 0;
    m_nFlipHor = 0;
    m_nFlipVert = 0;

    m_nTriggerMode = 0;
    m_nFrameEvents = 0;
    m_nTriggerEvents = 0;

    UpdateData(FALSE);

    // Open a camera
    OpenCamera();

    // Deactivate button "Start"
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

    GetDlgItem(IDC_RADIO_SOFTWARE)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_HI_LO)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_LO_HI)->EnableWindow(FALSE);

    // Check if trigger mode is supported
    INT nSupportedTriggerModes = is_SetExternalTrigger(m_hCam, IS_GET_SUPPORTED_TRIGGER_MODE);

    if ((nSupportedTriggerModes & IS_SET_TRIGGER_SOFTWARE) == IS_SET_TRIGGER_SOFTWARE)
    {
        GetDlgItem(IDC_RADIO_SOFTWARE)->EnableWindow(TRUE);
    }

    if ((nSupportedTriggerModes & IS_SET_TRIGGER_HI_LO) == IS_SET_TRIGGER_HI_LO)
    {
        GetDlgItem(IDC_RADIO_HI_LO)->EnableWindow(TRUE);
    }

    if ((nSupportedTriggerModes & IS_SET_TRIGGER_LO_HI) == IS_SET_TRIGGER_LO_HI)
    {
        GetDlgItem(IDC_RADIO_LO_HI)->EnableWindow(TRUE);
    }

    return true;
}


void CuEyeSimpleTriggerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CuEyeSimpleTriggerDlg::OnPaint()
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
HCURSOR CuEyeSimpleTriggerDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD OnButtonStart()
//
// DESCRIPTION: start capture and return immediately
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleTriggerDlg::OnButtonStart()
{ 
    if( m_hCam == 0 )
    {
        OpenCamera();
    }

    if( m_hCam != 0 )
    {
        // Capture video (live or trigger)
        is_CaptureVideo( m_hCam, IS_DONT_WAIT );

        // Disable button 'Load Parameters'
        GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow(FALSE);

        m_bLive = TRUE;

        // Deactivate button "Start"
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

        // Activate button "Stop"
        GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD OnButtonStop()
//
// DESCRIPTION: stop capture and return immediately
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleTriggerDlg::OnButtonStop()
{
    // Stop immediately - force stop
    if( m_hCam != 0 )
    {
        is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP );

        m_bLive = FALSE;

        // Activate button "Start"
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

        // Deactivate button "Stop"
        GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
    }

    // Enable button 'Load Parameters'
    GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow(TRUE);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD OnBnClickedButtonResetCounters()
//
// DESCRIPTION: reset the frame and trigger counters
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleTriggerDlg::OnBnClickedButtonResetCounters()
{
    // Reset the counters
    m_nFrameEvents = 0;
    m_nTriggerEvents = 0;

    UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD OnBnClickedButtonLoadParameter()
//
// DESCRIPTION: - loads parameters from an ini file
//              - reallocates the memory
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleTriggerDlg::OnBnClickedButtonLoadParameter()
{

    if ( m_hCam == 0 )
        OpenCamera();

    if ( m_hCam != 0 )
    {
        if( is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS)
        {
            // determine live capture
            BOOL bWasLive = (BOOL)(is_CaptureVideo( m_hCam, IS_GET_LIVE ));
            if( bWasLive )
                is_StopLiveVideo(m_hCam, IS_FORCE_VIDEO_STOP);

            ClearSequence();

            // realloc image mem with actual sizes and depth.
            FreeImageMems();

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

            INT nRet = AllocImageMems(nAllocSizeX, nAllocSizeY, m_nBitsPerPixel);

            // memory initialization
            if (IS_SUCCESS != nRet)
            {
                AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING);
            }

            InitSequence();

            // display initialization
            imageSize.s32Width = m_nSizeX;
            imageSize.s32Height = m_nSizeY;

            // Set the AOI with the correct size
            is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

            // run live image again
            if( bWasLive )
                is_CaptureVideo(m_hCam, IS_DONT_WAIT);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD OnButtonExit()
//
// DESCRIPTION: - stop capture
//              - free used image memory
//              - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleTriggerDlg::OnButtonExit()
{
    ExitCamera();
    PostQuitMessage( 0 );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD OpenCamera()
//
// DESCRIPTION: - Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeSimpleTriggerDlg::OpenCamera()
{
    INT nRet = IS_NO_SUCCESS;
    ExitCamera();

    // Init camera (open next available camera)
    m_hCam = (HIDS) 0;
    nRet = InitCamera(&m_hCam, m_hWndDisplay);
    if (nRet == IS_SUCCESS)
    {
        // Retrieve original image size
        is_GetSensorInfo(m_hCam, &m_sInfo);

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        UpdateData(TRUE);
        nRet = InitDisplayMode();

        if (nRet == IS_SUCCESS)
        {
            // Enable Messages
            is_EnableMessage(m_hCam,    IS_DEVICE_REMOVED,      GetSafeHwnd());
            is_EnableMessage(m_hCam,    IS_DEVICE_RECONNECTED,  GetSafeHwnd());
            is_EnableMessage(m_hCam,    IS_FRAME,               GetSafeHwnd());
            is_EnableMessage(m_hCam,    IS_TRIGGER,             GetSafeHwnd());

            // Start capture
            if(is_CaptureVideo( m_hCam, IS_WAIT ) == IS_SUCCESS)
            {
                m_bLive = TRUE;
            }
        }
        else
        {
            AfxMessageBox(TEXT("Initializing the display mode failed!"), MB_ICONWARNING );
        }

        return true;
    }
    else
    {
        AfxMessageBox(TEXT("No uEye camera could be opened !"), MB_ICONWARNING );
        return false;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD OnUEyeMessage()
//
// DESCRIPTION: - handles the messages from the uEye camera
//              - messages must be enabled using is_EnableMessage()
//
///////////////////////////////////////////////////////////////////////////////
LRESULT CuEyeSimpleTriggerDlg::OnUEyeMessage( WPARAM wParam, LPARAM lParam )
{
    UpdateData(FALSE);

    switch ( wParam )
    {
    case IS_DEVICE_REMOVED:
        Beep( 400, 50 );
        break;

    case IS_DEVICE_RECONNECTED:
        Beep( 400, 50 );
        break;

    case IS_FRAME:
        {
            m_nFrameEvents++;

            char* pLast = NULL;
            INT lMemoryId = 0;
            INT lSequenceId = 0;

            if (IS_SUCCESS == GetLastMem(&pLast, lMemoryId, lSequenceId))
            {
                INT nRet = is_LockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, pLast);

                if (IS_SUCCESS == nRet)
                {
                    is_RenderBitmap(m_hCam, lMemoryId, m_hWndDisplay, m_nRenderMode);
                    is_UnlockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, pLast);
                }
            }
        }

        break;

    case IS_TRIGGER:
        m_nTriggerEvents++;
        break;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD ExitCamera()
//
// DESCRIPTION: - exits the instance of the camera
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleTriggerDlg::ExitCamera()
{
    if( m_hCam != 0 )
    {
        // Disable messages
        is_EnableMessage(m_hCam,    IS_DEVICE_REMOVED,      NULL);
        is_EnableMessage(m_hCam,    IS_DEVICE_RECONNECTED,  NULL);
        is_EnableMessage(m_hCam,    IS_FRAME,               NULL );
        is_EnableMessage(m_hCam,    IS_TRIGGER,             NULL );

        // Stop live video
        is_StopLiveVideo( m_hCam, IS_WAIT );

        ClearSequence();
        FreeImageMems();

        // Close camera
        is_ExitCamera( m_hCam );
        m_hCam = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD InitDisplayMode()
//
// DESCRIPTION: - initializes the display mode
//
///////////////////////////////////////////////////////////////////////////////
int CuEyeSimpleTriggerDlg::InitDisplayMode()
{
    INT nRet = IS_NO_SUCCESS;

    if (m_hCam == NULL)
    {
        return IS_NO_SUCCESS;
    }

    ClearSequence();
    FreeImageMems();

    // Set display mode to DIB
    nRet = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
    if (m_sInfo.nColorMode == IS_COLORMODE_BAYER)
    {
        // Setup the color depth to the current windows setting
        is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
    }
    else if (m_sInfo.nColorMode == IS_COLORMODE_CBYCRY)
    {
        // For CBYCRY camera models use RGB32 mode
        m_nColorMode = IS_CM_BGRA8_PACKED;
        m_nBitsPerPixel = 32;
    }
    else
    {
        // For monochrome camera models use Y8 mode
        m_nColorMode = IS_CM_MONO8;
        m_nBitsPerPixel = 8;
    }

    nRet = AllocImageMems(m_nSizeX, m_nSizeY, m_nBitsPerPixel);

    if (nRet == IS_SUCCESS)
    {
        InitSequence();
        // Set the desired color mode
        is_SetColorMode(m_hCam, m_nColorMode);

        // Set the image size to capture
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
    }
    else
    {
        AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING);
    }

    return nRet;
}


void CuEyeSimpleTriggerDlg::OnClose()
{
    ExitCamera();
    CDialog::OnClose();
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD OnBnClickedRadioTrigger()
//
// DESCRIPTION: - deactivates or changes the trigger mode
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSimpleTriggerDlg::OnBnClickedRadioTrigger()
{
    if (!UpdateData(TRUE))
    {
        return;
    }

    if(m_bLive == TRUE)
    {
        is_StopLiveVideo(m_hCam, IS_WAIT);
    }

    // Trigger off
    if(m_nTriggerMode == 0)
    {
        is_SetExternalTrigger(m_hCam, IS_SET_TRIGGER_OFF);
    }
    // Software trigger
    if(m_nTriggerMode == 1)
    {
        is_SetExternalTrigger(m_hCam, IS_SET_TRIGGER_SOFTWARE);
    }
    // Hardware trigger falling edge
    else if(m_nTriggerMode == 2)
    {
        is_SetExternalTrigger(m_hCam, IS_SET_TRIGGER_HI_LO);
    }
    // Hardware trigger rising edge
    else if(m_nTriggerMode == 3)
    {
        is_SetExternalTrigger(m_hCam, IS_SET_TRIGGER_LO_HI);
    }

    if(m_bLive == TRUE)
    {
        is_CaptureVideo(m_hCam, IS_DONT_WAIT);
    }
}


INT CuEyeSimpleTriggerDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CuEyeSimpleTriggerDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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

INT CuEyeSimpleTriggerDlg::AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel)
{
    INT nRet = IS_SUCCESS;

    for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
    {
        nRet = is_AllocImageMem(m_hCam, nSizeX, nSizeY, nBitsPerPixel, &(iter->pcImageMemory), &(iter->lMemoryId));

        if (IS_SUCCESS != nRet)
        {
            FreeImageMems();
            break;
        }
    }

    return nRet;
}

INT CuEyeSimpleTriggerDlg::FreeImageMems(void)
{
    INT nRet = IS_SUCCESS;

    // Free the allocated buffer
    for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
    {
        if (NULL != iter->pcImageMemory)
        {
            nRet = is_FreeImageMem(m_hCam, iter->pcImageMemory, iter->lMemoryId);
        }

        iter->pcImageMemory = NULL;
        iter->lMemoryId = 0;
        iter->lSequenceId = 0;
    }

    return nRet;
}

INT CuEyeSimpleTriggerDlg::InitSequence(void)
{
    INT nRet = IS_SUCCESS;

    int i = 0;
    for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter, i++)
    {
        nRet = is_AddToSequence(m_hCam, iter->pcImageMemory, iter->lMemoryId);

        if (IS_SUCCESS != nRet)
        {
            ClearSequence();
            break;
        }

        iter->lSequenceId = i + 1;
    }

    return nRet;
}

INT CuEyeSimpleTriggerDlg::ClearSequence(void)
{
    return is_ClearSequence(m_hCam);
}

INT CuEyeSimpleTriggerDlg::GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId)
{
    INT nRet = IS_NO_SUCCESS;

    int dummy = 0;
    char *pLast = NULL;
    char *pMem = NULL;

    nRet = is_GetActSeqBuf(m_hCam, &dummy, &pMem, &pLast);

    if ((IS_SUCCESS == nRet) && (NULL != pLast))
    {
        nRet = IS_NO_SUCCESS;

        for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
        {
            if (pLast == iter->pcImageMemory)
            {
                *ppLastMem = iter->pcImageMemory;
                lMemoryId = iter->lMemoryId;
                lSequenceId = iter->lSequenceId;
                nRet = IS_SUCCESS;

                break;
            }
        }
    }

    return nRet;
}