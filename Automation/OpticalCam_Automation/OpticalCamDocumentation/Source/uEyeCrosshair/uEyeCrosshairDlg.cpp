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
#include "uEyeCrosshair.h"
#include "uEyeCrosshairDlg.h"

#include "CameraConfigDlg.h"
#include "ProgramConfigDlg.h"

#include "version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialog
{
public:

    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

private:

    virtual void DoDataExchange(CDataExchange* pDX);

    BOOL OnInitDialog ();
    DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog ()
{
    CDialog::OnInitDialog();

    CString Str;
    GetDlgItem(IDC_STATIC_COPYRIGHT)->GetWindowText(Str);
    Str.Append(_T(CURRENT_YEAR));
    GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(Str);

    return TRUE;
}


uEyeCrosshairDlg::uEyeCrosshairDlg(CWnd* pParent /*=NULL*/)
: CDialog(uEyeCrosshairDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void uEyeCrosshairDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(uEyeCrosshairDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage)
    ON_WM_CLOSE()
    ON_WM_SIZE()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_LBUTTONDOWN()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_PROGRAM_CONFIGURATION, &uEyeCrosshairDlg::OnProgramConfiguration)
END_MESSAGE_MAP()


BOOL uEyeCrosshairDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);

    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString (IDS_ABOUTBOX);

        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, ID_CONFIGURATION, L"Configuration...");
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon (m_hIcon, TRUE);
    SetIcon (m_hIcon, FALSE);

    // Load Xml file or set default settings
    m_CameraOptions.bAES          = TRUE;
    m_CameraOptions.bAGC          = TRUE;
    m_CameraOptions.bAWB          = TRUE;
    m_CameraOptions.bMirrorHor    = FALSE;
    m_CameraOptions.bMirrorVer    = FALSE;
    m_CameraOptions.nContrast     = 0;
    m_CameraOptions.nLum          = 99;
    m_CameraOptions.nColorMode    = 0;
    m_CameraOptions.nAESskipFrame = 4;
    m_CameraOptions.nAGCskipFrame = 4;

    m_ProgramOptions.bStartFullscreen = 0;
    m_ProgramOptions.DrawColor        = RGB (255, 255, 255);
    m_ProgramOptions.KeyColor         = RGB (0, 0, 0);
    m_ProgramOptions.nCircleRadius    = 0;
    m_ProgramOptions.nPencilWidth     = 3;
    m_ProgramOptions.strPicturePath   = "";
    m_ProgramOptions.bShowPicture     = 0;
    m_ProgramOptions.nPicturePosX     = 0;
    m_ProgramOptions.nPicturePosY     = 0;
    m_ProgramOptions.nCrossPosX       = -1;
    m_ProgramOptions.nCrossPosY       = -1;
    m_ProgramOptions.m_nCanvasSizeX   = 0;
    m_ProgramOptions.m_nCanvasSizeY   = 0;

    LoadConfig ();

    m_hWndDisplay = GetDlgItem (IDC_DISPLAY)->m_hWnd;
    m_hCam = 0;
    m_nDisplayMode = IS_SET_DM_DIRECT3D;

    //Open the camera
    OpenCamera();

    // set crossshair standard position
    if(m_ProgramOptions.nCrossPosX == -1 || m_ProgramOptions.nCrossPosY == -1)
    {
        m_ProgramOptions.nCrossPosX = m_nSizeX / 2;
        m_ProgramOptions.nCrossPosY = m_nSizeY / 2;
    }

    UpdateCanvasSizeProgramOptions();

    //draw the objects
    DrawObjects();

    return TRUE;
}

BOOL uEyeCrosshairDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_KEYDOWN)
    {
        // avoid close
        if ((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN))
        {
            return 0;
        }

    }

    return CDialog::PreTranslateMessage(pMsg);
}

void uEyeCrosshairDlg::OnSysCommand (UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal ();
    }
    else
    {
        if((nID) == ID_CONFIGURATION)
        {
            OpenConfigurationDlg();
        }
        else
        {
            CDialog::OnSysCommand (nID, lParam);
        }
    }
}


void uEyeCrosshairDlg::OnPaint ()
{
    if (IsIconic ())
    {
        CPaintDC dc (this);

        SendMessage (WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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


HCURSOR uEyeCrosshairDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


bool uEyeCrosshairDlg::OpenCamera ()
{
    INT nRet = IS_NO_SUCCESS;
    ExitCamera ();

    // init camera (open next available camera)
    m_hCam = (HIDS) 0;
    nRet = InitCamera (&m_hCam, m_hWndDisplay);
    if (nRet == IS_SUCCESS)
    {
        //Check if DirectX is available on the local machine
        if(is_DirectRenderer (m_hCam, DR_CHECK_COMPATIBILITY, NULL , NULL) != IS_SUCCESS)
        {
            AfxMessageBox(_T("This Demo requires support of Direct3D.\nThis usually means that either DirectX or the graphics card driver is not installed properly"),
                MB_OK | MB_ICONSTOP);
            PostQuitMessage (0);
            return false;
        }

        // Setup the color depth to the current windows setting
        is_GetColorDepth (m_hCam, &m_nBitsPerPixel, &m_nColorMode);
        is_SetColorMode  (m_hCam, m_nColorMode);

        // retrieve original image size
        is_GetSensorInfo (m_hCam, &m_sInfo);

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

        // Display initialization (Set to Direct3D)
        //set to black/white if ColorMode == 1
        if(!m_CameraOptions.nColorMode)
        {
            nRet = is_SetDisplayMode (m_hCam, m_nDisplayMode);
        }
        else
        {
            nRet = is_SetDisplayMode (m_hCam, m_nDisplayMode | IS_SET_DM_MONO);
        }

        if (nRet != IS_SUCCESS)
        {
            CString Msg;
            switch (nRet)
            {
            case IS_DR_LIBRARY_NOT_FOUND:
                Msg = "ERROR: is_SetDisplayMode(). Please check if you have installed the latest DirectX (March 2009)";
                break;
            case IS_DR_DEVICE_OUT_OF_MEMORY:
                Msg = "ERROR: is_SetDisplayMode(). The video memory of the graphics adapter is insufficient";
                break;
            case IS_DR_CANNOT_CREATE_SURFACE:
                Msg = "ERROR: is_SetDisplayMode(). The image or overlay surface could not be created";
                break;
            case IS_DR_CANNOT_CREATE_VERTEX_BUFFER:
                Msg = "ERROR: is_SetDisplayMode(). The vertex buffer could not be created";
                break;
            case IS_DR_CANNOT_CREATE_TEXTURE:
                Msg = "ERROR: is_SetDisplayMode(). The texture could not be created";
                break;
            default:
                Msg = "ERROR: is_SetDisplayMode()";
            }

            AfxMessageBox (Msg);
            PostQuitMessage (0);
        }

        // Get the default key color for transparency
        UINT OverlayKeyColor[3];
        if (is_DirectRenderer (m_hCam, DR_GET_OVERLAY_KEY_COLOR, (void*)OverlayKeyColor, sizeof(OverlayKeyColor)) == IS_SUCCESS)
        {
            m_ProgramOptions.KeyColor = RGB (OverlayKeyColor[0], OverlayKeyColor[1], OverlayKeyColor[2]); 
        }
        else
        {
            AfxMessageBox (L"Error: Keycolor can not be read", MB_ICONERROR);
            m_ProgramOptions.KeyColor = RGB (0, 0, 0);
        }

        // Read the maximum overlay size
        UINT OverlaySize[2];
        if (is_DirectRenderer (m_hCam, DR_GET_MAX_OVERLAY_SIZE, (void*) OverlaySize, sizeof (OverlaySize)) == IS_SUCCESS)
        {
            m_nOverlaySizeX = OverlaySize[0];
            m_nOverlaySizeY = OverlaySize[1];
        }
        else
        {
            AfxMessageBox (L"Error: Can not read max overlay size", MB_ICONERROR);
        }

        //exec the configuration
        ExecConfig ();

        if ((OverlaySize[0] < (UINT) m_nSizeX) || (OverlaySize[1] < (UINT) m_nSizeY))
        {
            AfxMessageBox (L"The maximum possible overlay size is smaller than the image size!");
        }

        // Enable Messages
        is_EnableMessage (m_hCam, IS_DEVICE_REMOVED,     GetSafeHwnd ());
        is_EnableMessage (m_hCam, IS_DEVICE_RECONNECTED, GetSafeHwnd ());
        is_EnableMessage (m_hCam, IS_FRAME,              GetSafeHwnd ());

        // if the overlay should be scaled as well: DR_ENABLE_SCALING
        is_DirectRenderer (m_hCam, DR_ENABLE_IMAGE_SCALING, NULL, NULL);
        // start live video
        is_CaptureVideo   (m_hCam, IS_WAIT);
    }
    else
    {
        AfxMessageBox(TEXT("No uEye camera could be opened !"), MB_ICONWARNING );
        PostQuitMessage (0);
    }

    return true;
}


void uEyeCrosshairDlg::ExitCamera ()
{
    if (m_hCam != 0)
    {
        // Disable messages
        is_EnableMessage (m_hCam, IS_FRAME, NULL);

        // Stop live video
        is_StopLiveVideo (m_hCam, IS_WAIT);

        // Close camera
        is_ExitCamera (m_hCam);
        m_hCam = NULL;
    }
}


LRESULT uEyeCrosshairDlg::OnUEyeMessage (WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case IS_DEVICE_REMOVED:
        Beep (400, 50);
        break;

    case IS_DEVICE_RECONNECTED:
        Beep (400, 50);
        break;

    case IS_FRAME:
        // is_RenderBitmap( m_hCam, m_lMemoryId, m_hWndDisplay, IS_RENDER_FIT_TO_WINDOW );
        break;
    }    

    return 0;
}


void uEyeCrosshairDlg::OnClose ()
{
    //save configuration
    SaveConfig();
    ExitCamera ();
    CDialog::OnClose ();
}


void uEyeCrosshairDlg::OnSize (UINT nType, int cx, int cy)
{
    CDialog::OnSize (nType, cx, cy);

    CRect rec;
    GetWindowRect  (&rec);
    ScreenToClient (&rec);

    UpdateCanvasSizeProgramOptions();

    if (GetDlgItem (IDC_DISPLAY))
    {
        if(nType == SIZE_MAXIMIZED)
        {
            m_ProgramOptions.bStartFullscreen = true;
        }
        else
        {
            m_ProgramOptions.bStartFullscreen = false;
        }

        // get size of caption
        int nCyCaption = ::GetSystemMetrics (SM_CYCAPTION);

        // resize display and redraw objects
        GetDlgItem    (IDC_DISPLAY)->MoveWindow (rec.left + 0, rec.top + nCyCaption, rec.Width () - 0, rec.Height () - nCyCaption);
        Sleep(40);
        DrawObjects ();
    }
}


void uEyeCrosshairDlg::OnWindowPosChanging (WINDOWPOS* lpwndpos)
{
    CDialog::OnWindowPosChanging (lpwndpos);
}

void uEyeCrosshairDlg::UpdateCanvasSizeProgramOptions()
{
    CRect rec;
    GetWindowRect(&rec);

    // get size of caption
    int nCyCaption = ::GetSystemMetrics (SM_CYCAPTION);

    m_ProgramOptions.m_nCanvasSizeX = rec.Width();
    m_ProgramOptions.m_nCanvasSizeY = rec.Height() - nCyCaption;
}

void uEyeCrosshairDlg::DrawObjects()
{
    // clear before drawing
    is_DirectRenderer (m_hCam, DR_CLEAR_OVERLAY, NULL, NULL);

    // Set new KeyColor
    UINT OverlayKeyColor[3];
    OverlayKeyColor[0] = GetRValue (m_ProgramOptions.KeyColor);
    OverlayKeyColor[1] = GetGValue (m_ProgramOptions.KeyColor);
    OverlayKeyColor[2] = GetBValue (m_ProgramOptions.KeyColor);

    if (is_DirectRenderer (m_hCam, DR_SET_OVERLAY_KEY_COLOR, (void*) OverlayKeyColor, sizeof (OverlayKeyColor)) != IS_SUCCESS)
    {
        // error handling
    }

    CRect rec;
    GetWindowRect(&rec);

    // Set the overlay to the maximum (window) size
    UINT Size[2];

    Size[0] = rec.Width();
    Size[1] = rec.Height();

    is_DirectRenderer (m_hCam, DR_SET_OVERLAY_SIZE, (void*)&Size, sizeof (Size));

    CDC* pdc   = NULL;
    HDC  m_hDc = NULL;

    if (is_DirectRenderer (m_hCam, DR_GET_OVERLAY_DC, (void*) &m_hDc, sizeof (m_hDc)) == IS_SUCCESS)
    {
        pdc = CDC::FromHandle (m_hDc);
    }

    if (pdc != NULL)
    {
        COLORREF OldBkColor;

        // Black background color
        OldBkColor = pdc->SetBkColor (RGB (255, 255, 255));

        // Create pen
        const int nPencilWidth = 3; 
        CPen NewPen (PS_SOLID, m_ProgramOptions.nPencilWidth, m_ProgramOptions.DrawColor);
        CBrush NewBrush(RGB (0, 0, 0));

        // Save the old brushes
        CPen* pOldPen = NULL;
        CBrush *OldBrush = NULL;

        pOldPen = pdc->SelectObject (&NewPen);
        OldBrush = pdc->SelectObject(&NewBrush);

        // draw image
        if(m_ProgramOptions.bShowPicture)
        {
            HANDLE hBitmap = NULL;
            hBitmap = LoadImage (NULL, m_ProgramOptions.strPicturePath, IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);

            if (hBitmap != NULL)
            {
                BITMAP bitmap;
                GetObject (hBitmap, sizeof (BITMAP), &bitmap);

                // Get the width and height of the image
                int width, height;
                width = bitmap.bmWidth;
                height = bitmap.bmHeight;

                HDC hImageDC = CreateCompatibleDC (m_hDc);
                SelectObject (hImageDC, hBitmap);
                BitBlt (m_hDc, m_ProgramOptions.nPicturePosX, m_ProgramOptions.nPicturePosY, width, height, hImageDC, 0, 0, SRCCOPY);

                DeleteDC (hImageDC);
                DeleteObject (&bitmap);
                DeleteObject (hBitmap);
            }
        }

        RECT wndRect;
        GetWindowRect(&wndRect);

        //// Draw circle
        RECT rect1 = {
            static_cast<LONG>(m_ProgramOptions.nCrossPosX - m_ProgramOptions.nCircleRadius),
            static_cast<LONG>(m_ProgramOptions.nCrossPosY - m_ProgramOptions.nCircleRadius),
            static_cast<LONG>(m_ProgramOptions.nCrossPosX + m_ProgramOptions.nCircleRadius),
            static_cast<LONG>(m_ProgramOptions.nCrossPosY + m_ProgramOptions.nCircleRadius)};
            pdc->Ellipse (&rect1);

            // Draw crosshair
            pdc->MoveTo (m_ProgramOptions.nCrossPosX, 0);
            pdc->LineTo (m_ProgramOptions.nCrossPosX, rec.Height());

            //left to right
            pdc->MoveTo (0, m_ProgramOptions.nCrossPosY);
            pdc->LineTo (rec.Width(), m_ProgramOptions.nCrossPosY);

            // restore old color settings
            pdc->SetBkColor   (OldBkColor);
            pdc->SelectObject (pOldPen);
            pdc->SelectObject(OldBrush);

            // Delete pens and brushes
            DeleteObject (NewPen);
            DeleteObject (NewBrush);
    }

    if (is_DirectRenderer (m_hCam, DR_RELEASE_OVERLAY_DC, NULL, NULL) == IS_SUCCESS)
    {
        m_hDc = NULL;
    }

    is_DirectRenderer (m_hCam, DR_SHOW_OVERLAY, NULL, NULL);
}


void uEyeCrosshairDlg::OnLButtonDown (UINT nFlags, CPoint point)
{
    // Open the configuration dialog with a left mouse click
    OpenConfigurationDlg();
}


bool uEyeCrosshairDlg::LoadConfig()
{
    // read of the xml file

    // open the file
    TiXmlDocument doc;
    if(!doc.LoadFile("config.xml"))
    {
        return false;
    }

    TiXmlNode* node = 0;
    node = doc.FirstChild("Config"); // set the beginning to "config"

    // 3 settings to pass through
    for(TiXmlElement *element = node->ToElement()->FirstChildElement(); element != 0; element = element->NextSiblingElement())
    {
        // pass through the program settings
        if(!strcmp(element->Value(), "WindowPosition"))
        {
            //get the window position settings
            int nX, nY, nWidth, nHeight;
            for(TiXmlElement* ele = element->FirstChildElement(); ele != 0; ele = ele->NextSiblingElement())
            {
                if(!strcmp(ele->Value(), "X"))
                {
                    nX = (atoi(ele->GetText()));
                }

                if(!strcmp(ele->Value(), "Y"))
                {
                    nY = (atoi(ele->GetText()));
                }

                if(!strcmp(ele->Value(), "Width"))
                {
                    nWidth = (atoi(ele->GetText()));
                }

                if(!strcmp(ele->Value(), "Height"))
                {
                    nHeight = (atoi(ele->GetText()));
                }
            }

            //set the window position
            SetWindowPos(&CWnd::wndTop, nX, nY, nWidth, nHeight, SWP_NOZORDER);
        }

        if(!strcmp(element->Value(), "Program"))
        {
            // pass through the program settings
            for(TiXmlElement* ele = element->FirstChildElement(); ele != 0; ele = ele->NextSiblingElement())
            {
                if(!strcmp(ele->Value(), "ShowPicture"))
                {
                    m_ProgramOptions.bShowPicture = (atoi(ele->GetText()) != 0);
                }

                if(!strcmp(ele->Value(), "PicturePath"))
                {
                    m_ProgramOptions.strPicturePath = ele->GetText();
                }

                if(!strcmp(ele->Value(), "PencilWidth"))
                {
                    m_ProgramOptions.nPencilWidth = (atoi(ele->GetText()));
                }

                if(!strcmp(ele->Value(), "DrawColor"))
                {
                    // go to the child an pass through
                    int nRed,nGreen,nBlue;
                    for(TiXmlElement* el = ele->FirstChildElement(); el != 0; el = el->NextSiblingElement())
                    {
                        if(!strcmp(el->Value(), "Red"))
                        {
                            nRed = (atoi(el->GetText()));
                        }

                        if(!strcmp(el->Value(), "Green"))
                        {
                            nGreen = (atoi(el->GetText()));
                        }

                        if(!strcmp(el->Value(), "Blue"))
                        {
                            nBlue = (atoi(el->GetText()));
                        }
                    }

                    m_ProgramOptions.DrawColor = RGB(nRed,nGreen,nBlue);
                }

                if(!strcmp(ele->Value(), "KeyColor"))
                {
                    // go to the child an pass through
                    int nRed,nGreen,nBlue;
                    for(TiXmlElement* el = ele->FirstChildElement(); el != 0; el = el->NextSiblingElement())
                    {
                        if(!strcmp(el->Value(), "Red"))
                        {
                            nRed = (atoi(el->GetText()));
                        }

                        if(!strcmp(el->Value(), "Green"))
                        {
                            nGreen = (atoi(el->GetText()));
                        }

                        if(!strcmp(el->Value(), "Blue"))
                        {
                            nBlue = (atoi(el->GetText()));
                        }
                    }

                    m_ProgramOptions.KeyColor = RGB(nRed,nGreen,nBlue);
                }

                if(!strcmp(ele->Value(), "PicturePosition"))
                {
                    // go to the child an pass through
                    for(TiXmlElement* el = ele->FirstChildElement(); el != 0; el = el->NextSiblingElement())
                    {
                        if(!strcmp(el->Value(), "X"))
                        {
                            m_ProgramOptions.nPicturePosX = (atoi(el->GetText()));
                        }

                        if(!strcmp(el->Value(), "Y"))
                        {
                            m_ProgramOptions.nPicturePosY = (atoi(el->GetText()));
                        }
                    }
                }

                if(!strcmp(ele->Value(), "CrossPosition"))
                {
                    // go to the child an pass through
                    for(TiXmlElement* el = ele->FirstChildElement(); el != 0; el = el->NextSiblingElement())
                    {
                        if(!strcmp(el->Value(), "X"))
                        {
                            m_ProgramOptions.nCrossPosX = (atoi(el->GetText()));
                        }

                        if(!strcmp(el->Value(), "Y"))
                        {
                            m_ProgramOptions.nCrossPosY = (atoi(el->GetText()));
                        }
                    }
                }

                if(!strcmp(ele->Value(), "CircleRadius"))
                {
                    m_ProgramOptions.nCircleRadius = atoi(ele->GetText());
                }

                if(!strcmp(ele->Value(), "Fullscreen"))
                {
                    m_ProgramOptions.bStartFullscreen = (atoi(ele->GetText()) != 0);
                }
            }
        }

        if(!strcmp(element->Value(), "Camera"))
        {
            // pass through the camera settings
            for(TiXmlElement* ele = element->FirstChildElement(); ele != 0; ele = ele->NextSiblingElement())
            {
                CString str = CString(ele->Value());
                str = ele->GetText();

                if(!strcmp(ele->Value(), "AWB"))
                {
                    m_CameraOptions.bAWB = (atoi(ele->GetText()) != 0);
                }

                if(!strcmp(ele->Value() , "AGC"))
                {
                    // go to the child an pass through
                    for(TiXmlElement* el = ele->FirstChildElement(); el != 0; el = el->NextSiblingElement())
                    {
                        if(!strcmp(el->Value(), "Enable"))
                        {
                            m_CameraOptions.bAGC = (atoi(el->GetText()) != 0);
                        }

                        if(!strcmp(el->Value(), "FrameSkip"))
                        {
                            m_CameraOptions.nAGCskipFrame = (atoi(el->GetText()));
                        }
                    }
                }

                if(!strcmp(ele->Value() , "AES"))
                {
                    // go to the child an pass through
                    for(TiXmlElement* el = ele->FirstChildElement(); el != 0; el = el->NextSiblingElement())
                    {
                        if(!strcmp(el->Value(), "Enable"))
                        {
                            m_CameraOptions.bAES = (atoi(el->GetText()) != 0);
                        }

                        if(!strcmp(el->Value(), "FrameSkip"))
                        {
                            m_CameraOptions.nAESskipFrame = (atoi(el->GetText()));
                        }
                    }
                }

                if(!strcmp(ele->Value() , "MirrorVer"))
                {
                    m_CameraOptions.bMirrorVer = (atoi(ele->GetText()) != 0);
                }

                if(!strcmp(ele->Value() , "MirrorHor"))
                {
                    m_CameraOptions.bMirrorHor = (atoi(ele->GetText()) != 0);
                }

                if(!strcmp(ele->Value() , "Contrast"))
                {
                    m_CameraOptions.nContrast = atoi(ele->GetText());
                }

                if(!strcmp(ele->Value(),"Lum"))
                {
                    m_CameraOptions.nLum = atoi(ele->GetText());
                }

                if(!strcmp(ele->Value(),"ColorMode"))
                {
                    m_CameraOptions.nColorMode = atoi(ele->GetText());
                }
            }
        }
    }

    return true;
}


bool uEyeCrosshairDlg::SaveConfig()
{
    // saves the settings in a xml file
    // the old file will be overwritten

    CString strTemp;

    TiXmlDocument doc;
    TiXmlElement ElementRoot("Config" );
    TiXmlElement ElementProgram("Program" );
    TiXmlElement ElementCamera("Camera");
    TiXmlElement ElementWindowPosition("WindowPosition");

    // program settings

    //getwindowpos settings
    WINDOWPLACEMENT Placement;
    Placement.length = sizeof(WINDOWPLACEMENT);
    theApp.m_pMainWnd->GetWindowPlacement(&Placement);

    // Save x coordinate
    strTemp.Format(L"%d", Placement.rcNormalPosition.left);
    TiXmlElement  ElementWindownPicturePosX("X");
    TiXmlText     TextWindownPicturePosX(static_cast<const char*>(CStringA(strTemp)));
    ElementWindownPicturePosX.InsertEndChild(TextWindownPicturePosX);
    ElementWindowPosition.InsertEndChild(ElementWindownPicturePosX);

    // Save y coordinate
    strTemp.Format(L"%d", Placement.rcNormalPosition.top);
    TiXmlElement  ElementWindownPicturePosY("Y");
    TiXmlText     TextWindownPicturePosY(static_cast<const char*>(CStringA(strTemp)));
    ElementWindownPicturePosY.InsertEndChild(TextWindownPicturePosY);
    ElementWindowPosition.InsertEndChild(ElementWindownPicturePosY);

    // Save width
    strTemp.Format(L"%d", Placement.rcNormalPosition.right - Placement.rcNormalPosition.left);
    TiXmlElement  ElementWindowPositionWidth("Width");
    TiXmlText     TextWindowPositionWidth(static_cast<const char*>(CStringA(strTemp)));
    ElementWindowPositionWidth.InsertEndChild(TextWindowPositionWidth);
    ElementWindowPosition.InsertEndChild(ElementWindowPositionWidth);

    // Save height
    strTemp.Format(L"%d", Placement.rcNormalPosition.bottom - Placement.rcNormalPosition.top);
    TiXmlElement  ElementWindowPositionHeight("Height");
    TiXmlText     TextWindowPositionHeight(static_cast<const char*>(CStringA(strTemp)));
    ElementWindowPositionHeight.InsertEndChild(TextWindowPositionHeight);
    ElementWindowPosition.InsertEndChild(ElementWindowPositionHeight);

    ElementRoot.InsertEndChild(ElementWindowPosition);

    // pencil width
    strTemp.Format(L"%d",m_ProgramOptions.nPencilWidth);
    TiXmlElement ElementPencilWidth("PencilWidth");
    TiXmlText TextPencilWidth(static_cast<const char*>(CStringA(strTemp)));

    ElementPencilWidth.InsertEndChild(TextPencilWidth);
    ElementProgram.InsertEndChild(ElementPencilWidth);

    // draw color
    TiXmlElement ElementDrawColor("DrawColor" );

    // red
    strTemp.Format(L"%d",GetRValue(m_ProgramOptions.DrawColor));
    TiXmlElement ElementRed ("Red");
    TiXmlText TextRed(static_cast<const char*>(CStringA(strTemp)));

    ElementRed.InsertEndChild(TextRed);
    ElementDrawColor.InsertEndChild(ElementRed);

    // green
    strTemp.Format(L"%d",GetGValue(m_ProgramOptions.DrawColor));
    TiXmlElement ElementGreen ("Green");
    TiXmlText TextGreen (static_cast<const char*>(CStringA(strTemp)));

    ElementGreen.InsertEndChild(TextGreen);
    ElementDrawColor.InsertEndChild(ElementGreen);

    // blue
    strTemp.Format(L"%d",GetBValue(m_ProgramOptions.DrawColor));
    TiXmlElement ElementBlue ("Blue");
    TiXmlText TextBlue	(static_cast<const char*>(CStringA(strTemp)));

    ElementBlue.InsertEndChild(TextBlue);
    ElementDrawColor.InsertEndChild(ElementBlue);

    ElementProgram.InsertEndChild(ElementDrawColor);

    // draw color
    TiXmlElement ElementKeyColor("KeyColor" );

    // red
    strTemp.Format(L"%d",GetRValue(m_ProgramOptions.KeyColor));
    TiXmlElement ElementKeyRed ("Red");
    TiXmlText TextKeyRed(static_cast<const char*>(CStringA(strTemp)));

    ElementKeyRed.InsertEndChild(TextKeyRed);
    ElementKeyColor.InsertEndChild(ElementKeyRed);

    // green
    strTemp.Format(L"%d",GetGValue(m_ProgramOptions.KeyColor));
    TiXmlElement ElementKeyGreen ("Green");
    TiXmlText TextKeyGreen (static_cast<const char*>(CStringA(strTemp)));

    ElementKeyGreen.InsertEndChild(TextKeyGreen);
    ElementKeyColor.InsertEndChild(ElementKeyGreen);

    // blue
    strTemp.Format(L"%d",GetBValue(m_ProgramOptions.KeyColor));
    TiXmlElement ElementKeyBlue ("Blue");
    TiXmlText TextKeyBlue (static_cast<const char*>(CStringA(strTemp)));

    ElementKeyBlue.InsertEndChild(TextKeyBlue);
    ElementKeyColor.InsertEndChild(ElementKeyBlue);

    ElementProgram.InsertEndChild(ElementKeyColor);

    //	circle radius
    strTemp.Format(L"%d",m_ProgramOptions.nCircleRadius);
    TiXmlElement ElementCircleRadius("CircleRadius");
    TiXmlText TextCircleRadius(static_cast<const char*>(CStringA(strTemp)));

    ElementCircleRadius.InsertEndChild(TextCircleRadius);
    ElementProgram.InsertEndChild(ElementCircleRadius);

    // Fullscreen
    strTemp.Format(L"%d",m_ProgramOptions.bStartFullscreen);
    TiXmlElement ElementFullscreen("Fullscreen");
    TiXmlText TextFullscreen(static_cast<const char*>(CStringA(strTemp)));

    ElementFullscreen.InsertEndChild(TextFullscreen);
    ElementProgram.InsertEndChild(ElementFullscreen);

    // Show picture
    strTemp.Format(L"%d",m_ProgramOptions.bShowPicture);
    TiXmlElement ElementShowPicture("ShowPicture");
    TiXmlText TextShowPicture(static_cast<const char*>(CStringA(strTemp)));

    ElementShowPicture.InsertEndChild(TextShowPicture);
    ElementProgram.InsertEndChild(ElementShowPicture);

    // Picture position
    TiXmlElement ElementPosition("PicturePosition");

    strTemp.Format(L"%d",m_ProgramOptions.nPicturePosX);
    TiXmlElement ElementnPicturePosX("X");
    TiXmlText TextnPicturePosX(static_cast<const char*>(CStringA(strTemp)));

    ElementnPicturePosX.InsertEndChild(TextnPicturePosX);
    ElementPosition.InsertEndChild(ElementnPicturePosX);

    strTemp.Format(L"%d",m_ProgramOptions.nPicturePosY);
    TiXmlElement ElementnPicturePosY("Y");
    TiXmlText TextnPicturePosY(static_cast<const char*>(CStringA(strTemp)));

    ElementnPicturePosY.InsertEndChild(TextnPicturePosY);
    ElementPosition.InsertEndChild(ElementnPicturePosY);

    ElementProgram.InsertEndChild(ElementPosition);

    // Picture path
    TiXmlElement ElementPicturePath("PicturePath");
    TiXmlText TextPicturePath(static_cast<const char*>(CStringA(m_ProgramOptions.strPicturePath)));

    ElementPicturePath.InsertEndChild(TextPicturePath);
    ElementProgram.InsertEndChild(ElementPicturePath);

    // crossshar position
    TiXmlElement ElementCrossPosition("CrossPosition");

    strTemp.Format(L"%d",m_ProgramOptions.nCrossPosX);
    TiXmlElement ElementCrossPosX("X");
    TiXmlText TextCrossPosX(static_cast<const char*>(CStringA(strTemp)));

    ElementCrossPosX.InsertEndChild(TextCrossPosX);
    ElementCrossPosition.InsertEndChild(ElementCrossPosX);

    strTemp.Format(L"%d",m_ProgramOptions.nCrossPosY);
    TiXmlElement ElementCrossPosY("Y");
    TiXmlText TextCrossPosY(static_cast<const char*>(CStringA(strTemp)));

    ElementCrossPosY.InsertEndChild(TextCrossPosY);
    ElementCrossPosition.InsertEndChild(ElementCrossPosY);

    ElementProgram.InsertEndChild(ElementCrossPosition);

    // camera settings
    // AWB
    strTemp.Format(L"%d",m_CameraOptions.bAWB);
    TiXmlElement ElementAWB("AWB");
    TiXmlText TextAWB(static_cast<const char*>(CStringA(strTemp)));

    ElementAWB.InsertEndChild(TextAWB);
    ElementCamera.InsertEndChild(ElementAWB);

    // AGC
    TiXmlElement ElementAGC("AGC");

    strTemp.Format(L"%d",m_CameraOptions.bAGC);
    TiXmlElement ElementAGCEnable("Enable");
    TiXmlText TextAGC(static_cast<const char*>(CStringA(strTemp)));

    ElementAGCEnable.InsertEndChild(TextAGC);
    ElementAGC.InsertEndChild(ElementAGCEnable);

    strTemp.Format(L"%d",m_CameraOptions.nAGCskipFrame);
    TiXmlElement ElementAGCskipFrame("FrameSkip");
    TiXmlText TextAGCskipFrame(static_cast<const char*>(CStringA(strTemp)));

    ElementAGCskipFrame.InsertEndChild(TextAGCskipFrame);
    ElementAGC.InsertEndChild(ElementAGCskipFrame);

    ElementCamera.InsertEndChild(ElementAGC);

    // AES

    TiXmlElement ElementAES("AES");

    strTemp.Format(L"%d",m_CameraOptions.bAES);
    TiXmlElement ElementAESEnable("Enable");
    TiXmlText TextAES(static_cast<const char*>(CStringA(strTemp)));

    ElementAESEnable.InsertEndChild(TextAES);
    ElementAES.InsertEndChild(ElementAESEnable);

    strTemp.Format(L"%d",m_CameraOptions.nAESskipFrame);
    TiXmlElement ElementAESskipFrame("FrameSkip");
    TiXmlText TextAESskipFrame(static_cast<const char*>(CStringA(strTemp)));

    ElementAESskipFrame.InsertEndChild(TextAESskipFrame);
    ElementAES.InsertEndChild(ElementAESskipFrame);

    ElementCamera.InsertEndChild(ElementAES);

    // vertical mirror
    strTemp.Format(L"%d",m_CameraOptions.bMirrorVer);
    TiXmlElement ElementMirrorVer("MirrorVer");
    TiXmlText TextMirrorVer(static_cast<const char*>(CStringA(strTemp)));

    ElementMirrorVer.InsertEndChild(TextMirrorVer);
    ElementCamera.InsertEndChild(ElementMirrorVer);

    // horizontal mirror
    strTemp.Format(L"%d",m_CameraOptions.bMirrorHor);
    TiXmlElement ElementMirrorHor("MirrorHor");
    TiXmlText TextMirrorHor(static_cast<const char*>(CStringA(strTemp)));

    ElementMirrorHor.InsertEndChild(TextMirrorHor);
    ElementCamera.InsertEndChild(ElementMirrorHor);

    //contrast
    strTemp.Format(L"%d",m_CameraOptions.nContrast);
    TiXmlElement ElementContrast("Contrast");
    TiXmlText TextContrast(static_cast<const char*>(CStringA(strTemp)));

    ElementContrast.InsertEndChild(TextContrast);
    ElementCamera.InsertEndChild(ElementContrast);

    //brightness
    strTemp.Format(L"%d",m_CameraOptions.nLum);
    TiXmlElement ElementLum("Lum");
    TiXmlText TextLum(static_cast<const char*>(CStringA(strTemp)));

    ElementLum.InsertEndChild(TextLum);
    ElementCamera.InsertEndChild(ElementLum);

    // colormode
    strTemp.Format(L"%d", m_CameraOptions.nColorMode);
    TiXmlElement ElementColorMode("ColorMode");
    TiXmlText TextColorMode(static_cast<const char*>(CStringA(strTemp)));

    ElementColorMode.InsertEndChild(TextColorMode);
    ElementCamera.InsertEndChild(ElementColorMode);

    // add the program and camera elements to the root element
    ElementRoot.InsertEndChild(ElementProgram);
    ElementRoot.InsertEndChild(ElementCamera);

    doc.InsertEndChild(ElementRoot);
    doc.SaveFile(std::string("config.xml"));

    return true;
}


void uEyeCrosshairDlg::ExecConfig ()
{
    // exec the camera settings
    is_SetRopEffect (m_hCam,IS_SET_ROP_MIRROR_LEFTRIGHT, m_CameraOptions.bMirrorHor ? 1 : 0, 0);
    is_SetRopEffect (m_hCam,IS_SET_ROP_MIRROR_UPDOWN,    m_CameraOptions.bMirrorVer ? 1 : 0, 0);

    if (m_ProgramOptions.bStartFullscreen)
    {
        ShowWindow (SW_SHOWMAXIMIZED);
    }

    is_SetHardwareGain(m_hCam, m_CameraOptions.nContrast, 0, 0, 0);

    if (m_CameraOptions.bAES)
    {
        // set skip frame
        double dbl = (double) m_CameraOptions.nAESskipFrame;

        is_SetAutoParameter (m_hCam, IS_SET_AUTO_SKIPFRAMES, &dbl, 0);

        // enable auto shutter
        double dEnable = 1;
        is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, 0);
    }
    else
    {
        double dExposure = 0.0;
        dExposure = m_CameraOptions.nLum;

        is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE,(void*)&dExposure, sizeof(dExposure));
    }

    if (m_CameraOptions.bAGC)
    {
        // set skip frame
        double dbl = (double) m_CameraOptions.nAGCskipFrame;

        is_SetAutoParameter (m_hCam,IS_SET_AUTO_SKIPFRAMES, &dbl, 0);

        dbl = 1.0;
        is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_GAIN, &dbl, 0);
    }
    else
    {
        double dbl = 0.0;
        is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_GAIN, &dbl, 0);
    }

    double dbl = (m_CameraOptions.bAWB && (m_CameraOptions.nColorMode == 1)) ? 1.0 : 0.0;
    is_SetAutoParameter (m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dbl, 0);

    if (m_CameraOptions.nColorMode == 1)
    {
        double dbl = (double) IS_CCOR_ENABLE_HQ_ENHANCED;
        is_SetColorCorrection (m_hCam, 1, &dbl);
    }
    else
    {
        double dbl = (double) IS_CCOR_DISABLE;
        is_SetColorCorrection (m_hCam, 0, &dbl);
    }

    UINT nEdgeEnhancement = IS_EDGE_EN_WEAK;
    is_EdgeEnhancement(m_hCam, IS_EDGE_ENHANCEMENT_CMD_SET,
        (void*)&nEdgeEnhancement, sizeof(nEdgeEnhancement));
}


void uEyeCrosshairDlg::OnContextMenu (CWnd* pWnd, CPoint point)
{
    //shows a context menu to open the settings dialog
    CRect rect;
    GetWindowRect (rect);

    CMenu menu;
    menu.LoadMenu (IDR_MENU_MAIN);

    if ((menu != 0) && (menu.GetSubMenu (0) != NULL))
    {
        menu.GetSubMenu (0)->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
    }
}


void uEyeCrosshairDlg::OnProgramConfiguration ()
{
    // shows the settings dialog
    OpenConfigurationDlg ();
}


void uEyeCrosshairDlg::OpenConfigurationDlg ()
{
    // Show the settings dialog
    CCameraConfigDlg dlgCameraConfig;
    dlgCameraConfig.SetParent (this, &m_CameraOptions);

    CProgramConfigDlg dlgProgramConfig;
    dlgProgramConfig.SetParent (this, &m_ProgramOptions);

    dlgCameraConfig.m_psp.dwFlags &= ~PSP_HASHELP;
    dlgProgramConfig.m_psp.dwFlags &= ~PSP_HASHELP;

    CPropertySheet dlgSheet (L"Settings");
    dlgSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
    dlgSheet.m_psh.dwFlags &= ~(PSH_HASHELP); 

    dlgSheet.AddPage(&dlgCameraConfig);
    dlgSheet.AddPage(&dlgProgramConfig);

    CameraOptions OldCameraOptions;
    ProgramOptions OldProgramOptions;

    OldCameraOptions = m_CameraOptions;
    OldProgramOptions = m_ProgramOptions;

    if (dlgSheet.DoModal() == IDOK)
    {
        // we will save onClose
    }
    else
    {
        // restore the old settings
        m_CameraOptions = OldCameraOptions;
        m_ProgramOptions = OldProgramOptions;

        ExecConfig  ();
        DrawObjects ();
    }
}


void uEyeCrosshairDlg::GetMaxImageSize(UINT *pnSizeX, UINT *pnSizeY)
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


INT uEyeCrosshairDlg::InitCamera (HIDS *hCam, HWND hWnd)
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