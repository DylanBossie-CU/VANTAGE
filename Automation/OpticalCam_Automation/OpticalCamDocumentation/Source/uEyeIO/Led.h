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

///////////////////////////////////////////////////////////////////////////////
// Led.h : header file
// Visual Source Safe: $Revision: 1 $
//
// Led static control. Will display a LED in 4 different colors and two shapes.
///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-1999 Monte Variakojis ( monte@apollocom.com )
// All rights reserved - not to be sold.
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LEDWND_H__2D837381_FFEC_11D1_A1CE_00A024D311C0__INCLUDED_)
#define AFX_LEDWND_H__2D837381_FFEC_11D1_A1CE_00A024D311C0__INCLUDED_

#if _MSC_VER >= 1000
 #pragma once
#endif // _MSC_VER >= 1000


///////////////////////////////////////////////////////////////////////////////

#define LED_SIZE      16      // Led is 16 x 16 pixels
#define BIG_LED_SIZE  24      // BigLed is 28 x 28 pixels


/////////////////////////////////////////////////////////////////////////////
// CLed window

class CLed : public CWnd//CStatic
{
  public:
    typedef enum { LED_ROUND  = 0,              // Circle starts at row 0
                   LED_SQUARE = 4 } CLedShape;  // squares start at row 4

    typedef enum { LED_COLOR_RED    = 0,             // Row 0
                   LED_COLOR_GREEN  = 1,             // Row 1
                   LED_COLOR_YELLOW = 2,             // Row 2
                   LED_COLOR_BLUE   = 3} CLedColor;  // Row 3

    typedef enum { LED_ON       = 0,              // Column 0
                   LED_OFF      = 1,              // Column 1
                   LED_DISABLED = 2 } CLedMode;   // Column 2

    typedef enum { LED_NORMAL = 0,              // normal
                   LED_BIG    = 1} CLedSize;    // big

  protected:
    CLedColor    m_eLedColor;
    CLedMode     m_eLedMode;
    CLedShape    m_eLedShape;
    CLedSize     m_eLedSize;
    bool         m_bPingEnabled;
    bool         m_bBlinkEnabled;
    int          m_nBlinkCount;
    CBitmap      m_LedBitmap;
    int          m_nLedSize;
	  CToolTipCtrl m_ctrlToolTip;
    bool         m_bUserAction;
    HBITMAP      m_hResBitmap;
    DWORD        m_dwBlinkTimeout;
    DWORD        m_dwPingTimeout;
    CString      m_strWndClassName;
    HCURSOR      m_hHandCursor;
    HCURSOR      m_hStandardCursor;

    static int m_nWndCnt;

  // Construction
  public:
    CLed ();
    
    BOOL Create (CDialog* pParentDlg, UINT uiDlgCtrl);
    BOOL Create (CDialogBar* pParentDlg, UINT uiDlgCtrl);

  // Attributes
  public:
    void UseNormalLeds      ();
    void UseBigLeds         ();
    void EnableUserAction   (bool bEnable = true);

    // Tooltip
    CString GetToolTip ();
	  CString SetToolTip (CString strNew);
    CString SetToolTip (UINT uiToolTipRes, ...);

  // Operations
  public:
    void     SetLed     (CLedColor nLedColor, CLedMode nMode, CLedShape nShape, CLedSize nSize = LED_NORMAL, bool bUpdate = true);
    void     SetLed     (CLedColor nLedColor, CLedMode nMode, bool bUpdate = true)  { SetLed (nLedColor, nMode, m_eLedShape, m_eLedSize, bUpdate); }
    void     SetLed     (CLedMode  nMode, bool bUpdate = true)                      { SetLed (m_eLedColor, nMode, m_eLedShape, m_eLedSize, bUpdate); }
    void     SetLed     (CLedShape nShape, bool bUpdate = true)                     { SetLed (m_eLedColor, m_eLedMode, nShape, m_eLedSize, bUpdate); }

    CLedMode GetLedMode ()                        { return m_eLedMode; }
    void     Ping       (DWORD dwTimeout = 1000);
    void     Blink      (DWORD dwTimeout = 1000, int nBlinkCnt = -1, bool bStop = false);
    void     BlinkOff   ()                        { Blink (0, -1, true); }

  protected:
    void Redraw        ();

    bool RegisterWindow   (UINT uiStyle = 0, HICON hIcon = NULL, HCURSOR hCursor = NULL, HBRUSH hbrBackground = NULL);
    void UnregisterWindow ()   { UnregisterClass (m_strWndClassName, AfxGetInstanceHandle ()); }

  // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLed)
	  virtual BOOL PreTranslateMessage (MSG* pMsg);
    //}}AFX_VIRTUAL

  // Implementation
  public:
    void DrawLed (CDC* pDC, int nLEDColor, int nMode, int nShape);
    virtual ~CLed ();

    BOOL Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

    // Generated message map functions
  protected:
    //{{AFX_MSG(CLed)
    afx_msg int  OnCreate      (LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint       ();
    afx_msg void OnTimer       (UINT_PTR nIDEvent);
    afx_msg BOOL OnEraseBkgnd  (CDC* pDC);
    afx_msg void OnDestroy     ();
    afx_msg void OnEnable      (BOOL bEnable);
    afx_msg void OnLButtonUp   (UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP ()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEDWND_H__2D837381_FFEC_11D1_A1CE_00A024D311C0__INCLUDED_)
