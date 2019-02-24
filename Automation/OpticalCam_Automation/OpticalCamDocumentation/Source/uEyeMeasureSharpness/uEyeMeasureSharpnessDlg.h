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

#if !defined(AFX_UEYEMEASURESHARPNESSDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_)
#define AFX_UEYEMEASURESHARPNESSDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"

class CuEyeMeasureSharpnessDlg;

class DisplayControl : public CStatic
{

public:

    DisplayControl();
    ~DisplayControl();

    void SetParent(CuEyeMeasureSharpnessDlg* pParent);
    CuEyeMeasureSharpnessDlg* GetParent() { return m_pParent; }

private:

    CuEyeMeasureSharpnessDlg* m_pParent;

    INT	    m_nStartX;
    INT	    m_nStartY;
    INT	    m_nCurrX;
    INT	    m_nCurrY;

    CPen    m_penRed;
	CPen    m_penGreen;
	
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()   
};



class CuEyeMeasureSharpnessDlg : public CDialog
{

public:

	CuEyeMeasureSharpnessDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_UEYEMEASURESHARPNESS_DIALOG };

    HICON						m_hIcon;

	HIDS						m_hCam;
	HWND						m_hWndDisplay;
	INT							m_nColorMode;
	INT							m_nBitsPerPixel;
    INT							m_s32ImageWidth;
    INT							m_s32ImageHeight;
	
	INT							m_lMemoryId;
	char*						m_pcImageMemory;

	SENSORINFO					m_sInfo;	
    INT							m_nRenderMode;	
    
    INT							m_s32Sharpness[5];
    
    MEASURE_SHARPNESS_AOI_INFO	m_sharpnessInfo[5];

    INT							m_nCurrentAOI;

    double						m_f64FactorX;
    double						m_f64FactorY;

    RECT						m_rcImageWindow;
    RECT                        m_rcWindow;
    
	CComboBox					m_comboAoiPresets;
    CComboBox                   m_comboAoi;

    DisplayControl              m_display;

    CFont                       m_Font;
    COLORREF                    m_color, m_colorRed, m_colorGreen;
    
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();
    void ExitCamera();
    int  InitDisplayMode();
    void LoadParameters();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);
    void UpdateSizeControls();
    void UpdateSharpness();
    INT GetImagePos(INT &xPos, INT &yPos);

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonLoadParameter();
	afx_msg void OnButtonExit();
    afx_msg void OnClose();
    afx_msg BOOL PreTranslateMessage(MSG *pMsg);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnEnKillfocusEditSize();
	afx_msg void OnCbnSelchangeComboAoiPresets();
    afx_msg void OnCbnSelchangeComboAoi();
    afx_msg void OnBnClickedButtonResetAoi();

	LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()     
};


#endif // !defined(AFX_UEYEMEASURESHARPNESSDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_)
