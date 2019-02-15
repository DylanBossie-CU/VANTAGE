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
#include "uEyeMeasureSharpness.h"
#include "uEyeMeasureSharpnessDlg.h"
#include "version.h"

extern CuEyeMeasureSharpnessApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CAboutDlg : public CDialog
{
public:

    CAboutDlg ();
    enum { IDD = IDD_ABOUTBOX };

private:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange(CDataExchange* pDX);   

    DECLARE_MESSAGE_MAP ()
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


void CAboutDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP (CAboutDlg, CDialog)
END_MESSAGE_MAP()



CuEyeMeasureSharpnessDlg::CuEyeMeasureSharpnessDlg(CWnd* pParent /*=NULL*/)
: CDialog(CuEyeMeasureSharpnessDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


BEGIN_MESSAGE_MAP(CuEyeMeasureSharpnessDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage)
    ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAMETER, OnBnClickedButtonLoadParameter)
    ON_WM_CLOSE()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
    ON_EN_KILLFOCUS(IDC_EDIT_X, OnEnKillfocusEditSize)
    ON_EN_KILLFOCUS(IDC_EDIT_Y, OnEnKillfocusEditSize)
    ON_EN_KILLFOCUS(IDC_EDIT_WIDTH, OnEnKillfocusEditSize)
    ON_EN_KILLFOCUS(IDC_EDIT_HEIGHT, OnEnKillfocusEditSize)
    ON_CBN_SELCHANGE(IDC_COMBO_AOI_PRESETS, OnCbnSelchangeComboAoiPresets)
    ON_CBN_SELCHANGE(IDC_COMBO_AOI, &CuEyeMeasureSharpnessDlg::OnCbnSelchangeComboAoi)
    ON_BN_CLICKED(IDC_BUTTON_RESET_AOI, &CuEyeMeasureSharpnessDlg::OnBnClickedButtonResetAoi)
END_MESSAGE_MAP()


void CuEyeMeasureSharpnessDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_AOI_PRESETS, m_comboAoiPresets);
    DDX_Control(pDX, IDC_COMBO_AOI, m_comboAoi);
}


BOOL CuEyeMeasureSharpnessDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

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

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_hWndDisplay = GetDlgItem(IDC_DISPLAY)->m_hWnd;	

    m_pcImageMemory = NULL;
    m_lMemoryId = 0;

    m_hCam = 0;
    m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;//IS_RENDER_NORMAL;//IS_RENDER_FIT_TO_WINDOW;

    for (INT i = 0; i < 5; i++)
    {
        m_s32Sharpness[i] = 0;
    }

    int s32ImageWidth = 0;
    int s32ImageHeight = 0;

    m_nCurrentAOI = 0;

    OpenCamera();

    MEASURE_SHARPNESS_AOI_INFO sharpnessInfo;
    sharpnessInfo.u32NumberAOI		= 0;
    sharpnessInfo.rcAOI.s32X		= 0;
    sharpnessInfo.rcAOI.s32Y		= 0;
    sharpnessInfo.rcAOI.s32Width	= m_s32ImageWidth;
    sharpnessInfo.rcAOI.s32Height	= m_s32ImageHeight;

    is_Measure(m_hCam, IS_MEASURE_CMD_SHARPNESS_AOI_SET, (void*)&sharpnessInfo, sizeof(sharpnessInfo));

    GetDlgItem(IDC_DISPLAY)->GetClientRect(&m_rcImageWindow);

    INT s32DisplayWidth   = m_rcImageWindow.right  - m_rcImageWindow.left + 1;
    INT s32DisplayHeight  = m_rcImageWindow.bottom - m_rcImageWindow.top  + 1;

    m_f64FactorX = (double)s32DisplayWidth  / (double)m_s32ImageWidth;
    m_f64FactorY = (double)s32DisplayHeight / (double)m_s32ImageHeight;

    m_comboAoiPresets.InsertString(0, L"Manual");
    m_comboAoiPresets.SetItemData(0, 0);
    m_comboAoiPresets.InsertString(1, L"Middle and corners");
    m_comboAoiPresets.SetItemData(1, IS_MEASURE_SHARPNESS_AOI_PRESET_1);
    m_comboAoiPresets.SetCurSel(0);

    m_comboAoi.InsertString(0, L"1");
    m_comboAoi.SetItemData(0, 0);
    m_comboAoi.InsertString(1, L"2");
    m_comboAoi.SetItemData(1, 1);
    m_comboAoi.InsertString(2, L"3");
    m_comboAoi.SetItemData(2, 2);
    m_comboAoi.InsertString(3, L"4");
    m_comboAoi.SetItemData(3, 3);
    m_comboAoi.InsertString(4, L"5");
    m_comboAoi.SetItemData(4, 4);
    m_comboAoi.SetCurSel(0);

    OnCbnSelchangeComboAoi();

    CRect Display;
    CWnd* pPlaceholderWnd = GetDlgItem(IDC_DISPLAY);
    pPlaceholderWnd->GetWindowRect(&Display);
    ScreenToClient(&Display);
    pPlaceholderWnd->DestroyWindow();
    m_display.CreateEx(0, NULL, NULL, WS_CHILD | WS_VISIBLE, Display, this, IDC_DISPLAY);
    m_display.ShowWindow(TRUE);

    m_Font.CreateFont (14, 0, 0, 0, 0, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T ("Tahoma")); 

    m_colorRed      = RGB(240, 0, 0);
    m_colorGreen    = RGB(0, 200, 0);

    GetClientRect(&m_rcWindow);

    OnTimer(1);

    return TRUE;
}


void CuEyeMeasureSharpnessDlg::OnTimer(UINT_PTR nIDEvent)
{
    KillTimer(1);

    UpdateSharpness();

    SetTimer(1, 100, NULL);
}


void CuEyeMeasureSharpnessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        KillTimer(1);

        CAboutDlg dlgAbout;
        dlgAbout.DoModal();

        SetTimer(1, 100, NULL);
    }
    else
    {
        CDialog::OnSysCommand (nID, lParam);
    }
}


void CuEyeMeasureSharpnessDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this);

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


HCURSOR CuEyeMeasureSharpnessDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


void CuEyeMeasureSharpnessDlg::OnBnClickedButtonLoadParameter()
{
    if (m_hCam == 0)
    {
        OpenCamera();
    }

    if (m_hCam != 0)
    {
        if ((is_ParameterSet(m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS) && (m_pcImageMemory != NULL))
        {
            // Realloc image mem with actual sizes and depth.
            is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);

            IS_SIZE_2D imageSize;
            is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

            INT nAllocSizeX  = 0;
            INT nAllocSizeY  = 0;

            m_s32ImageWidth  = nAllocSizeX = imageSize.s32Width;
            m_s32ImageHeight = nAllocSizeY = imageSize.s32Height;

            UINT nAbsPosX = 0;
            UINT nAbsPosY = 0;

            // Absolute pos?
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

            switch(is_SetColorMode(m_hCam, IS_GET_COLOR_MODE))
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

            // Memory initialization
            is_AllocImageMem(m_hCam, nAllocSizeX, nAllocSizeY, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId);

            // Set memory active
            is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);

            // Display initialization
            imageSize.s32Width = m_s32ImageWidth;
            imageSize.s32Height = m_s32ImageHeight;

            // Set the AOI with the correct size
            is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

            GetDlgItem(IDC_DISPLAY)->GetClientRect(&m_rcImageWindow);

            INT s32DisplayWidth   = m_rcImageWindow.right  - m_rcImageWindow.left + 1;
            INT s32DisplayHeight  = m_rcImageWindow.bottom - m_rcImageWindow.top  + 1;

            m_f64FactorX = (double)s32DisplayWidth  / (double)m_s32ImageWidth;
            m_f64FactorY = (double)s32DisplayHeight / (double)m_s32ImageHeight;
        }
    }  
}


void CuEyeMeasureSharpnessDlg::OnButtonExit() 
{	
    m_Font.DeleteObject();

    ExitCamera();
    PostQuitMessage(0);
}


void CuEyeMeasureSharpnessDlg::OnClose()
{
    m_Font.DeleteObject();

    ExitCamera();
    CDialog::OnClose();
}


LRESULT CuEyeMeasureSharpnessDlg::OnUEyeMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case IS_DEVICE_REMOVED:
        Beep(400, 50);
        break;
    case IS_DEVICE_RECONNECTED:
        Beep(400, 50);
        break;
    case IS_FRAME:

        break;
    }    
    return 0;
}


/******************************************************************************************************/
/*                                                                                                    */
/*									General Eye functions										      */
/*																									  */
/******************************************************************************************************/
bool CuEyeMeasureSharpnessDlg::OpenCamera()
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

        GetMaxImageSize(&m_s32ImageWidth, &m_s32ImageHeight);

        UpdateData(TRUE);
        nRet = InitDisplayMode();

        if (nRet == IS_SUCCESS)
        {
            // Enable Messages
            is_EnableMessage(m_hCam, IS_DEVICE_REMOVED, GetSafeHwnd());
            is_EnableMessage(m_hCam, IS_DEVICE_RECONNECTED, GetSafeHwnd());
            is_EnableMessage(m_hCam, IS_FRAME, GetSafeHwnd());
        }
        else
        {
            AfxMessageBox(TEXT("Initializing the display mode failed!"), MB_ICONWARNING );
        }

        /* Set maximum possible pixel clock */
        UINT nRange[3];
        ZeroMemory(nRange, sizeof(nRange));
        nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)nRange, sizeof(nRange));
        if (nRet == IS_SUCCESS)
        {
            INT nPclk = nRange[1];
            nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_SET, (void*)&nPclk, sizeof(nPclk));
        }

        /* Set maximum possible framerate */
        double dblMin, dblMax, dblInc;
        nRet = is_GetFrameTimeRange(m_hCam, &dblMin, &dblMax, &dblInc);
        if (nRet == IS_SUCCESS)
        {
            double dblFramerate = 0;
            nRet = is_SetFrameRate(m_hCam, 1 / dblMin, &dblFramerate);
        }

        /* Set maximum possible exposure time */
        double dblExposure = 0;
        nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dblExposure, sizeof(dblExposure));

        /* Set gamma to 1.6 */
        INT nGamma = 160;
        nRet = is_Gamma(m_hCam, IS_GAMMA_CMD_SET, (void*)&nGamma, sizeof(nGamma));

        return true;
    }
    else
    {
        AfxMessageBox(TEXT("No uEye camera could be opened !"), MB_ICONWARNING );
        return false;
    }
}


INT CuEyeMeasureSharpnessDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CuEyeMeasureSharpnessDlg::ExitCamera()
{
    if (m_hCam != 0)
    {
        // Disable messages
        is_EnableMessage(m_hCam, IS_FRAME, NULL);

        // Free the allocated buffer
        if (m_pcImageMemory != NULL)
        {
            is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
        }

        m_pcImageMemory = NULL;

        // Close camera
        is_ExitCamera(m_hCam);
        m_hCam = NULL;
    }
}


int CuEyeMeasureSharpnessDlg::InitDisplayMode()
{
    INT nRet = IS_NO_SUCCESS;

    if (m_hCam == NULL)
    {
        return IS_NO_SUCCESS;
    }

    if (m_pcImageMemory != NULL)
    {
        is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
    }
    m_pcImageMemory = NULL;

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

    /*
    m_nColorMode = IS_SET_CM_Y8;
    m_nBitsPerPixel = 8;

    m_nColorMode = IS_SET_CM_RGB32;
    m_nBitsPerPixel = 32;

    m_nColorMode = IS_SET_CM_RGB24;
    m_nBitsPerPixel = 24;
    */

    // allocate an image memory.
    if (is_AllocImageMem(m_hCam, m_s32ImageWidth, m_s32ImageHeight, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId ) != IS_SUCCESS)
    {
        AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING);
    }
    else
    {
        is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
    }

    if (nRet == IS_SUCCESS)
    {
        // set the desired color mode
        is_SetColorMode(m_hCam, m_nColorMode);

        // set the image size to capture
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_s32ImageWidth;
        imageSize.s32Height = m_s32ImageHeight;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
    }   

    return nRet;
}


void CuEyeMeasureSharpnessDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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


/******************************************************************************************************/
/*                                                                                                    */
/*									Sharpness measure functions										  */
/*																									  */
/******************************************************************************************************/
void CuEyeMeasureSharpnessDlg::UpdateSharpness()
{
    if (m_display.GetParent() == NULL)
    {
        m_display.SetParent(this);
    }

    INT nRet = IS_SUCCESS;

    nRet = is_FreezeVideo(m_hCam, IS_WAIT);

    INT s32Sharpness = 0;

    for (INT i = 0; i < 5 ; i++)
    {
        m_sharpnessInfo[i].u32NumberAOI = i;
        nRet = is_Measure(m_hCam, IS_MEASURE_CMD_SHARPNESS_AOI_INQUIRE, (void*)&m_sharpnessInfo[i], sizeof(m_sharpnessInfo[i]));

        if (nRet == IS_SUCCESS)
        {
            s32Sharpness = m_sharpnessInfo[i].u32SharpnessValue;

            if (s32Sharpness > m_s32Sharpness[i])
            {
                m_s32Sharpness[i] = s32Sharpness;
            }
        }
    }

    CDC* pDC = GetDC();

    if ((abs((INT)m_sharpnessInfo[m_nCurrentAOI].u32SharpnessValue - m_s32Sharpness[m_nCurrentAOI])) < 2)
    {
        m_color = m_colorGreen;
    }
    else
    {
        m_color = m_colorRed;
    }

    pDC->SelectObject(&m_Font);

    RECT rcTest;

    INT nOffsetX  = 680;
    INT nOffsetY  = 754;

    rcTest.left = m_rcWindow.left + nOffsetX;
    rcTest.top  = m_rcWindow.top + nOffsetY;
    rcTest.right = m_rcWindow.left + nOffsetX + m_sharpnessInfo[m_nCurrentAOI].u32SharpnessValue;
    rcTest.bottom = m_rcWindow.top + nOffsetY + 16;
    pDC->FillSolidRect(&rcTest, m_color);  

    rcTest.left = m_rcWindow.left + nOffsetX + m_sharpnessInfo[m_nCurrentAOI].u32SharpnessValue;
    rcTest.top  = m_rcWindow.top + nOffsetY;
    rcTest.right = m_rcWindow.left + nOffsetX + 180;
    rcTest.bottom = m_rcWindow.top + nOffsetY + 16;
    pDC->FillSolidRect(&rcTest, GetSysColor(COLOR_BTNFACE)); 

    CString Str;
    Str.Format(L"%d", m_sharpnessInfo[m_nCurrentAOI].u32SharpnessValue);
    rcTest.left += 2;
    pDC->DrawText(Str, &rcTest, DT_LEFT);

    INT nOffsetY2 = nOffsetY + 16;

    rcTest.left = m_rcWindow.left + nOffsetX - 1;
    rcTest.top  = m_rcWindow.top + nOffsetY2;
    rcTest.right = m_rcWindow.left + nOffsetX + 180;
    rcTest.bottom = m_rcWindow.top + nOffsetY2 + 16;
    pDC->FillSolidRect(&rcTest, GetSysColor(COLOR_BTNFACE)); 

    pDC->MoveTo(m_rcWindow.left + nOffsetX + m_s32Sharpness[m_nCurrentAOI] - 1, m_rcWindow.top + nOffsetY2 + 14);
    pDC->LineTo(m_rcWindow.left + nOffsetX + m_s32Sharpness[m_nCurrentAOI] - 1, m_rcWindow.top + nOffsetY2);
    pDC->LineTo(m_rcWindow.left + nOffsetX - 1, m_rcWindow.top + nOffsetY2);
    pDC->LineTo(m_rcWindow.left + nOffsetX - 1, m_rcWindow.top + nOffsetY - 1);

    rcTest.left = m_rcWindow.left + nOffsetX + m_s32Sharpness[m_nCurrentAOI];
    Str.Format(L"%d", m_s32Sharpness[m_nCurrentAOI]);
    rcTest.left += 2;
    pDC->DrawText(Str, &rcTest, DT_LEFT);

    ReleaseDC(pDC);

    m_display.Invalidate();
}

INT CuEyeMeasureSharpnessDlg::GetImagePos(INT &xPos, INT &yPos)
{
    UINT nAbsPosX = 0;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_X_ABS, (void*) &nAbsPosX, sizeof(nAbsPosX));

    UINT nAbsPosY = 0;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_Y_ABS, (void*) &nAbsPosY, sizeof(nAbsPosY));

    IS_RECT rectAOI;

    INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_AOI, (void*) &rectAOI, sizeof(rectAOI));
    if (nRet == IS_SUCCESS)
    {
        xPos = !nAbsPosX ? rectAOI.s32X : 0;
        yPos = !nAbsPosY ? rectAOI.s32Y : 0;
    }

    return IS_SUCCESS;
}

void CuEyeMeasureSharpnessDlg::UpdateSizeControls()
{
    CString Str;

    MEASURE_SHARPNESS_AOI_INFO sharpnessInfo;
    sharpnessInfo.u32NumberAOI = m_nCurrentAOI;
    INT nRet = is_Measure(m_hCam, IS_MEASURE_CMD_SHARPNESS_AOI_INQUIRE, (void*)&sharpnessInfo, sizeof(sharpnessInfo));
    if (nRet == IS_SUCCESS)
    {
        Str.Format(L"%d", sharpnessInfo.rcAOI.s32X);
        GetDlgItem(IDC_EDIT_X)->SetWindowText(Str);

        Str.Format(L"%d", sharpnessInfo.rcAOI.s32Y);
        GetDlgItem(IDC_EDIT_Y)->SetWindowText(Str);

        Str.Format(L"%d", sharpnessInfo.rcAOI.s32Width);
        GetDlgItem(IDC_EDIT_WIDTH)->SetWindowText(Str);

        Str.Format(L"%d", sharpnessInfo.rcAOI.s32Height);
        GetDlgItem(IDC_EDIT_HEIGHT)->SetWindowText(Str);
    }
    else
    {
        SetDlgItemInt(IDC_EDIT_X, 0);
        SetDlgItemInt(IDC_EDIT_Y, 0);
        SetDlgItemInt(IDC_EDIT_WIDTH, 0);
        SetDlgItemInt(IDC_EDIT_HEIGHT, 0);
    }
}


void CuEyeMeasureSharpnessDlg::OnEnKillfocusEditSize()
{
    MEASURE_SHARPNESS_AOI_INFO sharpnessInfo;

    sharpnessInfo.u32NumberAOI     = m_nCurrentAOI;
    sharpnessInfo.rcAOI.s32X       = GetDlgItemInt(IDC_EDIT_X);
    sharpnessInfo.rcAOI.s32Y       = GetDlgItemInt(IDC_EDIT_Y); 
    sharpnessInfo.rcAOI.s32Width   = GetDlgItemInt(IDC_EDIT_WIDTH);
    sharpnessInfo.rcAOI.s32Height  = GetDlgItemInt(IDC_EDIT_HEIGHT); 

    is_Measure(m_hCam, IS_MEASURE_CMD_SHARPNESS_AOI_SET, (void*)&sharpnessInfo, sizeof(sharpnessInfo));
}


BOOL CuEyeMeasureSharpnessDlg::PreTranslateMessage(MSG *pMsg)
{
    if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
    {
        if(GetDlgItem(IDC_EDIT_WIDTH)->GetFocus() == GetDlgItem(IDC_EDIT_WIDTH))
        {
            OnEnKillfocusEditSize();
            return TRUE;
        }
        else if(GetDlgItem(IDC_EDIT_HEIGHT)->GetFocus() == GetDlgItem(IDC_EDIT_HEIGHT))
        {
            OnEnKillfocusEditSize();
            return TRUE;
        }
        else if(GetDlgItem(IDC_EDIT_X)->GetFocus() == GetDlgItem(IDC_EDIT_X))
        {
            OnEnKillfocusEditSize();
            return TRUE;
        }
        else if(GetDlgItem(IDC_EDIT_Y)->GetFocus() == GetDlgItem(IDC_EDIT_Y))
        {
            OnEnKillfocusEditSize();
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CuEyeMeasureSharpnessDlg::OnCbnSelchangeComboAoiPresets()
{
    INT nPreset = (INT)m_comboAoiPresets.GetItemData(m_comboAoiPresets.GetCurSel());

    is_Measure(m_hCam, IS_MEASURE_CMD_SHARPNESS_AOI_SET_PRESET, (void*)&nPreset, sizeof(nPreset));

    for (INT i = 0; i < 5; i++)
    {
        m_s32Sharpness[i] = 0;
    }
}

void CuEyeMeasureSharpnessDlg::OnBnClickedButtonClear()
{
    m_s32Sharpness[m_nCurrentAOI] = 0;
}

void CuEyeMeasureSharpnessDlg::OnCbnSelchangeComboAoi()
{
    m_nCurrentAOI = (INT)m_comboAoi.GetItemData(m_comboAoi.GetCurSel());

    UpdateSizeControls();
}

void CuEyeMeasureSharpnessDlg::OnBnClickedButtonResetAoi()
{
    m_sharpnessInfo[m_nCurrentAOI].rcAOI.s32X = 0;
    m_sharpnessInfo[m_nCurrentAOI].rcAOI.s32Y = 0;
    m_sharpnessInfo[m_nCurrentAOI].rcAOI.s32Width	= 0;
    m_sharpnessInfo[m_nCurrentAOI].rcAOI.s32Height	= 0;
    m_s32Sharpness[m_nCurrentAOI] = 0;

    is_Measure(m_hCam, IS_MEASURE_CMD_SHARPNESS_AOI_SET, (void*)&m_sharpnessInfo[m_nCurrentAOI], sizeof(m_sharpnessInfo[m_nCurrentAOI]));

    UpdateSizeControls();
}


BEGIN_MESSAGE_MAP(DisplayControl, CStatic)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


DisplayControl::DisplayControl()
{
    m_pParent = NULL;

    m_penRed.CreatePen(PS_SOLID, 1, RGB(240, 0, 0));
    m_penGreen.CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
}


DisplayControl::~DisplayControl()
{
    m_penRed.DeleteObject();
    m_penGreen.DeleteObject();
}


void DisplayControl::SetParent(CuEyeMeasureSharpnessDlg* pParent = NULL)
{
    m_pParent = pParent;
}


void DisplayControl::OnPaint()
{
    if (m_pParent == NULL)
    {
        return;
    }

    CPaintDC dc(this);

    INT nRet = is_RenderBitmap(m_pParent->m_hCam, m_pParent->m_lMemoryId, this->GetSafeHwnd(), m_pParent->m_nRenderMode);

    for (INT i = 0; i < 5 ; i++)
    {
        INT s32PosX  = m_pParent->m_sharpnessInfo[i].rcAOI.s32X;
        INT s32PosY  = m_pParent->m_sharpnessInfo[i].rcAOI.s32Y;
        INT s32PosX2 = m_pParent->m_sharpnessInfo[i].rcAOI.s32X + m_pParent->m_sharpnessInfo[i].rcAOI.s32Width  - 1;
        INT s32PosY2 = m_pParent->m_sharpnessInfo[i].rcAOI.s32Y + m_pParent->m_sharpnessInfo[i].rcAOI.s32Height - 1;

        if ((s32PosX != s32PosX2) && (s32PosY != s32PosY2))
        {
            int OffsetX = 0, OffsetY = 0;
            m_pParent->GetImagePos(OffsetX, OffsetY);

            /* Scale positions */
            s32PosX = (INT) ((double) (s32PosX - OffsetX) * m_pParent->m_f64FactorX);
            s32PosY = (INT) ((double) (s32PosY - OffsetY) * m_pParent->m_f64FactorY);
            s32PosX2 = (INT) ((double) (s32PosX2 - OffsetX) * m_pParent->m_f64FactorX);
            s32PosY2 = (INT) ((double) (s32PosY2 - OffsetY) * m_pParent->m_f64FactorY);

            if (i == m_pParent->m_nCurrentAOI)
            {
                dc.SelectObject(m_penRed);
            }
            else
            {
                dc.SelectObject(m_penGreen);
            }

            dc.MoveTo(s32PosX, s32PosY);
            dc.LineTo(s32PosX2, s32PosY);
            dc.LineTo(s32PosX2, s32PosY2);
            dc.LineTo(s32PosX, s32PosY2);
            dc.LineTo(s32PosX, s32PosY);

            if (m_pParent->m_sharpnessInfo[m_pParent->m_nCurrentAOI].u32SharpnessValue == m_pParent->m_s32Sharpness[m_pParent->m_nCurrentAOI])
            {
                dc.SelectObject(m_penGreen);
            }
            else
            {
                dc.SelectObject(m_penRed);
            }
        }
    }

    /* Current mouse rectangle */
    if ((m_nStartX != -1) && (m_nCurrX != -1))
    {
        INT s32TempX = ((INT)((double)m_nStartX / m_pParent->m_f64FactorX));
        s32TempX = (INT)((double)s32TempX * m_pParent->m_f64FactorX);

        INT s32TempY = ((INT)((double)m_nStartY / m_pParent->m_f64FactorY));
        s32TempY = (INT)((double)s32TempY * m_pParent->m_f64FactorY);

        INT s32TempX2 = ((INT)((double)m_nCurrX / m_pParent->m_f64FactorX));
        s32TempX2 = (INT)((double)s32TempX2 * m_pParent->m_f64FactorX);

        INT s32TempY2 = ((INT)((double)m_nCurrY / m_pParent->m_f64FactorY));
        s32TempY2 = (INT)((double)s32TempY2 * m_pParent->m_f64FactorY);

        dc.SelectObject(m_penRed);

        dc.MoveTo(s32TempX,  s32TempY);
        dc.LineTo(s32TempX2, s32TempY);
        dc.LineTo(s32TempX2, s32TempY2);
        dc.LineTo(s32TempX,  s32TempY2);
        dc.LineTo(s32TempX,  s32TempY);
    }
}

void DisplayControl::OnLButtonDown(UINT nFlags, CPoint point)
{  
    m_nStartX = point.x;
    m_nStartY = point.y;  

    m_nCurrX = -1;
    m_nCurrY = -1;
}


void DisplayControl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if ((m_nStartX != -1) && (m_nCurrX != -1))
    {
        if (m_nCurrX < m_nStartX)
        {
            // switch
            INT ntemp = m_nStartX;
            m_nStartX = m_nCurrX;
            m_nCurrX = ntemp;
        }

        if (m_nCurrY < m_nStartY)
        {
            // switch
            INT ntemp = m_nStartY;
            m_nStartY = m_nCurrY;
            m_nCurrY = ntemp;
        }

        int xPos, yPos, width, height;
        int OffsetX = 0, OffsetY = 0;

        m_pParent->GetImagePos(OffsetX, OffsetY);

        xPos = (INT) ((double) m_nStartX / m_pParent->m_f64FactorX + OffsetX);
        yPos = (INT) ((double) m_nStartY / m_pParent->m_f64FactorY + OffsetY);
        width = (INT) ((double) (m_nCurrX - m_nStartX + 1) / m_pParent->m_f64FactorX);
        height = (INT) ((double) (m_nCurrY - m_nStartY + 1) / m_pParent->m_f64FactorY);

        /* Calculate position in original image */
        m_pParent->m_sharpnessInfo[m_pParent->m_nCurrentAOI].u32NumberAOI = m_pParent->m_nCurrentAOI;
        m_pParent->m_sharpnessInfo[m_pParent->m_nCurrentAOI].rcAOI.s32X = xPos;
        m_pParent->m_sharpnessInfo[m_pParent->m_nCurrentAOI].rcAOI.s32Y = yPos;
        m_pParent->m_sharpnessInfo[m_pParent->m_nCurrentAOI].rcAOI.s32Width = width;
        m_pParent->m_sharpnessInfo[m_pParent->m_nCurrentAOI].rcAOI.s32Height = height;

        is_Measure(m_pParent->m_hCam, IS_MEASURE_CMD_SHARPNESS_AOI_SET, (void*)&m_pParent->m_sharpnessInfo[m_pParent->m_nCurrentAOI], sizeof(m_pParent->m_sharpnessInfo[m_pParent->m_nCurrentAOI]));

        m_nStartX = -1;

        m_pParent->m_s32Sharpness[m_pParent->m_nCurrentAOI] = 0;

        m_pParent->UpdateSizeControls();

        m_pParent->m_comboAoiPresets.SetCurSel(0);
    }
    else
    {
        RECT rc;

        for (INT i = 0; i < 5; i++)
        {
            rc.left = m_pParent->m_sharpnessInfo[i].rcAOI.s32X;
            rc.top  = m_pParent->m_sharpnessInfo[i].rcAOI.s32Y;
            rc.right = m_pParent->m_sharpnessInfo[i].rcAOI.s32X + m_pParent->m_sharpnessInfo[i].rcAOI.s32Width - 1;
            rc.bottom = m_pParent->m_sharpnessInfo[i].rcAOI.s32Y + m_pParent->m_sharpnessInfo[i].rcAOI.s32Height - 1;

            rc.left  = (INT)((double)rc.left * m_pParent->m_f64FactorX);
            rc.top  = (INT)((double)rc.top * m_pParent->m_f64FactorY);
            rc.right = (INT)((double)rc.right * m_pParent->m_f64FactorX);
            rc.bottom = (INT)((double)rc.bottom * m_pParent->m_f64FactorY);


            if (PtInRect(&rc, point))
            {
                m_pParent->m_nCurrentAOI = i;
                m_pParent->UpdateSizeControls();
                m_pParent->m_comboAoi.SetCurSel(i);
            }
        }
    }
}


void DisplayControl::OnMouseMove(UINT nFlags, CPoint point)
{
    if (nFlags & MK_LBUTTON)
    {
        m_nCurrX = point.x;
        m_nCurrY = point.y;
    }
}
