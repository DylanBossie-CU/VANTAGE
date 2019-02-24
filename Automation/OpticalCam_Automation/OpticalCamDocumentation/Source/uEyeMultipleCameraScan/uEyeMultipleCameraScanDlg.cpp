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

// uEyeMultipleCameraScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uEyeMultipleCameraScan.h"
#include "uEyeMultipleCameraScanDlg.h"
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
// CUEyeMultipleCameraScanDlg dialog

CUEyeMultipleCameraScanDlg::CUEyeMultipleCameraScanDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUEyeMultipleCameraScanDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CUEyeMultipleCameraScanDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUEyeMultipleCameraScanDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUEyeMultipleCameraScanDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUEyeMultipleCameraScanDlg, CDialog)
    //{{AFX_MSG_MAP(CUEyeMultipleCameraScanDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
    ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
    ON_BN_CLICKED(IDC_CHECK_ERR_REP, OnCheckErrRep)
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUEyeMultipleCameraScanDlg message handlers

BOOL CUEyeMultipleCameraScanDlg::OnInitDialog()
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
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE); // Set big icon
    SetIcon(m_hIcon, FALSE); // Set small icon

    bitmap = LoadBitmap( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_UEYE_ENABLED));
    hPen = CreatePen (PS_SOLID, 2, RGB( 255,0,0));

    // functions
    OpenCamera();

    return TRUE; // return TRUE  unless you set the focus to a control
}

void CUEyeMultipleCameraScanDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUEyeMultipleCameraScanDlg::OnPaint()
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

        // refresh display for all cameras
        for( int i=0 ; i<m_CamCount ; i++ )
        {
            is_RenderBitmap( m_hCam[i], m_lMemoryId[i], m_hWndDisplay[i], IS_RENDER_FIT_TO_WINDOW );
        }
    }
}

void CUEyeMultipleCameraScanDlg::OnDestroy()
{
    CDialog::OnDestroy();
    OnButtonExit();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUEyeMultipleCameraScanDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeMultipleCameraScanDlg::OpenCamera()
//
// DESCRIPTION: Opens handles to all connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeMultipleCameraScanDlg::OpenCamera()
{
    int nMaxCams = MAX_CAM;
    CString str;

    // enable the dialog based error report
    m_Ret = is_SetErrorReport(m_hCam[0], IS_ENABLE_ERR_REP); //or IS_DISABLE_ERR_REP;
    if( m_Ret != IS_SUCCESS )
    {
        AfxMessageBox(L"ERROR: Can not enable the automatic uEye error report!" , MB_ICONEXCLAMATION, 0 );
        return false;
    }
    m_bErrRep = true;
    CheckDlgButton( IDC_CHECK_ERR_REP, BST_CHECKED );

    // get camera count
    is_GetNumberOfCameras ( &m_CamCount );
    SetDlgItemInt(IDC_EDIT_NUM_CAM , m_CamCount );

    if(m_CamCount > nMaxCams )
    {
        str.Format(L"%d camera connected! \n%d cameras will be used, ", m_CamCount, nMaxCams );
        AfxMessageBox( str , MB_ICONEXCLAMATION, 0 );
    }

    // init cameras
    for( int i=0 ; i<m_CamCount ; i++ )
    {
        // get display handle
        m_hWndDisplay[i] = GetDlgItem( IDC_STATIC_DISPLAY_0 +i )->m_hWnd;

        m_hCam[i] = (HIDS) 0; // open next camera
        m_Ret = InitCamera( &m_hCam[i], NULL );

        if( m_Ret == IS_SUCCESS )
        {
            // setup the color depth to the current windows setting
            is_GetColorDepth(m_hCam[i], &m_nBitsPerPixel[i], &m_nColorMode[i]);
            is_SetColorMode(m_hCam[i], m_nColorMode[i]);

            // get board info
            CAMINFO CameraInfo;
            is_GetCameraInfo( m_hCam[i], &CameraInfo );

            // get sensor info
            SENSORINFO SensorInfo;
            is_GetSensorInfo(m_hCam[i], &SensorInfo );

            // init image size
            GetMaxImageSize(m_hCam[i], &(m_nSizeX[i]), &(m_nSizeY[i]));

            IS_SIZE_2D imageSize;
            imageSize.s32Width = m_nSizeX[i];
            imageSize.s32Height = m_nSizeY[i];

            is_AOI(m_hCam[i], IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

            // display initialization
            is_SetDisplayMode(m_hCam[i], IS_SET_DM_DIB);

            // alloc an image memory for each camera
            is_AllocImageMem( m_hCam[i],
                m_nSizeX[i],
                m_nSizeY[i],
                m_nBitsPerPixel[i],
                &m_pcImageMemory[i],
                &m_lMemoryId[i]);

            is_SetImageMem( m_hCam[i], m_pcImageMemory[i], m_lMemoryId[i]);

            //get the minimum Pixelclock and set it to guarantee bandwidth
            UINT nRange[3];
            ZeroMemory(nRange, sizeof(nRange));

            // Get pixel clock range
            INT nRet = is_PixelClock(m_hCam[i], IS_PIXELCLOCK_CMD_GET_RANGE, (void*)nRange, sizeof(nRange));
            if (nRet == IS_SUCCESS)
            {
                UINT nMin = nRange[0];
                UINT nMax = nRange[1];
                UINT nInc = nRange[2];

                nRet = is_PixelClock(m_hCam[i], IS_PIXELCLOCK_CMD_SET, (void*)&nMin, sizeof(nMin));
            }

            double defExp = 0.0;
            is_Exposure(m_hCam[i], IS_EXPOSURE_CMD_GET_EXPOSURE_DEFAULT,(void*)&defExp, sizeof(defExp));
            is_Exposure(m_hCam[i], IS_EXPOSURE_CMD_SET_EXPOSURE,(void*)&defExp, sizeof(defExp));

            // ensure theres no trigger mode enabled
            is_SetExternalTrigger(m_hCam[i],IS_SET_TRIGGER_OFF );

            //GUI
            SetDlgItemText(IDC_EDIT_CAM_TYPE_0+i,CString(SensorInfo.strSensorName));
            str.Format(L"ID: %d", CameraInfo.Select);
            SetDlgItemText(IDC_EDIT_CAM_ID_0+i, str);
            str.Format(L"S/N: %s", CString(CameraInfo.SerNo));
            SetDlgItemText(IDC_EDIT_CAM_SN_0+i, str);
            GetDlgItem( IDC_CHECK_CAM_0+i )->EnableWindow(TRUE);
            CheckDlgButton( IDC_CHECK_CAM_0+i, BST_CHECKED);
            CStatic *pDisplayWnd = (CStatic*) GetDlgItem( IDC_STATIC_DISPLAY_0+i );

            pDisplayWnd->SetBitmap(bitmap);
        }
        else
        {
            str.Format(L"ERROR: Cannot open uEye camera #%d ",i );
            AfxMessageBox( str , MB_ICONEXCLAMATION, 0 );
        }
    } // end for

    // get OS version
    OSVERSIONINFO os;
    ZeroMemory(&os, sizeof(OSVERSIONINFO));
    os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (GetVersionEx(&os))
    {
        if ((os.dwMajorVersion == 5) && (os.dwMinorVersion == 0))
        {
            str.Format(L"Windows 2000");
        }
        else if ((os.dwMajorVersion == 5) && (os.dwMinorVersion == 1))
        {
            str.Format(L"Windows XP");
        }
        else if((os.dwMajorVersion == 5) && (os.dwMinorVersion == 2))
        {
            str.Format(L"Windows Server 2003");
        }
        else if ((os.dwMajorVersion == 6) && (os.dwMinorVersion == 0))
        {
            str.Format(L"Windows Vista");
        }
        else if ((os.dwMajorVersion == 6) && (os.dwMinorVersion == 1))
        {
            str.Format(L"Windows 7");
        }
        else
        {
            str.Format(L"Unknown");
        }
    }
    else
    {
        str.Format(L"Unknown");
    }

    SetDlgItemText(IDC_EDIT_OS, str);

    // determine uEye driver version
    INT nDriver = is_GetDLLVersion(); 
    str.Format(L"%d.%02d.%04d",(nDriver>>24), (nDriver>>16&0xff), (nDriver&0xffff) );
    SetDlgItemText(IDC_EDIT_DRIVER_VERSION, str );

    SetDlgItemText(IDC_EDIT_IDS, L"www.uEye.com" );

    return true;
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeTriggerDlg::CUEyeMultipleCameraScanDlg
//
// DESCRIPTION: - exit the camera
//              - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeMultipleCameraScanDlg::OnButtonExit()
{
    DeleteObject(bitmap);
    DeleteObject(hPen);

    // memory and events are automatically released
    for( int i=0 ; i<m_CamCount ; i++ )
    {
        is_ExitCamera( m_hCam[i] );
    }
    // terminate
    PostQuitMessage( 0 );
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeMultipleCameraScanDlg::OnButtonScan()
//
// DESCRIPTION: - acquire (scan) all cameras
//              - display images
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeMultipleCameraScanDlg::OnButtonScan()
{
    // lock button - do not press twice
    GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(FALSE);

    // loop over all cameras
    for (int i = 0; i < m_CamCount; i++)
    {
        // scan camera when it is checked in the scan list
        if (IsDlgButtonChecked(IDC_CHECK_CAM_0 + i) == BST_CHECKED)
        {
            // display red rectangle overlay for active camera
            DrawRectangle( i );

            // capture
            m_Ret = is_FreezeVideo( m_hCam[i], IS_WAIT );
            if (m_Ret != IS_SUCCESS)
            {
                // error during capture
                CStatic *pDisplayWnd = (CStatic*) GetDlgItem(IDC_STATIC_DISPLAY_0 + i);
                pDisplayWnd->SetBitmap( ::LoadBitmap( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_UEYE_TRANSFER_FAILED)));
            }
            else
            {
                // image is captured, so we display it
                is_RenderBitmap( m_hCam[i], m_lMemoryId[i], m_hWndDisplay[i], IS_RENDER_FIT_TO_WINDOW );
            }
        }
    }

    // unlock button
    GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(TRUE);
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeMultipleCameraScanDlg::DrawRectangle(INT nCamera)
//
// DESCRIPTION: Draw recangle into the camera display window
//
// PARAMTERS:   nCamera: Camera index.
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeMultipleCameraScanDlg::DrawRectangle(INT nCamera)
{
    //variables
    RECT myRect;
    POINT pt;

    // get coordinates for rectangle
    GetDlgItem( IDC_STATIC_DISPLAY_0+nCamera)->GetClientRect( &myRect );
    myRect.left++;
    myRect.right--;
    myRect.top++;
    myRect.bottom--;

    // get DC and draw rectangle
    CDC* pdc = GetDlgItem( IDC_STATIC_DISPLAY_0+nCamera)->GetDC();
    if(pdc != NULL)
    {
        SelectObject (pdc->m_hDC, hPen);
        MoveToEx (pdc->m_hDC, myRect.left, myRect.top, &pt);
        LineTo (pdc->m_hDC, myRect.right, myRect.top);
        LineTo (pdc->m_hDC, myRect.right, myRect.bottom);
        LineTo (pdc->m_hDC, myRect.left, myRect.bottom);
        LineTo (pdc->m_hDC, myRect.left, myRect.top);
        this->ReleaseDC(pdc);
    }
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CUEyeMultipleCameraScanDlg::OnCheckErrRep()
//
// DESCRIPTION: enable/disable the SDK error report
//
///////////////////////////////////////////////////////////////////////////////

void CUEyeMultipleCameraScanDlg::OnCheckErrRep()
{
    if( m_bErrRep == true )
    {
        for ( int i = 0; i < m_CamCount; i++ )
        {
            is_SetErrorReport(m_hCam[i], IS_DISABLE_ERR_REP );
        }
        m_bErrRep = false;
    }
    else
    {
        for ( int i = 0; i < m_CamCount; i++ )
        {
            is_SetErrorReport(m_hCam[i], IS_ENABLE_ERR_REP );
        }
        m_bErrRep = true;
    }
}


INT CUEyeMultipleCameraScanDlg::InitCamera (HIDS *hCam, HWND hWnd)
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
        Str3.Format(L"%s %d %s", Str1, nUploadTime / 1000, Str2);
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


void CUEyeMultipleCameraScanDlg::GetMaxImageSize(HIDS hCam, INT *pnSizeX, INT *pnSizeY)
{
    // Check if the camera supports an arbitrary AOI
    // Only the ueye xs does not support an arbitrary AOI
    INT nAOISupported = 0;
    BOOL bAOISupported = TRUE;
    if (is_ImageFormat(hCam,
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
        is_GetSensorInfo (hCam, &sInfo);
        *pnSizeX = sInfo.nMaxWidth;
        *pnSizeY = sInfo.nMaxHeight;
    }
    else
    {
        // Only ueye xs
        // Get image size of the current format
        IS_SIZE_2D imageSize;
        is_AOI(hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

        *pnSizeX = imageSize.s32Width;
        *pnSizeY = imageSize.s32Height;
    }
}

/*
EOF
*/