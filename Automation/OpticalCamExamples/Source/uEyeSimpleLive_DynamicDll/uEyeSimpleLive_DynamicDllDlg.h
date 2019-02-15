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

// uEyeSimpleLive_DynamicDllDlg.h : header file
#include "uEye.h"
#include "Dynamic_uEye_api.h"
#pragma once


class CuEyeSimpleLive_DynamicDllDlg : public CDialog
{

public:
	CuEyeSimpleLive_DynamicDllDlg(CWnd* pParent = NULL);


	enum { IDD = IDD_UEYESIMPLELIVE_DYNAMICDLL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

private:
	HICON m_hIcon;

	// uEye
	CuEyeDll	m_ueye;	// dynamic uEye wrapper class
	HIDS		m_hCam;	// camera handle
	BOOL		b_Live;	// live mode enabled (true/false)
	

	BOOL CanExit();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonLive();
	afx_msg void OnBnClickedButtonSnap();
	afx_msg void OnBnClickedButtonParameters();

	DECLARE_MESSAGE_MAP()
};
