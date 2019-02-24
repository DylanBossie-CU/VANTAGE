//==========================================================================//
//                                                                          //
//  Copyright (C) 2006 - 2018                                               //
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


// uEyeStealDlg.cpp : implementation file
#include "stdafx.h"
#include "uEyeSteal.h"
#include "uEyeStealDlg.h"
#include ".\ueyestealdlg.h"
#include "version.h"


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

    enum { IDD = IDD_ABOUTBOX };

private:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange (CDataExchange* pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP ()
};


CAboutDlg::CAboutDlg () : CDialog(CAboutDlg::IDD)
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


void CAboutDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange (pDX);
}


BEGIN_MESSAGE_MAP (CAboutDlg, CDialog)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CUEyeStealDlg dialog
CUEyeStealDlg::CUEyeStealDlg (CWnd* pParent /*=NULL*/)
: CDialog (CUEyeStealDlg::IDD, pParent)
, m_nStealFormat (FALSE)
{
    m_hIcon = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);
}


void CUEyeStealDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange (pDX);
    DDX_Radio (pDX, IDC_RADIO_CM_RGB16, m_nStealFormat);
}


BEGIN_MESSAGE_MAP (CUEyeStealDlg, CDialog)
    ON_WM_SYSCOMMAND ()
    ON_WM_PAINT ()
    ON_WM_QUERYDRAGICON ()
    ON_BN_CLICKED (IDC_BUTTON_EXIT, OnButtonExit)
    ON_BN_CLICKED (IDC_BUTTON_START_LIVE, OnButtonStartLive)
    ON_BN_CLICKED (IDC_BUTTON_STOP_LIVE, OnButtonStopLive)
    ON_BN_CLICKED (IDC_BUTTON_STEAL, OnButtonSteal)
    ON_BN_CLICKED (IDC_CHECK_SCALE, OnCheckScaling)
    ON_BN_CLICKED (IDC_BUTTON_LOAD_PARAM, OnButtonLoadParam)
    ON_BN_CLICKED (IDC_RADIO_CM_Y8, OnRadioStealFormat)
    ON_BN_CLICKED (IDC_RADIO_CM_RGB16, OnRadioStealFormat)
    ON_BN_CLICKED (IDC_RADIO_CM_RGB24, OnRadioStealFormat)
    ON_BN_CLICKED (IDC_RADIO_CM_RGB32, OnRadioStealFormat)
    ON_BN_CLICKED (IDC_RADIO_CM_BAYER, OnRadioStealFormat)
    ON_MESSAGE (IS_UEYE_MESSAGE, OnUEyeMessage)
    ON_WM_CLOSE ()
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
// CUEyeStealDlg message handlers
BOOL CUEyeStealDlg::OnInitDialog ()
{
    CDialog::OnInitDialog ();

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT ((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT (IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu (FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString (IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty ())
        {
            pSysMenu->AppendMenu (MF_SEPARATOR);
            pSysMenu->AppendMenu (MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon (m_hIcon, TRUE); // Set big icon
    SetIcon (m_hIcon, FALSE); // Set small icon

    // path of application
    /*
    m_SaveFileName = _T (__argv[0]);
    int nPos = m_SaveFileName.ReverseFind ('\\');
    m_SaveFileName = m_SaveFileName.Left (nPos);
    */

    m_bFirstTime = TRUE;
    m_SaveFileName = "-1";

    OpenCamera ();
    OnButtonStartLive ();

    return TRUE;  // return TRUE  unless you set the focus to a control
}


void CUEyeStealDlg::OnSysCommand (UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal ();
    }
    else
    {
        CDialog::OnSysCommand (nID, lParam);
    }
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUEyeStealDlg::OnPaint ()
{
    if (IsIconic ())
    {
        CPaintDC dc (this); // device context for painting

        SendMessage (WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc (), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics (SM_CXICON);
        int cyIcon = GetSystemMetrics (SM_CYICON);
        CRect rect;
        GetClientRect (&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon (x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint ();
    }
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUEyeStealDlg::OnQueryDragIcon ()
{
    return (HCURSOR) m_hIcon;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeStealDlg::OpenCamera()
//
// DESCRIPTION: Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeStealDlg::OpenCamera ()
{
    INT nRet = 0;

    // close any opened cameras
    if (m_hCam != 0)
    {
        is_ExitCamera (m_hCam);
    }

    // init display handles
    m_hWndLive = GetDlgItem (IDC_STATIC_DISP_LIVE)->GetSafeHwnd (); // handle to live display window
    m_hWndSteal= GetDlgItem (IDC_STATIC_DISP_STEAL)->GetSafeHwnd ();// handle to steal image window

    // init memory variables
    m_pcStealImgMem = NULL;
    m_lStealMemId = 0;

    // init camera
    m_hCam = (HIDS) 0;  // open next camera
    if (InitCamera (&m_hCam, m_hWndLive) == IS_SUCCESS)
    {
        //Check if DirectX is available on the local machine
        if(is_DirectRenderer (m_hCam, DR_CHECK_COMPATIBILITY, NULL , NULL) != IS_SUCCESS)
        {
            AfxMessageBox(_T("This Demo requires support of Direct3D.\nThis usually means that either DirectX or the graphics card driver is not installed properly") ,
                MB_OK | MB_ICONSTOP);
            PostQuitMessage (0);
            return false;
        }
        // enable the dialog based error report
        //nRet = is_SetErrorReport (m_hCam, IS_ENABLE_ERR_REP); // or IS_DISABLE_ERR_REP);
        //if (nRet != IS_SUCCESS)
        //{
        //    AfxMessageBox (L"ERROR: Can not enable the automatic uEye error report!" , MB_ICONEXCLAMATION, 0);
        //    return false;
        //}

        // get sensor info
        SENSORINFO SensorInfo;
        is_GetSensorInfo (m_hCam, &SensorInfo);

        // init image size to sensor size by default
        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

        is_SetColorMode(m_hCam, IS_CM_BGRA8_PACKED);

        // display initialization
        is_SetDisplayMode (m_hCam, IS_SET_DM_DIRECT3D);

        // Scale image
        is_DirectRenderer (m_hCam, DR_ENABLE_SCALING, NULL, NULL);
        CheckDlgButton (IDC_CHECK_SCALE, BST_CHECKED);

        // setup the color mode for steal
        if (SensorInfo.nColorMode == IS_COLORMODE_MONOCHROME)
        {
            // monochrome camera
            m_nColorMode = IS_CM_MONO8;
            m_nBitsPerPixel = 8;
            m_nStealFormat = 3;
        }
        else
        {
            // color camera
            m_nColorMode = IS_CM_BGR8_PACKED;
            m_nBitsPerPixel = 24;
            m_nStealFormat = 1;
        }

        UpdateData (FALSE);

        INT nMode = m_nColorMode;
        is_DirectRenderer (m_hCam, DR_SET_STEAL_FORMAT, (void*)&nMode, sizeof (nMode));

        // allocate memory for steal
        AllocateStealMemory ();

        // enable some usefull uEye messages
        is_EnableMessage (m_hCam, IS_CAPTURE_STATUS, this->GetSafeHwnd ());
        is_EnableMessage (m_hCam, IS_DEVICE_REMOVED, this->GetSafeHwnd ());
        is_EnableMessage (m_hCam, IS_DEVICE_RECONNECTED, this->GetSafeHwnd ());
        is_EnableMessage (m_hCam, IS_FRAME, this->GetSafeHwnd ());
        is_EnableMessage (m_hCam, IS_STEAL_VIDEO, this->GetSafeHwnd ());

        GetDlgItem (IDC_BUTTON_LOAD_PARAM)->EnableWindow (true);
    }
    else
    {
        AfxMessageBox (L"ERROR: Cannot open uEye camera!" , MB_ICONEXCLAMATION, 0);
        PostQuitMessage (0);
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeStealDlg::OnButtonExit ()
//
// DESCRIPTION: - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeStealDlg::OnButtonExit ()
{
    // memory and events are automatically released
    is_ExitCamera (m_hCam);

    // terminate
    PostQuitMessage (0);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeStealDlg::OnUEyeMessage
//
// DESCRIPTION: handles the uEye messages
//
///////////////////////////////////////////////////////////////////////////////
LRESULT CUEyeStealDlg::OnUEyeMessage (WPARAM wParam, LPARAM lParam)
{    
    switch (wParam)
    {
    case IS_STEAL_VIDEO:
        if (m_hCam != 0)
        {
            // display in second window
            is_RenderBitmap (m_hCam, m_lStealMemId, m_hWndSteal, IS_RENDER_FIT_TO_WINDOW);

            // save
            if (m_bFirstTime)
            {
                CFileDialog FileDlg(FALSE, L"bmp", L"steal_image.bmp", 0, L"Bitmap (*.bmp)|*.bmp|All Files (*.*)|*.*||");
                if (IDOK == FileDlg.DoModal())
                {
                    m_SaveFileName = FileDlg.GetPathName();
                    m_bFirstTime = FALSE;
                }
            }

            if (m_SaveFileName != "-1")
            {
                // Save image as bitmap
                IMAGE_FILE_PARAMS ImageFileParams;

                USES_CONVERSION;
                ImageFileParams.pwchFileName    = T2W(m_SaveFileName.GetBuffer());
                ImageFileParams.pnImageID       = (UINT*)&m_lStealMemId; // valid ID
                ImageFileParams.ppcImageMem     = &m_pcStealImgMem; // valid buffer
                ImageFileParams.nFileType       = IS_IMG_BMP; 
                ImageFileParams.nQuality        = 0;

                is_ImageFile(m_hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams, sizeof(ImageFileParams));

                // increment counter
                m_nCntSteal++;
                SetDlgItemInt (IDC_EDIT_CNT_STEAL, m_nCntSteal);
            }
        }

        break;

    case IS_FRAME:
        // increment counter
        m_nCntFrame++;
        SetDlgItemInt (IDC_EDIT_CNT_FRAME, m_nCntFrame);
        break;

    case IS_CAPTURE_STATUS:
        // increment counter
        m_nCntTransFail++;
        SetDlgItemInt (IDC_EDIT_CNT_TRANS_FAIL, m_nCntTransFail);
        break;

    case IS_DEVICE_REMOVED:
        Beep (5000,100);
        AfxMessageBox (L"ERROR: Camera disconnect!" , MB_ICONEXCLAMATION, 0);
        break;

    case IS_DEVICE_RECONNECTED:
        Beep (100,100);
        AfxMessageBox (L"INFO: Camera reconnect!" , MB_ICONINFORMATION , 0);
        break;

    case IS_NEW_DEVICE:
        // todo: add your handler
        break;

    case IS_DEVICE_REMOVAL:
        // todo: add your handler
        break;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeStealDlg::OnButtonStartLive()
//
// DESCRIPTION: Start the live display
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeStealDlg::OnButtonStartLive ()
{
    if (m_hCam == 0)
    {
        OpenCamera ();
    }

    // uEye
    is_CaptureVideo (m_hCam, IS_WAIT);

    // GUI
    GetDlgItem (IDC_BUTTON_START_LIVE)->EnableWindow (false);
    GetDlgItem (IDC_BUTTON_STOP_LIVE)->EnableWindow (true);
    GetDlgItem (IDC_BUTTON_STEAL)->EnableWindow (true);
    GetDlgItem (IDC_BUTTON_LOAD_PARAM)->EnableWindow (false);

    m_nCntTransFail = 0;
    SetDlgItemInt (IDC_EDIT_CNT_TRANS_FAIL, m_nCntTransFail);

    m_nCntFrame = 0;
    SetDlgItemInt (IDC_EDIT_CNT_FRAME, m_nCntTransFail);

    m_nCntSteal	= 0;
    SetDlgItemInt (IDC_EDIT_CNT_STEAL, m_nCntSteal);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeStealDlg::OnButtonStopLive ()
//
// DESCRIPTION: Stop live display
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeStealDlg::OnButtonStopLive ()
{
    // uEye
    if (m_hCam != 0)
    {
        is_StopLiveVideo (m_hCam, IS_WAIT);
    }

    // GUI
    GetDlgItem (IDC_BUTTON_START_LIVE)->EnableWindow (true);
    GetDlgItem (IDC_BUTTON_STOP_LIVE)->EnableWindow (false);
    GetDlgItem (IDC_BUTTON_STEAL)->EnableWindow (false);
    GetDlgItem (IDC_BUTTON_LOAD_PARAM)->EnableWindow (true);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeStealDlg::OnButtonSteal ()
//
// DESCRIPTION: Steal a frame
//              Display and save is done in the STEAL event/message handler
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeStealDlg::OnButtonSteal ()
{
    //steal !

    if ((m_hCam != 0) && (m_lStealMemId != 0))
    {
        INT wait = IS_DONT_WAIT;
        is_DirectRenderer (m_hCam, DR_STEAL_NEXT_FRAME, (void*)&wait, sizeof (wait));
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeStealDlg::OnCheckDDOvl()
//
// DESCRIPTION: Use the display mode Overlay Surface with automatic scaling
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeStealDlg::OnCheckScaling ()
{  
    // scale image 
    if (IsDlgButtonChecked (IDC_CHECK_SCALE))
    {
        is_DirectRenderer (m_hCam, DR_ENABLE_SCALING, NULL, NULL);
    }
    else
    {
        is_DirectRenderer (m_hCam, DR_DISABLE_SCALING, NULL, NULL);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeStealDlg::OnButtonLoadParam ()
//
// DESCRIPTION: Load camera parameters from file.
//              Rebuilt the sequence based on the loaded parameters
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeStealDlg::OnButtonLoadParam ()
{
    // load parameters from file
    if (m_hCam == 0)
    {
        OpenCamera ();
    }

    is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL);

    // realloc memory
    IS_SIZE_2D imageSize;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

    m_nSizeX = imageSize.s32Width;
    m_nSizeY = imageSize.s32Height;

    m_nColorMode = is_SetColorMode (m_hCam, IS_GET_COLOR_MODE);
    switch (m_nColorMode)
    {
    case IS_CM_MONO8:

        is_SetDisplayMode (m_hCam, IS_SET_DM_DIRECT3D | IS_SET_DM_MONO);
        break;

    case IS_CM_SENSOR_RAW8:

        is_SetDisplayMode (m_hCam, IS_SET_DM_DIRECT3D | IS_SET_DM_BAYER);
        break;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeStealDlg::OnRadioStealFormat ()
//
// DESCRIPTION: Change the steal format
//              Reallocate buffer memory
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeStealDlg::OnRadioStealFormat ()
{
    UpdateData (TRUE);

    switch (m_nStealFormat)
    {
    case 0:
        // RGB16
        m_nBitsPerPixel =  16;
        m_nColorMode = IS_CM_BGR565_PACKED;
        break;

    case 1:
        // RGB24
        m_nBitsPerPixel =  24;
        m_nColorMode = IS_CM_BGR8_PACKED;
        break;

    case 2:
        // RGB32
        m_nBitsPerPixel =  32;
        m_nColorMode = IS_CM_BGRA8_PACKED;
        break;

    case 3:
        // go Y8
        m_nBitsPerPixel =  8;
        m_nColorMode = IS_CM_MONO8;
        break;

    case 4:
        // Bayer
        m_nBitsPerPixel =  8;
        m_nColorMode = IS_CM_SENSOR_RAW8;
        break;
    }

    is_DirectRenderer (m_hCam, DR_SET_STEAL_FORMAT, (void*)&m_nColorMode, sizeof (m_nColorMode));

    // allocate memory for steal
    AllocateStealMemory ();
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD:      CUEyeStealDlg::AllocateStealMemory ()
//
// DESCRIPTION: Allocate an image buffer for steal and activate it.
//              Cleanup previously allocated image memory.
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeStealDlg::AllocateStealMemory ()
{
    if (m_hCam != 0)
    {
        // memory free?
        if (m_pcStealImgMem != NULL)
        {
            is_FreeImageMem (m_hCam, m_pcStealImgMem, m_lStealMemId);
        }

        // allocate memory for steal
        if (is_AllocImageMem (m_hCam,
            m_nSizeX,
            m_nSizeY,
            m_nBitsPerPixel,
            &m_pcStealImgMem,
            &m_lStealMemId) != IS_SUCCESS)
        {
            AfxMessageBox (L"ERROR: cannot allocate buffer memory for steal" , MB_ICONEXCLAMATION, 0);
        }

        // activate image memory
        is_SetImageMem (m_hCam, m_pcStealImgMem, m_lStealMemId);
    }
}


void CUEyeStealDlg::OnClose ()
{
    // memory and events are automatically released
    is_ExitCamera (m_hCam);

    CDialog::OnClose ();
}


INT CUEyeStealDlg::InitCamera (HIDS *hCam, HWND hWnd)
{
    INT nRet = is_InitCamera (hCam, hWnd);
    /************************************************************************************************/
    /*                                                                                              */
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*                                                                                              */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */
    /*  is "OR"-ed to the deviceID. This flag allows an automatic upload of the firmware.           */
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


void CUEyeStealDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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
