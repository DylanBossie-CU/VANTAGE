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

#include "uEye.h"
#include "tinyxml/tinyxml.h"
#include <string>


struct CameraOptions
{
	bool bAWB;
	bool bAGC;
	bool bAES;
	int nAGCskipFrame;
	int nAESskipFrame;

	bool bMirrorVer;
	bool bMirrorHor;
	unsigned int nColorMode;

	unsigned int nContrast;
	unsigned int nLum;
};

struct ProgramOptions
{
	COLORREF DrawColor;
	COLORREF KeyColor;
	unsigned int nPencilWidth;
	unsigned int nCircleRadius;
	CString strPicturePath;

	unsigned int nPicturePosX;
	unsigned int nPicturePosY;

    unsigned int nCrossPosX;
	unsigned int nCrossPosY;

    unsigned int m_nCanvasSizeX;
    unsigned int m_nCanvasSizeY;

	bool bStartFullscreen;
	bool bShowPicture;
};

class uEyeCrosshairDlg : public CDialog
{

private:

	CameraOptions m_CameraOptions;
	ProgramOptions m_ProgramOptions;

	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	HWND	m_hWndDisplay;		// handle to display window
	INT		m_nColorMode;		// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;	// number of bits needed to store one pixel

	SENSORINFO m_sInfo;			// sensor information struct

	INT		m_nDisplayMode;
	INT     m_nOverlaySizeX;
    INT     m_nOverlaySizeY;


	LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);

	INT InitCamera (HIDS *hCam, HWND hWnd);
	bool OpenCamera();
    void ExitCamera();
    void GetMaxImageSize(UINT *pnSizeX, UINT *pnSizeY);
    void UpdateCanvasSizeProgramOptions();

    virtual void DoDataExchange(CDataExchange* pDX);

public:
    
    HIDS				m_hCam;		    // handle to camera
	UINT		        m_nSizeX;		// width of the image
	UINT        		m_nSizeY;		// height of the image

	void DrawObjects();			        // draw the objects
	bool LoadConfig();			        // load the settings from a xml file
	bool SaveConfig();			        // save the settings to a xml file
	void ExecConfig();			        // exec camera settings
	void OpenConfigurationDlg();        // show the settings dialog

    uEyeCrosshairDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CAMERA_DIALOG };

	afx_msg void OnClose                ();
	afx_msg void OnSize                 (UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging    (WINDOWPOS* lpwndpos);
	afx_msg void OnLButtonDown          (UINT nFlags, CPoint point);
	afx_msg void OnContextMenu          (CWnd* pWnd, CPoint point);
	afx_msg void OnProgramConfiguration ();
};
