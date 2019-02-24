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


#if !defined(AFX_UEYEPIXELPEEKDLG_H__F63076AA_00AD_4D55_ACAF_DB1240D54C6E__INCLUDED_)
#define AFX_UEYEPIXELPEEKDLG_H__F63076AA_00AD_4D55_ACAF_DB1240D54C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "uEye.h"



class CUEyePixelPeekDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CUEyePixelPeekDlg dialog

class CUEyePixelPeekDlg : public CDialog
{
	DECLARE_DYNAMIC(CUEyePixelPeekDlg);
	friend class CUEyePixelPeekDlgAutoProxy;

// Construction
public:
	CUEyePixelPeekDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CUEyePixelPeekDlg();

	// camera variables
	HIDS	m_hCam;				// handle to camera
	HWND	m_hWndDisp;			// handle to main display window
	HWND	m_hWndDispPixPeek;	// handle to pixel peek display window
	INT		m_Ret;			    // return value of uEye SDK functions
	INT		m_nColorMode;	    // Y8/RGB16/RGB24/RGB32 etc.
	INT		m_nBitsPerPixel;	// number of bits needed store one pixel
	INT		m_nBytesPerPixel;	// number of bytes needed store one pixel
	INT		m_nSizeX;		    // width of video 
	INT		m_nSizeY;		    // height of video
	INT		m_nPitch;		    // pitch of image lines
	INT		m_nImageSize;		// size of image in bytes
	INT		m_nPPSizeX;		    // width of pixel peek video 
	INT		m_nPPSizeY;		    // height of pixel peek  video


	// memory buffers
	INT		m_lMemId;			// camera main memory - buffer ID
	char*	m_pcImgMem;			// camera main memory - pointer to buffer
	INT		m_lPPMemId;			// camera pixel peek memory - buffer ID
	char*	m_pcPPImgMem;		// camera pixel peek memory - pointer to buffer


	// camera functions
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool CamOpen();
	bool CamAllocMem();
	bool CamAcqDisp();
	bool CamDisp();
	bool CamSetColorMode();
	void CamSetColorModeGUI();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

	// pixel peek functions
	bool PixelPeekDisp();
	bool PixelPeekAllocMem();
	bool PixelPeekMemCopy(INT CenterX, INT CenterY);

	enum { IDD = IDD_UEYEPIXELPEEK_DIALOG };
	CButton	m_ctrlBottonAcquire;

private:

    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	CUEyePixelPeekDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonAcquire();
	afx_msg void OnButtonLoadParam();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRadioColorY8();
	afx_msg void OnRadioColorRgb24();
	afx_msg LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_UEYEPIXELPEEKDLG_H__F63076AA_00AD_4D55_ACAF_DB1240D54C6E__INCLUDED_)
