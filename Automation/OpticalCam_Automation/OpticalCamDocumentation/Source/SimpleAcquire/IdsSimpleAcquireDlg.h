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
//  image processing boards and software.									//
//                                                                          //
//==========================================================================//

#if !defined(AFX_IDSSIMPLEACQUIREDLG_H__478B93A8_52DA_4AEA_A524_E79E9C2B6601__INCLUDED_)
#define AFX_IDSSIMPLEACQUIREDLG_H__478B93A8_52DA_4AEA_A524_E79E9C2B6601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"


/////////////////////////////////////////////////////////////////////////////
// CIdsSimpleAcquireDlg dialog
class CIdsSimpleAcquireDlg : public CDialog
{
// Construction
public:
	CIdsSimpleAcquireDlg(CWnd* pParent = NULL);	// standard constructor

    enum { IDD = IDD_IDSSIMPLEACQUIRE_DIALOG };

private:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
	HICON	m_hIcon;

	// uEye varibles
	HIDS	m_hCam;			// handle to camera
	HWND	m_hWndDisplay;	// handle to diplay window
	INT		m_Ret;			// return value of uEye SDK functions
	INT		m_nColorMode;	// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;// number of bits needed store one pixel
	INT		m_nSizeX;		// width of video 
	INT		m_nSizeY;		// height of video
	INT		m_lMemoryId;	// grabber memory - buffer ID
	char*	m_pcImageMemory;// grabber memory - pointer to buffer
    INT     m_nRenderMode;  // render  mode
    SENSORINFO m_sInfo;	    // sensor information struct

    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();
    void LoadParameters();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonAcquire();
	afx_msg void OnBnClickedButtonLoadParameter();
    afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
    
};

#endif // !defined(AFX_IDSSIMPLEACQUIREDLG_H__478B93A8_52DA_4AEA_A524_E79E9C2B6601__INCLUDED_)
