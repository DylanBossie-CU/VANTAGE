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

#if !defined(AFX_UEYEMULTIPLECAMERASCANDLG_H__032C633F_EDBA_4BE1_B892_7459C86840D5__INCLUDED_)
#define AFX_UEYEMULTIPLECAMERASCANDLG_H__032C633F_EDBA_4BE1_B892_7459C86840D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"


/////////////////////////////////////////////////////////////////////////////
// CUEyeMultipleCameraScanDlg dialog
class CUEyeMultipleCameraScanDlg : public CDialog
{
	// some defines
	#define MAX_CAM 8					// maximum number of cameras

	// camera variables
	HIDS	m_hCam[MAX_CAM];	        // handle to camera
	HWND	m_hWndDisplay[MAX_CAM];	    // handle to diplay window
	INT		m_nColorMode[MAX_CAM];		// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel[MAX_CAM];	// number of bits needed store one pixel
	INT		m_nSizeX[MAX_CAM];		    // width of video 
	INT		m_nSizeY[MAX_CAM];		    // height of video
	INT		m_Ret;			            // return value of uEye SDK functions
	int		m_CamCount;					// number of active cameras
	bool	m_bErrRep;					// enable/disable the error report

	// memory buffers
	INT		m_lMemoryId[MAX_CAM];			// camera memory - buffer ID
	char*	m_pcImageMemory[MAX_CAM];		// camera memory - pointer to buffer

    // camera functions
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();
	void DrawRectangle( INT nCamera);
    void GetMaxImageSize(HIDS hCam, INT *pnSizeX, INT *pnSizeY);


public:
	CUEyeMultipleCameraScanDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_UEYEMULTIPLECAMERASCAN_DIALOG };

private:

    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	HICON m_hIcon;
	HBITMAP bitmap;
	HPEN hPen;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonScan();
	afx_msg void OnCheckErrRep();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_UEYEMULTIPLECAMERASCANDLG_H__032C633F_EDBA_4BE1_B892_7459C86840D5__INCLUDED_)
