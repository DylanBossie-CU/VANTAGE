//==========================================================================//
//                                                                          //
//  Copyright (C) 2004-2018                                                 //
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

///////////////////////////////////////////////////////////////////////////////
// Led.cpp : implementation file
// Visual Source Safe: $Revision: 1 $
//
// Led static control. Will display a LED in 4 different colors and two shapes.
///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-1999 Monte Variakojis ( monte@apollocom.com )
// All rights reserved - not to be sold.
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "Led.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLed

int CLed::m_nWndCnt = 0;

BEGIN_MESSAGE_MAP (CLed, CWnd)
    //{{AFX_MSG_MAP(CLed)
    ON_WM_PAINT ()
    ON_WM_TIMER ()
    ON_WM_CREATE ()
    ON_WM_ERASEBKGND ()
    ON_WM_DESTROY ()
    ON_WM_ENABLE ()
    ON_WM_LBUTTONUP ()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP ()


#define TIMER_ID_PING   1 // Timer Ping ID
#define TIMER_ID_BLINK  2 // Timer Ping ID


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// ........... CLed::CLed .....................................................
// Constructor

CLed::CLed ()
{
    RegisterWindow (0);

    m_eLedColor       = LED_COLOR_RED;
    m_eLedMode        = LED_OFF;
    m_eLedShape       = LED_ROUND;

    m_bBlinkEnabled   = false;
    m_bPingEnabled    = false;
    m_bUserAction     = false;
    m_nBlinkCount     = -1;
    m_dwBlinkTimeout  = 0;
    m_dwPingTimeout   = 0;

    m_hResBitmap      = NULL;
    m_hHandCursor     = NULL;
    m_hStandardCursor = NULL;

    UseNormalLeds ();
}


// ........... CLed::~CLed ....................................................
// Destructor

CLed::~CLed ()
{
    if (m_LedBitmap.GetSafeHandle ()!= NULL)
        m_LedBitmap.Detach ();

    if (m_hResBitmap != NULL)
        ::DeleteObject (m_hResBitmap);

    m_hResBitmap = NULL;

    if (!m_strWndClassName.IsEmpty ())
        UnregisterWindow ();
}


/////////////////////////////////////////////////////////////////////////////
// CLed message handlers

// ........... CLed::Create ...................................................
//

BOOL CLed::Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    return CWnd::Create (m_strWndClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


// ........... CLed::OnCreate .................................................
//

int CLed::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate (lpCreateStruct) == -1)
        return -1;

    return 0;
}


// ........... CLed::Create ...................................................
//

BOOL CLed::Create (CDialog* pParentDlg, UINT uiDlgCtrl)
{
    CWnd* pWnd = NULL;

    if ((pParentDlg != NULL) && ((pWnd = pParentDlg->GetDlgItem (uiDlgCtrl)) != NULL))
    {
        CRect rect;

        pWnd->GetWindowRect (rect);
        pWnd->DestroyWindow (); // pCtrl was just a placeholder;

        pParentDlg->ScreenToClient (rect);

        return Create (L"", WS_CHILD | WS_VISIBLE, rect, pParentDlg, uiDlgCtrl);
    }

    return FALSE;
}


// ........... CLed::Create ...................................................
//

BOOL CLed::Create (CDialogBar* pParentDlg, UINT uiDlgCtrl)
{
    CWnd* pWnd = NULL;

    if ((pParentDlg != NULL) && ((pWnd = pParentDlg->GetDlgItem (uiDlgCtrl)) != NULL))
    {
        CRect rect;

        pWnd->GetWindowRect (rect);
        pWnd->DestroyWindow (); // pCtrl was just a placeholder;

        pParentDlg->ScreenToClient (rect);

        return Create (L"", WS_CHILD | WS_VISIBLE, rect, pParentDlg, uiDlgCtrl);
    }

    return FALSE;
}


// ........... CLed::OnPaint ..................................................
//

void CLed::OnPaint ()
{
    CPaintDC dc (this); // device context for painting
    DrawLed (&dc, m_eLedColor, m_eLedMode, m_eLedShape);

    // Do not call CWnd::OnPaint() for painting messages
}


// ........... CLed::DrawLed ..................................................
// Description: This method will draw the LED to the specified DC.
//
// Entry:
//        CDC *pDC - DC to draw to
//
//        int iLedColor - Where color is defined by:
//          LED_COLOR_RED
//          LED_COLOR_GREEN
//          LED_COLOR_YELLOW
//          LED_COLOR_BLUE
//
//        int iMode - where mode is defined by:
//          LED_ON
//          LED_OFF
//          LED_DISABLED
//
//        int iShape - where shape is defined by:
//          LED_ROUND
//          LED_SQUARE

void CLed::DrawLed (CDC* pDC, int nLEDColor, int nMode, int nShape)
{
    CRect rect;
    GetClientRect(&rect);

    //
    // Center led within an over sized window
    //
    if(rect.Width() >= m_nLedSize && rect.Height() >= m_nLedSize)
    {
        int nWidth = rect.Width();
        int nHeight = rect.Height();
        rect.left += (nWidth - LED_SIZE)/2;
        rect.right -= (nWidth - LED_SIZE)/2;
        rect.top += (nHeight - LED_SIZE)/2;
        rect.bottom -= (nHeight - LED_SIZE)/2;
    }

    //
    // Prepare temporary DCs and bitmaps
    //
    CBitmap TransBitmap;
    TransBitmap.CreateBitmap(m_nLedSize,m_nLedSize,1,1,NULL);
    CBitmap bitmapTemp;
    CBitmap* pBitmap = &m_LedBitmap;
    CDC srcDC;
    CDC dcMask;
    CDC TempDC;
    TempDC.CreateCompatibleDC(pDC);
    srcDC.CreateCompatibleDC(pDC);
    dcMask.CreateCompatibleDC(pDC);

    CBitmap* pOldBitmap = srcDC.SelectObject(pBitmap);
    CBitmap* pOldMaskbitmap = dcMask.SelectObject(&TransBitmap);
    bitmapTemp.CreateCompatibleBitmap(pDC,m_nLedSize,m_nLedSize);

    //
    // Work with tempDC and bitmapTemp to reduce flickering
    //
    CBitmap *pOldBitmapTemp = TempDC.SelectObject(&bitmapTemp);
    TempDC.BitBlt(0, 0, m_nLedSize, m_nLedSize, pDC, rect.left, rect.top, SRCCOPY);

    //
    // Create mask
    //
    COLORREF OldBkColor = srcDC.SetBkColor(RGB(255,0,255));
    dcMask.BitBlt(0, 0, m_nLedSize, m_nLedSize,&srcDC, nMode*m_nLedSize, (nLEDColor+nShape)*m_nLedSize, SRCCOPY);
    TempDC.SetBkColor(OldBkColor);

    //
    // Using the IDB_LEDS bitmap, index into the bitmap for the appropriate
    // LED. By using the mask color (RGB(255,0,255)) a mask has been created
    // so the bitmap will appear transparent.
    //
    TempDC.BitBlt(0, 0, m_nLedSize, m_nLedSize, &srcDC, nMode*m_nLedSize, (nLEDColor+nShape)*m_nLedSize, SRCINVERT);
    TempDC.BitBlt(0, 0, m_nLedSize, m_nLedSize, &dcMask, 0, 0, SRCAND); 
    TempDC.BitBlt(0, 0, m_nLedSize, m_nLedSize, &srcDC, nMode*m_nLedSize, (nLEDColor+nShape)*m_nLedSize, SRCINVERT);

    //
    // Since the actual manipulation is done to tempDC so there is minimal
    // flicker, it is now time to draw the result to the screen.
    //
    pDC->BitBlt(rect.left, rect.top, m_nLedSize, m_nLedSize, &TempDC, 0, 0, SRCCOPY);

    //
    // House cleaning
    //
    srcDC.SelectObject(pOldBitmap);
    dcMask.SelectObject(pOldMaskbitmap);
    TempDC.SelectObject(pOldBitmapTemp);
    VERIFY(srcDC.DeleteDC());
    VERIFY(dcMask.DeleteDC());
    VERIFY(TempDC.DeleteDC());
    VERIFY(TransBitmap.DeleteObject());
    VERIFY(bitmapTemp.DeleteObject());
}


// ........... CLed::SetLed ...................................................
// Description: This method will draw and set led parameters.
//
// Entry:   int iLedColor - Where color is defined by:
//          LED_COLOR_RED
//          LED_COLOR_GREEN
//          LED_COLOR_YELLOW
//          LED_COLOR_BLUE
//
//        int iMode - where mode is defined by:
//          LED_ON
//          LED_OFF
//          LED_DISABLED
//
//        int iShape - where shape is defined by:
//          LED_ROUND
//          LED_SQUARE

void CLed::SetLed (CLedColor nLedColor, CLedMode nMode, CLedShape nShape, CLedSize nSize, bool bUpdate)
{

    if (nSize != m_eLedSize)
    {
        if (nSize == LED_BIG)
            UseBigLeds ();
        else
            UseNormalLeds ();
    }

    m_eLedColor = nLedColor;
    m_eLedMode  = nMode;
    m_eLedShape = nShape;


    if (bUpdate)
        Redraw ();
}


// ........... CLed::Redraw ...................................................
//

void CLed::Redraw ()
{
    if (IsWindow (GetSafeHwnd ()) && IsWindowVisible ())
    {
        CDC* pDC = GetDC ();

        if (pDC != NULL)
        {
            DrawLed (pDC, m_eLedColor, m_eLedMode, m_eLedShape);
            ReleaseDC (pDC);
        }
    }
}


// ........... CLed::Ping .....................................................
// Description: This method will turn the led on for dwTimeout milliseconds and
//              then turn it off.
//
// Entry:       DWORD dwTimeout - Time out in  milliseconds

void CLed::Ping (DWORD dwTimeout)
{
    if (m_bBlinkEnabled)
    {
        KillTimer (TIMER_ID_BLINK);
        m_dwBlinkTimeout = 0;
    }

    if (m_bPingEnabled)
        KillTimer (TIMER_ID_PING);

    m_bPingEnabled  = true;
    m_bBlinkEnabled = false;
    m_nBlinkCount   = -1;
    m_dwPingTimeout = dwTimeout;

    SetLed   (CLed::LED_ON);
    SetTimer (TIMER_ID_PING, dwTimeout, NULL);
}


// ........... CLed::Blink ....................................................
// Description: This method will turn the periodically led on and off
//
// Entry:       DWORD dwTimeout - Time out in  milliseconds

void CLed::Blink (DWORD dwTimeout, int nBlinkCnt, bool bStop)
{
    if (m_bBlinkEnabled && (bStop || (dwTimeout < 0)))
        KillTimer (TIMER_ID_BLINK);

    if (m_bPingEnabled)
    {
        KillTimer (TIMER_ID_PING);
        m_dwPingTimeout = 0;
    }

    m_bPingEnabled = false;

    if (dwTimeout > 0)
    {
        m_nBlinkCount = nBlinkCnt;
        m_dwBlinkTimeout = dwTimeout;

        if (bStop || !m_bBlinkEnabled)
        {
            m_bBlinkEnabled = true;

            SetLed   (CLed::LED_ON);
            SetTimer (TIMER_ID_BLINK, dwTimeout, NULL);
        }
    }
    else
    {
        m_bBlinkEnabled  = false;
        m_nBlinkCount    = -1;
        m_dwBlinkTimeout = 0;

        SetLed (CLed::LED_OFF);
    }
}


// ........... CLed::OnTimer ..................................................
//

void CLed::OnTimer (UINT_PTR nIDEvent) 
{
    switch (nIDEvent)
    {
    case TIMER_ID_PING:
        SetLed (CLed::LED_OFF);
        KillTimer (TIMER_ID_PING);
        m_bPingEnabled  = false;
        m_dwPingTimeout = 0;
        break;

    case TIMER_ID_BLINK:
        switch (m_eLedMode)
        {
        case LED_ON:
            SetLed (CLed::LED_OFF);

            if (m_nBlinkCount > 0)
            {
                m_nBlinkCount--;

                if (m_nBlinkCount == 0)
                {
                    KillTimer (TIMER_ID_BLINK);

                    m_bBlinkEnabled  = false;
                    m_nBlinkCount    = -1;
                    m_dwBlinkTimeout = 0;
                }
            }

            break;

        case LED_OFF:
            SetLed (CLed::LED_ON);
            break;
        }

        break;
    }

    CWnd::OnTimer (nIDEvent);
}


// ........... CLed::OnEraseBkgnd .............................................
//

BOOL CLed::OnEraseBkgnd (CDC* /*pDC*/)
{
    // No background rendering
    return TRUE;
}


// ........... CLed::OnEnable .................................................
//

void CLed::OnEnable (BOOL bEnable)
{
    CWnd::OnEnable (bEnable);
    Invalidate (FALSE);
}


// ........... CLed::OnDestroy ................................................
//

void CLed::OnDestroy ()
{
    if (m_bBlinkEnabled)
        KillTimer (TIMER_ID_BLINK);

    m_bBlinkEnabled  = false;
    m_nBlinkCount    = -1;
    m_dwBlinkTimeout = 0;

    if (m_bPingEnabled)
        KillTimer (TIMER_ID_PING);

    m_bPingEnabled  = false;
    m_dwPingTimeout = 0;
}


// ........... CLed::UseNormalLeds ............................................
//

void CLed::UseNormalLeds ()
{
    m_nLedSize = LED_SIZE;
    m_eLedSize = LED_NORMAL;

    if (m_LedBitmap.GetSafeHandle () != NULL)
        m_LedBitmap.Detach ();

    HBITMAP hResBmp = ::LoadBitmap (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDB_BITMAP_LEDS));

    if (hResBmp != NULL)
    {
        m_LedBitmap.Attach (hResBmp);

        if (m_hResBitmap != NULL)
            ::DeleteObject (m_hResBitmap);

        m_hResBitmap = hResBmp;
    }
}


// ........... CLed::UseBigLeds ...............................................
//

void CLed::UseBigLeds ()
{
    m_nLedSize = BIG_LED_SIZE;
    m_eLedSize = LED_BIG;

    if (m_LedBitmap.GetSafeHandle () != NULL)
        m_LedBitmap.Detach ();

    HBITMAP hResBmp = ::LoadBitmap (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDB_BITMAP_BIG_LEDS));

    if (hResBmp != NULL)
    {
        m_LedBitmap.Attach (hResBmp);

        if (m_hResBitmap != NULL)
            ::DeleteObject (m_hResBitmap);

        m_hResBitmap = hResBmp;
    }
}


// ........... CLed::GetToolTip ...............................................
//

CString CLed::GetToolTip ()
{
    CString strTool = _T ("");

    if (IsWindow (m_ctrlToolTip.GetSafeHwnd ()))
        m_ctrlToolTip.GetText (strTool, this);

    return strTool;
}


// ........... CLed::SetToolTip ...............................................
//

CString CLed::SetToolTip (CString strNew)
{
    CString strTool = _T ("");

    if (IsWindow (GetSafeHwnd ()))
    {
        if (!IsWindow (m_ctrlToolTip.GetSafeHwnd ()))
        {
            CString strTool;
            GetWindowText (strTool);

            if (m_ctrlToolTip.Create (this))
                m_ctrlToolTip.AddTool (this, strNew);
        }
        else
        {
            m_ctrlToolTip.GetText (strTool, this);
            m_ctrlToolTip.UpdateTipText (strNew , this);
        }
    }

    return strTool;
}


// ........... CLed::PreTranslateMessage ......................................
//

BOOL CLed::PreTranslateMessage (MSG* pMsg)
{
    if ((pMsg != NULL) && (pMsg->message >= WM_MOUSEFIRST) && (pMsg->message <= WM_MOUSELAST))
    {
        if (IsWindow (m_ctrlToolTip.GetSafeHwnd ()))
            m_ctrlToolTip.RelayEvent (pMsg);
    }

    return CWnd::PreTranslateMessage (pMsg);
}


// ........... CLed::EnableUserAction .........................................
//

void CLed::EnableUserAction (bool bEnable)
{
    m_bUserAction = bEnable;
    Redraw ();
}


// ........... CLed::OnLButtonUp ..............................................
//

void CLed::OnLButtonUp (UINT nFlags, CPoint point)
{
    if (m_bUserAction && IsWindow (GetSafeHwnd ()))
    {
        CWnd* pParent = GetParent    ();
        int   nDlgId  = GetDlgCtrlID ();

        if ((nDlgId > 0) && (pParent != NULL) && IsWindow (pParent->GetSafeHwnd ()))
            pParent->SendMessage (WM_COMMAND, nDlgId, 0);
    }

    CWnd::OnLButtonUp (nFlags, point);
}


// ........... CLed::RegisterWindow ...........................................
//

bool CLed::RegisterWindow (UINT uiStyle, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground)
{
    // Register your unique class name that you wish to use
    WNDCLASS wndcls;

    memset (&wndcls, 0, sizeof (WNDCLASS));   // start with NULL defaults

    wndcls.style         = uiStyle;
    wndcls.lpfnWndProc   = ::DefWindowProc;
    wndcls.hInstance     = AfxGetInstanceHandle ();
    wndcls.hIcon         = hIcon;
    wndcls.hCursor       = hCursor;
    wndcls.hbrBackground = hbrBackground;
    wndcls.lpszMenuName  = NULL;

    // Specify your own class name for using FindWindow later
    m_strWndClassName.Format (L"%d_CLed", ++m_nWndCnt);
    wndcls.lpszClassName = m_strWndClassName;

    // Register the new class and exit if it fails
    if (AfxRegisterClass (&wndcls) == TRUE)
        return true;

    return false;
}

