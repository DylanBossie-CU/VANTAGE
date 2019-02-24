//==========================================================================//
//                                                                          //
//  Copyright (C) 2004 - 2018												//
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

// IdsSimpleLiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IdsSimpleLive.h"
#include "IdsSimpleLiveDlg.h"
#include ".\idssimplelivedlg.h"

extern CIdsSimpleLiveApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIdsSimpleLiveDlg dialog

CIdsSimpleLiveDlg::CIdsSimpleLiveDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIdsSimpleLiveDlg::IDD, pParent)
, m_cnNumberOfSeqMemory(3)
{
    //{{AFX_DATA_INIT(CIdsSimpleLiveDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIdsSimpleLiveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIdsSimpleLiveDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIdsSimpleLiveDlg, CDialog)
    //{{AFX_MSG_MAP(CIdsSimpleLiveDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAMETER, OnBnClickedButtonLoadParameter)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdsSimpleLiveDlg message handlers

BOOL CIdsSimpleLiveDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // will only be enabled if in stop mode
    GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow(FALSE);

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE); // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    // get handle to display window
    m_hWndDisplay = GetDlgItem( IDC_DISPLAY )->m_hWnd;

    m_vecMemory.reserve(m_cnNumberOfSeqMemory);
    m_vecMemory.resize(m_cnNumberOfSeqMemory);

    m_hCam = 0;
    m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;
    m_nPosX = 0;
    m_nPosY = 0;
    m_nFlipHor = 0;
    m_nFlipVert = 0;

    // open a camera handle
    OpenCamera();

    return true;
}


void CIdsSimpleLiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIdsSimpleLiveDlg::OnPaint() 
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
HCURSOR CIdsSimpleLiveDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleLiveDlg::OnButtonStart()
//
// DESCRIPTION: start live display and return immediately
//
///////////////////////////////////////////////////////////////////////////////
void CIdsSimpleLiveDlg::OnButtonStart()
{
    if( m_hCam == 0 )
        OpenCamera();

    if( m_hCam != 0 )
    {
        // Capture live video
        is_CaptureVideo( m_hCam, IS_WAIT );

        // Disable button 'Load Parameters'
        GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow(FALSE);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleLiveDlg::OnButtonStop()
//
// DESCRIPTION: stop live display and return immediately
//
///////////////////////////////////////////////////////////////////////////////
void CIdsSimpleLiveDlg::OnButtonStop()
{
    // stop live immediately - force stop
    if( m_hCam != 0 )
        is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP );

    // Enable button 'Load Parameters'
    GetDlgItem(IDC_BUTTON_LOAD_PARAMETER)->EnableWindow(TRUE);
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleLiveDlg::OnBnClickedButtonLoadParameter()
//
// DESCRIPTION: - loads parameters from an ini file
//              - reallocates the memory
//
///////////////////////////////////////////////////////////////////////////////
void CIdsSimpleLiveDlg::OnBnClickedButtonLoadParameter()
{
    if ( m_hCam == 0 )
        OpenCamera();

    if ( m_hCam != 0 )
    {
        if (is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS)
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

            switch(is_SetColorMode( m_hCam, IS_GET_COLOR_MODE ) )
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
            if (IS_SUCCESS != AllocImageMems(nAllocSizeX, nAllocSizeY, m_nBitsPerPixel))
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
// METHOD CIdsSimpleLiveDlg::OnButtonExit()
//
// DESCRIPTION: - stop live display
//              - frees used image memory
//              - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CIdsSimpleLiveDlg::OnButtonExit()
{
    ExitCamera();
    PostQuitMessage( 0 );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleLiveDlg::OpenCamera()
//
// DESCRIPTION: - Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CIdsSimpleLiveDlg::OpenCamera()
{
    INT nRet = IS_NO_SUCCESS;
    ExitCamera();

    // init camera (open next available camera)
    m_hCam = (HIDS) 0;
    nRet = InitCamera(&m_hCam, m_hWndDisplay);
    if (nRet == IS_SUCCESS)
    {
        // Get sensor info
        is_GetSensorInfo(m_hCam, &m_sInfo);

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        UpdateData(TRUE);
        nRet = InitDisplayMode();

        if (nRet == IS_SUCCESS)
        {
            // Enable Messages
            is_EnableMessage(m_hCam, IS_DEVICE_REMOVED, GetSafeHwnd());
            is_EnableMessage(m_hCam, IS_DEVICE_RECONNECTED, GetSafeHwnd());
            is_EnableMessage(m_hCam, IS_FRAME, GetSafeHwnd());

            // start live video
            is_CaptureVideo( m_hCam, IS_WAIT );

        }
        else
            AfxMessageBox(TEXT("Initializing the display mode failed!"), MB_ICONWARNING );

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
// METHOD CIdsSimpleLiveDlg::OnUEyeMessage()
//
// DESCRIPTION: - handles the messages from the uEye camera
//              - messages must be enabled using is_EnableMessage()
//
///////////////////////////////////////////////////////////////////////////////
LRESULT CIdsSimpleLiveDlg::OnUEyeMessage( WPARAM wParam, LPARAM lParam )
{
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
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleLiveDlg::ExitCamera()
//
// DESCRIPTION: - exits the instance of the camera
//
///////////////////////////////////////////////////////////////////////////////
void CIdsSimpleLiveDlg::ExitCamera()
{
    if( m_hCam != 0 )
    {
        // Disable messages
        is_EnableMessage( m_hCam, IS_FRAME, NULL );

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
// METHOD CIdsSimpleLiveDlg::InitDisplayMode()
//
// DESCRIPTION: - initializes the display mode
//
///////////////////////////////////////////////////////////////////////////////
int CIdsSimpleLiveDlg::InitDisplayMode()
{
    INT nRet = IS_NO_SUCCESS;

    if (m_hCam == NULL)
        return IS_NO_SUCCESS;

    ClearSequence();
    FreeImageMems();

    // Set display mode to DIB
    nRet = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
    if (m_sInfo.nColorMode == IS_COLORMODE_BAYER)
    {
        // setup the color depth to the current windows setting
        is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
    }
    else if (m_sInfo.nColorMode == IS_COLORMODE_CBYCRY)
    {
        // for color camera models use RGB32 mode
        m_nColorMode = IS_CM_BGRA8_PACKED;
        m_nBitsPerPixel = 32;
    }
    else
    {
        // for monochrome camera models use Y8 mode
        m_nColorMode = IS_CM_MONO8;
        m_nBitsPerPixel = 8;
    }

    nRet = AllocImageMems(m_nSizeX, m_nSizeY, m_nBitsPerPixel);

    if (nRet == IS_SUCCESS)
    {
        InitSequence();

        // set the desired color mode
        is_SetColorMode(m_hCam, m_nColorMode);

        // set the image size to capture
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


void CIdsSimpleLiveDlg::OnClose()
{
    ExitCamera();
    CDialog::OnClose();
}


INT CIdsSimpleLiveDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CIdsSimpleLiveDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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

INT CIdsSimpleLiveDlg::AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel)
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

INT CIdsSimpleLiveDlg::FreeImageMems(void)
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

INT CIdsSimpleLiveDlg::InitSequence(void)
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

INT CIdsSimpleLiveDlg::ClearSequence(void)
{
    return is_ClearSequence(m_hCam);
}

INT CIdsSimpleLiveDlg::GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId)
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