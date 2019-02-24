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


#if !defined(AFX_UEYESTEALDLG_H__8DD10CA8_DFCE_41FC_B418_90DC60C1A1FB__INCLUDED_)
#define AFX_UEYESTEALDLG_H__8DD10CA8_DFCE_41FC_B418_90DC60C1A1FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"

/////////////////////////////////////////////////////////////////////////////
// CUEyeStealDlg dialog
class CUEyeStealDlg : public CDialog
{
public:
	CUEyeStealDlg (CWnd* pParent = NULL);	// standard constructor

private:
	// camera variables
	HIDS	m_hCam;			    // handle to camera
	HWND	m_hWndLive;	        // handle to live diplay window
	HWND	m_hWndSteal;	    // handle to steal image window
	INT		m_nColorMode;	    // Y8/RGB16/RGB24/RGB32
	INT		m_nBitsPerPixel;    // number of bits needed store one pixel
    INT     m_nSizeX;           // image size
    INT     m_nSizeY;           // image size
    INT     m_nStealFormat;     // Pixel format for the steal operation 

    // steal memory buffers
    INT		m_lStealMemId;	    // camera memory - buffer ID
    char*	m_pcStealImgMem;    // camera memory - pointer to buffer
    CString m_SaveFileName;
    
    BOOL    m_bFirstTime;

    // camera functions
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera (void);
	void AllocateStealMemory (void);
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

    // message counters
    unsigned int m_nCntTransFail;
    unsigned int m_nCntFrame;
    unsigned int m_nCntSteal;

	enum { IDD = IDD_UEYESTEAL_DIALOG };

	virtual void DoDataExchange (CDataExchange* pDX);	// DDX/DDV support

    HICON m_hIcon;

	virtual BOOL OnInitDialog ();
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam);
	afx_msg void OnPaint ();
	afx_msg HCURSOR OnQueryDragIcon ();
	afx_msg void OnButtonExit ();
	afx_msg void OnButtonStartLive ();
	afx_msg void OnButtonStopLive ();
	afx_msg void OnButtonSteal ();
	afx_msg void OnCheckScaling ();
	afx_msg void OnButtonLoadParam ();
	afx_msg void OnRadioStealFormat ();
	afx_msg LRESULT OnUEyeMessage (WPARAM wParam, LPARAM lParam);
    afx_msg void OnClose ();
	DECLARE_MESSAGE_MAP ()   
};

#endif // !defined(AFX_UEYESTEALDLG_H__8DD10CA8_DFCE_41FC_B418_90DC60C1A1FB__INCLUDED_)
