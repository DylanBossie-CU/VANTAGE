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
#include "afxcmn.h"
#include "ueye.h"

// Sort function for the CListCtrl
static int CALLBACK SortListCtrl(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

// Index of the column to sort (default = 4 -> Model) 
static int SortColumn = 4;

// Sort direction (Default = ascending) 
static bool Direction = true;

// CDlgSelectCamera-Dialogfeld

class CDlgSelectCamera : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectCamera)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

public:

	CDlgSelectCamera(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CDlgSelectCamera();

	// Dialogfelddaten
	enum { IDD = IDD_DLGSELECTCAMERA };

	virtual BOOL OnInitDialog();
	CListCtrl   m_ctrlCameras;
	int         m_nDevID;
	CString     m_sModel;
	CImageList  m_imgList;
	BOOL        m_bFirmwareUpload;

	LRESULT OnUEyeMessage( WPARAM wParam, LPARAM lParam );

	void RefreshList();
	void EmptyList(int nIndex = -1);

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLvnItemActivateListCameras(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();	
	afx_msg void OnLvnColumnclickListCameras(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
