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

#if !defined(AFX_UEYEDIRECTRENDERERDLG_H__FDFC861E_720B_4FBD_A0C1_73B13899E92B__INCLUDED_)
#define AFX_UEYEDIRECTRENDERERDLG_H__FDFC861E_720B_4FBD_A0C1_73B13899E92B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"


class CUEyeDirectRendererDlg : public CDialog
{
public:

    CUEyeDirectRendererDlg (CWnd* pParent = NULL);

private:
	
    BOOL m_bInit;

    CRect m_rcClient, m_rcDialog;
    CRect m_rcImageWidth, 
          m_rcImageHeight,
          m_rcOverlayWidth,
          m_rcOverlayHeight,
          m_rcCheckMono,
          m_rcCheckShowOverlay,
          m_rcCheckAllowScaling,
          m_rcCheckAllowImageScaling,
          m_rcCheckShowMovingOverlay,
          m_rcCheckSemiTransparent,
          m_rcCheckOverlayFromFile,
          m_rcLogo,
          m_rcRadioVsyncOff,
          m_rcRadioVsyncAuto,
          m_rcRadioVsyncUser,
          m_rcRadioOpenGL,
          m_rcRadioDirect3D,
          m_rcStaticSyncPos,
          m_rcEditSyncPos,
          m_rcSpinSyncPos,
          m_rcKeyColor,
          m_rcKeyColorText,
          m_rcBtnLive,
          m_rcBtnStopLive,
          m_rcBtnLoadParameter,
          m_rcBtnExit;

    HICON m_hIcon;

    HDC m_hDc;

	// Camera variables
	HIDS	m_hCam;			    
	HWND	m_hWndDisplay;		
	INT		m_nColorMode;	    
	INT	    m_nBitsPerPixel;    
	INT	    m_nSizeX;		
	INT	    m_nSizeY;		
	INT		m_nDisplayMode;		
    INT     m_nOverlaySizeX;    
    INT     m_nOverlaySizeY;  
    INT     m_nOverlayOffsetX;  
    INT     m_nOverlayOffsetY; 
	
    // Key color
    COLORREF m_rgbKeyColor;      

    // Check boxes
    BOOL    m_bShowOverlay;
    BOOL    m_bShowMovingOverlay;
    BOOL    m_bLoadOverlayFromFile;
    BOOL    m_bShowSemiTransparent;
    BOOL    m_bMono;
    BOOL    m_bScaleImage;
    BOOL    m_bScaleImageAndOverlay;

    // Vsync
    INT     m_nVsync;
    UINT    m_SyncPosition;
    UINT    m_nUserSyncMin;
    UINT    m_nUserSyncMax;
    
    // DirectRender Mode
    INT     m_nDirectRenderMode;

	// Timer
	UINT_PTR	m_nTimer;
  
	// Camera functions
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera ();

    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

    void UpdateImageSize ();
    void UpdateOverlaySize ();
    void ClearDisplayBackground ();

    void SaveControlPosition(INT nControlId, CRect *pRectControl);
    void MoveControlPosition(INT nControlId, CRect rectControl, INT cy);

	enum { IDD = IDD_UEYEDIRECTRENDERER_DIALOG };

	virtual void DoDataExchange (CDataExchange* pDX);
	virtual BOOL OnInitDialog ();
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam);
	afx_msg void OnPaint ();
	afx_msg HCURSOR OnQueryDragIcon ();
	afx_msg void OnButtonLive ();
	afx_msg void OnButtonStopLive ();
	afx_msg void OnButtonExit ();
	afx_msg void OnCheckAllowScaling ();
	afx_msg void OnTimer (UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor (CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonLoadParameter ();
	afx_msg LRESULT OnUEyeMessage (WPARAM wParam, LPARAM lParam);
    afx_msg void OnStnClickedKeyColor ();
    afx_msg void OnClose ();
    afx_msg void OnBnClickedCheckShowOverlay ();
    afx_msg void OnBnClickedCheckShowMovingOverlay ();
    afx_msg void OnBnClickedCheckSemiTransparent ();
    afx_msg void OnBnClickedRadioVsync ();
    afx_msg void OnDeltaposSpinSyncPosition (NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg BOOL PreTranslateMessage (MSG *pMsg);
    afx_msg void OnBnClickedCheckMono ();
    afx_msg void OnBnClickedCheckOverlayFromFile ();
    afx_msg void OnBnClickedCheckAllowImageScaling();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnBnClickedRadioRenderMode();

    DECLARE_MESSAGE_MAP ()
};

#endif // !defined(AFX_UEYEDIRECTRENDERERDLG_H__FDFC861E_720B_4FBD_A0C1_73B13899E92B__INCLUDED_)
