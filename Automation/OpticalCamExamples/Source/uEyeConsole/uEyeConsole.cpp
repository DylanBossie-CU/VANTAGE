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

// uEyeConsole.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung
//

#include "stdafx.h"
#include <sstream>

BOOL CreateDisplayWindow();
BOOL DeleteDisplayWindow();
BOOL ToggleLive();
BOOL ToggleDisplay();
INT InitCamera (HIDS *hCam, HWND hWnd);
BOOL OpenCamera();
BOOL CloseCamera();
void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);
int InitDisplayMode();
INT AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel);
INT FreeImageMems(void);
INT InitSequence(void);
INT ClearSequence(void);
INT GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId);
BOOL ProcessuEyeMessages(UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI ConsoleDispWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#define MSG_QUIT_THREAD         (WM_USER + 123)

#define CONSOLEDISPCLASS         "ConsoleDispClass"
#define CONSOLEDISPWINDOW        "ConsoleDispWindow"

enum _disp_mode
{
    e_disp_mode_bitmap = 0,
    e_disp_mode_directdraw
};

struct Memory
{
    char* pcImageMemory;
    INT lMemoryId;
    INT lSequenceId;
};

BOOL            m_boLive;
BOOL            m_boDisplay;
HCAM            m_hCam = NULL;
CAMINFO         m_ci;
SENSORINFO      m_si;
int             m_nSizeX, m_nSizeY;
int             m_nDispSizeX, m_nDispSizeY;
Memory          m_arMemory[3];
int             m_nColorMode;
int             m_nBitsPerPixel;
int             m_nDispModeSel;
HWND            m_hwndDisp;
ATOM            m_atom;


int _tmain(int argc, _TCHAR* argv[])
{
    int key = 0;
    int boRet = 0;
    MSG msg;
    char str[128];
    RECT rc;

    m_nDispSizeX = 640;
    m_nDispSizeY = 480;
    m_boLive = FALSE;
    m_boDisplay = FALSE;
    m_nDispModeSel = e_disp_mode_bitmap;

    memset(m_arMemory, 0, sizeof(m_arMemory));

    _cputs("\nKey commands:\n");
    _cputs("\n\tL = live on/off\n");
    _cputs("\n\tD = display on/off\n");
    _cputs("\n\tQ = quit\n\n");

    //creates the display window
    CreateDisplayWindow();
    // open a camera
    if(OpenCamera())
    {
        // print some camera data
        _cputs("Active camera:\n");
        sprintf(str, "\tModel:\t%s\n", m_si.strSensorName);
        _cputs(str);
        sprintf(str, "\tSerNo:\t%s\n", m_ci.SerNo);
        _cputs(str);
        sprintf(str, "\tID:\t%d\n", m_ci.Select);
        _cputs(str);

        // adjust the window size, so the client area is a fraction of the
        // camera image size and max 640 by 480
        SetWindowPos(m_hwndDisp, NULL, 0, 0, m_nDispSizeX, m_nDispSizeY, SWP_NOMOVE | SWP_NOZORDER);
        GetClientRect(m_hwndDisp, &rc);
        SetWindowPos(m_hwndDisp, NULL, 0, 0,
            2 * m_nDispSizeX - (rc.right - rc.left),
            2 * m_nDispSizeY - (rc.bottom - rc.top),
            SWP_NOMOVE | SWP_NOZORDER);
        ShowWindow(m_hwndDisp, SW_SHOW);

        // enable live
        ToggleLive();
        // enable drawing
        ToggleDisplay();
    }
    else
        _cputs("\nERROR:  can't open camera\n");

    // Handle window and input messages
    do
    {
        // Reduces high cpu load
        Sleep(1);

        // get all messages
        boRet = PeekMessage(&msg, NULL /*m_hwndDisp*/, 0, 0, PM_REMOVE);
        if(boRet == 0) // WM_QUIT
        {
        }
        else if(boRet == -1) // error
        {
        }
        else
        {
            // forward the messages to the window procedure
            DispatchMessage(&msg);
        }

        // Get input messages
        if(_kbhit())
        {
            key = _getch();

            switch(key)
            {
            case 'Q':
            case 'q':
                // exit the camera
                CloseCamera();
                key = 'q';
                break;

            case 'L':
            case 'l':
                // turn live mode on or off
                ToggleLive();
                break;

            case 'D':
            case 'd':
                // turn bitmap display mode on or off
                ToggleDisplay();
                break;
            }   // end switch(key)
        }
    }while(key != 'q');

    DeleteDisplayWindow();

    return 0;
}


BOOL OpenCamera()
{
    int nRet;

    // init camera
    m_hCam = (HCAM)0; // open next camera
    nRet = InitCamera (&m_hCam, m_hwndDisp); // init camera
    if( nRet == IS_SUCCESS )
    {
        is_GetCameraInfo( m_hCam, &m_ci);

        // retrieve original image size
        is_GetSensorInfo( m_hCam, &m_si);

        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        // calc the display size so that it is a integer fraction
        // of the sensor size and max 640 by 480
        int n = 1;
        do
        {
            m_nDispSizeX = m_nSizeX / n;
            n++;
        }
        while(m_nDispSizeX > 640);

        m_nDispSizeY = m_nSizeY / (n - 1);

        // setup the bitmap or directdraw display mode
        nRet = InitDisplayMode();

        if(nRet == IS_SUCCESS)
        {
            // Enable Messages
            nRet = is_EnableMessage( m_hCam, IS_DEVICE_REMOVED, m_hwndDisp );
            nRet = is_EnableMessage( m_hCam, IS_DEVICE_RECONNECTED, m_hwndDisp );
            nRet = is_EnableMessage( m_hCam, IS_FRAME, m_hwndDisp );
        }   // end if( nRet == IS_SUCCESS )
    }   // end if( nRet == IS_SUCCESS )

    return (nRet == IS_SUCCESS);
}


BOOL CloseCamera()
{
    BOOL boRet = FALSE;

    if( m_hCam != 0 )
    {
        is_EnableMessage( m_hCam, IS_FRAME, NULL );
        is_StopLiveVideo( m_hCam, IS_WAIT );

        ClearSequence();
        FreeImageMems();

        is_ExitCamera( m_hCam );
        m_hCam = NULL;
        boRet = TRUE;
    }

    return boRet;
}

int InitDisplayMode()
{
    int nRet = IS_NO_SUCCESS;

    if (m_hCam == NULL)
    {
        return IS_NO_SUCCESS;
    }

    // if some image memory exist already then free it
    ClearSequence();
    FreeImageMems();

    switch(m_nDispModeSel)
    {
    case e_disp_mode_directdraw:
        // if initializiing the direct3d mode succeeded then
        // leave the switch tree else run through to bitmap mode
        nRet = is_SetDisplayMode( m_hCam,
            IS_SET_DM_DIRECT3D );
        if(nRet == IS_SUCCESS )
        {
            // setup the color depth to the current VGA settiqng
            nRet = is_GetColorDepth( m_hCam, &m_nBitsPerPixel, &m_nColorMode );
            break;
        }
        else
            m_nDispModeSel = e_disp_mode_bitmap;

    case e_disp_mode_bitmap:
        nRet = is_SetDisplayMode( m_hCam, IS_SET_DM_DIB);
        if(nRet == IS_SUCCESS)
        {
            if( m_si.nColorMode == IS_COLORMODE_BAYER )
            {
                // for color camera models use RGB24 mode
                m_nColorMode = IS_CM_BGR8_PACKED;
                m_nBitsPerPixel = 24;
            }
            else if( m_si.nColorMode == IS_COLORMODE_CBYCRY )
            {
                // for CBYCRY camera models use RGB32 mode
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
        }   // end if(nRet == IS_SUCCESS)
        break;
    }

    if(nRet == IS_SUCCESS)
    {
        // set the image memory only for the bitmap mode when allocated
        nRet = InitSequence();

        // set the desired color mode
        nRet = is_SetColorMode( m_hCam, m_nColorMode );
        // set the image size to capture
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        nRet = is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));
    }   // end if(nRet == IS_SUCCESS)

    return nRet;
}

INT AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel)
{
    INT nRet = IS_SUCCESS;

    for (int i = 0; i < sizeof(m_arMemory) / sizeof(Memory); i++)
    {
        nRet = is_AllocImageMem(m_hCam, nSizeX, nSizeY, nBitsPerPixel, &(m_arMemory[i].pcImageMemory), &(m_arMemory[i].lMemoryId));

        if (IS_SUCCESS != nRet)
        {
            FreeImageMems();
            break;
        }
    }

    return nRet;
}

INT FreeImageMems(void)
{
    INT nRet = IS_SUCCESS;

    for (int i = 0; i < sizeof(m_arMemory) / sizeof(Memory); i++)
    {
        if (NULL != m_arMemory[i].pcImageMemory)
        {
            nRet = is_FreeImageMem(m_hCam, m_arMemory[i].pcImageMemory, m_arMemory[i].lMemoryId);
        }

        m_arMemory[i].pcImageMemory = NULL;
        m_arMemory[i].lMemoryId = 0;
        m_arMemory[i].lSequenceId = 0;
    }

    return nRet;
}

INT InitSequence(void)
{
    INT nRet = IS_SUCCESS;

    for (int i = 0; i < sizeof(m_arMemory) / sizeof(Memory); i++)
    {
        nRet = is_AddToSequence(m_hCam, m_arMemory[i].pcImageMemory, m_arMemory[i].lMemoryId);

        if (IS_SUCCESS != nRet)
        {
            ClearSequence();
            break;
        }
    }

    return nRet;
}

INT ClearSequence(void)
{
    return is_ClearSequence(m_hCam);
}

INT GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId)
{
    INT nRet = IS_NO_SUCCESS;

    int dummy = 0;
    char *pLast = NULL;
    char *pMem = NULL;

    nRet = is_GetActSeqBuf(m_hCam, &dummy, &pMem, &pLast);

    if ((IS_SUCCESS == nRet) && (NULL != pLast))
    {
        nRet = IS_NO_SUCCESS;

        for (int i = 0; i < sizeof(m_arMemory) / sizeof(Memory); i++)
        {
            if (pLast == m_arMemory[i].pcImageMemory)
            {
                *ppLastMem = m_arMemory[i].pcImageMemory;
                lMemoryId = m_arMemory[i].lMemoryId;
                lSequenceId = m_arMemory[i].lSequenceId;
                nRet = IS_SUCCESS;

                break;
            }
        }
    }

    return nRet;
}

BOOL CreateDisplayWindow()
{
    WNDCLASSEX wcx;
    HMODULE hInstance = GetModuleHandle(NULL);
    if (GetClassInfoEx (hInstance, _T(CONSOLEDISPCLASS), &wcx) == 0)
    {
        // Fill in the window class structure with parameters that describe the main window.
        wcx.cbSize        = sizeof(wcx);          // size of structure
        wcx.style         = CS_HREDRAW | CS_NOCLOSE | CS_SAVEBITS | CS_VREDRAW | WS_OVERLAPPED;
        wcx.lpfnWndProc   = ConsoleDispWndProc;   // points to window procedure
        wcx.cbClsExtra    = 0;                    // no extra class memory
        wcx.cbWndExtra    = 0;                    // no extra window memory
        wcx.hInstance     = hInstance;            // handle to instance
        wcx.hIcon         = NULL;                 // no icon
        wcx.hCursor       = NULL;                 // no cursor
        wcx.lpszMenuName  = NULL;                 // name of menu resource
        wcx.lpszClassName = _T(CONSOLEDISPCLASS); // name of window class
        wcx.hIconSm       = NULL;                 // small class icon
        wcx.hbrBackground = NULL;

        // Register the window class.
        m_atom = RegisterClassEx(&wcx);

        if(m_atom != NULL)
        {
            // create our display window
            m_hwndDisp = CreateWindow(_T(CONSOLEDISPCLASS) , // name of window class
                L"Camera display window", // title-bar string
                WS_OVERLAPPEDWINDOW,      // top-level window
                CW_USEDEFAULT,            // default horizontal position
                CW_USEDEFAULT,            // default vertical position
                m_nDispSizeX,             // default width
                m_nDispSizeY,             // default height
                (HWND) NULL,              // no owner window
                (HMENU) NULL,             // use class menu
                hInstance,                // handle to application instance
                (LPVOID) NULL);           // no window-creation data

            if (!m_hwndDisp)
            {
                DWORD a = GetLastError();
                LPVOID lpMsgBuf;
                FormatMessage( 	FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, a,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR) &lpMsgBuf,	0, NULL );
                // Free the buffer.
                LocalFree( lpMsgBuf );
                ::UnregisterClass (_T(CONSOLEDISPCLASS), hInstance);
                m_atom = NULL;
            }
        }
    }

    return (m_atom != NULL);
}

BOOL DeleteDisplayWindow()
{
    BOOL boRet = FALSE;

    if(m_atom != NULL)
    {
        HMODULE hInstance = GetModuleHandle(NULL);

        SendMessage(m_hwndDisp, WM_CLOSE, 0, 0);
        SendMessage(m_hwndDisp, WM_DESTROY, 0, 0);

        UnregisterClass (_T(CONSOLEDISPCLASS), hInstance);

        m_atom = NULL;
        m_hwndDisp = NULL;
    }

    return boRet;
}

BOOL ToggleLive()
{
    BOOL boRet = FALSE;

    if(m_hCam != NULL)
    {
        m_boLive ^= 1;

        if(m_boLive)
            is_CaptureVideo(m_hCam, IS_DONT_WAIT);
        else
            is_StopLiveVideo(m_hCam, IS_WAIT);
    }

    return boRet;
}

BOOL ToggleDisplay()
{
    BOOL boRet = FALSE;

    m_boDisplay ^= 1;

    ShowWindow(m_hwndDisp, m_boDisplay ? SW_SHOW : SW_HIDE);

    return boRet;
}

LRESULT WINAPI ConsoleDispWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rc;

    switch ( msg )
    {
    case IS_UEYE_MESSAGE:
        ProcessuEyeMessages(msg, wParam, lParam);
        return 0;

    case WM_CREATE: 
        // Initialize the window.
        return 0; 

    case WM_ERASEBKGND:
        // we need to refresh the background only if live or
        // display mode is off
        if(!m_boLive || !m_boDisplay)
        {
            if(GetUpdateRect(hwnd, &rc, FALSE))
            {
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
                EndPaint(hwnd, &ps);
            }
        }
        return 1;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


BOOL ProcessuEyeMessages(UINT msg, WPARAM wParam, LPARAM lParam)
{
    BOOL boRet = FALSE;

    switch(msg)
    {
    case IS_UEYE_MESSAGE:
        switch(wParam)
        {
        case IS_FRAME:
            // if in bitmap mode and the display flag is enabled then
            // draw the actual image buffer
            if((m_nDispModeSel == e_disp_mode_bitmap) && m_boDisplay)
            {
                char* pLast = NULL;
                INT lMemoryId = 0;
                INT lSequenceId = 0;

                if (IS_SUCCESS == GetLastMem(&pLast, lMemoryId, lSequenceId))
                {
                    INT nRet = is_LockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, pLast);

                    if (IS_SUCCESS == nRet)
                    {
                        is_RenderBitmap(m_hCam, lMemoryId, m_hwndDisp, IS_RENDER_FIT_TO_WINDOW);
                        is_UnlockSeqBuf(m_hCam, IS_IGNORE_PARAMETER, pLast);
                    }
                }
            }
            break;

        case IS_CAPTURE_STATUS:
            Beep(4000, 10);
            break;

        case IS_DEVICE_RECONNECTED:
            Beep(1000, 200);
            break;

        case IS_MEMORY_MODE_FINISH:
            break;

        case IS_DEVICE_REMOVED:
            Beep(3000, 200);
            break;

        case IS_DEVICE_REMOVAL:
            break;

        case IS_NEW_DEVICE:
            break;

        }   // end switch(pmsg->wParam)

        // tell that this message has been processed
        boRet = TRUE;
        break;  // end case IS_UEYE_MESSAGE:
    }   // end switch(msg)

    return boRet;
}


INT InitCamera (HIDS *hCam, HWND hWnd)
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

        std::ostringstream stringStream;
        stringStream << "This camera requires a new firmware. The upload will take about ";
        stringStream << nUploadTime / 1000;
        stringStream << " seconds. Please wait ...";
        std::string MessageBoxString = stringStream.str();
        MessageBoxA (NULL, MessageBoxString.c_str(), "", MB_ICONWARNING);

        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS) (((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD);
        nRet = is_InitCamera (hCam, hWnd);
    }

    return nRet;
}


void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
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
