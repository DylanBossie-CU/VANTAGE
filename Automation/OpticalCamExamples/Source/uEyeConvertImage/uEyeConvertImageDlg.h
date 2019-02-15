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

#if !defined(AFX_UEYECONVERTIMAGEDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_)
#define AFX_UEYECONVERTIMAGEDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "uEye.h"


class ConvertImageDlg : public CDialog
{

public:
	ConvertImageDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CONVERTIMAGE_DIALOG };

private:

    virtual void DoDataExchange(CDataExchange* pDX);

    HICON	m_hIcon;

	HIDS						m_hCam;	
	HWND						m_hWndDisplay;	

	INT							m_nPixelFormat;
	INT							m_nBitsPerPixel;
	INT							m_nSizeX;
	INT							m_nSizeY;
	INT							m_nSourceId;
	char*						m_pSourceBuffer;
	INT							m_nDestId;
	char*						m_pDestBuffer;
	SENSORINFO					m_sInfo;		
    INT							m_nRenderMode;
    
	BOOL						m_bShowOriginalImage;
	BOOL						m_bShowOriginalSize;
	BOOL						m_bShowYUVWarning;
	
	BUFFER_CONVERSION_PARAMS	m_ConversionParams;
	INT							m_nSelectedIndexComboDestFormat;

	BOARDINFO					m_cameraInfo;

	CComboBox					m_comboColorFormat;
	CComboBox					m_comboColorFormatRaw;
	CComboBox					m_comboPixelConverter;
	CComboBox					m_comboColorCorrection;

	CSliderCtrl					m_sliderSaturation;
	CSliderCtrl					m_sliderGamma;
	CSliderCtrl					m_sliderEdgeEnhancement;

	char*						m_pbmpInfo;
	INT							m_nSizeDestX;
	INT							m_nSizeDestY;
	INT							m_nSizeSourceX;
	INT							m_nSizeSourceY;

	INT							m_nMinEdgeEnhancement;
	INT							m_nMaxEdgeEnhancement;

	INT							m_nPlanarLayer;

	void FillComboColorFormat();
	void FillComboColorFormatRaw();
	void FillComboPixelConverter();
	void FillComboColorCorrection();
	void InitSliderSaturation();
	void InitSliderGamma();
	void InitSliderEdgeEnhancement();

	INT InitCamera (HIDS *hCam, HWND hWnd);
	BOOL OpenCamera();
    void ExitCamera();
    INT  InitDisplayMode();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);
	BOOL CheckPixelFormat(INT nPixelFormat);
	void UpdateBitmapInfo();
	void DisplayImage();
	INT GetBitsPerPixel();
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExit();
    afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboPixelformat();
	afx_msg void OnCbnSelchangeComboPixelconverter();
	afx_msg void OnCbnSelchangeComboColorcorrection();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckShowOriginal();
	afx_msg void OnBnClickedButtonConvert();
	afx_msg void OnBnClickedCheckOriginalSize();
	afx_msg void OnDeltaposSpinSaturation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinGamma(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditSaturation();
	afx_msg void OnEnKillfocusEditGamma();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnDeltaposSpinEdgeEnhancement(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditEdgeEnhancement();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()  	
};

#endif // !defined(AFX_UEYECONVERTIMAGEDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_)
