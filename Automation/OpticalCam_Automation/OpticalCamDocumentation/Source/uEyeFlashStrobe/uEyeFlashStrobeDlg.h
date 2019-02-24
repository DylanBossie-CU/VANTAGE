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

// uEyeFlashStrobeDlg.h : header file
//

#if !defined(AFX_UEYEFLASHSTROBEDLG_H__F526E4DB_7A88_459A_AD53_DBF468225BD6__INCLUDED_)
#define AFX_UEYEFLASHSTROBEDLG_H__F526E4DB_7A88_459A_AD53_DBF468225BD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "uEye.h"


/////////////////////////////////////////////////////////////////////////////
// CUEyeFlashStrobeDlg dialog

class CUEyeFlashStrobeDlg : public CDialog
{
// Construction
public:
	CUEyeFlashStrobeDlg(CWnd* pParent = NULL);	// standard constructor


	// camera variables
	HIDS	m_hCam;				// handle to camera
	HWND	m_hWndDisplay;		// handle to diplay window
	INT		m_Ret;			    // return value of uEye SDK functions
	INT		m_nColorMode;	    // Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;	// number of bits needed store one pixel
	INT		m_nSizeX;		    // width of video 
	INT		m_nSizeY;		    // height of video
	SENSORINFO m_SensorInfo;	// Info struct

	// memory buffers
	INT		m_lMemoryId;	    // camera memory - buffer ID
	char*	m_pcImageMemory;    // camera memory - pointer to buffer

	// capture varaibles
	DWORD m_nTimeout;   // timeout for image acquisition
	bool  m_ContAcq;    // countinuous image acquistion is enabled
	bool  m_RunAcq;     // the image acquisition is running
	int   m_nCntFail;   // count the transfer failed messages

	// frame event
	HANDLE m_hEv;
	HANDLE m_hThread;
	DWORD m_dwThrdParam;
	DWORD m_dwThreadID;
	bool  m_boRunThread;

	 // trigger
	INT m_nTriggerMode;

	// camera functions
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();
    void ExitCamera();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

	// event functions
	void ThreadProc();


// Dialog Data
	//{{AFX_DATA(CUEyeFlashStrobeDlg)
	enum { IDD = IDD_UEYEFLASHSTROBE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUEyeFlashStrobeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUEyeFlashStrobeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonCapture();
	afx_msg void OnButtonCaptureStop();
	afx_msg void OnCheckCont();
	afx_msg void OnRadioFlashOff();
	afx_msg void OnRadioFlashActLo();
	afx_msg void OnRadioFlashActHi();
	afx_msg void OnRadioFlashDoutLo();
	afx_msg void OnRadioFlashDoutHi();
	//}}AFX_MSG
	afx_msg void OnBnClickedButtonGlobalFlash();
	afx_msg void OnBnClickedButtonLoadParameter();
	afx_msg LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UEYEFLASHSTROBEDLG_H__F526E4DB_7A88_459A_AD53_DBF468225BD6__INCLUDED_)
