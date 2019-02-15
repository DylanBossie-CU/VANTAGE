//==========================================================================//
//                                                                          //
//  Copyright (C) 2010 - 2018                                               //
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


// uEye_DirectShow_Demo_Dlg.h : Headerdatei
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <stack>
#include <dshow.h>

#include "uEye_DirectShow_DeviceListener.h"


// uEye_DirectShow_Demo_Dlg Dialogfeld
class uEye_DirectShow_Demo_Dlg : public CDialog
{
// Konstruktion
public:
	uEye_DirectShow_Demo_Dlg(CWnd* pParent = NULL);	// Standardkonstruktor
    ~uEye_DirectShow_Demo_Dlg();

// Dialogfelddaten
	enum { IDD = IDD_UEYE_DIRECTSHOW_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	afx_msg BOOL PreTranslateMessage(MSG *pMsg);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
    bool DirectShow_Init();
    bool DirectShow_Deinit();

    bool VideoSourcesList_Fill();
    bool VideoSourcesList_Refill();
    bool VideoSourcesList_Clear();

    bool FilterGraph_Create( IMoniker* pMoniker);
    bool FilterGraph_Destroy();
    bool FilterGraph_Start();
    bool FilterGraph_Pause();
    bool FilterGraph_Stop();

    /*
    The reconnecting mechanism was designed to handle the disconnecting and connecting of simple
    filter graphs. It was not tested for complex graphs which are splitting or merging, so it may
    or may not work with such graphs.
    */
    bool FilterGraphPins_Connect();
    bool FilterGraphPins_Disconnect();

    bool ExposureControl_Init();
    bool ExposureControl_Deinit();
    bool ExposureControl_UpdateFilter();
    bool ExposureControl_UpdateWidgets();
    bool ExposureControl_AcquireAutoControlState( bool& rbAutoControlActive);
    bool ExposureControl_SetAutoControlState( bool bAutoControlActive);
    bool ExposureControl_StartAutoTimer();
    bool ExposureControl_StopAutoTimer();
    bool ExposureControl_OnAutoTimer();

    bool HwTrigger_ForceTrigger();

    // caller to release the pin
    IPin* GetPin(   IBaseFilter*    pFilter, 
                    PIN_DIRECTION   pinDirection,
                    const GUID*     pMajorMediaType= NULL,
                    const GUID*     pPinCategory= NULL,
                    LPCSTR          szFiltername= NULL);

    bool ShowPinProperties();
    bool ShowFilterProperties();

    bool EnumerateFilters();

    bool UpdateGraphState( int iGraphState);

private:
    // lists the recognized DirectShow video sources
    CComboBox m_comboVideoSources;

    // enumerate the filters in the current graph
    CButton m_bnDumpGraph;

    // exposure control
    CSliderCtrl m_sliderExposureTime;
    // display minimum exposure time
    CStatic m_labelMinExposureTime;
    // display maximum exposure time
    CStatic m_labelMaxExposureTime;
    // display current exposure time
    CStatic m_labelCurrentExposureTime;
    // display current frame rate
    CStatic m_labelCurrentFramerate;
    // activate auto exposure control
    CButton m_chkAutoExposureControlActive;

    // start button
    CButton m_bnGraphStart;
    // stop button
    CButton m_bnGraphStop;
    // pause button
    CButton m_bnGraphPause;

    // 'force trigger' button
    CButton m_bnForceTrigger;

    // graph state display
    CStatic m_labelGraphState;

    // the currently active video source
    IBaseFilter* m_pActiveVideoSource;
    // the currently active Filter Graph Manager
    IGraphBuilder* m_pActiveFilterGraphManager;

    // automatic device list refresh
    DeviceListener* m_devListener;
    CString m_currentDeviceName;

    // the graph state
    int m_iGraphState;

	bool m_bKeyDown;

    // pin reconnecting
    std::stack<std::pair<CComPtr<IPin>, CComPtr<IPin> > > m_connections;

public:
    afx_msg void OnCbnSelchangeComboVideosources();
    afx_msg void OnBnClickedButtonStartgraph();
    afx_msg void OnBnClickedButtonPausegraph();
    afx_msg void OnBnClickedButtonStopgraph();
    afx_msg void OnBnClickedButtonPinProperties();
    afx_msg void OnBnClickedButtonExit();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonFilterProperties();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnNMReleasedcaptureSliderExposuretime(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedCheckAutoExposureControlActive();
    afx_msg void OnBnClickedButtonForceTrigger();
    afx_msg void OnBnClickedButtonDumpGraph();
};
