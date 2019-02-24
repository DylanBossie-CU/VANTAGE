//==========================================================================//
//																			//
//	Copyright (C) 2009 - 2018												//
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
//																			//
//==========================================================================//

#pragma once

#include "uEyeCrosshairDlg.h"
#include "afxcmn.h"

class CCameraConfigDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CCameraConfigDlg)

private:
	CDialog *dlg;
	CameraOptions *m_Options;

	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:

    enum { IDD = IDD_CAMERA_CONFIG_DLG };

	void SetParent(CDialog *parent, CameraOptions *pOptions);

	CSliderCtrl m_ctrlContrast;
	CSliderCtrl m_ctrlLum;
    CSliderCtrl m_ctrlExposure;
    CSliderCtrl m_ctrlGain;

    CCameraConfigDlg();
	virtual ~CCameraConfigDlg();

	afx_msg void OnNMCustomdrawSldContrast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSldLum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChkAwb();
	afx_msg void OnBnClickedChkAes();
	afx_msg void OnBnClickedChkAgc();
	afx_msg void OnBnClickedRdColor();
	afx_msg void OnBnClickedRdBlackwhite();
	afx_msg void OnBnClickedChkVertical();
	afx_msg void OnBnClickedChkHorizontal();
    afx_msg void OnNMCustomdrawSldExposure(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMCustomdrawSldGain(NMHDR *pNMHDR, LRESULT *pResult);
};
