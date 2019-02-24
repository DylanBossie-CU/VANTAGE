//==========================================================================//
//																			//
//	Copyright (C) 2004 - 2018												//
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

#include "uEye.h"

#define COMPORTBASE		100

#define MAX_REG_LEN		255

class CuEyeComportDemoDlg : public CDialog
{
 private:

    UINT ComportNumber;

	HIDS	m_hCam;

	HANDLE hVirt, hCOM1;
	
	CString StringTitleVirtualComport;

	CComboBox ComboBaudVirt, ComboDataVirt, ComboStopVirt, ComboParityVirt;
	CComboBox ComboBaudCOM1, ComboDataCOM1, ComboStopCOM1, ComboParityCOM1;
	CComboBox ComboDirection, ComboAdditionalCharVirt, ComboAdditionalCharCOM1;

	CButton BtnConnectVirt, BtnConnectCOM1, BtnSend, CheckboxBroadcastComport;
	
	CEdit EditSourceFile, EditDestFile;

	CStatic LblSourceBytes, LblDestBytes;

	CProgressCtrl ProgressBar;

	int				m_nBroadcastComportNumber;
	int				m_nGpioDirection;

	bool			m_bDialogAlive;
	DWORD			FileSize;

	bool			m_bVirtOpen, m_bCOM1Open;
	bool			m_bThreadAliveVirt, m_bThreadAliveCOM1;
	bool			m_bThreadAliveSendCOM1ToVirt, m_bThreadAliveSendVirtToCOM1;

	DWORD			m_EventMask;
	
	HANDLE			m_hEventArrayVirt[2], m_hEventArrayCOM1[2];;
	HANDLE			m_hShutdownEventVirt, m_hShutdownEventCOM1;

	CWinThread		*m_ThreadVirt, *m_ThreadCOM1;
	CWinThread		*m_ThreadSendCOM1ToVirt, *m_ThreadSendVirtToCOM1;
	
	COMMTIMEOUTS	m_lpOldTimeOutsVirt, m_lpOldTimeOutsCOM1;
	DCB				m_dcbOldVirt, m_dcbOldCOM1;

    OVERLAPPED		m_ovReadVirt, m_ovReadCOM1;
	OVERLAPPED      m_ovWriteVirt, m_ovWriteCOM1;

	HICON m_hIcon;

	static UINT ThreadVirt (LPVOID pParam);
	static UINT ThreadCOM1 (LPVOID pParam);
	static UINT ThreadSend (LPVOID pParam);
	static UINT ThreadSendCOM1ToVirt (LPVOID pParam);
	static UINT ThreadSendVirtToCOM1 (LPVOID pParam);

    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();
	void CloseCamera();

	bool OpenVirt();
	bool CloseVirt();
	void SendMessageVirt();
	void ReceiveMessageVirt();
	
	bool OpenCOM1();
	bool CloseCOM1();
	void SendMessageCOM1();
	void ReceiveMessageCOM1();

	void FillComboComport();
	void FillComboDirection();
	void FillComboAdditionalChar();

	void UpdateSendControls(bool Enable);

	int  GetBroadcastComportNumber();

public:
	CuEyeComportDemoDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_UEYECOMPORTDEMO_DIALOG };

	virtual void DoDataExchange(CDataExchange* pDX);	
	
	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg BOOL PreTranslateMessage(MSG *pMsg);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnSendVirt();
	afx_msg void OnBnClickedBtnClearVirt();
	afx_msg void OnBnClickedBtnConnectVirt();
	afx_msg void OnBnClickedBtnConnectCom1();
	afx_msg void OnBnClickedBtnSendCom1();
	afx_msg void OnBnClickedBtnClearCom1();
	afx_msg void OnCbnSelchangeComboBaudVirt();
	afx_msg void OnCbnSelchangeComboBaudCom1();
	afx_msg void OnBnClickedCheckBroadcast();
	afx_msg void OnBnClickedBtnSourceFile();
	afx_msg void OnBnClickedBtnDestFile();
	afx_msg void OnBnClickedBtnSendFile();
	afx_msg void OnBnClickedRadioGpioDirection();
	afx_msg void OnBnClickedBtnConfigure();
};
