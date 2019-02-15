//==========================================================================//
//                                                                          //
//  Copyright (C) 2009 - 2018                                               //
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
#include "uEyeDirectRenderer.h"
#include "uEyeDirectRendererDlg.h"
#include ".\ueyedirectrendererdlg.h"
#include "version.h"

// if Visual Studio 2005
#if _MSC_VER >= 1400	
#include <strsafe.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable : 4995 4996)

class CAboutDlg : public CDialog
{
public:

    CAboutDlg ();
    enum { IDD = IDD_ABOUTBOX };

private:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange (CDataExchange* pDX);   
    DECLARE_MESSAGE_MAP ()
};


CAboutDlg::CAboutDlg () : CDialog (CAboutDlg::IDD)
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
CUEyeDirectRendererDlg::CUEyeDirectRendererDlg (CWnd* pParent)
: CDialog (CUEyeDirectRendererDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);

    m_bShowOverlay = FALSE;
    m_bShowMovingOverlay = FALSE;
    m_bLoadOverlayFromFile = FALSE;
    m_bShowSemiTransparent = FALSE;
    m_bMono = FALSE;
    m_bInit = FALSE;
    m_bScaleImage = FALSE;
    m_bScaleImageAndOverlay = FALSE;
}


void CUEyeDirectRendererDlg::DoDataExchange (CDataExchange* pDX)
{
    CDialog::DoDataExchange (pDX);

    DDX_Check (pDX, IDC_CHECK_SHOW_OVERLAY, m_bShowOverlay);
    DDX_Check (pDX, IDC_CHECK_SHOW_MOVING_OVERLAY, m_bShowMovingOverlay);
    DDX_Check (pDX, IDC_CHECK_OVERLAY_FROM_FILE, m_bLoadOverlayFromFile);
    DDX_Check (pDX, IDC_CHECK_SEMI_TRANSPARENT, m_bShowSemiTransparent);
    DDX_Check (pDX, IDC_CHECK_ALLOW_SCALING, m_bScaleImageAndOverlay);
    DDX_Check (pDX, IDC_CHECK_ALLOW_IMAGE_SCALING, m_bScaleImage);
    DDX_Check (pDX, IDC_CHECK_MONO, m_bMono);
    DDX_Radio (pDX, IDC_RADIO_VSYNC_OFF, m_nVsync);
    DDX_Radio (pDX, IDC_RADIO_DIRECT3D, m_nDirectRenderMode);
}


BEGIN_MESSAGE_MAP (CUEyeDirectRendererDlg, CDialog)
    ON_WM_SYSCOMMAND ()
    ON_WM_PAINT ()
    ON_WM_QUERYDRAGICON ()
    ON_BN_CLICKED (IDC_BUTTON_LIVE, OnButtonLive)
    ON_BN_CLICKED (IDC_BUTTON_STOP_LIVE, OnButtonStopLive)
    ON_BN_CLICKED (IDC_BUTTON_EXIT, OnButtonExit)
    ON_BN_CLICKED (IDC_CHECK_ALLOW_SCALING, OnCheckAllowScaling)
    ON_BN_CLICKED (IDC_BUTTON_LOAD_PARAMETER, OnBnClickedButtonLoadParameter)
    ON_STN_CLICKED (IDC_KEY_COLOR, OnStnClickedKeyColor)
    ON_MESSAGE (IS_UEYE_MESSAGE, OnUEyeMessage)
    ON_WM_TIMER ()
    ON_WM_CTLCOLOR ()
    ON_WM_CLOSE ()
    ON_BN_CLICKED (IDC_CHECK_SHOW_OVERLAY, OnBnClickedCheckShowOverlay)
    ON_BN_CLICKED (IDC_CHECK_SHOW_MOVING_OVERLAY, OnBnClickedCheckShowMovingOverlay)
    ON_BN_CLICKED (IDC_CHECK_SEMI_TRANSPARENT, OnBnClickedCheckSemiTransparent)
    ON_BN_CLICKED (IDC_RADIO_VSYNC_OFF, OnBnClickedRadioVsync)
    ON_BN_CLICKED (IDC_RADIO_VSYNC_AUTO, OnBnClickedRadioVsync)
    ON_BN_CLICKED (IDC_RADIO_VSYNC_USER, OnBnClickedRadioVsync)
    ON_NOTIFY (UDN_DELTAPOS, IDC_SPIN_SYNC_POSITION, OnDeltaposSpinSyncPosition)
    ON_BN_CLICKED (IDC_CHECK_MONO, OnBnClickedCheckMono)
    ON_BN_CLICKED (IDC_CHECK_OVERLAY_FROM_FILE, OnBnClickedCheckOverlayFromFile)
    ON_BN_CLICKED(IDC_CHECK_ALLOW_IMAGE_SCALING, &CUEyeDirectRendererDlg::OnBnClickedCheckAllowImageScaling)
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_BN_CLICKED(IDC_RADIO_DIRECT3D, &CUEyeDirectRendererDlg::OnBnClickedRadioRenderMode)
    ON_BN_CLICKED(IDC_RADIO_OPENGL, &CUEyeDirectRendererDlg::OnBnClickedRadioRenderMode)
END_MESSAGE_MAP ()


/////////////////////////////////////////////////////////////////////////////
BOOL CUEyeDirectRendererDlg::OnInitDialog ()
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

    // Set the icon for this dialog.
    SetIcon (m_hIcon, TRUE);	
    SetIcon (m_hIcon, FALSE);

    // DC handle
    m_hDc = NULL;

    // Open next available camera 
    m_hCam = 0;       

    // Display mode = Standard Direct3D
    m_nDisplayMode = IS_SET_DM_DIRECT3D;

    m_nDirectRenderMode = 0;

    // Vsync (Auto)
    m_nVsync = 1;

    // user sync position
    m_SyncPosition = 500;

    // Print the sync position into the edit control
    CString Str;
    Str.Format (L"%d", m_SyncPosition);
    GetDlgItem (IDC_EDIT_SYNC_POS)->SetWindowText (Str);

    // Set display window handle
    m_hWndDisplay = GetDlgItem (IDC_DISPLAY)->GetSafeHwnd ();  

    // The overlay is located at (x = 0, y = 0)
    m_nOverlayOffsetX = 0;
    m_nOverlayOffsetY = 0;

    // Open a camera handle
    OpenCamera ();		

    // Start live display
    OnButtonLive ();     

    UpdateData(FALSE);

    GetClientRect(&m_rcClient);
    GetWindowRect(&m_rcDialog);

    SaveControlPosition(IDC_IMAGE_WIDTH, &m_rcImageWidth);
    SaveControlPosition(IDC_IMAGE_HEIGHT, &m_rcImageHeight);
    SaveControlPosition(IDC_OVERLAY_WIDTH, &m_rcOverlayWidth);
    SaveControlPosition(IDC_OVERLAY_HEIGHT, &m_rcOverlayHeight);
    SaveControlPosition(IDC_CHECK_MONO, &m_rcCheckMono);
    SaveControlPosition(IDC_CHECK_SHOW_OVERLAY, &m_rcCheckShowOverlay);
    SaveControlPosition(IDC_CHECK_ALLOW_SCALING, &m_rcCheckAllowScaling);
    SaveControlPosition(IDC_CHECK_ALLOW_IMAGE_SCALING, &m_rcCheckAllowImageScaling);
    SaveControlPosition(IDC_CHECK_SHOW_MOVING_OVERLAY, &m_rcCheckShowMovingOverlay);
    SaveControlPosition(IDC_CHECK_SEMI_TRANSPARENT, &m_rcCheckSemiTransparent);
    SaveControlPosition(IDC_CHECK_OVERLAY_FROM_FILE, &m_rcCheckOverlayFromFile);
    SaveControlPosition(IDC_STATIC, &m_rcLogo);
    SaveControlPosition(IDC_RADIO_DIRECT3D, &m_rcRadioDirect3D);
    SaveControlPosition(IDC_RADIO_OPENGL, &m_rcRadioOpenGL);
    SaveControlPosition(IDC_RADIO_VSYNC_OFF, &m_rcRadioVsyncOff);
    SaveControlPosition(IDC_RADIO_VSYNC_AUTO, &m_rcRadioVsyncAuto);
    SaveControlPosition(IDC_RADIO_VSYNC_USER, &m_rcRadioVsyncUser);
    SaveControlPosition(IDC_STATIC_SYNC_POS, &m_rcStaticSyncPos);
    SaveControlPosition(IDC_EDIT_SYNC_POS, &m_rcEditSyncPos);
    SaveControlPosition(IDC_SPIN_SYNC_POSITION, &m_rcSpinSyncPos);
    SaveControlPosition(IDC_KEY_COLOR, &m_rcKeyColor);
    SaveControlPosition(IDC_KC_TEXT, &m_rcKeyColorText);
    SaveControlPosition(IDC_BUTTON_LIVE, &m_rcBtnLive);
    SaveControlPosition(IDC_BUTTON_STOP_LIVE, &m_rcBtnStopLive);
    SaveControlPosition(IDC_BUTTON_LOAD_PARAMETER, &m_rcBtnLoadParameter);
    SaveControlPosition(IDC_BUTTON_EXIT, &m_rcBtnExit);

    m_bInit = TRUE;

    // Return TRUE  unless you set the focus to a control
    return TRUE;  
}


void CUEyeDirectRendererDlg::OnSysCommand (UINT nID, LPARAM lParam)
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


void CUEyeDirectRendererDlg::OnPaint () 
{
    if (IsIconic ())
    {
        // Device context for painting
        CPaintDC dc (this); 

        SendMessage (WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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


HCURSOR CUEyeDirectRendererDlg::OnQueryDragIcon ()
{
    return (HCURSOR) m_hIcon;
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OpenCamera()
//
///////////////////////////////////////////////////////////////////////////////
bool CUEyeDirectRendererDlg::OpenCamera ()
{
    // If we have an open camera, close it
    if (m_hCam != 0)
    {
        is_ExitCamera (m_hCam);
    }

    // Init camera (open next available camera)
    m_hCam = (HIDS) 0;  

    INT nRet = InitCamera (&m_hCam, m_hWndDisplay);	
    if (nRet == IS_SUCCESS)
    {
        // Get the color depth of the desktop and the corresponding color mode
        is_GetColorDepth (m_hCam, &m_nBitsPerPixel, &m_nColorMode);

        // Set the color mode
        if (is_SetColorMode (m_hCam, m_nColorMode) != IS_SUCCESS)
        {
            AfxMessageBox (L"ERROR: is_SetColorMode()");
        }

        // Get sensor info
        SENSORINFO SensorInfo;
        is_GetSensorInfo (m_hCam, &SensorInfo);

        // Init image size
        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        // Set the image size
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

        // Update the size in the static controls
        UpdateImageSize ();

        // Set the display mode to Direct3D
        nRet = is_SetDisplayMode (m_hCam, m_nDisplayMode);
        if (nRet != IS_SUCCESS)
        {
            CString Msg;

            // Direct3D is not supported (DirectX not installed)
            if (nRet == IS_DR_LIBRARY_NOT_FOUND)
            {
                Msg = "ERROR: is_SetDisplayMode(). Please check if you have installed the latest DirectX (March 2009)";
            }
            else if (nRet == IS_DR_DEVICE_OUT_OF_MEMORY)
            {
                Msg = "ERROR: is_SetDisplayMode(). The video memory of the graphics adapter is insufficient";
            }
            else if (nRet == IS_DR_CANNOT_CREATE_SURFACE)
            {
                Msg = "ERROR: is_SetDisplayMode(). The image or overlay surface could not be created";
            }
            else if (nRet == IS_DR_CANNOT_CREATE_VERTEX_BUFFER)
            {
                Msg = "ERROR: is_SetDisplayMode(). The vertex buffer could not be created";
            }
            else if (nRet == IS_DR_CANNOT_CREATE_TEXTURE)
            {
                Msg = "ERROR: is_SetDisplayMode(). The texture could not be created";
            }
            else if (nRet == IS_DR_CANNOT_CREATE_TEXTURE)
            {
                Msg = "ERROR: is_SetDisplayMode(). The texture could not be created";
            }
            // Unspecified error
            else
            {
                Msg = "This Demo requires support of Direct3D.\nThis usually means that either DirectX or the graphics card driver is not installed properly";
            }

            AfxMessageBox (Msg);
            PostQuitMessage (0);
        }

        /****************************************************************************/
        /*                                                                          */
        /*   For the whole DirectRenderer (Direct3D) functionality, the function    */
        /*   is_DirectRenderer() is used. The command is specified through the      */
        /*   second parameter. The third parameter is always a void pointer to an   */
        /*   object or an object array, depending on the command. The fourth        */
        /*   parameter is the size of the object/object array. Some commands do     */
        /*   not need an object and the size. In this case NULL is specified        */
        /*                                                                          */
        /****************************************************************************/

        // Get the default key color for transparency (3 UINT values for RGB)
        UINT OverlayKeyColor[3];
        if (is_DirectRenderer (m_hCam, DR_GET_OVERLAY_KEY_COLOR, (void*)OverlayKeyColor, sizeof (OverlayKeyColor)) == IS_SUCCESS)
        {
            m_rgbKeyColor = RGB (OverlayKeyColor[0], OverlayKeyColor[1], OverlayKeyColor[2]); 
        }
        else
        {
            AfxMessageBox(L"ERROR: Keycolor can not be read", MB_ICONERROR);
            m_rgbKeyColor = RGB (0, 0, 0);
        }   

        // Read the maximum overlay size (2 UINT values for the overlay width and height)
        UINT OverlaySize[2];
        if (is_DirectRenderer (m_hCam, DR_GET_MAX_OVERLAY_SIZE, (void*)OverlaySize, sizeof (OverlaySize)) == IS_SUCCESS)
        {
            m_nOverlaySizeX = OverlaySize[0];
            m_nOverlaySizeY = OverlaySize[1];

            // Update the size in the static controls
            UpdateOverlaySize ();
        }
        else
        {
            AfxMessageBox (L"ERROR: Can not read max overlay size", MB_ICONERROR); 
        }

        // The supported overlay size is smaller than the image size
        if ((OverlaySize[0] < (UINT)m_nSizeX) || (OverlaySize[1] < (UINT)m_nSizeY))
        {
            AfxMessageBox (L"The maximum possible overlay size is smaller than the image size!");
        }

        // Get the range of the user sync position (minimum and maximum of the display)
        UINT UserSync[2];
        if (is_DirectRenderer (m_hCam, DR_GET_USER_SYNC_POSITION_RANGE, (void*)UserSync, sizeof (UserSync)) == IS_SUCCESS)
        {
            m_nUserSyncMin = UserSync[0];
            m_nUserSyncMax = UserSync[1];
        }

        // Enable uEye messages
        is_EnableMessage (m_hCam, IS_DEVICE_REMOVED, this->GetSafeHwnd ());
        is_EnableMessage (m_hCam, IS_DEVICE_RECONNECTED, this->GetSafeHwnd ());
        is_EnableMessage (m_hCam, IS_OVERLAY_DATA_LOST, this->GetSafeHwnd ());
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
// CUEyeDirectRendererDlg::OnButtonLive ()
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnButtonLive ()
{
    // Start live mode
    if (is_CaptureVideo (m_hCam, IS_WAIT) != IS_SUCCESS)
    {
        AfxMessageBox (L"ERROR: is_CaptureVideo()");
    }

    GetDlgItem (IDC_BUTTON_LIVE)->EnableWindow (FALSE);
    GetDlgItem (IDC_BUTTON_STOP_LIVE)->EnableWindow (TRUE);
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnButtonStopLive ()
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnButtonStopLive ()
{
    // Stop live mode
    is_StopLiveVideo (m_hCam, IS_WAIT);

    GetDlgItem (IDC_BUTTON_LIVE)->EnableWindow (TRUE);
    GetDlgItem (IDC_BUTTON_STOP_LIVE)->EnableWindow (FALSE);
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnBnClickedButtonLoadParameter ()
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnBnClickedButtonLoadParameter ()
{
    // If no valid camera handle, open the next available camera
    if (m_hCam == 0)
    {
        OpenCamera ();
    }

    if (m_hCam != 0)
    {
        // Check if camera is in live mode
        BOOL bWasLive = (BOOL)(is_CaptureVideo (m_hCam, IS_GET_LIVE));
        if (bWasLive)
        {
            is_StopLiveVideo (m_hCam, IS_FORCE_VIDEO_STOP);
        }

        // Load the parameters from an ini-file
        if (is_ParameterSet (m_hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS)
        {
            // Get the new image size
            IS_SIZE_2D imageSize;
            is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

            m_nSizeX = imageSize.s32Width;
            m_nSizeY = imageSize.s32Height;

            // Update the size in the static controls
            UpdateImageSize ();

            // Update the maximum possible overlay size
            UINT OverlaySize[2];
            if (is_DirectRenderer (m_hCam, DR_GET_MAX_OVERLAY_SIZE, (void*)OverlaySize, sizeof (OverlaySize)) == IS_SUCCESS)
            {
                m_nOverlaySizeX = OverlaySize[0];
                m_nOverlaySizeY = OverlaySize[1];

                // Update the size in the static controls
                UpdateOverlaySize ();
            }
            else
            {
                AfxMessageBox (L"ERROR: Can not read max overlay size", MB_ICONERROR);
            }

            m_bShowOverlay = FALSE;
            m_bShowMovingOverlay = FALSE;
            m_bLoadOverlayFromFile = FALSE;

            UpdateData(FALSE);
        }

        // Clear the background of the window
        ClearDisplayBackground ();

        // Run live image again
        if (bWasLive)
        {
            is_CaptureVideo (m_hCam, IS_DONT_WAIT);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnButtonExit () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnButtonExit () 
{
    // Kill the timer
    KillTimer (m_nTimer);

    if (m_hCam != NULL)
    {
        // Events are automatically released
        if(is_ExitCamera (m_hCam) == IS_SUCCESS)
        {
            m_hCam = NULL;
        }
    }

    // Terminate program
    PostQuitMessage (0);
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnCheckAllowScaling ()
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnCheckAllowScaling () 
{
    // Allow scaling?
    if (IsDlgButtonChecked (IDC_CHECK_ALLOW_SCALING) == BST_CHECKED)
    {
        // Enable scaling of the image and overlay to the display window
        is_DirectRenderer (m_hCam, DR_ENABLE_SCALING, NULL, NULL);

        // Deactivate the image scaling
        ((CButton*)GetDlgItem(IDC_CHECK_ALLOW_IMAGE_SCALING))->SetCheck(BST_UNCHECKED);

        m_bScaleImage = FALSE;
        m_bScaleImageAndOverlay = TRUE;

        if (m_bShowMovingOverlay)
        {
            // Deactivate the moving logo
            OnBnClickedCheckShowMovingOverlay();

            // Activate the moving logo
            OnBnClickedCheckShowMovingOverlay();
        }
    }
    else
    {
        // Disable scaling to the display window
        is_DirectRenderer (m_hCam, DR_DISABLE_SCALING, NULL, NULL);

        m_bScaleImage = FALSE;
        m_bScaleImageAndOverlay = FALSE;

        if (m_bShowMovingOverlay)
        {
            // Deactivate the moving logo
            OnBnClickedCheckShowMovingOverlay();

            // Activate the moving logo
            OnBnClickedCheckShowMovingOverlay();
        }
    }

    // Clear the background of the window
    ClearDisplayBackground ();

    UINT Size[2];
    if (is_DirectRenderer (m_hCam, DR_GET_OVERLAY_SIZE, (void*)Size, sizeof (Size)) == IS_SUCCESS)
    {
        m_nOverlaySizeX = Size[0];
        m_nOverlaySizeY = Size[1];
        UpdateOverlaySize();
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnBnClickedCheckAllowImageScaling ()
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnBnClickedCheckAllowImageScaling ()
{
    // Allow scaling?
    if (IsDlgButtonChecked (IDC_CHECK_ALLOW_IMAGE_SCALING) == BST_CHECKED)
    {
        // Enable scaling of the image and overlay to the display window
        is_DirectRenderer (m_hCam, DR_ENABLE_IMAGE_SCALING, NULL, NULL);

        // Deactivate the scaling of image and overlay
        ((CButton*)GetDlgItem(IDC_CHECK_ALLOW_SCALING))->SetCheck(BST_UNCHECKED);

        m_bScaleImage = TRUE;
        m_bScaleImageAndOverlay = FALSE;

        if (m_bShowMovingOverlay)
        {
            // Deactivate the moving logo
            OnBnClickedCheckShowMovingOverlay();

            // Activate the moving logo
            OnBnClickedCheckShowMovingOverlay();
        }
    }
    else
    {
        // Disable scaling to the display window
        is_DirectRenderer (m_hCam, DR_DISABLE_SCALING, NULL, NULL);

        m_bScaleImage = FALSE;
        m_bScaleImageAndOverlay = FALSE;

        if (m_bShowMovingOverlay)
        {
            // Deactivate the moving logo
            OnBnClickedCheckShowMovingOverlay();

            // Activate the moving logo
            OnBnClickedCheckShowMovingOverlay();
        }
    }

    // Clear the background of the window
    ClearDisplayBackground ();

    UINT Size[2];
    if (is_DirectRenderer (m_hCam, DR_GET_OVERLAY_SIZE, (void*)Size, sizeof (Size)) == IS_SUCCESS)
    {
        m_nOverlaySizeX = Size[0];
        m_nOverlaySizeY = Size[1];
        UpdateOverlaySize();
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnUEyeMessage
//
///////////////////////////////////////////////////////////////////////////////
LRESULT CUEyeDirectRendererDlg::OnUEyeMessage (WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case IS_CAPTURE_STATUS:
        break;

    case IS_DEVICE_REMOVED:
        Beep (5000,100);
        AfxMessageBox (L"ERROR: Camera disconnected!", MB_ICONEXCLAMATION, 0);
        break;

    case IS_DEVICE_RECONNECTED:
        Beep (100,100);
        AfxMessageBox (L"INFO: Camera reconnected!", MB_ICONINFORMATION , 0);
        break;

    case IS_NEW_DEVICE:
        break;

    case IS_DEVICE_REMOVAL:
        break;

    case IS_OVERLAY_DATA_LOST:
        if (m_bShowOverlay)
        {
            OnBnClickedCheckShowOverlay();
        }
        else if (m_bShowMovingOverlay)
        {
            OnBnClickedCheckShowMovingOverlay();
        }
        else if (m_bLoadOverlayFromFile)
        {   
            m_bLoadOverlayFromFile = FALSE;
            UpdateData(FALSE);

            AfxMessageBox(L"The overlay was lost and must be loaded again", MB_ICONWARNING);
        }

        break;
    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnTimer (UINT nIDEvent) 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnTimer (UINT_PTR nIDEvent) 
{
    CDialog::OnTimer (nIDEvent);

    // We use random numers to generate the movement of the uEye logo
    static int SignX = 1;
    static int SignY = 1;

    int r = rand ();

    if (r <= RAND_MAX / 4)
    {
        m_nOverlayOffsetX += SignX * 3;
    }
    else if ((r > RAND_MAX / 4) && (r <= RAND_MAX / 2))
    {
        m_nOverlayOffsetY += SignY * 3;
    }
    else if ((r > RAND_MAX / 2) && (r <= (3 * RAND_MAX / 4)))
    {
        m_nOverlayOffsetX += SignX * 1;
        m_nOverlayOffsetY += SignY * 3;
    }
    else if (r > (3 * RAND_MAX / 4) && (r <= RAND_MAX))
    {
        m_nOverlayOffsetX += SignX * 3;
        m_nOverlayOffsetY += SignY * 1;
    }

    // Check the position of the overlay surface
    // It should not be outside of the image (although this is possible)
    if (m_nOverlayOffsetX > m_nSizeX)
    {
        m_nOverlayOffsetX = m_nSizeX;

        SignX = -SignX;
    }

    if (m_nOverlayOffsetX < 0)
    {
        m_nOverlayOffsetX = 0;

        SignX = -SignX;
    }

    if (m_nOverlayOffsetY > m_nSizeY)
    {
        m_nOverlayOffsetY = m_nSizeY;

        SignY = -SignY;
    }

    if (m_nOverlayOffsetY < 0)
    {
        m_nOverlayOffsetY = 0;

        SignY = -SignY;
    }

    // Set the new position of the overlay surface
    UINT Position[2];
    Position[0] = m_nOverlayOffsetX;
    Position[1] = m_nOverlayOffsetY;
    if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_POSITION, (void*)Position, sizeof (Position)) != IS_SUCCESS)
    {
        // error handling ?
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnCtlColor () 
//
///////////////////////////////////////////////////////////////////////////////
HBRUSH CUEyeDirectRendererDlg::OnCtlColor (CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    // Call the base class implementation first! Otherwise, it may
    // undo what we're trying to accomplish here.
    HBRUSH hbr = CDialog::OnCtlColor (pDC, pWnd, nCtlColor);
    CBrush brBK, *oldBrush;
    RECT rcBounds;

    // Test if its the right control
    if (pWnd->GetDlgCtrlID () == IDC_KEY_COLOR)
    {
        if (pWnd->IsWindowEnabled ())
        {
            brBK.CreateSolidBrush (m_rgbKeyColor);
        }
        else
        {
            brBK.CreateHatchBrush (HS_DIAGCROSS, m_rgbKeyColor);
        }

        pWnd->GetClientRect (&rcBounds);
        oldBrush = pDC->SelectObject (&brBK);
        pDC->FillRect (&rcBounds, &brBK);        
        pDC->SelectObject (oldBrush);
    }

    return hbr;
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnStnClickedKeyColor () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnStnClickedKeyColor ()
{
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16]; 

    // Array of custom colors     
    ZeroMemory (acrCustClr, 16 * sizeof (DWORD)); 
    acrCustClr[0] = m_rgbKeyColor;

    // Fill choosecolor structure
    ZeroMemory (&cc, sizeof (cc));
    cc.lStructSize = sizeof (cc);
    cc.hwndOwner = this->GetSafeHwnd ();
    cc.lpCustColors = (LPDWORD) acrCustClr;
    cc.rgbResult = m_rgbKeyColor;
    cc.Flags = CC_RGBINIT;

    if (ChooseColor (&cc))
    { 
        m_rgbKeyColor = cc.rgbResult;
    }

    // Set new KeyColor
    UINT OverlayKeyColor[3];
    OverlayKeyColor[0] = GetRValue (m_rgbKeyColor);
    OverlayKeyColor[1] = GetGValue (m_rgbKeyColor);
    OverlayKeyColor[2] = GetBValue (m_rgbKeyColor);
    if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_KEY_COLOR, (void*)OverlayKeyColor, sizeof (OverlayKeyColor)) != IS_SUCCESS)
    {
        // error handling ?
    }

    GetDlgItem (IDC_KEY_COLOR)->Invalidate ();

    Invalidate ();
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnClose () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnClose ()
{
    // Delete Timer
    KillTimer (m_nTimer );

    // Events are automatically released
    if (m_hCam != NULL)
    {
        if (is_ExitCamera (m_hCam) == IS_SUCCESS)
        {
            m_hCam = NULL;
        }
    }    

    CDialog::OnClose ();
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnBnClickedCheckShowOverlay () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnBnClickedCheckShowOverlay ()
{
    if (!UpdateData())
    {
        return;
    }

    // Kill the timer
    KillTimer (m_nTimer);

    GetDlgItem (IDC_KEY_COLOR)->EnableWindow (TRUE);
    GetDlgItem (IDC_KC_TEXT)->EnableWindow (TRUE);

    if (m_bShowOverlay)
    {
        m_bShowMovingOverlay = FALSE;
        m_bLoadOverlayFromFile = FALSE;
        UpdateData(FALSE);

        // Set new KeyColor (3 UINT values for RGB)
        UINT OverlayKeyColor[3];
        OverlayKeyColor[0] = GetRValue (m_rgbKeyColor);
        OverlayKeyColor[1] = GetGValue (m_rgbKeyColor);
        OverlayKeyColor[2] = GetBValue (m_rgbKeyColor);
        if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_KEY_COLOR, (void*)OverlayKeyColor, sizeof (OverlayKeyColor)) != IS_SUCCESS)
        {
            // error handling ?
        }

        // Set the overlay to the maximum size (2 UINT values for the width and height)
        UINT Size[2];
        if (is_DirectRenderer (m_hCam, DR_GET_MAX_OVERLAY_SIZE, (void*)Size, sizeof (Size)) == IS_SUCCESS)
        {
            m_nOverlaySizeX = Size[0];
            m_nOverlaySizeY = Size[1];
        }

        if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_SIZE, (void*)Size, sizeof (Size)) == IS_SUCCESS)
        {
            // Update the size in the static controls
            UpdateOverlaySize ();
        }

        // Set the overlay position to (0,0)
        UINT Position[2];
        Position[0] = 0;
        Position[1] = 0;
        if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_POSITION, (void*)Position, sizeof (Position)) != IS_SUCCESS)
        {
            // error handling ?
        }

        CDC* pdc = NULL;

        // Clear the overlay surface
        if (is_DirectRenderer (m_hCam, DR_CLEAR_OVERLAY, NULL, NULL) != IS_SUCCESS)
        {
            // error handling ?
        }

        // Get the DC of the overlay surface
        if (is_DirectRenderer (m_hCam, DR_GET_OVERLAY_DC, (void*)&m_hDc, sizeof (m_hDc)) == IS_SUCCESS)
        {
            pdc = CDC::FromHandle (m_hDc);
        }

        if (pdc != NULL)
        {
            COLORREF OldBkColor;

            // Black background color
            OldBkColor = pdc->SetBkColor (RGB(0, 0, 0) );

            // Create pens and brushes in different colors

            // red pen
            CPen NewPen1 (PS_SOLID, 20, RGB (255, 0, 0));

            // Yellow pen
            CPen NewPen2 (PS_SOLID, 20, RGB (255, 255, 0));

            // Green brush
            CBrush NewBrush1 (RGB (0, 255, 0));

            // Blue brush
            CBrush NewBrush2 (RGB (0, 0, 255));

            // Save the old brushes
            CPen *OldPen = NULL;
            CBrush *OldBrush = NULL;

            // Draw ellipse (red border, green body)
            OldPen = pdc->SelectObject (&NewPen1);        
            OldBrush = pdc->SelectObject (&NewBrush1);
            RECT rect1 = { 30, 30, 300, 300 };
            pdc->Ellipse (&rect1);

            // Draw ellipse (yellow border, blue body)
            pdc->SelectObject (&NewPen2);
            pdc->SelectObject (NewBrush2);
            RECT rect2 = { 400, 100, 600, 300 };
            pdc->Ellipse (&rect2);

            // restore old color settings
            pdc->SetBkColor (OldBkColor );
            pdc->SelectObject (OldPen);
            pdc->SelectObject (OldBrush);

            // Delete pens and brushes
            DeleteObject (NewPen1);
            DeleteObject (NewPen2);
            DeleteObject (NewBrush1);
            DeleteObject (NewBrush2);
        }

        // Release the overlay surface DC
        if (is_DirectRenderer (m_hCam, DR_RELEASE_OVERLAY_DC, NULL, NULL) == IS_SUCCESS)
        {
            m_hDc = NULL;
        }

        // Show the overlay surface
        is_DirectRenderer (m_hCam, DR_SHOW_OVERLAY, NULL, NULL);
    } 
    else
    {
        // Hide the overlay surface
        is_DirectRenderer (m_hCam, DR_HIDE_OVERLAY, NULL, NULL);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnBnClickedCheckShowMovingOverlay () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnBnClickedCheckShowMovingOverlay()
{
    if (!UpdateData())
    {
        return;
    }

    // Kill the timer
    KillTimer (m_nTimer );

    if (m_bShowMovingOverlay)
    {
        m_bShowOverlay = FALSE;
        m_bLoadOverlayFromFile = FALSE;
        UpdateData(FALSE);

        CDC* pdc = NULL;

        // Set KeyColor to RGB(255, 0, 0) (red)
        UINT OverlayKeyColor[3];
        OverlayKeyColor[0] = 255;
        OverlayKeyColor[1] = 0;
        OverlayKeyColor[2] = 0;
        if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_KEY_COLOR, (void*)OverlayKeyColor, sizeof (OverlayKeyColor)) != IS_SUCCESS)
        {
            // error handling ?
        }

        GetDlgItem (IDC_KEY_COLOR)->EnableWindow (FALSE);
        GetDlgItem (IDC_KC_TEXT)->EnableWindow (FALSE);

        // Set the size of the overlay surface to the size of the uEye logo
        UINT Size[2];
        Size[0] = 50;
        Size[1] = 57;
        if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_SIZE, (void*)Size, sizeof (Size)) == IS_SUCCESS)
        {
            CString Str;
            Str.Format (L"Overlay width: %d", Size[0]);
            GetDlgItem (IDC_OVERLAY_WIDTH)->SetWindowText (Str);

            Str.Format (L"Overlay height: %d", Size[1]);
            GetDlgItem (IDC_OVERLAY_HEIGHT)->SetWindowText (Str);
        }      

        // Clear the overlay surface
        if (is_DirectRenderer (m_hCam, DR_CLEAR_OVERLAY, NULL, NULL) != IS_SUCCESS)
        {
            // Error handling ?
        }

        // Get the DC of the overlay surface
        if (is_DirectRenderer (m_hCam, DR_GET_OVERLAY_DC, (void*)&m_hDc, sizeof (m_hDc)) == IS_SUCCESS)
        {
            pdc = CDC::FromHandle (m_hDc);
        }

        if (pdc != NULL)
        {
            // Create a bitmap and bitblt it into the overlay
            // We could also use is_DirectRenderer (m_hCam, DR_LOAD_OVERLAY_FROM_FILE, ...)
            CBitmap bitmap;
            if (bitmap.LoadBitmap (IDB_MOVING_LOGO) == TRUE)
            {
                CDC BitmapDC;
                BitmapDC.CreateCompatibleDC (pdc);
                BitmapDC.SelectObject (&bitmap);
                pdc->BitBlt (0, 0, 50, 57, &BitmapDC, 0, 0, SRCCOPY);
                DeleteObject (bitmap);
                ReleaseDC (&BitmapDC);
            }
        }

        // Release the overlay surface DC
        if (is_DirectRenderer (m_hCam, DR_RELEASE_OVERLAY_DC, NULL, NULL) == IS_SUCCESS)
        {
            m_hDc = NULL;
        }

        // Show the overlay surface
        is_DirectRenderer (m_hCam, DR_SHOW_OVERLAY, NULL, NULL);

        // Start timer to change the position of the logo
        m_nTimer = SetTimer (1, 50, NULL);
    } 
    else
    {
        // Kill the timer
        KillTimer (m_nTimer);

        GetDlgItem (IDC_KEY_COLOR)->EnableWindow (TRUE);
        GetDlgItem (IDC_KC_TEXT)->EnableWindow (TRUE);

        // Hide the overlay surface
        is_DirectRenderer (m_hCam, DR_HIDE_OVERLAY, NULL, NULL);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnBnClickedCheckSemiTransparent () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnBnClickedCheckSemiTransparent ()
{
    if (!UpdateData())
    {
        return;
    }

    if (m_bShowSemiTransparent)
    {
        // Enable the semi transparent overlay surface 
        is_DirectRenderer (m_hCam, DR_ENABLE_SEMI_TRANSPARENT_OVERLAY, NULL, NULL);
    }
    else
    {
        // Disable the semi transparent overlay surface
        is_DirectRenderer (m_hCam, DR_DISABLE_SEMI_TRANSPARENT_OVERLAY, NULL, NULL);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::UpdateImageSize ()
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::UpdateImageSize ()
{
    CString Str;

    Str.Format (L"Image width: %d", m_nSizeX);
    GetDlgItem (IDC_IMAGE_WIDTH)->SetWindowText (Str);

    Str.Format (L"Image height: %d", m_nSizeY);
    GetDlgItem (IDC_IMAGE_HEIGHT)->SetWindowText (Str);
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::UpdateOverlaySize () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::UpdateOverlaySize ()
{
    CString Str;

    Str.Format (L"Overlay width: %d", m_nOverlaySizeX);
    GetDlgItem (IDC_OVERLAY_WIDTH)->SetWindowText (Str);

    Str.Format (L"Overlay height: %d", m_nOverlaySizeY);
    GetDlgItem (IDC_OVERLAY_HEIGHT)->SetWindowText (Str);
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::ClearDisplayBackground ()
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::ClearDisplayBackground ()
{
    if (m_bScaleImageAndOverlay == TRUE)
    {
        return;
    }

    // Clear the background of the display
    RECT rect;
    GetDlgItem (IDC_DISPLAY)->GetWindowRect (&rect);
    ScreenToClient(&rect);

    CDC *pDC = GetDlgItem (IDC_DISPLAY)->GetDC ();
    if (pDC != NULL)
    {
        if (m_bScaleImage == FALSE)
        {
            INT diff_x = (rect.right - rect.left) - m_nSizeX - 2;
            INT diff_y = (rect.bottom - rect.top) - m_nSizeY - 2;

            if (diff_x > 0)
            {
                pDC->FillSolidRect (m_nSizeX, 0, diff_x, m_nSizeY, /*RGB(255,0,0));//*/GetSysColor (COLOR_BTNFACE));
            }

            if (diff_y > 0)
            {
                pDC->FillSolidRect (0, m_nSizeY, m_nSizeX + diff_x, diff_y, /*RGB(0,255,0));//*/GetSysColor (COLOR_BTNFACE));
            }
        }
        else
        {
            pDC->FillSolidRect (0, 0, rect.right - rect.left, rect.bottom - rect.top, GetSysColor (COLOR_BTNFACE));
        }

        GetDlgItem (IDC_DISPLAY)->ReleaseDC (pDC);
    }

    GetDlgItem (IDC_DISPLAY)->Invalidate ();
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnBnClickedRadioVsync () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnBnClickedRadioVsync ()
{
    UpdateData(TRUE);

    if (m_nVsync == 0)
    {
        GetDlgItem (IDC_SPIN_SYNC_POSITION)->EnableWindow (FALSE);
        GetDlgItem (IDC_STATIC_SYNC_POS)->EnableWindow (FALSE);
        GetDlgItem (IDC_EDIT_SYNC_POS)->EnableWindow (FALSE);

        // Set the vsync off
        is_DirectRenderer (m_hCam, DR_SET_VSYNC_OFF, NULL, NULL);
    }
    else if (m_nVsync == 1)
    {
        GetDlgItem (IDC_SPIN_SYNC_POSITION)->EnableWindow (FALSE);
        GetDlgItem (IDC_STATIC_SYNC_POS)->EnableWindow (FALSE);
        GetDlgItem (IDC_EDIT_SYNC_POS)->EnableWindow (FALSE);

        // Set the vsync to auto
        is_DirectRenderer (m_hCam, DR_SET_VSYNC_AUTO, NULL, NULL);
    }
    else if (m_nVsync == 2)
    {
        GetDlgItem (IDC_SPIN_SYNC_POSITION)->EnableWindow (TRUE);
        GetDlgItem (IDC_STATIC_SYNC_POS)->EnableWindow (TRUE);
        GetDlgItem (IDC_EDIT_SYNC_POS)->EnableWindow (TRUE);

        // Enable the user sync. The sync position must be in the correct range 
        if (is_DirectRenderer (m_hCam, DR_SET_USER_SYNC, (void*)&m_SyncPosition, sizeof (m_SyncPosition)) != IS_SUCCESS)
        {
            AfxMessageBox (L"ERROR: Invalid user sync position");
        }

        ((CButton*)GetDlgItem (IDC_CHECK_SHOW_OVERLAY))->SetCheck (BST_UNCHECKED);
        ((CButton*)GetDlgItem (IDC_CHECK_SHOW_MOVING_OVERLAY))->SetCheck (BST_UNCHECKED);
    }

    m_bShowOverlay = FALSE;
    m_bShowMovingOverlay = FALSE;
    m_bLoadOverlayFromFile = FALSE;

    UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnDeltaposSpinSyncPosition () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnDeltaposSpinSyncPosition (NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (pNMUpDown->iDelta < 0)
    {
        m_SyncPosition += 5;
    }
    else
    {
        m_SyncPosition -= 5;
    }

    if (m_SyncPosition < m_nUserSyncMin)
    {
        m_SyncPosition = m_nUserSyncMin;
    }

    if (m_SyncPosition > m_nUserSyncMax)
    {
        m_SyncPosition = m_nUserSyncMax;
    }

    CString Str;
    Str.Format (L"%d", m_SyncPosition);
    GetDlgItem (IDC_EDIT_SYNC_POS)->SetWindowText (Str);

    // Enable the user sync. The sync position must be in the correct range
    if (is_DirectRenderer (m_hCam, DR_SET_USER_SYNC, (void*)&m_SyncPosition, sizeof (m_SyncPosition)) != IS_SUCCESS)
    {
        AfxMessageBox (L"ERROR: Invalid position");
    }

    ((CButton*)GetDlgItem (IDC_CHECK_SHOW_OVERLAY))->SetCheck (BST_UNCHECKED);
    ((CButton*)GetDlgItem (IDC_CHECK_SHOW_MOVING_OVERLAY))->SetCheck (BST_UNCHECKED);
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::PreTranslateMessage () 
//
///////////////////////////////////////////////////////////////////////////////
BOOL CUEyeDirectRendererDlg::PreTranslateMessage (MSG *pMsg)
{
    // If the focus is on the edit control
    if(GetDlgItem (IDC_EDIT_SYNC_POS)->GetFocus () == GetDlgItem (IDC_EDIT_SYNC_POS))
    {
        if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
        {
            CString Str;
            GetDlgItem (IDC_EDIT_SYNC_POS)->GetWindowText (Str);

            UINT pos = _tstoi (Str);

            if (pos < m_nUserSyncMin)
            {
                pos = m_nUserSyncMin;
            }

            if (pos > m_nUserSyncMax)
            {
                pos = m_nUserSyncMax;
            }

            // Enable the user sync. The sync position must be in the correct range
            m_SyncPosition = pos;
            if (is_DirectRenderer (m_hCam, DR_SET_USER_SYNC, (void*)&m_SyncPosition, sizeof (m_SyncPosition)) != IS_SUCCESS)
            {
                AfxMessageBox (L"ERROR: Invalid position");
            }

            ((CButton*)GetDlgItem (IDC_CHECK_SHOW_OVERLAY))->SetCheck (BST_UNCHECKED);
            ((CButton*)GetDlgItem (IDC_CHECK_SHOW_MOVING_OVERLAY))->SetCheck (BST_UNCHECKED);

            // Update the edit control
            Str.Format (L"%d", m_SyncPosition);
            GetDlgItem (IDC_EDIT_SYNC_POS)->SetWindowText (Str);

            return TRUE;
        }
    }

    return FALSE;
}


void CUEyeDirectRendererDlg::OnBnClickedRadioRenderMode()
{
    INT nRet = 0;

    if (m_nDirectRenderMode == 0)
    {
        m_nDirectRenderMode = 1;
        m_nDisplayMode = IS_SET_DM_OPENGL;
        nRet = is_SetDisplayMode (m_hCam, m_nDisplayMode);

        GetDlgItem(IDC_RADIO_VSYNC_USER)->EnableWindow(FALSE);

        // scale image not supported @ opengl
        GetDlgItem(IDC_CHECK_ALLOW_IMAGE_SCALING)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_OVERLAY_FROM_FILE)->EnableWindow(FALSE);
    }
    else if (m_nDirectRenderMode == 1)
    {
        m_nDirectRenderMode = 0;
        m_nDisplayMode = IS_SET_DM_DIRECT3D;
        nRet = is_SetDisplayMode (m_hCam, m_nDisplayMode);

        GetDlgItem(IDC_RADIO_VSYNC_USER)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_ALLOW_IMAGE_SCALING)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_OVERLAY_FROM_FILE)->EnableWindow(TRUE);
    }

    m_bShowOverlay = FALSE;
    m_bMono = FALSE;
    m_bScaleImage = FALSE;
    m_bScaleImageAndOverlay = FALSE;
    m_bLoadOverlayFromFile = FALSE;
    m_bShowMovingOverlay = FALSE;
    m_bShowSemiTransparent = FALSE;

    is_DirectRenderer(m_hCam, DR_DISABLE_SCALING, NULL, NULL);
    is_DirectRenderer(m_hCam, DR_DISABLE_SEMI_TRANSPARENT_OVERLAY, NULL, NULL);

    UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnBnClickedCheckMono () 
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnBnClickedCheckMono ()
{
    // Kill the timer
    KillTimer (m_nTimer);

    UpdateData(TRUE);

    if (m_bMono)
    {
        m_nDisplayMode |= IS_SET_DM_MONO;    
    }
    else
    {
        m_nDisplayMode &= ~IS_SET_DM_MONO;
    }

    // Set the new display mode
    is_SetDisplayMode (m_hCam, m_nDisplayMode);

    if (m_bShowOverlay)
    {
        OnBnClickedCheckShowOverlay();
    }
    else if (m_bShowMovingOverlay)
    {
        OnBnClickedCheckShowMovingOverlay();
    }
    else if (m_bLoadOverlayFromFile)
    {   
        m_bLoadOverlayFromFile = FALSE;
        UpdateData(FALSE);

        AfxMessageBox(L"The overlay was lost and must be loaded again", MB_ICONWARNING);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// CUEyeDirectRendererDlg::OnBnClickedCheckOverlayFromFile ()
//
///////////////////////////////////////////////////////////////////////////////
void CUEyeDirectRendererDlg::OnBnClickedCheckOverlayFromFile ()
{
    UpdateData(TRUE);

    if (m_bLoadOverlayFromFile)
    {
        BOOL oldShowOverlay = m_bShowOverlay;
        BOOL oldMovingOverlay = m_bShowMovingOverlay;

        m_bShowOverlay = FALSE;
        m_bShowMovingOverlay = FALSE;
        UpdateData(FALSE);

        // Get the name of the image from a file dialog
        CString SourceName;
        CString Extension = "bmp";
        CString Filter = "Bitmap (*.bmp)|*.bmp|All Files (*.*)|*.*||";
        CFileDialog FileDlg (TRUE, Extension, NULL, 0, Filter);
        if (FileDlg.DoModal () != IDOK)
        {
            m_bLoadOverlayFromFile = FALSE;
            m_bShowOverlay = oldShowOverlay;
            m_bShowMovingOverlay = oldMovingOverlay;
            UpdateData(FALSE);
            return;
        }

        // Kill the timer
        KillTimer (m_nTimer);

        SourceName = FileDlg.GetPathName ();

        GetDlgItem (IDC_KEY_COLOR)->EnableWindow (TRUE);
        GetDlgItem (IDC_KC_TEXT)->EnableWindow (TRUE);

        // Set new KeyColor (3 UINT values for RGB)
        UINT OverlayKeyColor[3];
        OverlayKeyColor[0] = GetRValue (m_rgbKeyColor);
        OverlayKeyColor[1] = GetGValue (m_rgbKeyColor);
        OverlayKeyColor[2] = GetBValue (m_rgbKeyColor);
        if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_KEY_COLOR, (void*)OverlayKeyColor, sizeof (OverlayKeyColor)) != IS_SUCCESS)
        {
            // error handling ?
        }

        // Set the overlay to the maximum size
        UINT Size[2];
        if (is_DirectRenderer (m_hCam, DR_GET_MAX_OVERLAY_SIZE, (void*)Size, sizeof (Size)) == IS_SUCCESS)
        {
            m_nOverlaySizeX = Size[0];
            m_nOverlaySizeY = Size[1];
        }

        if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_SIZE, (void*)Size, sizeof (Size)) == IS_SUCCESS)
        {
            // Update the size in the static controls
            UpdateOverlaySize ();
        }

        // Set the overlay position to (0,0)
        UINT Position[2];
        Position[0] = 0;
        Position[1] = 0;
        if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_POSITION, (void*)Position, sizeof (Position)) != IS_SUCCESS)
        {
            // error handling ?
        }

        // Load the overlay surface from the file
        char szFilename[255];
        ZeroMemory (szFilename, sizeof (szFilename));
        strcpy (szFilename, CStringA(SourceName));
        INT nRet = is_DirectRenderer (m_hCam, DR_LOAD_OVERLAY_FROM_FILE, (void*)szFilename, NULL);
        if (nRet != IS_SUCCESS)
        {
            if (nRet == IS_NOT_SUPPORTED)
            {
                AfxMessageBox (L"ERROR: File cannot be loaded. It is probably no bitmap");
            }
            else
            {
                AfxMessageBox (L"ERROR: File cannot be loaded");
            }
        }

        // Show the overlay surface
        is_DirectRenderer (m_hCam, DR_SHOW_OVERLAY, NULL, NULL);
    }
    else
    {
        // Hide the overlay surface
        is_DirectRenderer (m_hCam, DR_HIDE_OVERLAY, NULL, NULL);
    }
}


INT CUEyeDirectRendererDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CUEyeDirectRendererDlg::OnSize(UINT nType, int cx, int cy)
{ 
    CDialog::OnSize(nType, cx, cy);

    if (m_bInit)
    {
        INT nDiff_y = cy - m_rcClient.Height(); 
        INT nDiff_x = cx - m_rcClient.Width();

        GetDlgItem(IDC_DISPLAY)->SetWindowPos(NULL, 8, 8, cx - 18, cy - 190, SWP_NOZORDER);
        GetDlgItem(IDC_DISPLAY)->Invalidate();

        MoveControlPosition(IDC_IMAGE_WIDTH, m_rcImageWidth, nDiff_y);
        MoveControlPosition(IDC_IMAGE_HEIGHT, m_rcImageHeight, nDiff_y);
        MoveControlPosition(IDC_OVERLAY_WIDTH, m_rcOverlayWidth, nDiff_y);
        MoveControlPosition(IDC_OVERLAY_HEIGHT, m_rcOverlayHeight, nDiff_y);
        MoveControlPosition(IDC_CHECK_MONO, m_rcCheckMono, nDiff_y);
        MoveControlPosition(IDC_CHECK_SHOW_OVERLAY, m_rcCheckShowOverlay, nDiff_y);
        MoveControlPosition(IDC_CHECK_ALLOW_SCALING, m_rcCheckAllowScaling, nDiff_y);
        MoveControlPosition(IDC_CHECK_ALLOW_IMAGE_SCALING, &m_rcCheckAllowImageScaling, nDiff_y);
        MoveControlPosition(IDC_CHECK_SHOW_MOVING_OVERLAY, m_rcCheckShowMovingOverlay, nDiff_y);
        MoveControlPosition(IDC_CHECK_SEMI_TRANSPARENT, m_rcCheckSemiTransparent, nDiff_y);
        MoveControlPosition(IDC_CHECK_OVERLAY_FROM_FILE, m_rcCheckOverlayFromFile, nDiff_y);
        MoveControlPosition(IDC_STATIC, m_rcLogo, nDiff_y);
        MoveControlPosition(IDC_RADIO_DIRECT3D, m_rcRadioDirect3D, nDiff_y);
        MoveControlPosition(IDC_RADIO_OPENGL, m_rcRadioOpenGL, nDiff_y);
        MoveControlPosition(IDC_RADIO_VSYNC_OFF, &m_rcRadioVsyncOff, nDiff_y);
        MoveControlPosition(IDC_RADIO_VSYNC_AUTO, &m_rcRadioVsyncAuto, nDiff_y);
        MoveControlPosition(IDC_RADIO_VSYNC_USER, &m_rcRadioVsyncUser, nDiff_y);
        MoveControlPosition(IDC_STATIC_SYNC_POS, &m_rcStaticSyncPos, nDiff_y);
        MoveControlPosition(IDC_EDIT_SYNC_POS, &m_rcEditSyncPos, nDiff_y);
        MoveControlPosition(IDC_SPIN_SYNC_POSITION, &m_rcSpinSyncPos, nDiff_y);
        MoveControlPosition(IDC_KEY_COLOR, &m_rcKeyColor, nDiff_y);
        MoveControlPosition(IDC_KC_TEXT, &m_rcKeyColorText, nDiff_y);
        MoveControlPosition(IDC_BUTTON_LIVE, &m_rcBtnLive, nDiff_y);
        MoveControlPosition(IDC_BUTTON_STOP_LIVE, &m_rcBtnStopLive, nDiff_y);
        MoveControlPosition(IDC_BUTTON_LOAD_PARAMETER, &m_rcBtnLoadParameter, nDiff_y);
        MoveControlPosition(IDC_BUTTON_EXIT, &m_rcBtnExit, nDiff_y);

        ClearDisplayBackground();
    }
}


void CUEyeDirectRendererDlg::SaveControlPosition(INT nControlId, CRect *pRectControl)
{
    GetDlgItem(nControlId)->GetWindowRect(pRectControl);
    ScreenToClient(pRectControl);
}


void CUEyeDirectRendererDlg::MoveControlPosition(INT nControlId, CRect rectControl, INT cy)
{
    GetDlgItem(nControlId)->SetWindowPos(NULL, 
        rectControl.left,
        rectControl.top + cy, 
        rectControl.Width(), 
        rectControl.Height(), 
        SWP_NOZORDER | SWP_NOSIZE
        );

    GetDlgItem(nControlId)->Invalidate();
}


void CUEyeDirectRendererDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    if (m_bInit)
    {
        lpMMI->ptMinTrackSize.x = m_rcDialog.Width();
        lpMMI->ptMinTrackSize.y = m_rcDialog.Height();
    }
}


void CUEyeDirectRendererDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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


#pragma warning (default : 4995 4996)