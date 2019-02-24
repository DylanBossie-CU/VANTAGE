//==========================================================================//
//                                                                          //
//  Copyright (C) 2011 - 2018                                               //
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

// uEyeSequenceAoiDlg.h : Headerdatei
//

#pragma once

#include "uEye.h"
#include <vector>
#include "afxcmn.h"

// memory needed for live display while using DIB
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

// CuEyeSequenceAoiDlg-Dialogfeld
class CuEyeSequenceAoiDlg : public CDialog
{
// Konstruktion
public:
	CuEyeSequenceAoiDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_UEYESEQUENCEAOI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    BOOL PreTranslateMessage(MSG* pMsg);
	

private:
// uEye varibles
	HIDS	m_hCam;				// handle to camera
	INT		m_nColorMode;		// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;	// number of bits needed store one pixel
	INT		m_nSizeX;			// width of image
	INT		m_nSizeY;			// height of image

    typedef std::vector<Memory> MemoryVector;
    MemoryVector m_vecMemory;
    const INT m_cnNumberOfSeqMemory;
    SENSORINFO m_sInfo;			// sensor information struct
    
    INT InitCamera (HIDS *hCam, HWND hWnd);
	void OpenCamera();
    void ExitCamera();
    int  InitDisplayMode();
    void LoadParameters();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

    // Properties Control
    double m_dblMinExp, m_dblMaxExp, m_dblIncExp;

    void InitProperties();
    void InitSequenceMode();
    void UpdateProperties();

    void UpdateExposure();
    bool IsSequenceAoi();

    int SetSequenceParam(AOI_SEQUENCE_PARAMS Param);
    int GetSequenceParam(int nIndex, AOI_SEQUENCE_PARAMS *pParam);

    void DisableEnableSpecificProperties(BOOL enable);

    INT AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel);
    INT FreeImageMems(void);
    INT InitSequence(void);
    INT ClearSequence(void);
    INT GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId);

    AOI_SEQUENCE_PARAMS m_SequenceParam;

    LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnClose();

    CSliderCtrl m_sliderAOI_X;
    CSliderCtrl m_sliderAOI_Y;
    CSliderCtrl m_sliderAOI_EXP;
    CSliderCtrl m_sliderAOI_MasterGain;
    CSliderCtrl m_sliderFps;

    CStatusBarCtrl *m_StatusBar;

    CComboBox m_comboCurrentSequence;

    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnDeltaposSpinAoiMastergain(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinAoiExp(NMHDR *pNMHDR, LRESULT *pResult);
    
    afx_msg void OnCbnSelchangeComboCurSequence();

    afx_msg void OnDeltaposSpinAoiPosX(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinAoiPosY(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinAoiWidth(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinAoiHeight(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinAoiX(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinAoiY(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonAoiSet();
    afx_msg void OnDeltaposSpinFps(NMHDR *pNMHDR, LRESULT *pResult);

public:
    void OnEnReturnEditAoiX();
    void OnEnReturnEditAoiY();
    void OnEnReturnEditAoiExp();
    void OnEnReturnEditAoiMastergain();
    void OnEnReturnEditFps();
    void OnEnReturnEditCycle();
    afx_msg void OnDestroy();
    afx_msg void OnDeltaposSpinAoiCycle(NMHDR *pNMHDR, LRESULT *pResult);
};
