//==========================================================================//
//                                                                          //
//  Copyright (C) 2011 - 2018                                               //
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

// uEyeSequenceAoiDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "uEyeSequenceAoi.h"
#include "uEyeSequenceAoiDlg.h"
#include "version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialogfelddaten
    enum { IDD = IDD_ABOUTBOX };

protected:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV-Unterstützung

    // Implementierung
protected:
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


// CuEyeSequenceAoiDlg-Dialogfeld




CuEyeSequenceAoiDlg::CuEyeSequenceAoiDlg(CWnd* pParent /*=NULL*/)
: CDialog(CuEyeSequenceAoiDlg::IDD, pParent),
m_StatusBar(0), m_cnNumberOfSeqMemory(3)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CuEyeSequenceAoiDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLIDER_AOI_X, m_sliderAOI_X);
    DDX_Control(pDX, IDC_SLIDER_AOI_EXP, m_sliderAOI_EXP);
    DDX_Control(pDX, IDC_SLIDER_AOI_Y, m_sliderAOI_Y);
    DDX_Control(pDX, IDC_SLIDER_AOI_MASTERGAIN, m_sliderAOI_MasterGain);
    DDX_Control(pDX, IDC_COMBO_CUR_SEQUENCE, m_comboCurrentSequence);
    DDX_Control(pDX, IDC_SLIDER_FPS, m_sliderFps);
}

BEGIN_MESSAGE_MAP(CuEyeSequenceAoiDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage)
    //}}AFX_MSG_MAP
    ON_WM_CLOSE()
    ON_WM_HSCROLL()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_MASTERGAIN, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiMastergain)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_EXP, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiExp)
    ON_CBN_SELCHANGE(IDC_COMBO_CUR_SEQUENCE, &CuEyeSequenceAoiDlg::OnCbnSelchangeComboCurSequence)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_POS_X, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiPosX)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_POS_Y, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiPosY)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_WIDTH, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiWidth)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_HEIGHT, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiHeight)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_X, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiX)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_Y, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiY)
    ON_BN_CLICKED(IDC_BUTTON_AOI_SET, &CuEyeSequenceAoiDlg::OnBnClickedButtonAoiSet)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FPS, &CuEyeSequenceAoiDlg::OnDeltaposSpinFps)
    ON_WM_DESTROY()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AOI_CYCLE, &CuEyeSequenceAoiDlg::OnDeltaposSpinAoiCycle)
END_MESSAGE_MAP()


// CuEyeSequenceAoiDlg-Meldungshandler

BOOL CuEyeSequenceAoiDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Hinzufügen des Menübefehls "Info..." zum Systemmenü.

    // IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
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

    // Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
    //  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
    SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
    SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

    m_StatusBar = new CStatusBarCtrl;
    m_StatusBar->Create(WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

    int nParts[4]= {120, 240, 360, -1};
    m_StatusBar->SetParts(4, nParts);

    // initialize the used variables
    m_vecMemory.reserve(m_cnNumberOfSeqMemory);
    m_vecMemory.resize(m_cnNumberOfSeqMemory);

    m_hCam = 0;

    // open a camera handle
    OpenCamera();

    // Initialize the sequences
    InitSequenceMode();

    // Initialize the property controls
    InitProperties();

    // Update the properties
    UpdateProperties();

    return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CuEyeSequenceAoiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CuEyeSequenceAoiDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // Gerätekontext zum Zeichnen

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Symbol in Clientrechteck zentrieren
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Symbol zeichnen
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CuEyeSequenceAoiDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::InitSequence() 
//
// DESCRIPTION: - Initialize the sequence mode
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSequenceAoiDlg::InitSequenceMode()
{
    // We always use all sequences
    INT nMask = IS_AOI_SEQUENCE_INDEX_AOI_1 |
        IS_AOI_SEQUENCE_INDEX_AOI_2 |
        IS_AOI_SEQUENCE_INDEX_AOI_3 |
        IS_AOI_SEQUENCE_INDEX_AOI_4;

    // enable sequence mode
    // we have proofed in OpenCamera() if sequence mode is supported
    is_AOI(m_hCam, IS_AOI_SEQUENCE_SET_ENABLE, (void*)&nMask, sizeof(nMask));
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::InitSequence() 
//
// DESCRIPTION: - Initialize the property controls
//              - Sets the range, min and max values and 
//                the start value of the controls
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSequenceAoiDlg::InitProperties()
{
    // helper variable for number conversions
    CString strTemp; 

    // fill sequence combobox
    m_comboCurrentSequence.InsertString(0, L"Camera");
    m_comboCurrentSequence.InsertString(1, L"Sequence 2");
    m_comboCurrentSequence.InsertString(2, L"Sequence 3");
    m_comboCurrentSequence.InsertString(3, L"Sequence 4");

    // set sequence 2 as active
    m_comboCurrentSequence.SetCurSel(1);

    // get sequence parameter
    GetSequenceParam(m_comboCurrentSequence.GetCurSel(), &m_SequenceParam);

    // set cycle repetitions
    SetDlgItemInt(IDC_EDIT_AOI_CYCLE, m_SequenceParam.s32NumberOfCycleRepetitions);

    // Set master gain range, min and max
    m_sliderAOI_MasterGain.SetRange(0, 100);
    GetDlgItem(IDC_TXT_AOI_GAIN_MIN)->SetWindowText(L"0");
    GetDlgItem(IDC_TXT_AOI_GAIN_MAX)->SetWindowText(L"100");

    // Set exposure range, min and max
    // We use our UpdateExposure() methode
    UpdateExposure();

    // Get the image size to initialize the aoi controls
    IS_RECT rectAOI;

    INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));
    if (nRet == IS_SUCCESS)
    {
        SetDlgItemInt(IDC_EDIT_AOI_POS_X, rectAOI.s32X);
        SetDlgItemInt(IDC_EDIT_AOI_POS_Y, rectAOI.s32Y);
        SetDlgItemInt(IDC_EDIT_AOI_WIDTH, rectAOI.s32Width);
        SetDlgItemInt(IDC_EDIT_AOI_HEIGHT, rectAOI.s32Height);


        // initialize aoi slider
        IS_POINT_2D aoiPoint;

        /* Get the maximum possible position */
        nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&aoiPoint, sizeof(aoiPoint));

        // Set aoi range, min and max
        m_sliderAOI_X.SetRange(0, aoiPoint.s32X);
        m_sliderAOI_Y.SetRange(0, aoiPoint.s32Y);

        GetDlgItem(IDC_TXT_AOI_X_MIN)->SetWindowText(L"0");
        GetDlgItem(IDC_TXT_AOI_Y_MIN)->SetWindowText(L"0");

        SetDlgItemInt(IDC_TXT_AOI_X_MAX, aoiPoint.s32X);
        SetDlgItemInt(IDC_TXT_AOI_Y_MAX, aoiPoint.s32Y);

    }
    // initialize fps
    // Get frametime range
    double dFpsMin = 0.0, dFpsMax = 0.0, dFpsInc = 0.0;
    is_GetFrameTimeRange(m_hCam, &dFpsMin, &dFpsMax, &dFpsInc);

    // set frmaetime range, min and max
    m_sliderFps.SetRange((int)(1 / dFpsMax * 100 ) , (int)(1 / dFpsMin * 100), TRUE);

    strTemp.Format(L"%.2f", 1 / dFpsMax);
    SetDlgItemText(IDC_TXT_FPS_MIN, strTemp);

    strTemp.Format(L"%.2f", 1 / dFpsMin);
    SetDlgItemText(IDC_TXT_FPS_MAX, strTemp);

    // get the current frametime
    double dFps = 0;
    is_SetFrameRate(m_hCam, IS_GET_FRAMERATE, &dFps);

    // initialize the frametime controls with the correct value
    m_sliderFps.SetPos( (int)(dFps * 100) );

    strTemp.Format(L"%.2f", dFps);
    SetDlgItemText(IDC_EDIT_FPS, strTemp);
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::InitSequence() 
//
// DESCRIPTION: - Updates the property controls
//              - Method is always calld if the user changes a value of a control
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSequenceAoiDlg::UpdateProperties()
{
    // if UpdateProperties is called, m_SequenceParam is always filled with the new values

    // helper variable for number conversions
    CString strTemp;

    // update exposure
    // update box
    strTemp.Format(L"%.2f", m_SequenceParam.dblExposure);
    GetDlgItem(IDC_EDIT_AOI_EXP)->SetWindowText(strTemp);

    //update slider
    m_sliderAOI_EXP.SetPos((int)(((m_SequenceParam.dblExposure - m_dblMinExp) / m_dblIncExp) + 0.5));

    // update master gain
    // update box
    strTemp.Format(L"%d", m_SequenceParam.s32Gain);
    GetDlgItem(IDC_EDIT_AOI_MASTERGAIN)->SetWindowText(strTemp);

    // update slider
    m_sliderAOI_MasterGain.SetPos(m_SequenceParam.s32Gain);

    // Set Aoi position
    m_sliderAOI_X.SetPos(m_SequenceParam.s32X);
    m_sliderAOI_Y.SetPos(m_SequenceParam.s32Y);

    SetDlgItemInt(IDC_EDIT_AOI_X, m_SequenceParam.s32X);
    SetDlgItemInt(IDC_EDIT_AOI_Y, m_SequenceParam.s32Y);

    /* Update AOI sequence positions */
    IS_POINT_2D aoiPoint;

    /* Get the maximum possible position */
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&aoiPoint, sizeof(aoiPoint));

    /* Initialize slider */
    m_sliderAOI_X.SetRange(0, aoiPoint.s32X);
    m_sliderAOI_Y.SetRange(0, aoiPoint.s32Y);

    /* set min and max */
    GetDlgItem(IDC_TXT_AOI_X_MIN)->SetWindowText(L"0");
    GetDlgItem(IDC_TXT_AOI_Y_MIN)->SetWindowText(L"0");

    SetDlgItemInt(IDC_TXT_AOI_X_MAX, aoiPoint.s32X);
    SetDlgItemInt(IDC_TXT_AOI_Y_MAX, aoiPoint.s32Y);

    // Update fps
    double dFps = 0;
    is_SetFrameRate(m_hCam, IS_GET_FRAMERATE, &dFps);

    m_sliderFps.SetPos( (int)(dFps * 100) );

    strTemp.Format(L"%.2f", dFps);
    SetDlgItemText(IDC_EDIT_FPS, strTemp);

    // update cycle repetitions
    SetDlgItemInt(IDC_EDIT_AOI_CYCLE, m_SequenceParam.s32NumberOfCycleRepetitions);

    // now check if we change the sequence param or the camera parameter
    // IsSequenceAoi returns true if the sequence param should be changed
    if (IsSequenceAoi())
    {
        DisableEnableSpecificProperties(TRUE);
        SetSequenceParam(m_SequenceParam);
    }
    else
    {
        DisableEnableSpecificProperties(FALSE);

        // change camera exposure
        INT range = m_sliderAOI_EXP.GetRangeMax();
        double dblExp = m_dblMinExp + (((double)m_sliderAOI_EXP.GetPos() / (double)(range)) * (m_dblMaxExp - m_dblMinExp));

        int nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dblExp, sizeof(dblExp));

        // change camera gain
        int nValue = 0;
        nValue = m_sliderAOI_MasterGain.GetPos();

        is_SetHardwareGain(m_hCam, nValue, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::OpenCamera() 
//
// DESCRIPTION: - Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSequenceAoiDlg::OpenCamera()
{
    INT nRet = IS_NO_SUCCESS;
    ExitCamera();

    // init camera (open next available camera)
    m_hCam = (HIDS) 0;
    nRet = InitCamera(&m_hCam, 0);
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

            // check if sequence mode is available
            INT nSequence = 0;
            nRet = is_AOI(m_hCam, IS_AOI_SEQUENCE_GET_SUPPORTED, &nSequence, sizeof(nSequence));

            if (nSequence ==  IS_AOI_SEQUENCE_INDEX_AOI_1)
            {
                AfxMessageBox(TEXT("AOI sequence is not supported!"), MB_ICONWARNING );
                ExitCamera();
                PostQuitMessage (0);
            }
        }
        else
        {
            AfxMessageBox(TEXT("Initializing the display mode failed!"), MB_ICONWARNING );
            ExitCamera();
            PostQuitMessage (0);
        }
    }
    else
    {
        AfxMessageBox(TEXT("No uEye camera could be opened !"), MB_ICONWARNING );
        PostQuitMessage (0);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::OnUEyeMessage() 
//
// DESCRIPTION: - handles the messages from the uEye camera
//				- messages must be enabled using is_EnableMessage()
//
///////////////////////////////////////////////////////////////////////////////
LRESULT CuEyeSequenceAoiDlg::OnUEyeMessage( WPARAM wParam, LPARAM lParam )
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

                UEYEIMAGEINFO ImageInfo;

                if (IS_SUCCESS == nRet)
                {
                    // read image info, to know which image belongs to which sequence index
                    nRet = is_GetImageInfo(m_hCam, lMemoryId, &ImageInfo, sizeof(ImageInfo));

                    is_UnlockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, pLast);
                }

                if (IS_SUCCESS == nRet)
                {
                    // get the correct display for the sequence
                    int nDisplay = IDC_DISPLAY_AOI_1;
                    switch (ImageInfo.wAOIIndex)
                    {
                        case IS_AOI_SEQUENCE_INDEX_AOI_1:
                            nDisplay = IDC_DISPLAY_AOI_1;
                            break;
                        case IS_AOI_SEQUENCE_INDEX_AOI_2:
                            nDisplay = IDC_DISPLAY_AOI_2;
                            break;
                        case IS_AOI_SEQUENCE_INDEX_AOI_3:
                            nDisplay = IDC_DISPLAY_AOI_3;
                            break;
                        case IS_AOI_SEQUENCE_INDEX_AOI_4:
                            nDisplay = IDC_DISPLAY_AOI_4;
                            break;
                    }
                    CString strTemp;
                    strTemp.Format(L"AOI Index: %d", ImageInfo.wAOIIndex);
                    m_StatusBar->SetText((strTemp), 0, 0);
                    strTemp.Format(L"AOI Cycle: %d", ImageInfo.wAOICycle);
                    m_StatusBar->SetText((strTemp), 1, 0);

                    CBitmap bmp;
                    if (bmp.CreateBitmap(m_nSizeX, m_nSizeY, 1, m_nBitsPerPixel, pLast))
                    {
                        CDC *pdc = GetDC();
                        if (pdc != NULL)
                        {
                            // create bitmap with the image data
                            CDC dcBmp;
                            if (dcBmp.CreateCompatibleDC(pdc) != NULL)
                            {
                                if (dcBmp.SelectObject(&bmp) != NULL)
                                {
                                    pdc->SetStretchBltMode(COLORONCOLOR);

                                    // Get the display position
                                    CRect rect;
                                    GetDlgItem(nDisplay)->GetWindowRect(&rect);
                                    ScreenToClient(&rect);

                                    // Read the image size
                                    IS_RECT rectAOI;
                                    INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_AOI, (void*) &rectAOI, sizeof(rectAOI));

                                    // draw the image into the display
                                    pdc->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
                                                    &dcBmp, 0, 0, rectAOI.s32Width, rectAOI.s32Height, SRCCOPY);

                                    // delete the bitmap
                                    dcBmp.DeleteDC();
                                }
                                else
                                {
                                    OutputDebugString(L"dcBmp.SelectObject() failed");
                                }
                            }
                            else
                            {
                                dcBmp.DeleteDC();
                                OutputDebugString(L"dcBmp.CreateCompatibleDC() failed");
                            }
                            // release our draw dc
                            ReleaseDC(pdc);
                        }
                        else
                        {
                            OutputDebugString(L"GetDC() failed");
                        }
                        bmp.DeleteObject();
                    }
                }
            }
        }

        break;
    }    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::ExitCamera() 
//
// DESCRIPTION: - exits the instance of the camera
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSequenceAoiDlg::ExitCamera()
{
    if( m_hCam != 0 )
    {
        // Disable messages
        is_EnableMessage( m_hCam, IS_FRAME, NULL );

        // Stop live video
        is_StopLiveVideo( m_hCam, IS_WAIT );

        // Free the allocated buffer
        ClearSequence();
        FreeImageMems();

        // Close camera
        is_ExitCamera( m_hCam );
        m_hCam = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::InitDisplayMode() 
//
// DESCRIPTION: - initializes the display mode
//
///////////////////////////////////////////////////////////////////////////////
int CuEyeSequenceAoiDlg::InitDisplayMode()
{
    INT nRet = IS_NO_SUCCESS;

    if (m_hCam == NULL)
        return IS_NO_SUCCESS;

    // Free the allocated buffer
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
        // for monochrome camera we also use RGB32, due we do not want to copy the image data for SelectObject()
        m_nColorMode = IS_CM_BGRA8_PACKED;
        m_nBitsPerPixel = 32;
    }

    nRet = AllocImageMems(m_nSizeX, m_nSizeY, m_nBitsPerPixel);

    if (IS_SUCCESS != nRet)
    {
        AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING);
        FreeImageMems();
    }

    nRet = InitSequence();

    if (IS_SUCCESS != nRet)
    {
        AfxMessageBox(TEXT("Init sequence failed!"), MB_ICONWARNING);
        ClearSequence();
        FreeImageMems();
    }

    if (nRet == IS_SUCCESS)
    {
        // set the desired color mode
        is_SetColorMode(m_hCam, m_nColorMode);

        // set the image size to capture
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
    }   

    return nRet;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::InitCamera() 
//
// DESCRIPTION: - initializes the camera and if neccessary uploads a new firmware
//
///////////////////////////////////////////////////////////////////////////////
INT CuEyeSequenceAoiDlg::InitCamera (HIDS *hCam, HWND hWnd)
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

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::InitCamera() 
//
// DESCRIPTION: - Returns the maximal image size
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeSequenceAoiDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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

void CuEyeSequenceAoiDlg::OnClose()
{
    // Close camera on exit
    ExitCamera();
    CDialog::OnClose();
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::SetSequenceParam() 
//
// DESCRIPTION: - Set the new sequence parameter
//
///////////////////////////////////////////////////////////////////////////////
int CuEyeSequenceAoiDlg::SetSequenceParam(AOI_SEQUENCE_PARAMS Param)
{
    return is_AOI(m_hCam, IS_AOI_SEQUENCE_SET_PARAMS, (void*)&Param, sizeof(Param));
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeSequenceAoiDlg::SetSequenceParam() 
//
// DESCRIPTION: - Get the current sequence parameter
//
///////////////////////////////////////////////////////////////////////////////
int CuEyeSequenceAoiDlg::GetSequenceParam(int nIndex, AOI_SEQUENCE_PARAMS *pParam)
{
    pParam->s32AOIIndex = nIndex;
    INT nRet = is_AOI(m_hCam, IS_AOI_SEQUENCE_GET_PARAMS, (void*)pParam, sizeof(AOI_SEQUENCE_PARAMS));
    return nRet;
}

BOOL CuEyeSequenceAoiDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN)
    {
        if ((pMsg->wParam == VK_ESCAPE))
        {
            // avoid close on escape press
            return 0;
        }
        if ((pMsg->wParam == VK_RETURN))
        {
            if (GetFocus() == GetDlgItem(IDC_EDIT_AOI_X))
            {
                // a value has changed --> assume it
                OnEnReturnEditAoiX();
            }
            if (GetFocus() == GetDlgItem(IDC_EDIT_AOI_Y))
            {
                // a value has changed --> assume it
                OnEnReturnEditAoiY();
            }
            if (GetFocus() == GetDlgItem(IDC_EDIT_AOI_EXP))
            {
                // a value has changed --> assume it
                OnEnReturnEditAoiExp();
            }
            if (GetFocus() == GetDlgItem(IDC_EDIT_AOI_MASTERGAIN))
            {
                // a value has changed --> assume it
                OnEnReturnEditAoiMastergain();
            }
            if (GetFocus() == GetDlgItem(IDC_EDIT_FPS))
            {
                // a value has changed --> assume it
                OnEnReturnEditFps();
            }
            if (GetFocus() == GetDlgItem(IDC_EDIT_AOI_CYCLE))
            {
                // a value has changed --> assume it
                OnEnReturnEditCycle();
            }
            if (GetFocus() == GetDlgItem(IDC_EDIT_AOI_POS_X) || GetFocus() == GetDlgItem(IDC_EDIT_AOI_POS_Y) ||
                GetFocus() == GetDlgItem(IDC_EDIT_AOI_WIDTH) || GetFocus() == GetDlgItem(IDC_EDIT_AOI_HEIGHT))
            {
                // a value has changed --> assume it
                // every control calls the same method
                // We call the button press event 
                OnBnClickedButtonAoiSet();
            }

            // avoid close on return press
            return 0;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CuEyeSequenceAoiDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // one of our slider has changed his value
    switch (pScrollBar->GetDlgCtrlID())
    {
        // we read the new value from the slider, write it in our sequence parameter and call UpdateProperties()
        // exposure slider
    case IDC_SLIDER_AOI_EXP:
        {
            INT range = m_sliderAOI_EXP.GetRangeMax();
            double dblExp = m_dblMinExp + (((double)m_sliderAOI_EXP.GetPos() / (double)(range)) * (m_dblMaxExp - m_dblMinExp));
            m_SequenceParam.dblExposure = dblExp;
            // Update the property controls
            UpdateProperties();
        }
        break;
        // master gain slider
    case IDC_SLIDER_AOI_MASTERGAIN:
        m_SequenceParam.s32Gain = m_sliderAOI_MasterGain.GetPos();
        // Update the property controls
        UpdateProperties();
        break;

        // one of the aoi position slider
    case IDC_SLIDER_AOI_X:
    case IDC_SLIDER_AOI_Y:
        m_SequenceParam.s32X = m_sliderAOI_X.GetPos();
        m_SequenceParam.s32Y = m_sliderAOI_Y.GetPos();
        // Update the property controls
        UpdateProperties();
        break;

        // framerate slider
    case IDC_SLIDER_FPS:
        {
            double dFps = 0, dDummy = 0;
            dFps = m_sliderFps.GetPos();
            dFps /= 100;

            // set the new framerate
            is_SetFrameRate(m_hCam, dFps, &dDummy);
            // Update the exposure controls due the new framerate
            UpdateExposure();
            // Update the property controls
            UpdateProperties();
        }
        break;

    default:
        break;
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiMastergain(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    INT &nMasterGain = m_SequenceParam.s32Gain;

    // check up or down and range
    if (pNMUpDown->iDelta < 0)
    {
        if (nMasterGain < 100)
        {
            nMasterGain++;
        }
    }
    else
    {
        if (nMasterGain > 0)
        {
            nMasterGain--;
        }
    }

    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiExp(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
    *pResult = 0;

    double &dExposure = m_SequenceParam.dblExposure;

    // check up or down
    if (pNMUpDown->iDelta < 0)
    {
        // our increment value
        dExposure += 20 * m_dblIncExp;
    }
    else
    {
        // our increment value
        dExposure -= 20 * m_dblIncExp;
    }

    // is our new value between the range
    if (dExposure < m_dblMinExp)
    {
        dExposure = m_dblMinExp;
    }
    if (dExposure > m_dblMaxExp)
    {
        dExposure = m_dblMaxExp;
    }

    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnCbnSelchangeComboCurSequence()
{
    // a sequence changed
    int nCurPosition = 0;
    nCurPosition = m_comboCurrentSequence.GetCurSel();

    switch (nCurPosition)
    {
    case 0: // IS_AOI_SEQUENCE_INDEX_AOI_1
        // we refer to the camera
        {
            // Update mastergain control
            int nGain = 0;
            nGain = is_SetHardwareGain(m_hCam, IS_GET_MASTER_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
            m_sliderAOI_MasterGain.SetPos(nGain);

            SetDlgItemInt(IDC_EDIT_AOI_MASTERGAIN, nGain);
        }
        {      
            // update exposure
            CString strTemp;
            double dExposure = 0.0;

            // get current exposure
            is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&dExposure, sizeof(dExposure));

            // set exposure value into the controls
            strTemp.Format(L"%.2f", dExposure);
            GetDlgItem(IDC_EDIT_AOI_EXP)->SetWindowText(strTemp);

            m_sliderAOI_EXP.SetPos((int)(((dExposure - m_dblMinExp) / m_dblIncExp) + 0.5));
        }
        // We do not call UpdateProperties() due we already have set everything here
        // But we disable the neccessary properties
        DisableEnableSpecificProperties(FALSE);
        break;
    case 1: // IS_AOI_SEQUENCE_INDEX_AOI_2
        GetSequenceParam(IS_AOI_SEQUENCE_INDEX_AOI_2, &m_SequenceParam);
        // Update the property controls
        UpdateProperties();
        break;
    case 2: // IS_AOI_SEQUENCE_INDEX_AOI_3
        GetSequenceParam(IS_AOI_SEQUENCE_INDEX_AOI_3, &m_SequenceParam);
        // Update the property controls
        UpdateProperties();
        break;
    case 3: // IS_AOI_SEQUENCE_INDEX_AOI_4
        GetSequenceParam(IS_AOI_SEQUENCE_INDEX_AOI_4, &m_SequenceParam);
        // Update the property controls
        UpdateProperties();
        break;
    default:
        break;
    }
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    // we need the maximal coordinates
    IS_POINT_2D pointMax;
    INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&pointMax , sizeof(pointMax ));

    // we need the increment
    IS_POINT_2D pointInc;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_INC, (void*)&pointInc , sizeof(pointInc ));

    // we need the aoi size
    IS_RECT rectAOI;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    // increment or decrement the aoi
    if (pNMUpDown->iDelta < 0)
    {
        rectAOI.s32X += pointInc.s32X;
    }
    else
    {
        rectAOI.s32X -= pointInc.s32X;
    }

    // is the new value between the range
    if (rectAOI.s32X > pointMax.s32X)
    {
        rectAOI.s32X = pointMax.s32X;
    }
    if (rectAOI.s32X < 0)
    {
        rectAOI.s32X = 0;
    }

    //set the aoi
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    // update aoi position
    SetDlgItemInt(IDC_EDIT_AOI_POS_X, rectAOI.s32X);
    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    // we need the maximal coordinates
    IS_POINT_2D pointMax;
    INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&pointMax , sizeof(pointMax ));

    // we need the increment
    IS_POINT_2D pointInc;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_INC, (void*)&pointInc , sizeof(pointInc ));

    // we need the aoi size
    IS_RECT rectAOI;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    // increment or decrement the aoi
    if (pNMUpDown->iDelta < 0)
    {
        rectAOI.s32Y += pointInc.s32Y;
    }
    else
    {
        rectAOI.s32Y -= pointInc.s32Y;
    }

    // is the new value between the range
    if (rectAOI.s32Y > pointMax.s32Y)
    {
        rectAOI.s32Y = pointMax.s32Y;
    }
    if (rectAOI.s32Y < 0)
    {
        rectAOI.s32Y = 0;
    }

    //set the aoi
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    // update aoi position
    SetDlgItemInt(IDC_EDIT_AOI_POS_Y, rectAOI.s32Y);
    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiWidth(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    // we need the maximal size
    IS_SIZE_2D sizeMax;
    INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE_MAX, (void*)&sizeMax , sizeof(sizeMax ));

    // we need the increment
    IS_SIZE_2D sizeInc;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE_INC, (void*)&sizeInc , sizeof(sizeInc ));

    // we need the aoi size
    IS_RECT rectAOI;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    // increment or decrement the aoi
    if (pNMUpDown->iDelta < 0)
    {
        rectAOI.s32Width += sizeInc.s32Width;
    }
    else
    {
        rectAOI.s32Width -= sizeInc.s32Width;
    }

    // is the new value between the range
    if (rectAOI.s32Width > sizeMax.s32Width)
    {
        rectAOI.s32Width = sizeMax.s32Width;
    }
    if (rectAOI.s32Width < 0)
    {
        rectAOI.s32Width = 0;
    }

    // set the aoi
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    // update aoi position
    SetDlgItemInt(IDC_EDIT_AOI_WIDTH, rectAOI.s32Width);
    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    // we need the maximal size
    IS_SIZE_2D sizeMax;
    INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE_MAX, (void*)&sizeMax , sizeof(sizeMax ));

    // we need the increment
    IS_SIZE_2D sizeInc;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE_INC, (void*)&sizeInc , sizeof(sizeInc ));

    // we need the aoi size
    IS_RECT rectAOI;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    // increment or decrement the aoi
    if (pNMUpDown->iDelta < 0)
    {
        rectAOI.s32Height += sizeInc.s32Height;
    }
    else
    {
        rectAOI.s32Height -= sizeInc.s32Height;
    }

    // is the new value between the range
    if (rectAOI.s32Height > sizeMax.s32Height)
    {
        rectAOI.s32Height = sizeMax.s32Height;
    }
    if (rectAOI.s32Height < 0)
    {
        rectAOI.s32Height = 0;
    }

    // set the aoi
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    // update aoi position
    SetDlgItemInt(IDC_EDIT_AOI_HEIGHT, rectAOI.s32Height);
    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiX(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    /* Get the maximum possible position */
    IS_POINT_2D aoiPoint;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&aoiPoint, sizeof(aoiPoint));

    int &nPosX = m_SequenceParam.s32X;

    // increment or decrement the aoi position
    if (pNMUpDown->iDelta < 0)
    {
        nPosX += 4;
    }
    else
    {
        nPosX -= 4;
    }

    // is the new value between the range
    if (nPosX < 0)
    {
        nPosX = 0;
    }
    if (nPosX > aoiPoint.s32X)
    {
        nPosX = aoiPoint.s32X;
    }

    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiY(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    /* Get the maximum possible position */
    IS_POINT_2D aoiPoint;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&aoiPoint, sizeof(aoiPoint));

    int &nPosY = m_SequenceParam.s32Y;

    // increment or decrement the aoi position
    if (pNMUpDown->iDelta < 0)
    {
        nPosY += 2;
    }
    else
    {
        nPosY -= 2;
    }

    // is the new value between the range
    if (nPosY < 0)
    {
        nPosY = 0;
    }
    if (nPosY > aoiPoint.s32Y)
    {
        nPosY = aoiPoint.s32Y;
    }

    // Update the property controls
    UpdateProperties();
}


void CuEyeSequenceAoiDlg::OnBnClickedButtonAoiSet()
{
    // We set the new aoi
    IS_RECT rectAOI;

    // Get the new aoi values
    rectAOI.s32X = GetDlgItemInt(IDC_EDIT_AOI_POS_X);
    rectAOI.s32Y = GetDlgItemInt(IDC_EDIT_AOI_POS_Y);
    rectAOI.s32Width = GetDlgItemInt(IDC_EDIT_AOI_WIDTH);
    rectAOI.s32Height = GetDlgItemInt(IDC_EDIT_AOI_HEIGHT);

    // get every borders for the aoi
    IS_POINT_2D pointMax;
    INT nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&pointMax , sizeof(pointMax ));

    IS_POINT_2D pointMin;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MIN, (void*)&pointMin , sizeof(pointMin ));

    IS_SIZE_2D sizeMax;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE_MAX, (void*)&sizeMax , sizeof(sizeMax ));

    IS_SIZE_2D sizeMin;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE_MIN, (void*)&sizeMin , sizeof(sizeMin ));

    IS_POINT_2D pointInc;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_INC, (void*)&pointInc , sizeof(pointInc ));

    IS_SIZE_2D sizeInc;
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE_INC, (void*)&sizeInc , sizeof(sizeInc ));

    // get the correct x step
    while (rectAOI.s32X % pointInc.s32X)
    {
        rectAOI.s32X--;
    }
    // get the correct y step
    while (rectAOI.s32Y % pointInc.s32Y)
    {
        rectAOI.s32Y--;
    }
    // get the correct width step
    while (rectAOI.s32Width % sizeInc.s32Width)
    {
        rectAOI.s32Width--;
    }
    // get the correct height step
    while (rectAOI.s32Height % sizeInc.s32Height)
    {
        rectAOI.s32Height--;
    }

    // check x position
    if (rectAOI.s32X > pointMax.s32X)
    {
        rectAOI.s32X = pointMax.s32X;
    }
    if (rectAOI.s32X < pointMin.s32X)
    {
        rectAOI.s32X = pointMin.s32X;
    }

    // check y position
    if (rectAOI.s32Y > pointMax.s32Y)
    {
        rectAOI.s32Y = pointMax.s32Y;
    }
    if (rectAOI.s32Y < pointMin.s32Y)
    {
        rectAOI.s32Y = pointMin.s32Y;
    }

    // check width
    if (rectAOI.s32Width > sizeMax.s32Width)
    {
        rectAOI.s32Width = sizeMax.s32Width;
    }
    if (rectAOI.s32Width < sizeMin.s32Width)
    {
        rectAOI.s32Width = sizeMin.s32Width;
    }

    // check height
    if (rectAOI.s32Height > sizeMax.s32Height)
    {
        rectAOI.s32Height = sizeMax.s32Height;
    }
    if (rectAOI.s32Height < sizeMin.s32Height)
    {
        rectAOI.s32Height = sizeMin.s32Height;
    }

    // set the new correct values
    SetDlgItemInt(IDC_EDIT_AOI_HEIGHT, rectAOI.s32Height);
    SetDlgItemInt(IDC_EDIT_AOI_WIDTH, rectAOI.s32Width);
    SetDlgItemInt(IDC_EDIT_AOI_POS_Y, rectAOI.s32Y);
    SetDlgItemInt(IDC_EDIT_AOI_POS_X, rectAOI.s32X);

    // set the aoi
    nRet = is_AOI(m_hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(rectAOI));

    UpdateExposure();

    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinFps(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    double min, max, timestep, dNewFps, dDummy;

    // Get the frame time range
    is_GetFrameTimeRange(m_hCam, &min, &max, &timestep);

    // caluclate maxfps, minfps, minstep
    double minfps, maxfps, minstep;
    maxfps = 1 / min;
    minfps = 1 / max;
    minstep = (1 / min) - (1 / (min + timestep));

    // get the current framerate
    is_SetFrameRate(m_hCam, IS_GET_FRAMERATE, &dNewFps);

    //increment, decrement framerate
    if (pNMUpDown->iDelta < 0)
    {
        dNewFps += (0.05 / minstep + 1) * minstep;
    }
    else
    {
        dNewFps -= (0.05 / minstep + 1) * minstep;
    }

    // is the new value between the range
    if (dNewFps > maxfps)
    {
        dNewFps = maxfps;
    }
    if (dNewFps < minfps)
    {
        dNewFps = minfps;
    }

    // set the new framerate
    is_SetFrameRate(m_hCam, dNewFps, &dDummy);

    // framerate changed --> update the exposure
    UpdateExposure();

    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnEnReturnEditAoiX()
{
    int nPos = 0;
    nPos = GetDlgItemInt(IDC_EDIT_AOI_X);

    /* Get the maximum possible position */
    IS_POINT_2D aoiPoint;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&aoiPoint, sizeof(aoiPoint));

    /* Get the correct value increment */
    IS_POINT_2D pointInc;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_INC, (void*)&pointInc , sizeof(pointInc ));

    // get the correct x step
    while (nPos % pointInc.s32X)
    {
        nPos--;
    }

    if (nPos < 0)
    {
        nPos = 0;
    }
    if (nPos > aoiPoint.s32X)
    {
        nPos = aoiPoint.s32X;
    }

    m_SequenceParam.s32X = nPos;
    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnEnReturnEditAoiY()
{
    int nPos = 0;
    nPos = GetDlgItemInt(IDC_EDIT_AOI_Y);

    /* Get the maximum possible position */
    IS_POINT_2D aoiPoint;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*)&aoiPoint, sizeof(aoiPoint));

    /* Get the correct value increment */
    IS_POINT_2D pointInc;
    is_AOI(m_hCam, IS_AOI_IMAGE_GET_POS_INC, (void*)&pointInc , sizeof(pointInc ));

    // get the correct y step
    while (nPos % pointInc.s32Y)
    {
        nPos--;
    }

    if (nPos < 0)
    {
        nPos = 0;
    }
    if (nPos > aoiPoint.s32Y)
    {
        nPos = aoiPoint.s32Y;
    }

    m_SequenceParam.s32Y = nPos;
    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnEnReturnEditAoiExp()
{
    CString strTemp;
    GetDlgItemText(IDC_EDIT_AOI_EXP, strTemp);

    double dExposure = _tstof(strTemp);

    // is the new value between the range
    if (dExposure < m_dblMinExp)
    {
        dExposure = m_dblMinExp;
    }
    if (dExposure > m_dblMaxExp)
    {
        dExposure = m_dblMaxExp;
    }

    m_SequenceParam.dblExposure = dExposure;
    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnEnReturnEditAoiMastergain()
{
    int nValue = 0;
    nValue = GetDlgItemInt(IDC_EDIT_AOI_MASTERGAIN);

    // is the new value between the range
    if (nValue < 0)
    {
        nValue = 0;
    }
    if (nValue > 100)
    {
        nValue = 100;
    }

    m_SequenceParam.s32Gain = nValue;
    // Update the property controls
    UpdateProperties();
}
void CuEyeSequenceAoiDlg::OnEnReturnEditFps()
{
    double min, max, timestep, dFps, dDummy;

    CString strTemp;
    GetDlgItemText(IDC_EDIT_FPS, strTemp);
    dFps = _tstof(strTemp);

    // Get the frame time range
    is_GetFrameTimeRange(m_hCam, &min, &max, &timestep);

    // calucalte max, and min fps
    double minfps, maxfps;
    maxfps = 1 / min;
    minfps = 1 / max;

    // is the new value between the range
    if (dFps > maxfps)
    {
        dFps = maxfps;
    }
    if (dFps < minfps)
    {
        dFps = minfps;
    }

    // set the new framerate
    is_SetFrameRate(m_hCam, dFps, &dDummy);

    // framerate changed --> update the exposure
    UpdateExposure();

    // Update the property controls
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::UpdateExposure()
{
    CString strTemp;
    double dTemp[3];

    // get exposure range
    is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void*)&dTemp, sizeof(dTemp));

    m_dblMinExp = dTemp[0];
    m_dblMaxExp = dTemp[1];
    m_dblIncExp = dTemp[2];

    INT nMaxSteps = (int)((m_dblMaxExp - m_dblMinExp) / m_dblIncExp + 0.0005);

    // set range
    m_sliderAOI_EXP.SetRange((int)m_dblMinExp, nMaxSteps, TRUE);

    // set min and max
    strTemp.Format(L"%.2f", m_dblMinExp);
    GetDlgItem(IDC_TXT_AOI_EXP_MIN)->SetWindowText(strTemp);

    strTemp.Format(L"%.2f", m_dblMaxExp);
    GetDlgItem(IDC_TXT_AOI_EXP_MAX)->SetWindowText(strTemp);

    // set slider value
    int value = (int)(((m_SequenceParam.dblExposure - m_dblMinExp) / m_dblIncExp) + 0.5);
    m_sliderAOI_EXP.SetPos(value);

    // hold the exposure value between the range
    GetDlgItemText(IDC_EDIT_AOI_EXP, strTemp);
    double dExposure = _tstof(strTemp);
    if (dExposure > m_dblMaxExp)
    {
        strTemp.Format(L"%.2f", dExposure);
        GetDlgItem(IDC_EDIT_AOI_EXP)->SetWindowText(strTemp);

        m_SequenceParam.dblExposure = m_dblMaxExp;
    }
}

bool CuEyeSequenceAoiDlg::IsSequenceAoi()
{
    if (m_comboCurrentSequence.GetCurSel() != 0)
    {
        // true if sequence mode is selected
        return true;
    }

    // no sequence mode
    return false;
}
void CuEyeSequenceAoiDlg::OnDestroy()
{
    CDialog::OnDestroy();

    delete m_StatusBar;
}

void CuEyeSequenceAoiDlg::OnDeltaposSpinAoiCycle(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;

    int nValue = 0;
    nValue = GetDlgItemInt(IDC_EDIT_AOI_CYCLE);

    if (pNMUpDown->iDelta < 0)
    {
        nValue++;
    }
    else
    {
        nValue--;
    }

    // is the new value between the range
    // values can be 0 - 255

    if (nValue < 0)
    {
        nValue = 0;
    }
    if(nValue > 255)
    {
        nValue = 255;
    }

    m_SequenceParam.s32NumberOfCycleRepetitions = nValue;
    UpdateProperties();
}

void CuEyeSequenceAoiDlg::OnEnReturnEditCycle()
{
    int nValue = 0;
    nValue = GetDlgItemInt(IDC_EDIT_AOI_CYCLE);

    // is the new value between the range
    // values can be 0 - 255

    if (nValue < 0)
    {
        nValue = 0;
    }
    if(nValue > 255)
    {
        nValue = 255;
    }

    m_SequenceParam.s32NumberOfCycleRepetitions = nValue;
    UpdateProperties();
}


void CuEyeSequenceAoiDlg::DisableEnableSpecificProperties(BOOL enable)
{
    GetDlgItem(IDC_EDIT_AOI_CYCLE)->EnableWindow(enable);

    m_sliderAOI_X.EnableWindow(enable);
    m_sliderAOI_Y.EnableWindow(enable);

    GetDlgItem(IDC_EDIT_AOI_X)->EnableWindow(enable);
    GetDlgItem(IDC_EDIT_AOI_Y)->EnableWindow(enable);
}

INT CuEyeSequenceAoiDlg::AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel)
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

INT CuEyeSequenceAoiDlg::FreeImageMems(void)
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

INT CuEyeSequenceAoiDlg::InitSequence(void)
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

INT CuEyeSequenceAoiDlg::ClearSequence(void)
{
    return is_ClearSequence(m_hCam);
}

INT CuEyeSequenceAoiDlg::GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId)
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