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
#include "uEyeConvertImage.h"
#include "uEyeConvertImageDlg.h"
#include "version.h"

extern ConvertImageApp theApp;

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

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

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
END_MESSAGE_MAP()


ConvertImageDlg::ConvertImageDlg(CWnd* pParent /*=NULL*/)
: CDialog(ConvertImageDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void ConvertImageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_PIXELFORMAT, m_comboColorFormat);
    DDX_Control(pDX, IDC_COMBO_PIXELFORMAT_RAW, m_comboColorFormatRaw);
    DDX_Control(pDX, IDC_COMBO_PIXELCONVERTER, m_comboPixelConverter);
    DDX_Control(pDX, IDC_COMBO_COLORCORRECTION, m_comboColorCorrection);
    DDX_Control(pDX, IDC_SLIDER_SATURATION, m_sliderSaturation);
    DDX_Control(pDX, IDC_SLIDER_GAMMA, m_sliderGamma);
    DDX_Control(pDX, IDC_SLIDER_EDGE_ENHANCEMENT, m_sliderEdgeEnhancement);
    DDX_Check(pDX, IDC_CHECK_SHOW_ORIGINAL, m_bShowOriginalImage);
    DDX_Check(pDX, IDC_CHECK_SHOW_ORIGINAL_SIZE, m_bShowOriginalSize);
}


BEGIN_MESSAGE_MAP(ConvertImageDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage)
    ON_WM_CLOSE()
    ON_CBN_SELCHANGE(IDC_COMBO_PIXELFORMAT, OnCbnSelchangeComboPixelformat)
    ON_CBN_SELCHANGE(IDC_COMBO_PIXELCONVERTER, OnCbnSelchangeComboPixelconverter)
    ON_CBN_SELCHANGE(IDC_COMBO_COLORCORRECTION, OnCbnSelchangeComboColorcorrection)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_CHECK_SHOW_ORIGINAL, OnBnClickedCheckShowOriginal)
    ON_BN_CLICKED(IDC_BUTTON_CONVERT, OnBnClickedButtonConvert)
    ON_BN_CLICKED(IDC_CHECK_SHOW_ORIGINAL_SIZE, OnBnClickedCheckOriginalSize)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SATURATION, &ConvertImageDlg::OnDeltaposSpinSaturation)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GAMMA, &ConvertImageDlg::OnDeltaposSpinGamma)
    ON_EN_KILLFOCUS(IDC_EDIT_SATURATION, &ConvertImageDlg::OnEnKillfocusEditSaturation)
    ON_EN_KILLFOCUS(IDC_EDIT_GAMMA, &ConvertImageDlg::OnEnKillfocusEditGamma)
    ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &ConvertImageDlg::OnBnClickedButtonDefault)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EDGE_ENHANCEMENT, &ConvertImageDlg::OnDeltaposSpinEdgeEnhancement)
    ON_EN_KILLFOCUS(IDC_EDIT_EDGE_ENHANCEMENT, &ConvertImageDlg::OnEnKillfocusEditEdgeEnhancement)
END_MESSAGE_MAP()


BOOL ConvertImageDlg::OnInitDialog()
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

    m_hWndDisplay = GetDlgItem(IDC_DISPLAY)->m_hWnd;

    m_pSourceBuffer = NULL;
    m_nSourceId = 0;
    m_pDestBuffer = NULL;
    m_nDestId = 0;
    m_hCam = 0;
    m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;

    m_bShowOriginalImage = FALSE;
    m_bShowOriginalSize  = FALSE;
    UpdateData(FALSE);

    OpenCamera();

    ZeroMemory(&m_cameraInfo, sizeof(m_cameraInfo));
    is_GetCameraInfo(m_hCam, &m_cameraInfo);

    FillComboColorFormat();
    FillComboColorFormatRaw();
    FillComboPixelConverter();
    FillComboColorCorrection();
    InitSliderSaturation();
    InitSliderGamma();
    InitSliderEdgeEnhancement();

    /* Init default values */
    m_ConversionParams.nDestPixelFormat = IS_CM_BGRA8_PACKED;
    m_ConversionParams.nDestPixelConverter = IS_CONV_MODE_SOFTWARE_3X3;
    m_ConversionParams.nDestColorCorrectionMode = IS_CCOR_DISABLE;
    m_ConversionParams.nDestGamma = 100;
    m_ConversionParams.nDestSaturationU = 100;
    m_ConversionParams.nDestSaturationV = 100;
    m_ConversionParams.nDestEdgeEnhancement = 0;

    m_pbmpInfo = NULL;
    UpdateBitmapInfo();

    m_bShowYUVWarning = TRUE;

    m_nPlanarLayer = -1;

    return true;
}


void ConvertImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void ConvertImageDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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


HCURSOR ConvertImageDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


void ConvertImageDlg::OnBnClickedButtonConvert()
{
    switch (m_sInfo.SensorID)
    {
    case IS_SENSOR_UI1360M:
    case IS_SENSOR_UI1360C:
    case IS_SENSOR_UI1370M:
    case IS_SENSOR_UI1370C:
        {
            UINT nPclk = 80;
            INT nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_SET, (void*)&nPclk, sizeof(nPclk));
        }
        break;
    }

    is_FreezeVideo(m_hCam, IS_WAIT);
}


void ConvertImageDlg::OnButtonExit()
{	
    if (m_pbmpInfo != NULL)
    {
        delete [] m_pbmpInfo;
        m_pbmpInfo = NULL;
    }

    ExitCamera();
    PostQuitMessage(0);
}


BOOL ConvertImageDlg::OpenCamera()
{
    INT nRet = IS_NO_SUCCESS;
    ExitCamera();

    /* Init camera (open next available camera) */
    m_hCam = (HIDS) 0;
    nRet = InitCamera(&m_hCam, m_hWndDisplay);
    if (nRet == IS_SUCCESS)
    {
        /* Get sensor info */
        is_GetSensorInfo(m_hCam, &m_sInfo);

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        UpdateData(TRUE);
        nRet = InitDisplayMode();

        if (nRet == IS_SUCCESS)
        {
            is_EnableMessage(m_hCam, IS_FRAME, GetSafeHwnd());
        }
        else
        {
            AfxMessageBox(TEXT("Initializing the display mode failed!"), MB_ICONWARNING);
        }

        return true;
    }
    else
    {
        AfxMessageBox(TEXT("No uEye camera could be opened !"), MB_ICONWARNING);
        return false;
    }
}


LRESULT ConvertImageDlg::OnUEyeMessage(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case IS_FRAME:
        {
            if (m_pSourceBuffer != NULL)
            {
                /*
                Be careful with the CMOSIS camera: The bayer pattern is shifted by 1 line.
                Only the y-mirrored image has the correct pattern. So we mirror the bayer image
                and mirror it back afterwards
                */
                switch (m_sInfo.SensorID)
                {
                case IS_SENSOR_UI1360M:
                case IS_SENSOR_UI1360C:
                case IS_SENSOR_UI1370M:
                case IS_SENSOR_UI1370C:
                    {
                        INT nBpp = m_nBitsPerPixel / 8;
                        char* pTemp = new char[m_nSizeY * m_nSizeX * nBpp];
                        if (pTemp)
                        {
                            INT nPitch = m_nSizeX * nBpp;
                            for (INT j = 0; j < m_nSizeY; j++)
                            {
                                memcpy(pTemp + (j * nPitch), m_pSourceBuffer + ((m_nSizeY - j - 1) * nPitch), nPitch);
                            }

                            memcpy(m_pSourceBuffer, pTemp, m_nSizeY * nPitch);

                            delete [] pTemp;
                        }
                    }
                    break;
                }

                /*
                These parameters must be set again, because the reallocation of the
                raw format has changed the pointer
                */
                m_ConversionParams.pSourceBuffer = m_pSourceBuffer;
                m_ConversionParams.pDestBuffer = m_pDestBuffer;


                LARGE_INTEGER nStart;
                LARGE_INTEGER nStop;

                nStart.QuadPart = nStop.QuadPart = 0;
                ::QueryPerformanceCounter(&nStart);

                INT nRet = is_Convert(m_hCam,
                    IS_CONVERT_CMD_APPLY_PARAMS_AND_CONVERT_BUFFER,
                    (void*)&m_ConversionParams,
                    sizeof(m_ConversionParams)
                    );

                ::QueryPerformanceCounter(&nStop);

                LARGE_INTEGER nFrequency;
                ::QueryPerformanceFrequency(&nFrequency);
                LONGLONG nElapsed = nStop.QuadPart - nStart.QuadPart;
                double dblMilliSeconds = ((double)nElapsed / (double)nFrequency.QuadPart) * 1000;
                CString Str;
                Str.Format(L"Conversion time: %2.2f ms", dblMilliSeconds);
                GetDlgItem(IDC_CONVERSION_TIME)->SetWindowText(Str);

                if (nRet == IS_SUCCESS)
                {
                    switch (m_sInfo.SensorID)
                    {
                    case IS_SENSOR_UI1360M:
                    case IS_SENSOR_UI1360C:
                    case IS_SENSOR_UI1370M:
                    case IS_SENSOR_UI1370C:
                        {
                            INT nBpp = GetBitsPerPixel() / 8;
                            char* pTemp = new char[m_nSizeY * m_nSizeX * nBpp];
                            if (pTemp)
                            {
                                INT nPitch = m_nSizeX * nBpp;
                                for (INT j = 0; j < m_nSizeY; j++)
                                {
                                    memcpy(pTemp + (j * nPitch), m_pDestBuffer + ((m_nSizeY - j - 1) * nPitch), nPitch);
                                }

                                memcpy(m_pDestBuffer, pTemp, m_nSizeY * nPitch);
                                delete [] pTemp;
                            }
                        }
                        break;
                    }

                    if (m_bShowOriginalImage)
                    {
                        /* Use uEye display function */
                        is_RenderBitmap(m_hCam, m_nSourceId, m_hWndDisplay, m_nRenderMode);
                    }
                    else
                    {
                        /* Use own function */
                        DisplayImage();
                    }
                }
                else if (nRet = IS_INVALID_PARAMETER)
                {
                    AfxMessageBox(L"Error: Invalid parameter!");
                }
                else
                {
                    AfxMessageBox(L"Error: Conversion!");
                }
            }
        }
        break;
    }

    return 0;
}


void ConvertImageDlg::ExitCamera()
{
    if (m_hCam != 0)
    {
        /* Disable messages */
        is_EnableMessage(m_hCam, IS_FRAME, NULL);

        /* Free the allocated buffer */
        if (m_pSourceBuffer != NULL)
        {
            is_FreeImageMem(m_hCam, m_pSourceBuffer, m_nSourceId);
            m_pSourceBuffer = NULL;
        }

        if (m_pDestBuffer != NULL)
        {
            is_FreeImageMem( m_hCam, m_pDestBuffer, m_nDestId);
            m_pDestBuffer = NULL;
        }

        /* Close camera */
        is_ExitCamera(m_hCam);
        m_hCam = NULL;
    }
}


int ConvertImageDlg::InitDisplayMode()
{
    INT nRet = IS_NO_SUCCESS;

    if (m_hCam == NULL)
    {
        return IS_NO_SUCCESS;
    }


    if (m_pSourceBuffer != NULL)
    {
        is_FreeImageMem( m_hCam, m_pSourceBuffer, m_nSourceId);
        m_pSourceBuffer = NULL;
    }

    /* Set display mode to DIB */
    nRet = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);

    /* Set color mode to RAW 8 */
    m_nPixelFormat = IS_CM_SENSOR_RAW8;
    m_nBitsPerPixel = 8;

    /* Allocate an image memory */
    if (is_AllocImageMem(m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pSourceBuffer, &m_nSourceId) != IS_SUCCESS)
    {
        AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING );
    }
    else
    {
        is_SetImageMem( m_hCam, m_pSourceBuffer, m_nSourceId);
    }

    if (nRet == IS_SUCCESS)
    {
        is_SetColorMode(m_hCam, m_nPixelFormat);

        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
    }

    return nRet;
}


void ConvertImageDlg::OnClose()
{
    if (m_pbmpInfo != NULL)
    {
        delete [] m_pbmpInfo;
        m_pbmpInfo = NULL;
    }

    ExitCamera();
    CDialog::OnClose();
}


INT ConvertImageDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void ConvertImageDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
    // Check if the camera supports an arbitrary AOI
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
        // Get maximum image size
        SENSORINFO sInfo;
        is_GetSensorInfo (m_hCam, &sInfo);
        *pnSizeX = sInfo.nMaxWidth;
        *pnSizeY = sInfo.nMaxHeight;
    }
    else
    {
        // Get image size of the current format
        IS_SIZE_2D imageSize;
        is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

        *pnSizeX = imageSize.s32Width;
        *pnSizeY = imageSize.s32Height;
    }
}


void ConvertImageDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    switch (pScrollBar->GetDlgCtrlID())
    {
    case IDC_SLIDER_SATURATION:
        {
            m_ConversionParams.nDestSaturationU = m_sliderSaturation.GetPos();
            m_ConversionParams.nDestSaturationV = m_sliderSaturation.GetPos();
            CString Str;
            Str.Format(L"%1.2f", ((double)m_sliderSaturation.GetPos()) / 100.0);
            GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(Str);
        }
        break;

    case IDC_SLIDER_GAMMA:
        {
            m_ConversionParams.nDestGamma = m_sliderGamma.GetPos();
            CString Str;
            Str.Format(L"%1.2f", ((double)m_sliderGamma.GetPos()) / 100.0);
            GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(Str);
        }
        break;

    case IDC_SLIDER_EDGE_ENHANCEMENT:
        {
            m_ConversionParams.nDestEdgeEnhancement = m_sliderEdgeEnhancement.GetPos();
            CString Str;
            Str.Format(L"%d", m_sliderEdgeEnhancement.GetPos());
            GetDlgItem(IDC_EDIT_EDGE_ENHANCEMENT)->SetWindowText(Str);
        }
        break;
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL ConvertImageDlg::PreTranslateMessage(MSG* pMsg)
{
    if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
    {
        if (GetDlgItem(IDC_EDIT_SATURATION)->GetFocus() == GetDlgItem(IDC_EDIT_SATURATION))
        {
            OnEnKillfocusEditSaturation();
            return TRUE;
        }
        else if (GetDlgItem(IDC_EDIT_GAMMA)->GetFocus() == GetDlgItem(IDC_EDIT_GAMMA))
        {
            OnEnKillfocusEditGamma();
            return TRUE;
        }
        else if (GetDlgItem(IDC_EDIT_EDGE_ENHANCEMENT)->GetFocus() == GetDlgItem(IDC_EDIT_EDGE_ENHANCEMENT))
        {
            OnEnKillfocusEditEdgeEnhancement();
            return TRUE;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}


void ConvertImageDlg::OnBnClickedButtonDefault()
{
    m_ConversionParams.nDestPixelFormat = IS_CM_BGRA8_PACKED;
    m_ConversionParams.nDestPixelConverter = IS_CONV_MODE_SOFTWARE_3X3;
    m_ConversionParams.nDestColorCorrectionMode = IS_CCOR_DISABLE;
    m_ConversionParams.nDestGamma = 100;
    m_ConversionParams.nDestSaturationU = 100;
    m_ConversionParams.nDestSaturationV = 100;
    m_ConversionParams.nDestEdgeEnhancement = 0;

    for (INT i = 0; i < m_comboColorFormat.GetCount(); i++)
    {
        if ((INT)m_comboColorFormat.GetItemData(i) == IS_CM_BGRA8_PACKED)
        {
            m_nSelectedIndexComboDestFormat = i;
        }
    }

    m_comboColorFormat.SetCurSel(m_nSelectedIndexComboDestFormat);
    OnCbnSelchangeComboPixelformat();

    m_comboPixelConverter.SetCurSel(0);
    m_comboColorCorrection.SetCurSel(0);

    CString Str;
    Str.Format(L"%1.2f", 1.00);
    GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(Str);
    m_sliderSaturation.SetPos(100);

    GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(Str);
    m_sliderGamma.SetPos(100);

    GetDlgItem(IDC_EDIT_EDGE_ENHANCEMENT)->SetWindowText(L"0");
    m_sliderEdgeEnhancement.SetPos(0);

    m_bShowOriginalImage = FALSE;
    m_bShowOriginalSize  = FALSE;
    UpdateData(FALSE);

    m_nPlanarLayer = -1;

    OnBnClickedCheckOriginalSize();
}


/*****************************************************************/
/*  Pixel format                                                 */
/*****************************************************************/
void ConvertImageDlg::FillComboColorFormat()
{
    INT nIndex = 0;

    m_comboColorFormat.InsertString(nIndex, L"------------------------------");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    m_comboColorFormat.InsertString(nIndex, L"Color pixel formats");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    m_comboColorFormat.InsertString(nIndex, L"------------------------------");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    if (CheckPixelFormat(IS_CM_BGRA12_UNPACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"BGRA12 (64 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_BGRA12_UNPACKED);
    }

    if (CheckPixelFormat(IS_CM_RGBA12_UNPACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"RGBA12 (64 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGBA12_UNPACKED);
    }

    if (CheckPixelFormat(IS_CM_BGR12_UNPACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"BGR12 (48 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_BGR12_UNPACKED);
    }

    if (CheckPixelFormat(IS_CM_RGB12_UNPACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"RGB12 (48 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGB12_UNPACKED);
    }

    if (CheckPixelFormat(IS_CM_BGR10_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"BGR10 packed (32 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_BGR10_PACKED);
    }

    if (CheckPixelFormat(IS_CM_BGR10_UNPACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"BGR10 (48 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_BGR10_UNPACKED);
    }

    if (CheckPixelFormat(IS_CM_BGR10_UNPACKED | IS_CM_PREFER_PACKED_SOURCE_FORMAT))
    {
        m_comboColorFormat.InsertString(nIndex, L"BGR10 (48 bits, ps)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_BGR10_UNPACKED | IS_CM_PREFER_PACKED_SOURCE_FORMAT);
    }

    if (CheckPixelFormat(IS_CM_RGB10_UNPACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"RGB10 (48 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGB10_UNPACKED);
    }

    if (CheckPixelFormat(IS_CM_RGB10_UNPACKED | IS_CM_PREFER_PACKED_SOURCE_FORMAT))
    {
        m_comboColorFormat.InsertString(nIndex, L"RGB10 (48 bits, ps)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGB10_UNPACKED | IS_CM_PREFER_PACKED_SOURCE_FORMAT);
    }

    if (CheckPixelFormat(IS_CM_BGRA8_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"BGRA8 (32 bits)");
        m_nSelectedIndexComboDestFormat = nIndex;
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_BGRA8_PACKED);
    }

    if (CheckPixelFormat(IS_CM_RGBA8_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"RGBA8 (32 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGBA8_PACKED);
    }

    if (CheckPixelFormat(IS_CM_BGR8_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"BGR8 (24 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_BGR8_PACKED);
    }

    if (CheckPixelFormat(IS_CM_RGB8_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"RGB8 (24 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGB8_PACKED);
    }

    if (CheckPixelFormat(IS_CM_BGR565_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"BGR565 (16 bits)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_BGR565_PACKED);
    }

    m_comboColorFormat.InsertString(nIndex, L"------------------------------");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    m_comboColorFormat.InsertString(nIndex, L"Planar pixel formats");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    m_comboColorFormat.InsertString(nIndex, L"------------------------------");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    if (CheckPixelFormat(IS_CM_RGB8_PLANAR))
    {
        m_comboColorFormat.InsertString(nIndex, L"RGB8 Planar (all)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGB8_PLANAR);

        m_comboColorFormat.InsertString(nIndex, L"RGB8 Planar (r)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGB8_PLANAR);

        m_comboColorFormat.InsertString(nIndex, L"RGB8 Planar (g)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGB8_PLANAR);

        m_comboColorFormat.InsertString(nIndex, L"RGB8 Planar (b)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_RGB8_PLANAR);
    }

    m_comboColorFormat.InsertString(nIndex, L"------------------------------");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    m_comboColorFormat.InsertString(nIndex, L"Mono pixel formats");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    m_comboColorFormat.InsertString(nIndex, L"------------------------------");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    if (CheckPixelFormat(IS_CM_MONO8))
    {
        m_comboColorFormat.InsertString(nIndex, L"MONO8");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_MONO8);
    }

    if (CheckPixelFormat(IS_CM_MONO10))
    {
        m_comboColorFormat.InsertString(nIndex, L"MONO10");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_MONO10);
    }

    if (CheckPixelFormat(IS_CM_MONO10 | IS_CM_PREFER_PACKED_SOURCE_FORMAT))
    {
        m_comboColorFormat.InsertString(nIndex, L"MONO10 (ps)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_MONO10 | IS_CM_PREFER_PACKED_SOURCE_FORMAT);
    }

    if (CheckPixelFormat(IS_CM_MONO12))
    {
        m_comboColorFormat.InsertString(nIndex, L"MONO12");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_MONO12);
    }

    m_comboColorFormat.InsertString(nIndex, L"------------------------------");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    m_comboColorFormat.InsertString(nIndex, L"YUV pixel formats");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    m_comboColorFormat.InsertString(nIndex, L"------------------------------");
    m_comboColorFormat.SetItemData(nIndex++, -1);

    if (CheckPixelFormat(IS_CM_UYVY_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"UYVY (YUV422)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_UYVY_PACKED);
    }

    if (CheckPixelFormat(IS_CM_UYVY_MONO_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"UYVY (YUV422 Mono)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_UYVY_MONO_PACKED);
    }

    if (CheckPixelFormat(IS_CM_UYVY_BAYER_PACKED))
    {
        m_comboColorFormat.InsertString(nIndex, L"UYVY (YUV422 Bayer)");
        m_comboColorFormat.SetItemData(nIndex++, IS_CM_UYVY_BAYER_PACKED);
    }

    m_comboColorFormat.SetCurSel(m_nSelectedIndexComboDestFormat);
}


void ConvertImageDlg::FillComboColorFormatRaw()
{
    m_comboColorFormatRaw.InsertString(0, L"RAW8");
    m_comboColorFormatRaw.InsertString(1, L"RAW10");
    m_comboColorFormatRaw.InsertString(2, L"RAW10 packed");
    m_comboColorFormatRaw.InsertString(3, L"RAW12");
    m_comboColorFormatRaw.InsertString(4, L"RAW16");

    m_comboColorFormatRaw.SetCurSel(0);
}


BOOL ConvertImageDlg::CheckPixelFormat(INT nPixelFormat)
{
    INT nCurrent, nDefault, nSupported;

    if (is_GetColorConverter(m_hCam, nPixelFormat, &nCurrent, &nDefault, &nSupported) != IS_SUCCESS)
    {
        return FALSE;
    }

    if ((nSupported & IS_CONV_MODE_SOFTWARE_3X3) ||
        (nSupported & IS_CONV_MODE_SOFTWARE_5X5) ||
        (nSupported & IS_CONV_MODE_SOFTWARE))
    {
        return TRUE;
    }

    return FALSE;
}


void ConvertImageDlg::OnCbnSelchangeComboPixelformat()
{
    INT nDestPixelFormat = (INT)m_comboColorFormat.GetItemData(m_comboColorFormat.GetCurSel());

    if (nDestPixelFormat == -1)
    {
        m_comboColorFormat.SetCurSel(m_nSelectedIndexComboDestFormat);
        return;
    }

    if (nDestPixelFormat == IS_CM_RGB8_PLANAR)
    {
        CString Str;
        m_comboColorFormat.GetLBText(m_comboColorFormat.GetCurSel(), Str);

        if ((Str.Find(L"(all)")) != -1)
        {
            m_nPlanarLayer = -1;
        }
        else if ((Str.Find(L"(r)")) != -1)
        {
            m_nPlanarLayer = 2;
        }
        else if ((Str.Find(L"(g)")) != -1)
        {
            m_nPlanarLayer = 1;
        }
        else if ((Str.Find(L"(b)")) != -1)
        {
            m_nPlanarLayer = 0;
        }
    }


    /* Mono 12 is only possible with the 12 bit raw source format */
    if ((nDestPixelFormat == IS_CM_MONO12)          ||
        (nDestPixelFormat == IS_CM_BGR10_PACKED)    ||
        (nDestPixelFormat == IS_CM_RGB10_PACKED)    ||
        (nDestPixelFormat == IS_CM_RGB12_UNPACKED)  ||
        (nDestPixelFormat == IS_CM_BGR12_UNPACKED)  ||
        (nDestPixelFormat == IS_CM_RGBA12_UNPACKED) ||
        (nDestPixelFormat == IS_CM_BGRA12_UNPACKED)
        )
    {
        m_nPixelFormat = IS_CM_SENSOR_RAW12;
        m_nBitsPerPixel = 16;
        m_comboColorFormatRaw.SetCurSel(3);
    }
    else if ((nDestPixelFormat == IS_CM_MONO10)     ||
        (nDestPixelFormat == IS_CM_BGR10_UNPACKED)  ||
        (nDestPixelFormat == IS_CM_RGB10_UNPACKED))
    {
        m_nPixelFormat = IS_CM_SENSOR_RAW10;
        m_nBitsPerPixel = 16;
        m_comboColorFormatRaw.SetCurSel(1);
    }
    else if ((nDestPixelFormat == (IS_CM_MONO10 | IS_CM_PREFER_PACKED_SOURCE_FORMAT))       ||
        (nDestPixelFormat == (IS_CM_BGR10_UNPACKED | IS_CM_PREFER_PACKED_SOURCE_FORMAT))    ||
        (nDestPixelFormat == (IS_CM_RGB10_UNPACKED | IS_CM_PREFER_PACKED_SOURCE_FORMAT)))
    {
        nDestPixelFormat &= ~IS_CM_PREFER_PACKED_SOURCE_FORMAT;
        m_nPixelFormat = IS_CM_SENSOR_RAW10 | IS_CM_PREFER_PACKED_SOURCE_FORMAT;
        m_nBitsPerPixel = 16;
        m_comboColorFormatRaw.SetCurSel(2);
    }
    /* in all other case we need the 8 bit raw format */
    else
    {
        m_nPixelFormat = IS_CM_SENSOR_RAW8;
        m_nBitsPerPixel = 8;
        m_comboColorFormatRaw.SetCurSel(0);
    }

    m_ConversionParams.nDestPixelFormat = nDestPixelFormat;

    m_nSelectedIndexComboDestFormat = m_comboColorFormat.GetCurSel();

    /* Reallocate memory */
    if (m_pSourceBuffer)
    {
        is_FreeImageMem(m_hCam, m_pSourceBuffer, m_nSourceId);
        m_pSourceBuffer = NULL;
    }

    if (is_AllocImageMem(m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pSourceBuffer, &m_nSourceId) != IS_SUCCESS)
    {
        AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING );
    }
    else
    {
        is_SetImageMem(m_hCam, m_pSourceBuffer, m_nSourceId);
    }

    /* Set the desired pixel format */
    is_SetColorMode(m_hCam, m_nPixelFormat);

    UpdateBitmapInfo();
}


/*****************************************************************/
/*  Pixel converter                                              */
/*****************************************************************/
void ConvertImageDlg::FillComboPixelConverter()
{
    if (m_sInfo.nColorMode == IS_COLORMODE_BAYER)
    {
        m_comboPixelConverter.InsertString(0, L"Debayer 3x3");
        m_comboPixelConverter.SetItemData(0, IS_CONV_MODE_SOFTWARE_3X3);

        m_comboPixelConverter.InsertString(1, L"Debayer 5x5");
        m_comboPixelConverter.SetItemData(1, IS_CONV_MODE_SOFTWARE_5X5);

        m_comboPixelConverter.SetCurSel(0);
    }
    else
    {
        m_comboPixelConverter.InsertString(0, L"Debayer 3x3");
        m_comboPixelConverter.SetItemData(0, IS_CONV_MODE_SOFTWARE);

        m_comboPixelConverter.SetCurSel(0);

        m_comboPixelConverter.EnableWindow(FALSE);
        m_comboColorCorrection.EnableWindow(FALSE);

        m_sliderSaturation.EnableWindow(FALSE);
        GetDlgItem(IDC_MIN_SATURATION)->EnableWindow(FALSE);
        GetDlgItem(IDC_MAX_SATURATION)->EnableWindow(FALSE);
    }
}


void ConvertImageDlg::OnCbnSelchangeComboPixelconverter()
{
    m_ConversionParams.nDestPixelConverter = (INT)m_comboPixelConverter.GetItemData(m_comboPixelConverter.GetCurSel());
}


/*****************************************************************/
/*  Color converter                                              */
/*****************************************************************/
void ConvertImageDlg::FillComboColorCorrection()
{
    m_comboColorCorrection.InsertString(0, L"Off");
    m_comboColorCorrection.SetItemData(0, IS_CCOR_DISABLE);

    m_comboColorCorrection.InsertString(1, L"Normal");
    m_comboColorCorrection.SetItemData(1, IS_CCOR_ENABLE_NORMAL);

    m_comboColorCorrection.InsertString(2, L"BG40");
    m_comboColorCorrection.SetItemData(2, IS_CCOR_ENABLE_BG40_ENHANCED);

    m_comboColorCorrection.InsertString(3, L"HQ");
    m_comboColorCorrection.SetItemData(3, IS_CCOR_ENABLE_HQ_ENHANCED);

    m_comboColorCorrection.SetCurSel(0);
}


void ConvertImageDlg::OnCbnSelchangeComboColorcorrection()
{
    m_ConversionParams.nDestColorCorrectionMode =
        (INT)m_comboColorCorrection.GetItemData(m_comboColorCorrection.GetCurSel());
}


/*****************************************************************/
/*  Saturation                                                   */
/*****************************************************************/
void ConvertImageDlg::InitSliderSaturation()
{
    m_sliderSaturation.SetRange(IS_MIN_SATURATION_U, IS_MAX_SATURATION_U);
    m_sliderSaturation.SetPos(IS_DEFAULT_SATURATION_U);
    GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(L"1.00");
}


void ConvertImageDlg::OnDeltaposSpinSaturation(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    INT nSaturation = m_sliderSaturation.GetPos();

    if (pNMUpDown->iDelta < 0)
    {
        nSaturation++;
    }
    else
    {
        nSaturation--;
    }

    if (nSaturation < 0)
    {
        nSaturation = 0;
    }
    else if (nSaturation > 200)
    {
        nSaturation = 200;
    }

    m_sliderSaturation.SetPos(nSaturation);
    m_ConversionParams.nDestSaturationU = nSaturation;
    m_ConversionParams.nDestSaturationV = nSaturation;
    CString Str;
    Str.Format(L"%1.2f", ((double)nSaturation) / 100.0);
    GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(Str);

    *pResult = 0;
}


void ConvertImageDlg::OnEnKillfocusEditSaturation()
{
    CString Str;
    GetDlgItem(IDC_EDIT_SATURATION)->GetWindowText(Str);

    INT nSaturation = (INT)(_tstof(Str) * 100.0);

    if (nSaturation < 0)
    {
        nSaturation = 0;
    }
    else if (nSaturation > 200)
    {
        nSaturation = 200;
    }

    m_sliderSaturation.SetPos(nSaturation);
    m_ConversionParams.nDestSaturationU = nSaturation;
    m_ConversionParams.nDestSaturationV = nSaturation;
    Str.Format(L"%1.2f", ((double)nSaturation) / 100.0);
    GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(Str);
}


/*****************************************************************/
/*  Gamma                                                        */
/*****************************************************************/
void ConvertImageDlg::InitSliderGamma()
{
    m_sliderGamma.SetRange(100, 220);
    m_sliderGamma.SetPos(100);
    GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(L"1.00");
}


void ConvertImageDlg::OnDeltaposSpinGamma(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    INT nGamma = m_sliderGamma.GetPos();

    if (pNMUpDown->iDelta < 0)
    {
        nGamma++;
    }
    else
    {
        nGamma--;
    }

    if (nGamma < 100)
    {
        nGamma = 100;
    }
    else if (nGamma > 220)
    {
        nGamma = 220;
    }

    m_sliderGamma.SetPos(nGamma);
    m_ConversionParams.nDestGamma = nGamma;
    CString Str;
    Str.Format(L"%1.2f", ((double)nGamma) / 100.0);
    GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(Str);

    *pResult = 0;
}


void ConvertImageDlg::OnEnKillfocusEditGamma()
{
    CString Str;
    GetDlgItem(IDC_EDIT_GAMMA)->GetWindowText(Str);

    INT nGamma = (INT)(_tstof(Str) * 100.0);

    if (nGamma < 100)
    {
        nGamma = 100;
    }
    else if (nGamma > 220)
    {
        nGamma = 220;
    }

    m_sliderGamma.SetPos(nGamma);
    m_ConversionParams.nDestGamma = nGamma;
    Str.Format(L"%1.2f", ((double)nGamma) / 100.0);
    GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(Str);
}


/*****************************************************************/
/*  EdgeEnhancement                                              */
/*****************************************************************/
void ConvertImageDlg::InitSliderEdgeEnhancement()
{
    INT u32Range[3];
    INT nRet = is_EdgeEnhancement(m_hCam, IS_EDGE_ENHANCEMENT_CMD_GET_RANGE, (void*)u32Range, sizeof(u32Range));
    if (nRet == IS_SUCCESS)
    {
        m_nMinEdgeEnhancement = u32Range[0];
        m_nMaxEdgeEnhancement = u32Range[1];

        m_sliderEdgeEnhancement.SetRange(m_nMinEdgeEnhancement, m_nMaxEdgeEnhancement);

        CString Str;
        Str.Format(L"%d", m_nMinEdgeEnhancement);
        GetDlgItem(IDC_MIN_EDGE_ENHANCEMENT)->SetWindowText(Str);
        Str.Format(L"%d", m_nMaxEdgeEnhancement);
        GetDlgItem(IDC_MAX_EDGE_ENHANCEMENT)->SetWindowText(Str);

        m_sliderEdgeEnhancement.SetPos(0);
        GetDlgItem(IDC_EDIT_EDGE_ENHANCEMENT)->SetWindowText(L"0");
    }
}


void ConvertImageDlg::OnDeltaposSpinEdgeEnhancement(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    INT nEdgeEnhancement = m_sliderEdgeEnhancement.GetPos();

    if (pNMUpDown->iDelta < 0)
    {
        nEdgeEnhancement++;
    }
    else
    {
        nEdgeEnhancement--;
    }

    if (nEdgeEnhancement < m_nMinEdgeEnhancement)
    {
        nEdgeEnhancement = m_nMinEdgeEnhancement;
    }
    else if (nEdgeEnhancement > m_nMaxEdgeEnhancement)
    {
        nEdgeEnhancement = m_nMaxEdgeEnhancement;
    }

    m_sliderEdgeEnhancement.SetPos(nEdgeEnhancement);
    m_ConversionParams.nDestEdgeEnhancement = nEdgeEnhancement;
    CString Str;
    Str.Format(L"%d", nEdgeEnhancement);
    GetDlgItem(IDC_EDIT_EDGE_ENHANCEMENT)->SetWindowText(Str);

    *pResult = 0;
}


void ConvertImageDlg::OnEnKillfocusEditEdgeEnhancement()
{
    CString Str;
    GetDlgItem(IDC_EDIT_EDGE_ENHANCEMENT)->GetWindowText(Str);

    INT nEdgeEnhancement = _tstoi(Str);

    if (nEdgeEnhancement < m_nMinEdgeEnhancement)
    {
        nEdgeEnhancement = m_nMinEdgeEnhancement;
    }
    else if (nEdgeEnhancement > m_nMaxEdgeEnhancement)
    {
        nEdgeEnhancement = m_nMaxEdgeEnhancement;
    }

    m_sliderEdgeEnhancement.SetPos(nEdgeEnhancement);
    m_ConversionParams.nDestEdgeEnhancement = nEdgeEnhancement;
    Str.Format(L"%d", nEdgeEnhancement);
    GetDlgItem(IDC_EDIT_EDGE_ENHANCEMENT)->SetWindowText(Str);
}


/*****************************************************************/
/*  Checkboxes                                                   */
/*****************************************************************/
void ConvertImageDlg::OnBnClickedCheckShowOriginal()
{
    UpdateData(TRUE);
}


void ConvertImageDlg::OnBnClickedCheckOriginalSize()
{
    UpdateData(TRUE);

    if (m_bShowOriginalSize)
    {
        m_nRenderMode = IS_RENDER_NORMAL;
    }
    else
    {
        m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;
    }

    UpdateBitmapInfo();
}


/*****************************************************************/
/*  Display bitmap                                               */
/*****************************************************************/
void ConvertImageDlg::UpdateBitmapInfo()
{
    CRect rcWindow;
    GetDlgItem(IDC_DISPLAY)->GetClientRect(rcWindow);

    m_nSizeSourceX = m_nSizeX;
    m_nSizeSourceY = m_nSizeY;

    m_nSizeDestX = rcWindow.Width();
    m_nSizeDestY = rcWindow.Height();

    if ((m_nRenderMode & IS_RENDER_FIT_TO_WINDOW) == 0)
    {
        m_nSizeSourceX = rcWindow.Width();
        m_nSizeSourceY = rcWindow.Height();
    }

    INT nBitsPixel = GetBitsPerPixel();
    INT nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (nBitsPixel <= 8 ? 256 : 3);

    if (m_pbmpInfo != NULL)
    {
        delete [] m_pbmpInfo;
        m_pbmpInfo = NULL;
    }

    m_pbmpInfo = new char[nSize];

    if (m_pbmpInfo == NULL)
    {
        return;
    }

    ZeroMemory(m_pbmpInfo, nSize);

    BITMAPINFOHEADER* dibhdr = (BITMAPINFOHEADER*)m_pbmpInfo;

    dibhdr->biSize = sizeof(BITMAPINFOHEADER);
    dibhdr->biWidth = m_nSizeX;
    dibhdr->biHeight = -m_nSizeY;
    dibhdr->biPlanes = 1;
    dibhdr->biBitCount = nBitsPixel;
    dibhdr->biCompression = BI_BITFIELDS;
    dibhdr->biSizeImage = m_nSizeX * m_nSizeY;
    dibhdr->biXPelsPerMeter = 0;
    dibhdr->biYPelsPerMeter = 0;
    dibhdr->biClrUsed = 256;
    dibhdr->biClrImportant = 256;

    int iR = 0;
    int iG = 1;
    int iB = 2;

    long* pl = (long*)((BITMAPINFO*)dibhdr)->bmiColors;

    switch (m_ConversionParams.nDestPixelFormat)
    {
    case IS_CM_BGRA12_UNPACKED:
    case IS_CM_RGBA12_UNPACKED:
        {
            dibhdr->biBitCount = 32;
            pl[iR] = 0x00FF0000;
            pl[iG] = 0x0000FF00;
            pl[iB] = 0x000000FF;
        }
        break;

    case IS_CM_BGR12_UNPACKED:
    case IS_CM_RGB12_UNPACKED:
        {
            dibhdr->biCompression = BI_RGB;
            dibhdr->biBitCount = 24;
        }
        break;

    case IS_CM_BGR10_UNPACKED:
    case IS_CM_RGB10_UNPACKED:
        {
            dibhdr->biCompression = BI_RGB;
            dibhdr->biBitCount = 24;
        }
        break;

    case IS_CM_BGRA8_PACKED:
    case IS_CM_RGBA8_PACKED:
        {
            pl[iR] = 0x00FF0000;
            pl[iG] = 0x0000FF00;
            pl[iB] = 0x000000FF;
        }
        break;

    case IS_CM_BGR8_PACKED:
    case IS_CM_RGB8_PACKED:
    case IS_CM_RGB8_PLANAR:
        {
            dibhdr->biCompression = BI_RGB;
        }
        break;

    case IS_CM_BGR10_PACKED:
        {
            pl[iR] = 0x3FF00000;
            pl[iG] = 0x000FFC00;
            pl[iB] = 0x000003FF;
        }
        break;

    case IS_CM_BGR565_PACKED:
        {
            pl[iR] = 0x0000F800;
            pl[iG] = 0x000007E0;
            pl[iB] = 0x0000001F;
        }
        break;

    case IS_CM_MONO8:
        {
            dibhdr->biCompression = BI_RGB;

            for (int i = 0; i < 256; i++)
            {
                ((BITMAPINFO*)dibhdr)->bmiColors[i].rgbBlue = i;
                ((BITMAPINFO*)dibhdr)->bmiColors[i].rgbRed = i;
                ((BITMAPINFO*)dibhdr)->bmiColors[i].rgbGreen = i;
            }
        }
        break;

    case IS_CM_UYVY_PACKED:
    case IS_CM_UYVY_MONO_PACKED:
    case IS_CM_UYVY_BAYER_PACKED:
    case IS_CM_CBYCRY_PACKED:
        {
            dibhdr->biCompression = BI_RGB;
        }
        break;

    case IS_CM_MONO10:
        {
            pl[iR] = 0x000003FF;
            pl[iG] = 0x000003FF;
            pl[iB] = 0x000003FF;
        }
        break;

    case IS_CM_MONO12:
        {
            pl[iR] = 0x00000FFF;
            pl[iG] = 0x00000FFF;
            pl[iB] = 0x00000FFF;
        }
        break;
    }

    if (m_pDestBuffer != NULL)
    {
        if (is_FreeImageMem(m_hCam, m_pDestBuffer, m_nDestId) == IS_SUCCESS)
        {
            m_pDestBuffer = NULL;
            m_nDestId = 0;
        }
    }

    INT nRet = is_AllocImageMem(m_hCam, m_nSizeX, m_nSizeY, nBitsPixel, &m_pDestBuffer, &m_nDestId);
}


void ConvertImageDlg::DisplayImage()
{
    CDC* pDC = GetDlgItem(IDC_DISPLAY)->GetDC();
    if (!pDC)
    {
        return;
    }

    pDC->SetStretchBltMode(COLORONCOLOR);

    UINT u32BitShift = 0;
    if ((m_ConversionParams.nDestPixelFormat == IS_CM_BGR12_UNPACKED)   ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_RGB12_UNPACKED)   ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_BGRA12_UNPACKED)  ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_RGBA12_UNPACKED))
    {
        u32BitShift = 4;
    }
    else if ((m_ConversionParams.nDestPixelFormat == IS_CM_BGR10_UNPACKED) ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_RGB10_UNPACKED))
    {
        u32BitShift = 2;
    }

    /* In case of BGR12, RGB12, BGRA12 and RGBA12 we must convert the 12 bit to 8 bit to show them */
    if ((m_ConversionParams.nDestPixelFormat == IS_CM_BGR12_UNPACKED)   ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_RGB12_UNPACKED)   ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_BGRA12_UNPACKED)  ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_RGBA12_UNPACKED)  ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_BGR10_UNPACKED)   ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_RGB10_UNPACKED)   ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_RGBA8_PACKED)     ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_RGB8_PACKED))
    {
        UINT u32DstSize = m_nSizeX * m_nSizeY * (((BITMAPINFO*)m_pbmpInfo)->bmiHeader.biBitCount / 8);
        char *pu8Mem = new char[u32DstSize];

        if (pu8Mem != NULL)
        {
            UINT u32SrcSize = m_nSizeX * m_nSizeY * 6;

            WORD *pu16Temp = (WORD*)m_ConversionParams.pDestBuffer;
            /* In case of RGB12, RGBA12 and RGB10 we must swap the colors */
            if ((m_ConversionParams.nDestPixelFormat == IS_CM_RGB12_UNPACKED)   ||
                (m_ConversionParams.nDestPixelFormat == IS_CM_RGBA12_UNPACKED)  ||
                (m_ConversionParams.nDestPixelFormat == IS_CM_RGB10_UNPACKED))
            {
                for (UINT j = 0; j < u32DstSize; j+=(((BITMAPINFO*)m_pbmpInfo)->bmiHeader.biBitCount / 8))
                {
                    pu8Mem[j] = pu16Temp[j+2] >> u32BitShift;
                    pu8Mem[j+1] = pu16Temp[j+1] >> u32BitShift;
                    pu8Mem[j+2] = pu16Temp[j] >> u32BitShift;
                }
            }
            else if ((m_ConversionParams.nDestPixelFormat == IS_CM_RGBA8_PACKED) ||
                (m_ConversionParams.nDestPixelFormat == IS_CM_RGB8_PACKED))
            {
                for (UINT j = 0; j < u32DstSize; j+=(((BITMAPINFO*)m_pbmpInfo)->bmiHeader.biBitCount / 8))
                {
                    pu8Mem[j + 2] = m_ConversionParams.pDestBuffer[j] >> u32BitShift;
                    pu8Mem[j + 1] = m_ConversionParams.pDestBuffer[j + 1] >> u32BitShift;
                    pu8Mem[j + 0] = m_ConversionParams.pDestBuffer[j + 2] >> u32BitShift;
                }
            }
            else
            {
                for (UINT j = 0; j < u32DstSize; j++)
                {
                    pu8Mem[j] = pu16Temp[j] >> u32BitShift;
                }
            }

            ::StretchDIBits(pDC->GetSafeHdc(),
                0,
                0,
                m_nSizeDestX,
                m_nSizeDestY,
                0,
                0,
                m_nSizeSourceX,
                m_nSizeSourceY,
                pu8Mem,
                (BITMAPINFO*)m_pbmpInfo,
                DIB_RGB_COLORS,
                SRCCOPY
                );

            delete [] pu8Mem;
        }
    }
    else
    {
        char *pu8Mem = m_ConversionParams.pDestBuffer;
        bool bAllocated = false;

        if (m_ConversionParams.nDestPixelFormat == IS_CM_RGB8_PLANAR)
        {
            /* Build a destination buffer for only one color component. The size is the same as the source size */
            UINT u32DstSize = m_nSizeX * m_nSizeY * 3;
            pu8Mem = new char[u32DstSize];
            ZeroMemory(pu8Mem, u32DstSize);

            if (pu8Mem != NULL)
            {
                bAllocated = true;
            }

            if (m_nPlanarLayer != -1)
            {
                /* The planar format is RGB, the StretchDIBits function wants BGR */

                /* m_nPlanarLayer: 0 = BLUE, 1 = GREEN, 2 = RED */

                /* Calculate the offset of the layer in the debayered image */
                UINT u32PlanarOffset = (2 - m_nPlanarLayer) * (m_nSizeX * m_nSizeY);

                /* Copy only one planar layer (the other 2 bytes are zero) */
                for (INT i = 0; i < m_nSizeX * m_nSizeY; i++)
                {
                    pu8Mem[i * 3 + m_nPlanarLayer] = m_ConversionParams.pDestBuffer[i + u32PlanarOffset];
                }
            }
            else
            {
                /* Copy the planar rgb layers back to a packed bgr format  */
                for (INT i = 0; i < m_nSizeX * m_nSizeY; i++)
                {
                    pu8Mem[i * 3 + 2] = m_ConversionParams.pDestBuffer[i];
                    pu8Mem[i * 3 + 1] = m_ConversionParams.pDestBuffer[i + (m_nSizeX * m_nSizeY)];
                    pu8Mem[i * 3 + 0] = m_ConversionParams.pDestBuffer[i + (2 * m_nSizeX * m_nSizeY)];
                }
            }
        }

        ::StretchDIBits(pDC->GetSafeHdc(),
            0,
            0,
            m_nSizeDestX,
            m_nSizeDestY,
            0,
            0,
            m_nSizeSourceX,
            m_nSizeSourceY,
            pu8Mem,
            (BITMAPINFO*)m_pbmpInfo,
            DIB_RGB_COLORS,
            SRCCOPY
            );

        if (bAllocated)
        {
            delete [] pu8Mem;
            pu8Mem = NULL;
        }
    }

    if ((m_ConversionParams.nDestPixelFormat == IS_CM_UYVY_PACKED)      ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_UYVY_MONO_PACKED) ||
        (m_ConversionParams.nDestPixelFormat == IS_CM_UYVY_BAYER_PACKED))
    {
        if (m_bShowYUVWarning)
        {
            m_bShowYUVWarning = FALSE;
            AfxMessageBox(L"The YUV pixelformats can not be displayed correctly!", MB_ICONWARNING);
        }
    }
}

INT ConvertImageDlg::GetBitsPerPixel()
{
    INT nBitsPixel = 0;

    switch (m_ConversionParams.nDestPixelFormat)
    {
    case IS_CM_BGRA12_UNPACKED:
    case IS_CM_RGBA12_UNPACKED:
        {
            nBitsPixel = 64;
        }
        break;

    case IS_CM_BGR12_UNPACKED:
    case IS_CM_RGB12_UNPACKED:
    case IS_CM_BGR10_UNPACKED:
    case IS_CM_RGB10_UNPACKED:
        {
            nBitsPixel = 48;
        }
        break;

    case IS_CM_BGRA8_PACKED:
    case IS_CM_RGBA8_PACKED:
    case IS_CM_BGR10_PACKED:
        {
            nBitsPixel = 32;
        }
        break;

    case IS_CM_BGR8_PACKED:
    case IS_CM_RGB8_PACKED:
    case IS_CM_RGB8_PLANAR:
        {
            nBitsPixel = 24;
        }
        break;

    case IS_CM_BGR565_PACKED:
        {
            nBitsPixel = 16;
        }
        break;

    case IS_CM_MONO8:
        {
            nBitsPixel = 8;
        }
        break;

    case IS_CM_MONO10:
    case IS_CM_MONO12:
    case IS_CM_UYVY_PACKED:
    case IS_CM_UYVY_MONO_PACKED:
    case IS_CM_UYVY_BAYER_PACKED:
        {
            nBitsPixel = 16;
        }
        break;
    }

    return nBitsPixel;
}
