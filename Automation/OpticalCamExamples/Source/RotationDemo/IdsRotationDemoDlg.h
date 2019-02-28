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

// IdsRotationDemoDlg.h	: header file
//

#if	!defined(AFX_IDSRotationDemoDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_)
#define	AFX_IDSRotationDemoDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_

#if	_MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000

#include "uEye.h"
#include <vector>
#include "afxcmn.h"


/////////////////////////////////////////////////////////////////////////////
// CIdsRotationDemoDlg dialog

//memorys used to store the different images
struct Memory
{
    Memory(void)
        : pcImageMemory(NULL)
        , lMemoryId(0)
        , lSequenceId(0)
    {
    }

    char* pcImageMemory;
    INT lMemoryId;
    INT lSequenceId;
};

class CIdsRotationDemoDlg :	public CDialog
{

public:

	CIdsRotationDemoDlg(CWnd* pParent =	NULL);	// standard	constructor

	enum { IDD = IDD_IDSRotationDemo_DIALOG	};

private:
	virtual	void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HICON	m_hIcon;

	// uEye	varibles
	HIDS	m_hCam;			// handle to camera
	INT		m_nColorMode;	// Y8/RGB24
	INT		m_nBitsPerPixel;// number of bits needed store one pixel
	INT		m_nSizeX;		// width of image
	INT		m_nSizeY;		// height of image


	SENSORINFO m_sInfo;		// sensor information struct
	INT		m_nRenderMode;	// render mode


	//Definitions used to rotate the original image
	HWND	m_hWndDisplayVert;	// handle to diplay	window

    typedef std::vector<Memory> MemoryVector;
    MemoryVector m_vecMemory;
    const INT m_cnNumberOfSeqMemory;

	INT		m_lMemoryIdVert;	// camera vertical memory -	buffer ID
	char*	m_pcImageMemoryVert;// camera vertical memory -	pointer	to buffer

	INT		m_lMemoryIdSnap;	// camera snap shot	memory - buffer	ID
	char*	m_pcImageMemorySnap;// camera snap shot	memory - pointer to	buffer

	INT		m_lMemoryIdSnapVert;	// camera snap shot	memory - buffer	ID
	char*	m_pcImageMemorySnapVert;// camera snap shot	memory - pointer to	buffer

    // Path to save the snapshots
    BOOL    m_bFirstTime;
    CString m_strPath;

	//check if the thread is running
	BOOL m_bThreadIsRunning;

	//last mode used IDS_LIVE_MODE/IDS_SNAP_MODE
	INT m_nLastMode;

	//used exposure time
	double m_ExposureTime;

	//control variables for the sliders
	CSliderCtrl m_ctrlSliderExposure;
	CSliderCtrl m_ctrlSliderGain;

	//functions used to control the camera
    INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();
	void ExitCamera();
	INT	 InitImageMemorys();
	void StartThread();
	void StopThread(BOOL bWait);
    void StopLiveVideo(INT nWait);

	//building the dialog
	void RepositionDialogItems();

	//declare the thread function as friend so that it can access all protected/private members of the dialog
	friend void ImageProcessing(void* pParam);
	
	//function to rotate the given image memory
	INT CopyuEyeMemory(char* pcSource, char* pcDestination, INT nWidth, INT nHeight, INT nBitsPerPixel);

	//used to display the image memory
	void DisplayuEyeImage(INT nMemoryID, INT nRenderMode);

    INT AllocImageMems(void);
    INT FreeImageMems(void);
    INT InitSequence(void);
    INT ClearSequence(void);
    INT GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId);

	//message function used to update the framerate from the thread
	afx_msg LRESULT OnUpdateFramerate(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	virtual	BOOL OnInitDialog();
	afx_msg	void OnSysCommand(UINT nID,	LPARAM lParam);
	afx_msg	void OnPaint();
	afx_msg	HCURSOR	OnQueryDragIcon();
	afx_msg	void OnButtonStart();
	afx_msg	void OnButtonStop();
	afx_msg	void OnButtonExit();
	afx_msg	void OnButtonSnap();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_IDSRotationDemoDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_)
