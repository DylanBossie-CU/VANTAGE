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
#include "afxwin.h"


class CProgramConfigDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CProgramConfigDlg)  

private:
	CDialog *dlg;
	ProgramOptions *m_Options;

	virtual void DoDataExchange(CDataExchange* pDX);
	void OnOK();
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:

    int m_nCanvasSizeX;
    int m_nCanvasSizeY;
    int m_nPosCrossX;
    int m_nPosCrossY;
    int m_nPicturePosX;
    int m_nPicturePosY;
    int m_nCircleRadius;
    CString m_strPicturePath;
    BOOL m_bShowPicture;
    BOOL m_bStartFullscreen;

	void SetParent(CDialog *parent, ProgramOptions *pOptions);
	CComboBox m_ctrlPencilWidth;

    enum { IDD = IDD_PROGRAM_CONFIG_DLG };

	afx_msg void OnBnClickedBtnChooseColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedChkStartfullscreen();
	afx_msg void OnCbnSelchangeCmbWidth();
	afx_msg void OnEnChangeEditCircleRadius();
	afx_msg void OnBnClickedBtnFolder();
	afx_msg void OnBnClickedChkShowPicture();
	afx_msg void OnBnClickedBtnChooseKeycolor();
	afx_msg void OnEnChangeEditnPicturePosY();
	afx_msg void OnEnChangeEditnPicturePosX();
    afx_msg void OnEnChangeEditCrossPosx();
    afx_msg void OnEnChangeEditCrossPosy();
    afx_msg void OnBnClickedButtonResetPos();
    afx_msg void OnEnChangeEditFolder();

    CProgramConfigDlg();
	virtual ~CProgramConfigDlg();
};
