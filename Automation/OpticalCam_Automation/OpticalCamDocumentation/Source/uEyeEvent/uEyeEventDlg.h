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

// uEyeEventDlg.h : header file

#if !defined(AFX_UEYEEVENTDLG_H__CF74BD34_912E_44D1_807C_D8BA425D887A__INCLUDED_)
#define AFX_UEYEEVENTDLG_H__CF74BD34_912E_44D1_807C_D8BA425D887A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"

enum _disp_mode 
{
  e_disp_mode_bitmap = 0,
  e_disp_mode_direct3d
};

/////////////////////////////////////////////////////////////////////////////
// CuEyeEventDlg dialog
class CuEyeEventDlg : public CDialog
{
 // Construction
 public:
	CuEyeEventDlg(CWnd* pParent = NULL);	// standard constructor

	// some definitions
	#define MAX_EV      32
	#define SEQ_BUFFERS 5

	// uEye varibles
	HIDS	m_hCam;				// handle to camera
	HWND	m_hWndDisplay;		// handle to diplay window
    INT     m_nDispModeSel;
	INT		m_Ret;				// return value of uEye SDK functions
	INT		m_nColorMode;		// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;	// number of bits needed store one pixel
	INT		m_nSizeX;			// width of image
	INT		m_nSizeY;			// height of image
	INT		m_nTimeout;			// timeout for image acquisition
    BOOL    m_bDoInit;
	
	// Memory and sequence buffers, no host memory needed for live display while using Direct3D
	INT		m_lMemoryId;					// camera memory - buffer ID
	char*	m_pcImageMemory;				// camera memory - pointer to buffer
	INT		m_lSeqMemId[SEQ_BUFFERS];		// camera memory - buffer ID
	char*	m_pcSeqImgMem[SEQ_BUFFERS];		// camera memory - pointer to buffer
	SENSORINFO m_sInfo;   // sensor information struct

	// Event variables
	HANDLE  m_hEv[MAX_EV];      // event handle array
	INT     m_nEvUI[MAX_EV];    // type of uEye event
	int     m_nEvCount[MAX_EV]; // event counter
	int     m_nEvChecked[MAX_EV]; // GUI flag for checking the radio button
	int     m_EvMax;            // maximum number of events we use

	// Thread
	HANDLE  m_hThread;			
	BOOL    m_boRunThread;
	DWORD   m_dwThreadID;

	// Camera functions
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();
    int  InitDisplayMode(int nModeSel);
    void ResetEvGuiStaes();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

	// Event functions
	bool EvEnumerate();
	bool EvGetIndex( INT nEvent, int* pnIndex);
	bool EvEnable(INT nEvent, bool bEnable);
	bool EvInit( INT nEvent);
	void ThreadProc();
	bool EvInitAll();
	bool EvTerminateAll();

	// Sequence functions
	bool SeqBuilt();
	bool SeqKill();

	// Timer & beep functions
	UINT  m_nTimer;
	DWORD m_BeepFreq;

	// Trigger functions
	void TriggerGuiEnable(bool bEnable);

	enum { IDD = IDD_UEYEEVENT_DIALOG };

 protected:

    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonLoadParameter();
	afx_msg void OnExit();
	afx_msg void OnButtonResetEvents();
	afx_msg void OnButtonFreeze();
	afx_msg void OnButtonStartLive();
	afx_msg void OnButtonStopLive();
	afx_msg void OnButtonSeqStart();
	afx_msg void OnButtonSeqStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRadioTrgOff();
	afx_msg void OnRadioTrgHw();
	afx_msg void OnRadioTrgSw();
	afx_msg void OnChangeEditTimeout();
	afx_msg void OnMove(int x, int y);
    afx_msg void OnBnClickedDispModeChange();
    afx_msg void OnClose();
	
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_UEYEEVENTDLG_H__CF74BD34_912E_44D1_807C_D8BA425D887A__INCLUDED_)
