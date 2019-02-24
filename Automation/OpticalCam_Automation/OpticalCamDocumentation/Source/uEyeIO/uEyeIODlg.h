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

#if !defined(AFX_UEYEIODLG_H__114117DC_4A75_473B_B997_F65D4B139688__INCLUDED_)
#define AFX_UEYEIODLG_H__114117DC_4A75_473B_B997_F65D4B139688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"
#include "led.h"


/////////////////////////////////////////////////////////////////////////////
// CUEyeIODlg dialog
class CUEyeIODlg : public CDialog
{
public:
	CUEyeIODlg(CWnd* pParent = NULL);	// standard constructor

	// camera variables
	HIDS	m_hCam;			// handle to camera
	UINT    m_nDeviceType;

	// timer variables
	UINT_PTR  m_nTimerInput;
	UINT_PTR  m_nTimerBlink;
	UINT  m_nElaspeInput;
	UINT  m_nElaspeBlink;

	// LED blink
	INT  m_nNumberOfBlinks;
	UINT m_nBlinkToggleState;

	// IO variables
	INT m_nDin;   
	INT m_nDout;
	INT m_nLed;

	// LEDs
	CLed LedIn;   // 0 or 1 
	CLed LedOut;  // IS_SET_FLASH_LOW or IS_SET_FLASH_HIGH
	CLed LedLed;  // 0 or 1 or toggle

	// camera functions
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();

	enum { IDD = IDD_UEYEIO_DIALOG };

private:

    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonDout();
	afx_msg void OnButtonLed();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadioLedMode();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_UEYEIODLG_H__114117DC_4A75_473B_B997_F65D4B139688__INCLUDED_)
