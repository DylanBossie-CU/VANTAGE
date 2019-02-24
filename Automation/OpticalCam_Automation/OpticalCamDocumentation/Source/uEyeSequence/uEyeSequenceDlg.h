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

// uEyeSequenceDlg.h : header file
//

#if !defined(AFX_UEYESEQUENCEDLG_H__4F658A51_59D7_40A3_8A0C_92C1F2F68746__INCLUDED_)
#define AFX_UEYESEQUENCEDLG_H__4F658A51_59D7_40A3_8A0C_92C1F2F68746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "uEye.h"

class CUEyeSequenceDlgAutoProxy;


class CUEyeSequenceDlg : public CDialog
{
public:
	CUEyeSequenceDlg(CWnd* pParent = NULL);	// standard constructor
	friend class CUEyeTriggerDlgAutoProxy;

	// some definitions
	#define MIN_SEQ_BUFFERS 3
	#define MAX_SEQ_BUFFERS 32767
	#define MAX_EV      32
	#define SEQ_RING    1
	#define SEQ_LIN     2
	#define NUM_REPLAY_WINDOWS  5	// we optional use 5 small windows for replay

	// camera variables
	HIDS	m_hCam;					// handle to camera
	HWND	m_hWndDisplay;			// handle to display window
	INT		m_Ret;					// return value of uEye SDK functions
	INT		m_nColorMode;			// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;		// number of bits needed store one pixel
	INT		m_nSizeX;				// width of video 
	INT		m_nSizeY;				// height of video
    SENSORINFO m_sInfo;			// sensor information struct

	// memory and sequence buffers
	INT		m_lSeqMemId[MAX_SEQ_BUFFERS];	// camera memory - buffer ID
	char*	m_pcSeqImgMem[MAX_SEQ_BUFFERS];	// camera memory - pointer to buffer
	int		m_nSeqNumId[MAX_SEQ_BUFFERS];	// varibale to hold the number of the sequence buffer Id
	int		m_SeqMode;						// ringbuffer or linear bufer

	// event variables
	HANDLE  m_hEv[MAX_EV];			// event handle array
	INT     m_nEvUI[MAX_EV];		// type of uEye event
	int     m_nEvCount[MAX_EV];		// event counter
	int     m_EvMax;				// maximum number of events we use

	// threads
	HANDLE  m_hThreadEvent;      
	BOOL    m_boRunThread;
	DWORD   m_dwThreadIDEvent;

	// camera functions
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool CamOpen();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

	// event functions
	bool EvEnumerate();
	bool EvGetIndex( INT nEvent, int* pnIndex);
	bool EvEnable(INT nEvent, bool bEnable);
	bool EvInit( INT nEvent);
	void ThreadProcEvent();
	bool EvInitAll();
	bool EvTerminateAll();

	// sequence functions
	bool SeqBuilt();
	bool SeqKill();

	// replay
	void DisplayReplayBuffer( void );
	void SeqShowStateInGUI(unsigned int nBufferIndex, unsigned int nBuffersMax, bool bUp );
	UINT m_nTimer;
	HWND	m_hWndDispReplay[NUM_REPLAY_WINDOWS];  // handles to replay display windows

	enum { IDD = IDD_UEYESEQUENCE_DIALOG };
	CEdit	m_ctrlSysMemTotal;
	CButton	m_ctrlButtonLoadParam;
	CButton	m_ctrlButtonReplayStop;
	CButton	m_ctrlButtonReplay;
	CSpinButtonCtrl	m_ctrlSpinReplay;
	CSliderCtrl	m_ctrlSliderReplay;
	CProgressCtrl	m_ctrlProgressReplay;
	CEdit	m_ctrlBufferReplay;
	CProgressCtrl	m_ctrlProgressRecord;
	CEdit	m_ctrlBufferIndex;
	CEdit	m_ctrlCurrentFps;
	CEdit	m_ctrlCamFps;
	CButton	m_ctrlButtonCaptureStop;
	CButton	m_ctrlButtonCapture;
	CEdit	m_ctrlTotalBufferSize;
	CEdit	m_ctrlBuffersInUse;
	CEdit	m_ctrlSysMemFree;
	CEdit	m_ctrlBufferStatus;
	CButton	m_ctrlButtonReallocBuf;
	CProgressCtrl	m_ctrlProgressMemAlloc;
	CEdit	m_ctrlBuffersNew;
	UINT	m_nBuffersInUse;
	UINT	m_nBuffersNew;
	CString	m_strIdsWeb;
	UINT	m_nCntCapturedBuffers;
	UINT	m_nCntCapturedSeq;
	UINT	m_nCntTransferErrors;
	UINT	m_nBufferIndex;
	int		m_nBufferStart;
	int		m_nBufferStop;
	int		m_nBufferReplay;
	DWORD	m_dwSingleBufferSize;
	DWORD	m_dwTotalBufferSize;
	DWORD	m_dwSysMemTotal;
	CString	m_strCamFps;
	CString	m_strCamType;
	CString	m_strCamSN;
	CString	m_strCamHwRev;
	CString	m_strCamDrvVer;
	CString	m_strCamSensorColor;
	int		m_nCamX;
	int		m_nCamY;
	int		m_nCamID;
	int		m_nCamPclk;
	CString	m_strCurrentFps;
	BOOL	m_bLoopDisplay;

private:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	CUEyeSequenceDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonExit();
	afx_msg void OnRadioBufferRing();
	afx_msg void OnRadioBufferLin();
	afx_msg void OnButtonCaptureStop();
	afx_msg void OnButtonCapture();
	afx_msg void OnButtonReallocteBuffers();
	afx_msg void OnButtonReplay();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButtonReplayStop();
	afx_msg void OnDeltaposSpinReplay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonLoadParameters();
	afx_msg void OnRadioCmY8();
	afx_msg void OnRadioCmRgb24();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_UEYESEQUENCEDLG_H__4F658A51_59D7_40A3_8A0C_92C1F2F68746__INCLUDED_)
