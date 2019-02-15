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

#if !defined(AFX_UEYETIMESTAMPDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_)
#define AFX_UEYETIMESTAMPDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "uEye.h"
#include <vector>
#include "afxwin.h"

enum _disp_mode
{
    e_disp_mode_bitmap = 0,
    e_disp_mode_directdraw
};

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

/////////////////////////////////////////////////////////////////////////////
class CuEyeTimestampDlg : public CDialog
{
private:
    virtual void DoDataExchange(CDataExchange* pDX);

    HICON m_hIcon;

    // uEye variables
    HIDS        m_hCam;             // handle to camera
    HWND        m_hWndDisplay;      // handle to diplay window
    INT         m_nColorMode;       // Y8/RGB16/RGB24/REG32
    INT         m_nBitsPerPixel;    // number of bits needed store one pixel
    INT         m_nSizeX;           // width of image
    INT         m_nSizeY;           // height of image
    INT         m_nPosX;            // left offset of image
    INT         m_nPosY;            // right offset of image
    INT         m_nTriggerMode;     // trigger mode
    INT         m_nFrameEvents;     // counter for the frame events
    INT         m_nTriggerEvents;   // counter for the trigger events
    BOOL        m_bLive;

    typedef std::vector<Memory> MemoryVector;
    MemoryVector m_vecMemory;
    const INT m_cnNumberOfSeqMemory;
    SENSORINFO  m_sInfo;            // sensor information struct
    INT         m_nRenderMode;      // render  mode
    INT         m_nFlipHor;         // horizontal flip flag
    INT         m_nFlipVert;        // vertical flip flag

    INT InitCamera(HIDS *hCam, HWND hWnd);
    bool OpenCamera();
    void ExitCamera();
    int  InitDisplayMode();
    void LoadParameters();
    void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);

    INT AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel);
    INT FreeImageMems(void);
    INT InitSequence(void);
    INT ClearSequence(void);
    INT GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId);

    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnButtonStart();
    afx_msg void OnButtonStop();
    afx_msg void OnBnClickedButtonLoadParameter();
    afx_msg void OnButtonExit();
    LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

public:

    CuEyeTimestampDlg(CWnd* pParent = NULL);

    enum { IDD = IDD_UEYETIMESTAMP_DIALOG };

    afx_msg void OnClose();
    afx_msg void OnBnClickedRadioTrigger();
    afx_msg void OnBnClickedButtonResetCounters();
};

#endif // !defined(AFX_UEYETIMESTAMPDLG_H__00EFA87C_7A28_4501_8760_D28D4F306A00__INCLUDED_)
