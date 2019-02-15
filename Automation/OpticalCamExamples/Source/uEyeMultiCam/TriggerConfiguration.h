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

// CTriggerConfiguration-Dialogfeld

#define MAX_TRIGGER_TIMEOUT   (60 * 60 * 1000)

class CTriggerConfiguration : public CPropertyPage
{
	DECLARE_DYNAMIC(CTriggerConfiguration)

public:

	void SetCameraHandle(Camera *pCam);

	CTriggerConfiguration();
	virtual ~CTriggerConfiguration();

	// Dialogfelddaten
	enum { IDD = IDD_DIALOG_TRIGGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	void	UpdateControls();
	void	InitControls();
	void	UpdateTriggerTimeout();

	BOOL PreTranslateMessage(MSG* pMsg);

private:
	// handle to camera
	Camera *m_pCam;

	// slider
	CSliderCtrl m_sliderTriggerDelay;
	CSliderCtrl m_sliderTriggerTimeout;

	// variable
	UINT	m_nTimeout;
	int		m_nDelay;

public:
	afx_msg void OnBnClickedButtonDefault();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnDeltaposSpinTriggerDelay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTriggerTimeout(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedRadioSoftware();
	afx_msg void OnBnClickedRadioFalling();
	afx_msg void OnBnClickedRadioRising();

	afx_msg void OnEnKillfocusEditDelay();
	afx_msg void OnEnKillfocusEditTimeout();

	afx_msg void OnBnClickedCheckTriggerDelay();
};
