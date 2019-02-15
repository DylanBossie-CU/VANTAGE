//==========================================================================//
//                                                                          //
//  Copyright (C) 2012 - 2018                                               //
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

#pragma once

#include "Camera.h"
#include "afxcmn.h"

// CImageConfiguration-Dialogfeld

class CImageConfiguration : public CPropertyPage
{
	DECLARE_DYNAMIC(CImageConfiguration)

public:
	CImageConfiguration();
	virtual ~CImageConfiguration();

	// Dialogfelddaten
	enum { IDD = IDD_DIALOG_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	void	UpdateControls();
	void	InitControls();
	void	UpdateGainControls();
	void	UpdateGammaControls();

	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	// handle to camera
	Camera *m_pCam;

	SENSORINFO m_SensorInfo;

	// slider
	CSliderCtrl m_sliderGain;
	CSliderCtrl m_sliderBlackLevel;
	CSliderCtrl m_sliderRed;
	CSliderCtrl m_sliderGreen;
	CSliderCtrl m_sliderBlue;
	CSliderCtrl m_sliderGamma;

	bool m_bBlackLevel; 

public:
	void SetCameraHandle(Camera *pCam);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnBnClickedGainboost();
	afx_msg void OnBnClickedCheckGamma();

	afx_msg void OnDeltaposBlacklevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposGain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposRedgain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposGreengain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposBluegain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposGamma(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnEnKillfocusEditMaster();
	afx_msg void OnEnKillfocusEditBlOffset();
	afx_msg void OnEnKillfocusEditRed();
	afx_msg void OnEnKillfocusEditGreen();
	afx_msg void OnEnKillfocusEditBlue();
	afx_msg void OnEnKillfocusEditGamma();
};
