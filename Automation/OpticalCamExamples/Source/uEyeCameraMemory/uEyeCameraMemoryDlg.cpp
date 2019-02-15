//==========================================================================//
//                                                                          //
//  Copyright (C) 2016 - 2018                                               //
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
#include "uEyeCameraMemory.h"
#include "uEyeCameraMemoryDlg.h"
#include "version.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Camera::Camera(void)
    : hCam(0)
    , m_cnNumberOfSeqMemory(3)
{
    m_vecMemory.reserve(m_cnNumberOfSeqMemory);
    m_vecMemory.resize(m_cnNumberOfSeqMemory);

    memset(&sensorInfo, 0, sizeof(sensorInfo));
    memset(&cameraInfo, 0, sizeof(cameraInfo));
}

INT Camera::AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel)
{
    INT nRet = IS_SUCCESS;

    for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
    {
        nRet = is_AllocImageMem(hCam, nSizeX, nSizeY, nBitsPerPixel, &(iter->pcImageMemory), &(iter->lMemoryId));

        if (IS_SUCCESS != nRet)
        {
            FreeImageMems();
            break;
        }
    }

    return nRet;
}

INT Camera::FreeImageMems(void)
{
    INT nRet = IS_SUCCESS;

    // Free the allocated buffer
    for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
    {
        if (NULL != iter->pcImageMemory)
        {
            nRet = is_FreeImageMem(hCam, iter->pcImageMemory, iter->lMemoryId);
        }

        iter->pcImageMemory = NULL;
        iter->lMemoryId = 0;
        iter->lSequenceId = 0;
    }

    return nRet;
}

INT Camera::InitSequence(void)
{
    INT nRet = IS_SUCCESS;

    int i = 0;
    for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter, i++)
    {
        nRet = is_AddToSequence(hCam, iter->pcImageMemory, iter->lMemoryId);

        if (IS_SUCCESS != nRet)
        {
            ClearSequence();
            break;
        }

        iter->lSequenceId = i + 1;
    }

    return nRet;
}

INT Camera::ClearSequence(void)
{
    return is_ClearSequence(hCam);
}

INT Camera::GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId)
{
    INT nRet = IS_NO_SUCCESS;

    int dummy = 0;
    char *pLast = NULL;
    char *pMem = NULL;

    nRet = is_GetActSeqBuf(hCam, &dummy, &pMem, &pLast);

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

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

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



CuEyeCameraMemoryDlg::CuEyeCameraMemoryDlg(CWnd* pParent /*=NULL*/)
: CDialog(CuEyeCameraMemoryDlg::IDD, pParent)
, m_Camera()
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CuEyeCameraMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ITER_RANGE, m_CbIteration);
    DDX_Control(pDX, IDC_COMBO_IMG_RANGE, m_CbImage);
}


BEGIN_MESSAGE_MAP(CuEyeCameraMemoryDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CuEyeCameraMemoryDlg::OnBnClickedButtonStartCapture)

    /* Trigger mode */
    ON_BN_CLICKED(IDC_RADIO_POST_TRIGGER_HI_LO, &CuEyeCameraMemoryDlg::OnBnClickedRadioPostTriggerHiLo)
    ON_BN_CLICKED(IDC_RADIO_PRE_TRIGGER_HI_LO, &CuEyeCameraMemoryDlg::OnBnClickedRadioPreTriggerHiLo)
    ON_BN_CLICKED(IDC_RADIO_POST_TRIGGER_LO_HI, &CuEyeCameraMemoryDlg::OnBnClickedRadioPostTriggerLoHi)
    ON_BN_CLICKED(IDC_RADIO_PRE_TRIGGER_LO_HI, &CuEyeCameraMemoryDlg::OnBnClickedRadioPreTriggerLoHi)

    /* Force trigger */
    ON_BN_CLICKED(IDC_BUTTON_FORCE_TRIGGER, &CuEyeCameraMemoryDlg::OnBnClickedButtonForceTrigger)

    /* Trigger burst */
    ON_EN_KILLFOCUS(IDC_EDIT_TRIGGER_BURST, &CuEyeCameraMemoryDlg::OnEnKillfocusEditTriggerBurst)

    /* Iteration */
    ON_CBN_SELCHANGE(IDC_COMBO_ITER_RANGE, &CuEyeCameraMemoryDlg::OnCbnSelchangeComboIteration)
    ON_BN_CLICKED(IDC_BUTTON_RELEASE, &CuEyeCameraMemoryDlg::OnBnClickedButtonRelease)

    /* Image */
    ON_CBN_SELCHANGE(IDC_COMBO_IMG_RANGE, &CuEyeCameraMemoryDlg::OnCbnSelchangeComboImage)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_IMAGE_ID, &CuEyeCameraMemoryDlg::OnDeltaposSpinImageId)

    /* Loop */
    ON_BN_CLICKED(IDC_BTN_LOOP, &CuEyeCameraMemoryDlg::OnBnClickedBtnLoop)

    ON_BN_CLICKED(IDC_BTN_EXIT, &CuEyeCameraMemoryDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()



BOOL CuEyeCameraMemoryDlg::OnInitDialog()
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

    /* We start with the post hardware trigger HI LO */
    ((CButton*)GetDlgItem(IDC_RADIO_POST_TRIGGER_HI_LO))->SetCheck(TRUE);

    /* Open our camera */
    OpenCamera();

    m_threadLoop = NULL;
    m_bThreadLoop = FALSE;

    return TRUE;
}


void CuEyeCameraMemoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CuEyeCameraMemoryDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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


HCURSOR CuEyeCameraMemoryDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


/********************************************************************************/
/*                              Exit program                                    */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnClose()
{
    ExitCamera();
    CDialog::OnClose();
}


void CuEyeCameraMemoryDlg::OnBnClickedBtnExit()
{
    ExitCamera();
    PostQuitMessage(0);
}


/********************************************************************************/
/*                              Handle messages                                 */
/********************************************************************************/
LRESULT CuEyeCameraMemoryDlg::OnUEyeMessage(WPARAM wParam, LPARAM lParam)
{
    switch ( wParam )
    {
    case IS_FRAME:
        {
            char* pLast = NULL;
            INT lMemoryId = 0;
            INT lSequenceId = 0;

            if (IS_SUCCESS == m_Camera.GetLastMem(&pLast, lMemoryId, lSequenceId))
            {
                INT nRet = is_LockSeqBuf(m_Camera.hCam, IS_IGNORE_PARAMETER, pLast);

                if (IS_SUCCESS == nRet)
                {
                    /* Display our memory */
                    is_RenderBitmap(m_Camera.hCam, lMemoryId, GetDlgItem(IDC_DISPLAY)->GetSafeHwnd(), IS_RENDER_FIT_TO_WINDOW);

                    // Update images in use and available
                    UEYEIMAGEINFO ImageInfo;
                    if (is_GetImageInfo(m_Camera.hCam, lMemoryId, &ImageInfo, sizeof(ImageInfo)) == IS_SUCCESS)
                    {
                        m_u32NumberImageBuffers = (INT) ImageInfo.dwImageBuffers;
                        m_u32NumberImageBuffersInUse = (INT) ImageInfo.dwImageBuffersInUse;
                        CString Str;
                        Str.Format(L"Image buffers: available = %d, in use = %d", ImageInfo.dwImageBuffers, ImageInfo.dwImageBuffersInUse);
                        GetDlgItem(IDC_IMAGE_BUFFERS)->SetWindowText(Str);
                    }

                    is_UnlockSeqBuf(m_Camera.hCam, IS_IGNORE_PARAMETER, pLast);
                }
            }
        }
        break;

    case IS_CAMERA_MEMORY:
        {
            if (m_bThreadLoop == FALSE)
            {
                /* Something has changed in the camera memory -> update */
                UpdateIterations();
            }
        }
        break;
    }
    return 0;
}


BOOL CuEyeCameraMemoryDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN)
    {
        if ((pMsg->wParam == VK_ESCAPE))
        {
            return 0;
        }
        if ((pMsg->wParam == VK_RETURN))
        {
            if (GetFocus() == GetDlgItem(IDC_EDIT_TRIGGER_BURST))
            {
                OnEnKillfocusEditTriggerBurst();
            }

            return 0;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}


/********************************************************************************/
/*                              Open uEye camera                                */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OpenCamera()
{
    int nRet = 0;

    /* Init camera */
    nRet = is_InitCamera(&m_Camera.hCam, 0);

    /************************************************************************************************/
    /*                                                                                              */
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*                                                                                              */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */
    /*  is "OR"-ed to m_Camera.hCam. This flag allows an automatic upload of the firmware.          */
    /*                                                                                              */
    /************************************************************************************************/
    if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
    {
        // Time for the firmware upload = 25 seconds by default
        INT nUploadTime = 25000;
        is_GetDuration (m_Camera.hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);

        CString Str1, Str2, Str3;
        Str1 = "This camera requires a new firmware. The upload will take about";
        Str2 = "seconds. Please wait ...";
        Str3.Format (L"%s %d %s", Str1, nUploadTime / 1000, Str2);
        AfxMessageBox (Str3, MB_ICONWARNING);

        // Set mouse to hourglass
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        m_Camera.hCam = (HIDS) (((INT)m_Camera.hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
        nRet = is_InitCamera (&m_Camera.hCam, 0);
    }

    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Opening camera failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    /* Get sensor information */
    nRet = is_GetSensorInfo(m_Camera.hCam, &m_Camera.sensorInfo);
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Receiving sensor information failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    /* Get camera information */
    nRet = is_GetCameraInfo(m_Camera.hCam, &m_Camera.cameraInfo);
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Receiving camera information failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    /* This program only works with ETH cameras */
    if ((m_Camera.cameraInfo.Type & IS_INTERFACE_TYPE_ETH) == 0)
    {
        AfxMessageBox(L"This application only works with uEye GigE cameras");
        PostQuitMessage(0);
    }

    m_Camera.ClearSequence();
    m_Camera.FreeImageMems();

    /* Allocate a memory buffer */
    if (m_Camera.sensorInfo.nColorMode == IS_COLORMODE_BAYER)
    {
        nRet = m_Camera.AllocImageMems(m_Camera.sensorInfo.nMaxWidth, m_Camera.sensorInfo.nMaxHeight, 24);

        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"Allocating memory failed with: %d", nRet);
            AfxMessageBox(strTemp);
            PostQuitMessage(0);
        }

        nRet = is_SetColorMode(m_Camera.hCam, IS_CM_BGR8_PACKED);
        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"SetColorMode failed: %d", nRet);
            AfxMessageBox(strTemp);
            PostQuitMessage(0);
        }
    }
    else
    {
        nRet = m_Camera.AllocImageMems(m_Camera.sensorInfo.nMaxWidth, m_Camera.sensorInfo.nMaxHeight, 8);

        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"Allocating memory failed with: %d", nRet);
            AfxMessageBox(strTemp);
            PostQuitMessage(0);
        }

        nRet = is_SetColorMode(m_Camera.hCam, IS_CM_MONO8);
        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"SetColorMode failed: %d", nRet);
            AfxMessageBox(strTemp);
            PostQuitMessage(0);
        }
    }

    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Allocating memory failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    m_Camera.InitSequence();

    /* Enable frame message */
    nRet = is_EnableMessage(m_Camera.hCam, IS_FRAME, GetSafeHwnd());
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Could not enable the frame message. Failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    /* Enable camera memory message */
    nRet = is_EnableMessage(m_Camera.hCam, IS_CAMERA_MEMORY, GetSafeHwnd());
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Could not enable the camera memory message. Failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    /* Enable hardware post-trigger mode */
    nRet = is_SetExternalTrigger(m_Camera.hCam, IS_SET_TRIGGER_HI_LO);
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Setting the external trigger mode failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    /* Set trigger timeout to a high value to avoid any timeout */
    nRet = is_SetTimeout(m_Camera.hCam, IS_TRIGGER_TIMEOUT, 1000000000);
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Setting the trigger timeout failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    UINT nCaps = 0;
    nRet = is_Transfer(m_Camera.hCam, TRANSFER_CMD_GET_IMAGE_DESTINATION_CAPABILITIES, (void*)&nCaps, sizeof(nCaps));
    if ((nRet == IS_SUCCESS) && ((nCaps & IS_TRANSFER_DESTINATION_DEVICE_MEMORY) != 0))
    {
        /* Get current image destination (is user memory by default) */
        TRANSFER_TARGET transferMode;
        nRet = is_Transfer(m_Camera.hCam, TRANSFER_CMD_GET_IMAGE_DESTINATION, (void*)&transferMode, sizeof(transferMode));
        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"Getting transfer mode failed with: %d", nRet);
            AfxMessageBox(strTemp);
            PostQuitMessage(0);
        }

        /* Enable camera memory mode */
        transferMode = IS_TRANSFER_DESTINATION_DEVICE_MEMORY;
        nRet = is_Transfer(m_Camera.hCam, TRANSFER_CMD_SET_IMAGE_DESTINATION, (void*)&transferMode, sizeof(transferMode));
        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"Setting transfer mode failed with: %d", nRet);
            AfxMessageBox(strTemp);
            PostQuitMessage(0);
        }
    }

    UpdateBurstSize();

    /* Update camera information controls */
    UpdateCameraInformation();
}


/********************************************************************************/
/*                              Close uEye camera                               */
/********************************************************************************/
void CuEyeCameraMemoryDlg::ExitCamera()
{
    if (m_Camera.hCam != 0)
    {
        /* Disable messages */
        is_EnableMessage(m_Camera.hCam, IS_FRAME, NULL);
        is_EnableMessage(m_Camera.hCam, IS_CAMERA_MEMORY, NULL);

        m_Camera.ClearSequence();
        m_Camera.FreeImageMems();

        /* Exit camera */
        is_ExitCamera(m_Camera.hCam);
        m_Camera.hCam = 0;
    }
}


/********************************************************************************/
/*                      Read information about the camera                       */
/********************************************************************************/
void CuEyeCameraMemoryDlg::UpdateCameraInformation()
{
    CString strTemp;

    /* Sensor name */
    strTemp.Format(L"Type: %s", CString(m_Camera.sensorInfo.strSensorName));
    SetDlgItemText(IDC_TEXT_CAMERA_TYPE, strTemp);

    /* Serial */
    strTemp.Format(L"Serial: %s", CString(m_Camera.cameraInfo.SerNo));
    SetDlgItemText(IDC_TEXT_CAMERA_SERIAL, strTemp);

    /* Resolution */
    strTemp.Format(L"Resolution: %dx%d", m_Camera.sensorInfo.nMaxWidth, m_Camera.sensorInfo.nMaxHeight);
    SetDlgItemText(IDC_TEXT_CAMERA_RESOLUTION, strTemp);

    /* Color mode */
    strTemp.Format(L"Color mode: %s", m_Camera.sensorInfo.nColorMode == IS_COLORMODE_BAYER ? L"Bayer (24Bit)" : L"Mono (8Bit)");
    SetDlgItemText(IDC_TEXT_CAMERA_COLORMODE, strTemp);
}


/********************************************************************************/
/*                          Start/Stop capturing                                */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnBnClickedButtonStartCapture()
{
    CString strTemp;
    GetDlgItemText(IDC_BUTTON_CAPTURE, strTemp);

    if (strTemp == "Start")
    {
        /* Start capture video */
        INT nRet = is_CaptureVideo(m_Camera.hCam, IS_DONT_WAIT);
        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"Capture video failed with: %d", nRet);
            AfxMessageBox(strTemp);
        }
        else
        {
            SetDlgItemText(IDC_BUTTON_CAPTURE, L"Stop");
            GetDlgItem(IDC_BUTTON_FORCE_TRIGGER)->EnableWindow(TRUE);
            EnableTriggerControls(FALSE);
        }
    }
    else
    {
        /* Stop capture video */
        int nRet = is_StopLiveVideo(m_Camera.hCam, IS_DONT_WAIT);
        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"is_StopLiveVideo failed with: %d", nRet);
            AfxMessageBox(strTemp);
        }
        else
        {
            SetDlgItemText(IDC_BUTTON_CAPTURE, L"Start");
            GetDlgItem(IDC_BUTTON_FORCE_TRIGGER)->EnableWindow(FALSE);
            EnableTriggerControls(TRUE);

            GetDlgItem(IDC_IMAGE_BUFFERS)->SetWindowText(L"");

            m_bThreadLoop = FALSE;
        }
    }

    UpdateIterations();
}


/********************************************************************************/
/*                              Release iterations                              */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnBnClickedButtonRelease()
{  
    /* Get iteration range */
    ID_RANGE iterRange;
    INT nRet = is_ImageBuffer(m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_GET_AVAILABLE_ITERATIONS, &iterRange, sizeof(iterRange));
    if (nRet == IS_SUCCESS)
    {
        nRet = is_ImageBuffer(m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_RELEASE_ITERATIONS, (void*)&iterRange.s32Last, sizeof(iterRange.s32Last));
        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"is_ImageBuffer failed with: %d", nRet);
            AfxMessageBox(strTemp);
        }
        else
        {
            UpdateIterations();
        }
    }
}


/********************************************************************************/
/*                          Handle iteration combo box                          */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnCbnSelchangeComboIteration()
{
    UpdateImageRange();
}


/********************************************************************************/
/*                          Handle image combo box                              */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnCbnSelchangeComboImage()
{
    if ((m_CbIteration.GetCount() != 0) && (m_CbImage.GetCount() != 0))
    {
        /* Get current iteration and image */
        IMGBUF_ITEM item;
        item.s32ImageID		= (INT)m_CbImage.GetItemData(m_CbImage.GetCurSel());
        item.u32IterationID	= (INT)m_CbIteration.GetItemData(m_CbIteration.GetCurSel());

        /* Transfer concrete image to user buffer */
        INT nRet = is_ImageBuffer(m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_TRANSFER_IMAGE, (void*)&item, sizeof(item));

        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"Transfer image to user buffer failed with: %d", nRet);
            AfxMessageBox(strTemp);
        }
    }
}


/********************************************************************************/
/*                          Handle image spin control                           */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnDeltaposSpinImageId(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (m_CbImage.GetCount() == 0)
    {
        return;
    }

    INT nIndex = m_CbImage.GetCurSel();

    if (pNMUpDown->iDelta < 0)
    {
        nIndex++;
    }
    else
    {
        nIndex--;
    }

    if (nIndex < 0)
    {
        nIndex = 0;
    }

    if (nIndex > m_CbImage.GetCount() - 1)
    {
        nIndex = m_CbImage.GetCount() - 1;
    }

    m_CbImage.SetCurSel(nIndex);

    IMGBUF_ITEM item;
    item.u32IterationID	= (UINT)m_CbIteration.GetItemData(m_CbIteration.GetCurSel());
    item.s32ImageID		= (INT)m_CbImage.GetItemData(nIndex);

    /* Transfer concrete image to user buffer */
    INT nRet = is_ImageBuffer(m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_TRANSFER_IMAGE, (void*)&item, sizeof(item));
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"Transfer image to user buffer failed with: %d", nRet);
        AfxMessageBox(strTemp);
    }

    *pResult = 0;
}


/********************************************************************************/
/*                          Handle force trigger button                         */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnBnClickedButtonForceTrigger()
{
    is_ForceTrigger(m_Camera.hCam);
}


/********************************************************************************/
/*                          Handle loop button                                  */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnBnClickedBtnLoop()
{
    if (m_bThreadLoop == FALSE)
    {
        GetDlgItem(IDC_BTN_LOOP)->SetWindowText(L"Stop loop");
        EnableAllControlsExceptLoop(FALSE);
        m_threadLoop = AfxBeginThread(ThreadLoop, this);
    }
    else
    {
        EnableAllControlsExceptLoop(TRUE);
        m_bThreadLoop = FALSE;
    }
}


/********************************************************************************/
/*                              Set trigger mode                                */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnBnClickedRadioPostTriggerHiLo()
{
    /* Enable hardware post-trigger (with trigger burst) */
    INT nRet = is_SetExternalTrigger(m_Camera.hCam, IS_SET_TRIGGER_HI_LO);
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"is_SetExternalTrigger failed with: %d", nRet);
        AfxMessageBox(strTemp);
    } 

    UpdateBurstSize();
}


void CuEyeCameraMemoryDlg::OnBnClickedRadioPostTriggerLoHi()
{
    /* Enable hardware post-trigger (with trigger burst) */
    INT nRet = is_SetExternalTrigger(m_Camera.hCam, IS_SET_TRIGGER_LO_HI);
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"is_SetExternalTrigger failed with: %d", nRet);
        AfxMessageBox(strTemp);
    } 

    UpdateBurstSize();
}


void CuEyeCameraMemoryDlg::OnBnClickedRadioPreTriggerHiLo()
{
    /* Enable hardware pre-trigger */
    INT nRet = is_SetExternalTrigger(m_Camera.hCam, IS_SET_TRIGGER_PRE_HI_LO);
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"is_SetExternalTrigger failed with: %d", nRet);
        AfxMessageBox(strTemp);
    } 

    UpdateBurstSize();
}


void CuEyeCameraMemoryDlg::OnBnClickedRadioPreTriggerLoHi()
{
    /* Enable hardware pre-trigger */
    INT nRet = is_SetExternalTrigger(m_Camera.hCam, IS_SET_TRIGGER_PRE_LO_HI);
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"is_SetExternalTrigger failed with: %d", nRet);
        AfxMessageBox(strTemp);
    }

    UpdateBurstSize();
}


/********************************************************************************/
/*                              Set trigger burst size                          */
/********************************************************************************/
void CuEyeCameraMemoryDlg::OnEnKillfocusEditTriggerBurst()
{
    INT nRet = IS_NO_SUCCESS;

    /* Update trigger burst size */
    m_u32BurstSize = GetDlgItemInt(IDC_EDIT_TRIGGER_BURST);

    if ((m_u32BurstSize >= m_rangeBurstSize.u32Minimum) &&
        (m_u32BurstSize <= m_rangeBurstSize.u32Maximum))
    {
        nRet = is_Trigger(m_Camera.hCam, IS_TRIGGER_CMD_SET_BURST_SIZE, (void*)&m_u32BurstSize, sizeof(m_u32BurstSize));
        if (nRet != IS_SUCCESS)
        {
            CString strTemp;
            strTemp.Format(L"is_Trigger failed with: %d", nRet);
            AfxMessageBox(strTemp);
        }
    }

    if (nRet == IS_NO_SUCCESS)
    {
        UpdateBurstSize();
    }
}


/********************************************************************************/
/*                      Update iteration and image controls                     */
/********************************************************************************/
void CuEyeCameraMemoryDlg::UpdateIterations()
{
    INT nRet = IS_SUCCESS;

    /* Update our iteration range */
    ID_RANGE iterRange;
    nRet = is_ImageBuffer(m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_GET_AVAILABLE_ITERATIONS, &iterRange, sizeof(iterRange));
    if (nRet == IS_SUCCESS)
    {
        /* Update iteration edit controls */
        SetDlgItemInt(IDC_EDIT_ITER_BEG, iterRange.s32First, FALSE);
        SetDlgItemInt(IDC_EDIT_ITER_END, iterRange.s32Last, FALSE);

        /* Update iteration combobox */
        m_CbIteration.ResetContent();

        INT nCounter = 0;
        for (INT i = iterRange.s32First; i <= iterRange.s32Last; ++i)
        {
            CString strTemp;
            strTemp.Format(L"%u", i);

            m_CbIteration.InsertString(nCounter, strTemp);
            m_CbIteration.SetItemData(nCounter++, i);
        }

        /* Set active */
        m_CbIteration.SetCurSel(0);
    }
    else
    {
        /* No iterations are available */
        m_CbIteration.ResetContent();
        SetDlgItemText(IDC_EDIT_ITER_BEG, L"");
        SetDlgItemText(IDC_EDIT_ITER_END, L"");
    }

    /* If we have iterations nRet == IS_SUCCESS */
    BOOL bEnableControls = nRet == IS_SUCCESS;

    m_CbIteration.EnableWindow(bEnableControls);
    GetDlgItem(IDC_BUTTON_RELEASE)->EnableWindow(bEnableControls);
    GetDlgItem(IDC_BTN_LOOP)->EnableWindow(bEnableControls);

    // Update image range
    UpdateImageRange();
}


void CuEyeCameraMemoryDlg::UpdateImageRange()
{
    if (m_CbIteration.GetCount() != 0)
    {
        /* Set iteration */
        IMGBUF_ITERATION_INFO iterationInfo;
        iterationInfo.u32IterationID = (INT)m_CbIteration.GetItemData(m_CbIteration.GetCurSel());

        /* Update image iteration */
        INT nRet = is_ImageBuffer(m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_GET_ITERATION_INFO, &iterationInfo, sizeof(iterationInfo));
        if (nRet == IS_SUCCESS)
        {
            m_CbImage.ResetContent();

            SetDlgItemInt(IDC_EDIT_IMG_BEG, iterationInfo.rangeImageID.s32First);
            SetDlgItemInt(IDC_EDIT_IMG_END, iterationInfo.rangeImageID.s32Last);

            /* Update image combobox */
            INT nCounter = 0;
            for (INT i = iterationInfo.rangeImageID.s32First; i <= iterationInfo.rangeImageID.s32Last; i++)
            {
                CString strTemp;
                strTemp.Format(L"%d", i);

                m_CbImage.InsertString(nCounter, strTemp);
                m_CbImage.SetItemData(nCounter++, i);
            }

            m_CbImage.EnableWindow(TRUE);

            GetDlgItem(IDC_BUTTON_RELEASE)->EnableWindow(TRUE);
            GetDlgItem(IDC_SPIN_IMAGE_ID)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_LOOP)->EnableWindow(TRUE);

            /* Transfer first image to user buffer */
            m_CbImage.SetCurSel(0);

            IMGBUF_ITEM item;
            item.u32IterationID	= iterationInfo.u32IterationID;
            item.s32ImageID		= iterationInfo.rangeImageID.s32First;

            nRet = is_ImageBuffer(m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_TRANSFER_IMAGE, (void*)&item, sizeof(item));
            if (nRet != IS_SUCCESS)
            {
                CString strTemp;
                strTemp.Format(L"Transfer image to user buffer failed with: %d", nRet);
                AfxMessageBox(strTemp);
            }

        }
    }
    else
    {
        m_CbImage.ResetContent();

        m_CbImage.EnableWindow(FALSE);

        GetDlgItem(IDC_BUTTON_RELEASE)->EnableWindow(FALSE);
        GetDlgItem(IDC_SPIN_IMAGE_ID)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_LOOP)->EnableWindow(FALSE);

        SetDlgItemText(IDC_EDIT_IMG_BEG, L"");
        SetDlgItemText(IDC_EDIT_IMG_END, L"");
    }
}


/********************************************************************************/
/*                          Update burst size controls                          */
/********************************************************************************/
void CuEyeCameraMemoryDlg::UpdateBurstSize()
{
    INT nRet = is_Trigger(m_Camera.hCam, IS_TRIGGER_CMD_GET_BURST_SIZE_RANGE, &m_rangeBurstSize, sizeof(m_rangeBurstSize));
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"GetBurstSizeRange failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    UINT u32BurstSize = 0;
    nRet = is_Trigger(m_Camera.hCam, IS_TRIGGER_CMD_GET_BURST_SIZE, &m_u32BurstSize, sizeof(m_u32BurstSize));
    if (nRet != IS_SUCCESS)
    {
        CString strTemp;
        strTemp.Format(L"GetBurstSize failed with: %d", nRet);
        AfxMessageBox(strTemp);
        PostQuitMessage(0);
    }

    CString Str;
    Str.Format(L"%d", m_u32BurstSize);
    GetDlgItem(IDC_EDIT_TRIGGER_BURST)->SetWindowText(Str);
}


/********************************************************************************/
/*                          Enable/Disable trigger controls                     */
/********************************************************************************/
void CuEyeCameraMemoryDlg::EnableTriggerControls(BOOL bEnable)
{
    GetDlgItem(IDC_RADIO_POST_TRIGGER_HI_LO)->EnableWindow(bEnable);
    GetDlgItem(IDC_RADIO_POST_TRIGGER_LO_HI)->EnableWindow(bEnable);
    GetDlgItem(IDC_RADIO_PRE_TRIGGER_HI_LO)->EnableWindow(bEnable);
    GetDlgItem(IDC_RADIO_PRE_TRIGGER_LO_HI)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_BURSTSIZE)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_TRIGGER_BURST)->EnableWindow(bEnable);
}


void CuEyeCameraMemoryDlg::EnableAllControlsExceptLoop(BOOL bEnable)
{
    GetDlgItem(IDC_STATIC_RANGE_BEGIN)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_RANGE_END)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_SELECTED_ID)->EnableWindow(bEnable);

    GetDlgItem(IDC_EDIT_ITER_BEG)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ITER_END)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_ITER_RANGE)->EnableWindow(bEnable);

    GetDlgItem(IDC_STATIC_IMAGE_RANGE_BEGIN)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_IMAGE_RANGE_END)->EnableWindow(bEnable);
    GetDlgItem(IDC_STATIC_SELECTED_IMAGE_ID)->EnableWindow(bEnable);

    GetDlgItem(IDC_EDIT_IMG_BEG)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_IMG_END)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_IMG_RANGE)->EnableWindow(bEnable);
    GetDlgItem(IDC_SPIN_IMAGE_ID)->EnableWindow(bEnable);

    GetDlgItem(IDC_BUTTON_CAPTURE)->EnableWindow(bEnable);
    GetDlgItem(IDC_BUTTON_FORCE_TRIGGER)->EnableWindow(bEnable);
    GetDlgItem(IDC_BUTTON_RELEASE)->EnableWindow(bEnable);
}


/********************************************************************************/
/*                              Thread for the loop                             */
/********************************************************************************/
UINT CuEyeCameraMemoryDlg::ThreadLoop(LPVOID pParam)
{
    CuEyeCameraMemoryDlg* pParent = (CuEyeCameraMemoryDlg*)pParam;
    if (pParent != NULL)
    {
        pParent->m_bThreadLoop = TRUE;

        /* Set iteration */
        IMGBUF_ITERATION_INFO iterationInfo;
        iterationInfo.u32IterationID = (INT)pParent->m_CbIteration.GetItemData(pParent->m_CbIteration.GetCurSel());

        IMGBUF_ITEM item;
        item.u32IterationID = iterationInfo.u32IterationID;

        /* Update image iteration */
        INT nRet = is_ImageBuffer(pParent->m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_GET_ITERATION_INFO, &iterationInfo, sizeof(iterationInfo));
        if (nRet == IS_SUCCESS)
        {
            INT nIndex = iterationInfo.rangeImageID.s32First;
            while (pParent->m_bThreadLoop == TRUE)
            {
                item.s32ImageID = nIndex;
                nRet = is_ImageBuffer(pParent->m_Camera.hCam, IS_IMGBUF_DEVMEM_CMD_TRANSFER_IMAGE, (void*)&item, sizeof(item));

                nIndex++;

                Sleep(50);

                if (nIndex > iterationInfo.rangeImageID.s32Last)
                {
                    nIndex = iterationInfo.rangeImageID.s32First;
                }
            }

            nIndex--;

            if (nIndex < iterationInfo.rangeImageID.s32First)
            {
                nIndex = iterationInfo.rangeImageID.s32Last;
            }

            for (INT i = 0; i < pParent->m_CbImage.GetCount(); i++)
            {
                if ((INT)pParent->m_CbImage.GetItemData(i) == nIndex)
                {
                    pParent->m_CbImage.SetCurSel(nIndex);
                    pParent->m_CbImage.Invalidate();
                }
            }
        }
    }

    pParent->GetDlgItem(IDC_BTN_LOOP)->SetWindowText(L"Start loop");

    return TRUE;
}
