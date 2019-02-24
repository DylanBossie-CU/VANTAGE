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


// uEyeInvertImageDlg.h : header file
//

#pragma once

#include "uEye.h"

// CuEyeInvertImageDlg dialog
class CuEyeInvertImageDlg : public CDialog
{

public:
	CuEyeInvertImageDlg(CWnd* pParent = NULL);	// standard constructor

	// camera variables
	HIDS	m_hCam;					// handle to camera
	HWND	m_hWndDispOrg;			// handle to display window of original camera image
	HWND	m_hWndDispInv;			// handle to display window of inverted image
	INT		m_nColorMode;			// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;		// number of bits needed store one pixel
	INT		m_nSizeX;				// width of video 
	INT		m_nSizeY;				// height of video

	// memory and sequence buffers
	INT		m_lMemIdOrg;	// camera memory - buffer ID
	char*	m_pcMemOrg;		// camera memory - pointer to buffer
	INT		m_lMemIdInv;	// inverted image - buffer ID
	char*	m_pcMemInv;		// inverted image - pointer to buffer

	// camera functions
	bool CamOpen();
	void GrabAndInvert();
	void InvertImageBuffer();
	void DisplayImageBuffers();

    afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonSnap();
    afx_msg void OnClose();

    enum { IDD = IDD_UEYEINVERTIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:

	HICON m_hIcon;

    INT InitCamera (HIDS *hCam, HWND hWnd);
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

	virtual BOOL OnInitDialog();
    virtual void OnCancel();    
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()	
};
