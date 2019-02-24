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

// CSizeConfiguration-Dialogfeld

class CSizeConfiguration : public CPropertyPage
{
	DECLARE_DYNAMIC(CSizeConfiguration)

public:
	CSizeConfiguration();
	virtual ~CSizeConfiguration();

	void SetCameraHandle(Camera *pCam);
	void SetView(CView *pView);

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// Dialogfelddaten
	enum { IDD = IDD_DIALOG_SIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	void UpdateControls();

	void  SetAoiWidth(int nValue);
	void  SetAoiHeight(int nValue);
	void  SetAoiLeft(int nValue);
	void  SetAoiTop(int nValue);

	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:
	// handle to camera
	Camera	*m_pCam;
    // handle to view
	CView *m_pView;

	int m_nWidth;
	int m_nHeight;
	int m_nXPos;
	int m_nYPos;

	int m_nWidthStep;
	int m_nHeightStep;
	int m_nXStep;
	int m_nYStep;

	int m_nMaxSensorWidth;
	int m_nMaxSensorHeight;	

	// slider
	CSliderCtrl m_sliderWidth;
	CSliderCtrl m_sliderHeight;
	CSliderCtrl m_sliderLeft;
	CSliderCtrl m_sliderTop;

	void ReallocMemory();

public:
	afx_msg void OnDeltaposSpinWidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinHeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTop(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedCheckWindow();	
};
