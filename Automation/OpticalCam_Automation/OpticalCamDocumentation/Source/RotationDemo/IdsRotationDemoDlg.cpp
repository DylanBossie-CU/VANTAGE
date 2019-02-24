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

// IdsRotationDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IdsRotationDemo.h"
#include "IdsRotationDemoDlg.h"
#include ".\idsRotationDemodlg.h"
#include <process.h>
#include "version.h"

//define for the message from the thread
#define  UPDATE_FRAMERATE WM_USER+1

extern CIdsRotationDemoApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//global list of the dialog items
//used for easy dynamic positioning
INT g_nDlgItemsID[] ={ IDC_BUTTON_START,
IDC_BUTTON_STOP,
IDC_BUTTON_SNAP,
IDC_BUTTON_EXIT,
IDC_STATIC_EXPOSURE,
IDC_SLIDER_EXPOSURE,
IDC_STATIC_GAIN,
IDC_SLIDER_GAIN};

INT g_nDlgItemsCount = 8;

//prototype of the thread
void ImageProcessing(void* pParam);

//defines used to fast switching between the modes
#define IDS_LIVE_MODE 1
#define IDS_SNAP_MODE 2

//events used to control the thread
HANDLE g_hFrameEvent;
HANDLE g_hCloseThread;
HANDLE g_hExitThread;

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
// CIdsRotationDemoDlg dialog

CIdsRotationDemoDlg::CIdsRotationDemoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIdsRotationDemoDlg::IDD, pParent)
, m_cnNumberOfSeqMemory(3)
{
    //{{AFX_DATA_INIT(CIdsRotationDemoDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIdsRotationDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIdsRotationDemoDlg)
    DDX_Control(pDX, IDC_SLIDER_EXPOSURE, m_ctrlSliderExposure);
    DDX_Control(pDX, IDC_SLIDER_GAIN, m_ctrlSliderGain);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIdsRotationDemoDlg, CDialog)
    //{{AFX_MSG_MAP(CIdsRotationDemoDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_MESSAGE(UPDATE_FRAMERATE,OnUpdateFramerate)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_SNAP, OnButtonSnap)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdsRotationDemoDlg message handlers

BOOL CIdsRotationDemoDlg::OnInitDialog()
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

    m_vecMemory.reserve(m_cnNumberOfSeqMemory);
    m_vecMemory.resize(m_cnNumberOfSeqMemory);

    m_hCam = 0;
    m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;

    m_lMemoryIdVert = 0;
    m_pcImageMemoryVert = NULL;

    m_lMemoryIdSnap = 0;
    m_pcImageMemorySnap = NULL;

    m_lMemoryIdSnapVert = 0;
    m_pcImageMemorySnapVert = NULL;

    m_bThreadIsRunning = FALSE;
    m_nLastMode = 0;

    //create the events to control the thread
    g_hCloseThread = CreateEvent(NULL, FALSE, FALSE, L"CloseThread");
    g_hExitThread = CreateEvent(NULL, FALSE, FALSE, L"ExitThread");

    //Get the handles to the display so we can show the images
    m_hWndDisplayVert = GetDlgItem( IDC_DISPLAY_VERT )->m_hWnd; // get handle to display window

    //set the window title
    SetWindowText(L"UI-1460-C RotationDemo");

    //position all the dialog items dynamically
    RepositionDialogItems();

    m_bFirstTime = TRUE;
    m_strPath = "-1";

    return true;
}



void CIdsRotationDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.

void CIdsRotationDemoDlg::OnPaint()
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

    if(m_nLastMode==IDS_LIVE_MODE)
        DisplayuEyeImage(m_lMemoryIdVert, IS_RENDER_NORMAL);
    if(m_nLastMode==IDS_SNAP_MODE)
        DisplayuEyeImage(m_lMemoryIdSnapVert, IS_RENDER_FIT_TO_WINDOW);
}



// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CIdsRotationDemoDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::OnButtonStart()
//
// DESCRIPTION: start live display and return immediately
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::OnButtonStart()
{
    INT nRet = IS_SUCCESS;

    //if there is no open camera open one
    if( m_hCam == 0 )
        OpenCamera();

    //if the camera is now open start the imaging thread
    if( m_hCam != 0 )
    {
        //Set the binning mode so that the UI1460C can reach up to 30fps
        nRet = is_SetBinning(m_hCam, IS_BINNING_4X_VERTICAL | IS_BINNING_4X_HORIZONTAL);

        SetDlgItemText(IDC_STATIC_MODE, L"Mode: Binning");

        //Set the frame rate of 25 fps
        double dDummy;
        nRet = is_SetFrameRate(m_hCam, 25.0, &dDummy);

        //set the exposure time
        nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&m_ExposureTime, sizeof(m_ExposureTime));

        ClearSequence();
        InitSequence();

        //start image acquisition
        nRet = is_CaptureVideo( m_hCam, IS_WAIT );

        //start the thread
        if(!m_bThreadIsRunning)
            StartThread();

        //disable the start button
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

        // remember that we are in live mode
        m_nLastMode = IDS_LIVE_MODE;
    }
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::OnButtonStop()
//
// DESCRIPTION: stop live display and return immediately
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::OnButtonStop()
{
    // stop live immediately - force stop
    StopLiveVideo(IS_FORCE_VIDEO_STOP);

    //enable the start button
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::OnButtonExit()
//
// DESCRIPTION: - stop live display
//              - frees used image memory
//              - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::OnButtonExit()
{
    //on exit close the camera and free the memory
    ExitCamera();
    PostQuitMessage( 0 );
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::OpenCamera()
//
// DESCRIPTION: - Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CIdsRotationDemoDlg::OpenCamera()
{
    INT nRet = IS_NO_SUCCESS;
    CString str;

    //close the camera if one is open
    ExitCamera();

    //initialize the next camera
    m_hCam = (HIDS)0;
    nRet = InitCamera(&m_hCam, NULL);
    if (nRet == IS_SUCCESS)
    {
        //Set the binning mode so that the UI1460C can reach up to 30fps
        nRet = is_SetBinning(m_hCam, IS_BINNING_4X_VERTICAL | IS_BINNING_4X_HORIZONTAL);
        SetDlgItemText(IDC_STATIC_MODE, L"Mode: Binning");

        if(nRet != IS_SUCCESS)
        {
            AfxMessageBox(TEXT("The camera has no binning mode!"), MB_ICONWARNING);
            ExitCamera();
            return false;
        }

        //set the maximum pixel clock
        UINT nRange[3];
        ZeroMemory(nRange, sizeof(nRange));

        // Get pixel clock range
        nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)nRange, sizeof(nRange));
        if (nRet == IS_SUCCESS)
        {
            UINT nMin = nRange[0];
            UINT nMax = nRange[1];
            UINT nInc = nRange[2];

            nRet = is_PixelClock(m_hCam, IS_PIXELCLOCK_CMD_SET, (void*)&nMax, sizeof(nMax));
        }

        //set 25 fps for the live display. If more frames will be used the cpu may be
        //occupied with the image acquisition
        double dMin, dMax, dInt, dDummy;
        nRet = is_SetFrameRate(m_hCam, 25.0, &dDummy);


        //set the maximum exposure time for the first time
        double dblRange[3];
        nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void*)dblRange, sizeof(dblRange));
        dMin = dblRange[0];
        dMax = dblRange[1];
        dInt = dblRange[2];

        nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dMax, sizeof(dMax));

        //and remember the value for later use
        nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&m_ExposureTime, sizeof(m_ExposureTime));

        //initialize the exposure slider
        m_ctrlSliderExposure.SetRange((INT)(dMin*1000), (INT)(dMax*1000));
        m_ctrlSliderExposure.SetPos((INT)(dMax*1000));
        str.Format(L"Exposure time: %.2fms", m_ExposureTime);
        SetDlgItemText(IDC_STATIC_EXPOSURE, str);

        //initialize the gain slider
        nRet = is_SetHardwareGain(m_hCam, 0, -1, -1, -1);
        m_ctrlSliderGain.SetRange(0, 100);
        m_ctrlSliderGain.SetPos(0);
        str.Format(L"Master gain: %d", 0);
        SetDlgItemText(IDC_STATIC_GAIN, str);

        //retrieve the sensor information
        nRet = is_GetSensorInfo(m_hCam, &m_sInfo);

        //store only the quarter image size because of the binning
        IS_SIZE_2D imageSize;
        nRet = is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

        m_nSizeX = imageSize.s32Width;
        m_nSizeY = imageSize.s32Height;

        UpdateData(TRUE);

        //Initialize the memory
        nRet = InitImageMemorys();

        //position all the dialog items dynamically
        RepositionDialogItems();

        if (nRet == IS_SUCCESS)
        {
            //Enable the frame event for the imaging thread
            g_hFrameEvent = CreateEvent(NULL, FALSE, TRUE, L"FrameEvent");
            nRet = is_InitEvent(m_hCam, g_hFrameEvent, IS_SET_EVENT_FRAME);
            nRet = is_EnableEvent(m_hCam, IS_SET_EVENT_FRAME);
        }
        else
            AfxMessageBox(TEXT("Initializing the memory failed!"), MB_ICONWARNING);

        return true;
    }
    else
    {
        AfxMessageBox (L"No uEye camera could be opened!", MB_ICONWARNING);
        return false;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::ExitCamera()
//
// DESCRIPTION: - exits the instance of the camera
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::ExitCamera()
{
    //if there is an open camera
    if( m_hCam != 0 )
    {
        //stop the live video
        StopLiveVideo(IS_WAIT);
        ClearSequence();
        FreeImageMems();

        //exit the camera
        is_ExitCamera( m_hCam );
        m_hCam = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::InitImageMemorys()
//
// DESCRIPTION: - initializes the memory for the different modes
//
///////////////////////////////////////////////////////////////////////////////
INT CIdsRotationDemoDlg::InitImageMemorys()
{
    INT nRet = IS_NO_SUCCESS;

    //if there is no open camera return immediately
    if (m_hCam == NULL)
        return IS_NO_SUCCESS;

    //if there is old memory free it
    ClearSequence();
    FreeImageMems();

    //set the bitmap mode so we have access to the memory
    nRet = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);

    //if this is a color camera (which should be on the UI1460C)
    if (m_sInfo.nColorMode == IS_COLORMODE_BAYER)
    {
        //set the color mode to 24 bits per pixel rgb
        m_nBitsPerPixel = 24;
        m_nColorMode = IS_CM_BGR8_PACKED;
    }
    else
    {
        // for monochrome camera models use Y8 mode
        m_nColorMode = IS_CM_MONO8;
        m_nBitsPerPixel = 8;
    }

    nRet = AllocImageMems();

    if (nRet == IS_SUCCESS)
    {
        InitSequence();
        // set the desired color mode
        is_SetColorMode(m_hCam, m_nColorMode);
    }
    else
    {
        AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING);
    }

    return nRet;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::OnClose()
//
// DESCRIPTION: - exits the application
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::OnClose()
{
    //exit the camera
    ExitCamera();

    CDialog::OnClose();
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::RepositionDialogItems()
//
// DESCRIPTION: - sets the dialog items dynamically on the right side of the dialog
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::RepositionDialogItems()
{
    CRect rect, rect2;

    INT nOffsetY=0;
    INT nOffsetX=20;

    //position the image display and resize it so that it fits to the screen
    GetDlgItem(IDC_DISPLAY_VERT)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    rect.top = 10;
    rect.left = 20;
    rect.right = rect.left + 384;
    rect.bottom = rect.top + 512;
    GetDlgItem(IDC_DISPLAY_VERT)->MoveWindow(rect);

    //position the other dialog items relative to the right border
    GetClientRect(&rect);

    nOffsetY+=10;

    //the buttons
    INT i=0;
    for(i=0; i<=3; i++)
    {
        rect2.left = rect.right - nOffsetX - 150;
        rect2.right = rect.right - nOffsetX;
        rect2.top = rect.top + 50*i + nOffsetY;
        rect2.bottom = rect.top + 50*i + nOffsetY + 40;
        GetDlgItem(g_nDlgItemsID[i])->MoveWindow(rect2);
    }

    nOffsetY+=200;

    //the logo;
    rect2.left = rect.right - nOffsetX - 150;
    rect2.right = rect.right - nOffsetX;
    rect2.top = rect.top + nOffsetY;
    rect2.bottom = rect.top + nOffsetY + 64;
    GetDlgItem(IDC_STATIC_LOGO)->MoveWindow(rect2);

    nOffsetY+=80;

    //the frame rate display
    rect2.left = rect.right - nOffsetX - 150;
    rect2.right = rect.right - nOffsetX;
    rect2.top = rect.top + nOffsetY;
    rect2.bottom = rect.top + nOffsetY + 20;
    GetDlgItem(IDC_STATIC_FPS)->MoveWindow(rect2);

    nOffsetY +=20;

    //the mode display
    rect2.left = rect.right - nOffsetX - 150;
    rect2.right = rect.right - nOffsetX;
    rect2.top = rect.top + nOffsetY;
    rect2.bottom = rect.top + nOffsetY + 20;
    GetDlgItem(IDC_STATIC_MODE)->MoveWindow(rect2);

    nOffsetY +=30;

    //the sliders and text items
    INT j=0;
    for(i=g_nDlgItemsCount-4; i<g_nDlgItemsCount ; i++)
    {
        rect2.left = rect.right - nOffsetX - 150;
        rect2.right = rect.right - nOffsetX;
        rect2.top = rect.top + 20*j + nOffsetY;
        rect2.bottom = rect.top + 20*j + nOffsetY + 20;
        GetDlgItem(g_nDlgItemsID[i])->MoveWindow(rect2);
        j++;
    }

    nOffsetY +=100;

    //the warning
    rect2.left = rect.right - nOffsetX - 150;
    rect2.right = rect.right - nOffsetX;
    rect2.top = rect.top + nOffsetY;
    rect2.bottom = rect.top + nOffsetY + 70;
    GetDlgItem(IDC_STATIC_WARNING)->MoveWindow(rect2);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::CopyMemory()
//
// DESCRIPTION: - copies the memory from standard memory to vertical memory
//
///////////////////////////////////////////////////////////////////////////////
INT CIdsRotationDemoDlg::CopyuEyeMemory(char* pcSource, char* pcDestination, INT nWidth, INT nHeight, INT nBitsPerPixel)
{
    INT nRet = IS_SUCCESS;

    //pointer to the original memory
    char* pMem = pcSource;

    //pointer to the vertical memory
    char* pMemVert = pcDestination;

    //pitch from pixel to pixel in Byte
    INT nPixelPitch = (nBitsPerPixel/8);

    INT x=0;
    INT y=0;

    try
    {

        //x and y coordinates from the original memory
        for(y=0; y<nHeight; y++)
        {
            //on increasing y jump one row in the original memory
            pMem=pcSource+(nWidth*nPixelPitch*y);

            //on increasing y jump one pixel back in the vertical memory
            pMemVert=pcDestination+nPixelPitch*(nHeight-1-y);

            for(x=0; x<nWidth; x++)
            {
                //if there are more than 1Byte per Pixel use this for loop
                for(INT i=0; i<nPixelPitch;i++)
                    pMemVert[i] = pMem[i]; //copy the pixels

                //on increasing x jump one pixel in the original memory
                pMem+=nPixelPitch;

                //on increasing x jump one row in the vertical memory
                pMemVert+=(nHeight*nPixelPitch);
            }
        }
    }
    catch(...)
    {
        nRet= IS_NO_SUCCESS;
    }
    return nRet;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::DisplayuEyeImage()
//
// DESCRIPTION: - renders the image in the display.
//              - Will be called from the thread
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::DisplayuEyeImage(INT nMemoryID, INT nRenderMode)
{
    INT nRet = IS_SUCCESS;
    nRet = is_RenderBitmap( m_hCam, nMemoryID, m_hWndDisplayVert, nRenderMode );
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::OnButtonSnap()
//
// DESCRIPTION: - create a snap shot with full resolution from the camera
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::OnButtonSnap()
{
    //if there is no open camera open one
    if( m_hCam == 0 )
        OpenCamera();

    if(m_hCam)
    {
        INT nRet = IS_SUCCESS;

        //if we come from an other mode we have to do some initializations
        if(m_nLastMode != IDS_SNAP_MODE)
        {

            // stop live immediately - force stop
            StopLiveVideo(IS_FORCE_VIDEO_STOP);

            //for full resolution we have to disable the binning
            nRet = is_SetBinning(m_hCam, IS_BINNING_DISABLE);
            SetDlgItemText(IDC_STATIC_MODE, L"Mode: Normal");

            //and set the larger snap memory for image acquisition
            nRet = is_SetImageMem(m_hCam, m_pcImageMemorySnap, m_lMemoryIdSnap);

            //the sensitivity of the UI1460-C will be increased by 3.5 while in binning mode
            //(see manual chapter 4.9.2) to reduce the effect that the snap shot image will be darker
            //because we disable the binning mode we have to raise the exposure time by 3.5
            //To do this the framerate has to be cut down also
            double dTempValue, dDummy;
            nRet = is_SetFrameRate (m_hCam, IS_GET_FRAMERATE, &dDummy);
            nRet = is_SetFrameRate (m_hCam, dDummy/3.5, &dDummy);

            dTempValue = m_ExposureTime * 3.5;
            nRet = is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dTempValue, sizeof(dTempValue));

            //remember our last mode, so we don't have to make these settings again
            m_nLastMode = IDS_SNAP_MODE;
        }

        //acquire the image
        nRet = is_FreezeVideo(m_hCam, IS_WAIT);

        //rotate the whole image by 90°
        nRet = CopyuEyeMemory(m_pcImageMemorySnap, m_pcImageMemorySnapVert, m_sInfo.nMaxWidth, m_sInfo.nMaxHeight, m_nBitsPerPixel);

        //display it in the window but scale it to half
        DisplayuEyeImage(m_lMemoryIdSnapVert, IS_RENDER_FIT_TO_WINDOW);

        //get current time and date to save the image
        CTime CurrentTime;
        CurrentTime = CTime::GetCurrentTime();
        CString strFileName;
        strFileName = CurrentTime.Format(L"%Y-%m-%d_%H-%M-%S.bmp");

        if (m_bFirstTime)
        {
            m_bFirstTime = FALSE;

            CFileDialog FileDlg(FALSE, L"bmp", strFileName, 0, L"Bitmap (*.bmp)|*.bmp|All Files (*.*)|*.*||");
            FileDlg.DoModal();

            CString temp;
            temp = FileDlg.GetPathName();
            strFileName = FileDlg.GetFileName();
            int index = temp.Find(strFileName);
            m_strPath = temp.Left(index);
        }

        if (m_strPath != "-1")
        {
            CString strFullName;
            strFullName.Format(L"%s%s", m_strPath, strFileName);

            // SAVE Image as bmp
            IMAGE_FILE_PARAMS ImageFileParams;

            USES_CONVERSION;
            ImageFileParams.pwchFileName    = T2W(strFullName.GetBuffer());
            ImageFileParams.pnImageID       = (UINT*)&m_lMemoryIdSnapVert; // valid ID
            ImageFileParams.ppcImageMem     = &m_pcImageMemorySnapVert; // valid buffer
            ImageFileParams.nFileType       = IS_IMG_BMP; 
            ImageFileParams.nQuality        = 0;

            nRet = is_ImageFile(m_hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams, sizeof(ImageFileParams));
        }

        //enable the start button
        GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::OnUpdateFramerate()
//
// DESCRIPTION: - Message function used by the thread because threads
//                must not have access to dialog items
//
///////////////////////////////////////////////////////////////////////////////
afx_msg LRESULT CIdsRotationDemoDlg::OnUpdateFramerate(WPARAM wParam, LPARAM lParam)
{
    CString strFramerate;
    strFramerate.Format(L"Framerate: %.2f fps", (double)(wParam)/1000.0);

    SetDlgItemText(IDC_STATIC_FPS, strFramerate);

    return 1;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::OnHScroll()
//
// DESCRIPTION: - function for both slider controls
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    INT nValue = 0;
    CString str;

    if(m_hCam)
    {
        //has the exposure slider been moved
        if((CSliderCtrl*)pScrollBar == &m_ctrlSliderExposure)
        {
            //get the position and set the exposure time
            //note that we use the slider intern with [µs] not [ms] because of the integer restriction
            nValue = m_ctrlSliderExposure.GetPos();

            m_ExposureTime = nValue / 1000.0;
            is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&m_ExposureTime, sizeof(m_ExposureTime));

            str.Format(L"Exposure time: %.2fms", m_ExposureTime);
            SetDlgItemText(IDC_STATIC_EXPOSURE, str);
        }
        //has the gain slider been moved
        else if ((CSliderCtrl*)pScrollBar == &m_ctrlSliderGain)
        {
            //get the position and set the hardware gain
            nValue = m_ctrlSliderGain.GetPos();
            is_SetHardwareGain(m_hCam, nValue, -1, -1, -1);
            str.Format(L"Master gain: %d", nValue);
            SetDlgItemText(IDC_STATIC_GAIN, str);
        }
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::StartThread()
//
// DESCRIPTION: - used to start the thread for image processing
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::StartThread()
{
    //start the thread and give it the pointer to the dialog as parameter
    _beginthread(ImageProcessing,0 , (void*)this);
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsRotationDemoDlg::StopThread()
//
// DESCRIPTION: - sets the event for stopping the thread and waits for his exit
//
///////////////////////////////////////////////////////////////////////////////
void CIdsRotationDemoDlg::StopThread(BOOL bWait) 
{
    SetEvent(g_hCloseThread);

    if(bWait)
    {
        WaitForSingleObject(g_hExitThread, 1000);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD ImageProcessing()
//
// DESCRIPTION: - thread for rotating the image in live mode
//
///////////////////////////////////////////////////////////////////////////////
void ImageProcessing(void* pParam)
{
    //retrieve the pointer to the dialog from the parameter
    CIdsRotationDemoDlg* dlg = (CIdsRotationDemoDlg*)pParam;

    //report the dialog that the thread is running
    dlg->m_bThreadIsRunning = TRUE;

    //combine the two handles in one array
    HANDLE Handles[2];
    Handles[0] = g_hFrameEvent;
    Handles[1] = g_hCloseThread;

    DWORD nRet=0;
    INT nuEyeRet = IS_SUCCESS;
    INT nFrameCounter = 0;
    INT nElapsed = 0;
    INT nFrameRate = 0;


    //we use the high performance timer of the pc to calculate the framerate
    LARGE_INTEGER liCounterStart, liCounterStop, liFrequency; 
    double dFrequency, dTime, dTimePerFrame;
    QueryPerformanceFrequency( &liFrequency);
    dFrequency = (double)liFrequency.QuadPart;

    //get the first start time
    QueryPerformanceCounter( &liCounterStart);

    do
    {
        //wait for either a new image has arrived or the processing has to be stopped
        nRet = WaitForMultipleObjects(2,Handles, FALSE, INFINITE);

        //if there is a new image
        if(nRet == WAIT_OBJECT_0)
        {
            char* pLast = NULL;
            INT lMemoryId = 0;
            INT lSequenceId = 0;

            if (IS_SUCCESS == dlg->GetLastMem(&pLast, lMemoryId, lSequenceId))
            {
                nuEyeRet = is_LockSeqBuf(dlg->m_hCam, IS_IGNORE_PARAMETER, pLast);

                if (IS_SUCCESS == nuEyeRet)
                {
                    //rotate the image in the rotated memory
                    nuEyeRet = dlg->CopyuEyeMemory(pLast, dlg->m_pcImageMemoryVert, dlg->m_nSizeX, dlg->m_nSizeY, dlg->m_nBitsPerPixel);

                    //display it in the window
                    dlg->DisplayuEyeImage(dlg->m_lMemoryIdVert, IS_RENDER_NORMAL);

                    //another frame processed
                    nFrameCounter++;

                    //every 50 frames we report the framerate back to the dialog
                    if (nFrameCounter == 50)
                    {
                        QueryPerformanceCounter(&liCounterStop);
                        dTime = (liCounterStop.QuadPart - liCounterStart.QuadPart) / dFrequency;
                        dTimePerFrame = dTime / 50.0;
                        nFrameRate = (INT) (1000.0 / dTimePerFrame);

                        //a thread must not have direct access to dialog items. So we have to make an
                        //indirection with the message. We send the frame rate as wParam
                        PostMessage(dlg->m_hWnd, UPDATE_FRAMERATE, nFrameRate, 0);

                        nFrameCounter = 0;
                        QueryPerformanceCounter(&liCounterStart);
                    }

                    is_UnlockSeqBuf(dlg->m_hCam, IS_IGNORE_PARAMETER, pLast);
                }
            }
        }


    }//do this until the thread has to be stopped
    while(nRet != WAIT_OBJECT_0+1);

    //notify the dialog that the thread won't run anymore
    dlg->m_bThreadIsRunning = FALSE;

    //set the event that the thread exits
    SetEvent(g_hExitThread);
    return;
}


INT CIdsRotationDemoDlg::InitCamera (HIDS *hCam, HWND hWnd)
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

void CIdsRotationDemoDlg::StopLiveVideo(INT nWait)
{
    if (0 != m_hCam)
    {
        is_StopLiveVideo(m_hCam, nWait);
    }

    //stop the thread
    if (m_bThreadIsRunning)
    {
        StopThread(TRUE);
    }

    if (0 != m_hCam)
    {
        ClearSequence();
    }
}

INT CIdsRotationDemoDlg::AllocImageMems(void)
{
    INT nRet = IS_SUCCESS;

    do 
    {
        for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
        {
            // allocate an image memory.
            nRet = is_AllocImageMem(m_hCam,
                                    m_nSizeX,
                                    m_nSizeY,
                                    m_nBitsPerPixel,
                                    &(iter->pcImageMemory),
                                    &(iter->lMemoryId));

            if (IS_SUCCESS != nRet)
            {
                break;
            }
        }

        if (IS_SUCCESS != nRet)
        {
            break;
        }

        //Allocate an image memory for the vertical binning image
        //m_nSizeY and m_nSizeX are switched
        //We get an uEye compatible memory with ID to use in the SDK functions
        nRet = is_AllocImageMem(m_hCam,
                                m_nSizeY,
                                m_nSizeX,
                                m_nBitsPerPixel,
                                &m_pcImageMemoryVert,
                                &m_lMemoryIdVert);

        if (IS_SUCCESS != nRet)
        {
            break;
        }

        //Allocate an image memory for the snap image
        nRet = is_AllocImageMem(m_hCam,
                                m_sInfo.nMaxWidth,
                                m_sInfo.nMaxHeight,
                                m_nBitsPerPixel,
                                &m_pcImageMemorySnap,
                                &m_lMemoryIdSnap);

        if (IS_SUCCESS != nRet)
        {
            break;
        }

        //Allocate an image memory for the rotated snap image
        nRet = is_AllocImageMem(m_hCam,
                                m_sInfo.nMaxHeight,
                                m_sInfo.nMaxWidth,
                                m_nBitsPerPixel,
                                &m_pcImageMemorySnapVert,
                                &m_lMemoryIdSnapVert);

        break;
    }
    while (true);

    if (IS_SUCCESS != nRet)
    {
        FreeImageMems();
    }

    return nRet;
}

INT CIdsRotationDemoDlg::FreeImageMems(void)
{
    INT nRet = IS_SUCCESS;

    //and the vertical snap shot image
    if (m_pcImageMemorySnapVert != NULL)
    {
        is_FreeImageMem(m_hCam, m_pcImageMemorySnapVert, m_lMemoryIdSnapVert);
        m_pcImageMemorySnapVert = NULL;
    }

    //also for the memory from our snap shot image
    if (m_pcImageMemorySnap != NULL)
    {
        is_FreeImageMem(m_hCam, m_pcImageMemorySnap, m_lMemoryIdSnap);
        m_pcImageMemorySnap = NULL;
    }

    //also for our vertical image
    if (m_pcImageMemoryVert != NULL)
    {
        is_FreeImageMem(m_hCam, m_pcImageMemoryVert, m_lMemoryIdVert);
        m_pcImageMemoryVert = NULL;
    }

    // Free the allocated buffer
    for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
    {
        if (NULL != iter->pcImageMemory)
        {
            do
            {
                nRet = is_FreeImageMem(m_hCam, iter->pcImageMemory, iter->lMemoryId);

                if (IS_SEQ_BUFFER_IS_LOCKED == nRet)
                {
                    ::Sleep(1);
                    continue;
                }

                break;
            }
            while (true);
        }

        iter->pcImageMemory = NULL;
        iter->lMemoryId = 0;
        iter->lSequenceId = 0;
    }

    return nRet;
}

INT CIdsRotationDemoDlg::InitSequence(void)
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

INT CIdsRotationDemoDlg::ClearSequence(void)
{
    return is_ClearSequence(m_hCam);
}

INT CIdsRotationDemoDlg::GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId)
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
