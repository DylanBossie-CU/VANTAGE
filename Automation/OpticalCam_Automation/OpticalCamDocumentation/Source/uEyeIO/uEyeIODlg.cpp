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

#include "stdafx.h"
#include "uEyeIO.h"
#include "uEyeIODlg.h"
#include "version.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();

protected:

	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString Str;
	GetDlgItem(IDC_STATIC_COPYRIGHT)->GetWindowText(Str);
	Str.Append(_T(CURRENT_YEAR));
	GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(Str);

	return TRUE;
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CUEyeIODlg::CUEyeIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUEyeIODlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CUEyeIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUEyeIODlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_DOUT, OnButtonDout)
	ON_BN_CLICKED(IDC_BUTTON_LED, OnButtonLed)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_NORMAL, &CUEyeIODlg::OnBnClickedRadioLedMode)
	ON_BN_CLICKED(IDC_RADIO_BLINK, &CUEyeIODlg::OnBnClickedRadioLedMode)
	ON_BN_CLICKED(IDC_RADIO_5_TIMES, &CUEyeIODlg::OnBnClickedRadioLedMode)
END_MESSAGE_MAP()


BOOL CUEyeIODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	// init variables
	m_nDin  = 0;
	m_nDout = IO_FLASH_MODE_CONSTANT_LOW;
    
	// create LEDs
	LedIn.Create(this, IDC_LED_IN);
	LedOut.Create(this, IDC_LED_OUT);
	LedLed.Create(this, IDC_LED_LED);

	// init timer for the check of the input signal
	m_nElaspeInput = 100; // 100msec
	m_nTimerInput = SetTimer(1, m_nElaspeInput, NULL);
	if (m_nTimerInput == 0)
	{
		AfxMessageBox(L"ERROR: No Windows timer available!" , MB_ICONEXCLAMATION, 0);
		PostQuitMessage(0);
	}

	// init timer for the LED blink
	m_nElaspeBlink = 400; 
	m_nTimerBlink = SetTimer(2, m_nElaspeBlink, NULL);
	if (m_nTimerBlink == 0)
	{
		AfxMessageBox(L"ERROR: No Windows timer available!" , MB_ICONEXCLAMATION, 0);
		PostQuitMessage(0);
	}

	OpenCamera();

	// Check "normal" mode radio button by default
	((CButton*)GetDlgItem(IDC_RADIO_NORMAL))->SetCheck(BST_CHECKED);

	// Do not blink by default
	m_nNumberOfBlinks = -1;

	// determine output variable
	UINT nMode = 0;
	INT nRet = is_IO (m_hCam, IS_IO_CMD_FLASH_GET_MODE, (void*)&nMode, sizeof(nMode));
	if (IS_SUCCESS != nRet)
	{
		AfxMessageBox(L"ERROR: is_IO(...IS_IO_CMD_FLASH_GET_MODE) !" , MB_ICONEXCLAMATION, 0);
		PostQuitMessage(0);
	}

	if (nMode == IO_FLASH_MODE_CONSTANT_HIGH)
	{
		LedOut.SetLed(CLed::LED_COLOR_RED, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
	}
	else
	{
		LedOut.SetLed(CLed::LED_COLOR_RED, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
	}

    // Check device type: For USB3 cameras we have other LED options than for USB2 and GigE cameras (see uEye.h)
    m_nDeviceType = is_GetCameraType(m_hCam);

    m_nLed = 0;

	// Check the LED state
	nRet = is_IO (m_hCam, IS_IO_CMD_LED_GET_STATE, &m_nLed, sizeof(m_nLed));
	if (IS_SUCCESS != nRet)
	{
        // ETH
        if ((IS_NOT_SUPPORTED == nRet) && (IS_INTERFACE_TYPE_ETH == (m_nDeviceType & IS_INTERFACE_TYPE_ETH)))
        {
            AfxMessageBox(L"ERROR: Configuring LED for ETH-Cameras not supported !", MB_ICONEXCLAMATION, 0);
            PostQuitMessage(0);
        }
        else
        {
            AfxMessageBox(L"ERROR: is_IO(...IS_IO_CMD_LED_GET_STATE) !", MB_ICONEXCLAMATION, 0);
            PostQuitMessage(0);
        }
	}

    // Check device type: For USB3 cameras we have other LED options than for USB2 and GigE cameras (see uEye.h)
    m_nDeviceType = is_GetCameraType(m_hCam);

    // USB3
    if ((m_nDeviceType & IS_INTERFACE_TYPE_USB3) == IS_INTERFACE_TYPE_USB3)
    {
        // LED enabled
        if (m_nLed == IO_LED_ENABLE)
        {
            LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
        }
        // LED disabled
        else
        {
            LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
        }
    }
    else
    // USB2 and GigE
    {
        CAMINFO camInfo;
        nRet = is_GetCameraInfo(m_hCam, &camInfo);
        bool isSE = (camInfo.Type & IS_CAMERA_TYPE_UEYE_USB_SE) == IS_CAMERA_TYPE_UEYE_USB_SE;

        int major;
        int minor;
        sscanf_s(camInfo.Version, "V%d.%d", &major, &minor);

        if(isSE && major > 14)
        {
            // LED enabled
            if (m_nLed == IO_LED_ENABLE)
            {
                LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
            }
            // LED disabled
            else
            {
                LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
            }
        }
        else
        {
            // if LED green
            if (m_nLed == IO_LED_STATE_2)
            {
                LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
            }
            // LED red
            else
            {
                LedLed.SetLed(CLed::LED_COLOR_RED, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
            }

            GetDlgItem(IDC_RADIO_NORMAL)->EnableWindow(FALSE);
            GetDlgItem(IDC_RADIO_5_TIMES)->EnableWindow(FALSE);
            GetDlgItem(IDC_RADIO_BLINK)->EnableWindow(FALSE);
        }
    }

	return TRUE;
}



void CUEyeIODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void CUEyeIODlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();

	}
}


HCURSOR CUEyeIODlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


bool CUEyeIODlg::OpenCamera()
{
	// open next available uEye camera
	m_hCam = (HIDS) 0;  
	INT nRet = InitCamera(&m_hCam, NULL);		
	if (IS_SUCCESS != nRet)
	{
		AfxMessageBox(L"ERROR: is_InitCamera !" , MB_ICONEXCLAMATION, 0);
		PostQuitMessage(0);
	}

    return true;
}


void CUEyeIODlg::OnButtonExit() 
{
	// delete Timer
	KillTimer (m_nTimerInput);
	KillTimer (m_nTimerBlink);

	// release camera
	INT nRet = is_ExitCamera(m_hCam);	
	if (IS_SUCCESS != nRet)
	{
		AfxMessageBox(L"ERROR: is_ExitCamera !" , MB_ICONEXCLAMATION, 0);
	}

	// terminate
	PostQuitMessage(0);
}


void CUEyeIODlg::OnButtonDout() 
{   
	// toggle output variable
	if (m_nDout == IO_FLASH_MODE_CONSTANT_HIGH)
	{
		m_nDout = IO_FLASH_MODE_CONSTANT_LOW;
		LedOut.SetLed(CLed::LED_COLOR_RED, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
	}
	else
	{
		m_nDout = IO_FLASH_MODE_CONSTANT_HIGH;
		LedOut.SetLed(CLed::LED_COLOR_RED, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
	}

	INT nRet = is_IO(m_hCam, IS_IO_CMD_FLASH_SET_MODE, (void*)&m_nDout, sizeof(m_nDout));
	if (IS_SUCCESS != nRet)
	{
		AfxMessageBox(L"ERROR: is_IO(...IS_IO_CMD_FLASH_SET_MODE) !" , MB_ICONEXCLAMATION, 0);
		PostQuitMessage(0);
	}
}


void CUEyeIODlg::OnTimer(UINT_PTR nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

	// timer for my DIO?
	if (nIDEvent == m_nTimerInput)
	{
		// static read the digital (trigger) input
		m_nDin = is_SetExternalTrigger(m_hCam, IS_GET_TRIGGER_STATUS);
    
		// update GUI for digital input
		CLed::CLedMode LedMode = CLed::LED_OFF;

		if (m_nDin!= 0)
		{
			LedMode = CLed::LED_ON;
		}

		LedIn.SetLed(CLed::LED_COLOR_GREEN, LedMode, CLed::LED_ROUND, CLed::LED_BIG);
		SetDlgItemInt(IDC_EDIT_DIN, m_nDin);
	}
	else if (nIDEvent == m_nTimerBlink)
	{
		// Camera LED //
		if (m_nNumberOfBlinks >= 0)
		{
			if (m_nBlinkToggleState == IO_LED_DISABLE)
			{	
				LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);	
				m_nBlinkToggleState = IO_LED_ENABLE;
			}
			else
			{
				LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);	
				m_nBlinkToggleState = IO_LED_DISABLE;
			}

			if (m_nNumberOfBlinks < 11)
			{
				m_nNumberOfBlinks--;
			}
		}
	}
}


void CUEyeIODlg::OnButtonLed() 
{
	INT nRet;

    // USB3
    if ((m_nDeviceType & IS_INTERFACE_TYPE_USB3) == IS_INTERFACE_TYPE_USB3)
    {	
        INT nCommand = 0;

        if (m_nLed == IO_LED_ENABLE)
        {
            m_nLed = IO_LED_DISABLE;
            LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
        }
        else
        {
            m_nLed = IO_LED_ENABLE;
            LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
        }

        nRet = is_IO(m_hCam, IS_IO_CMD_LED_SET_STATE, &m_nLed, sizeof(m_nLed));
    }
    else
	// USB2 and GigE devices
	{
        CAMINFO camInfo;
        nRet = is_GetCameraInfo(m_hCam, &camInfo);
        bool isSE = (camInfo.Type & IS_CAMERA_TYPE_UEYE_USB_SE) == IS_CAMERA_TYPE_UEYE_USB_SE;

        int major;
        int minor;
        sscanf_s(camInfo.Version, "V%d.%d", &major, &minor);

        if(isSE && major > 14)
        {
            if (m_nLed == IO_LED_ENABLE)
            {
                m_nLed = IO_LED_DISABLE;
                LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
            }
            else
            {
                m_nLed = IO_LED_ENABLE;
                LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
            }
        }
        else
        {
		    // LED red
		    if (m_nLed == IO_LED_STATE_1)
		    {
			    // Toggle to green
			    m_nLed = IO_LED_STATE_2;
			    LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
		    }
		    // LED green
		    else
		    {
			    // Toggle to red
			    m_nLed = IO_LED_STATE_1;
			    LedLed.SetLed(CLed::LED_COLOR_RED, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
		    }
        }

		nRet = is_IO(m_hCam, IS_IO_CMD_LED_SET_STATE, &m_nLed, sizeof(m_nLed));
	}

	if (IS_SUCCESS != nRet)
	{
		AfxMessageBox(L"ERROR: is_IO(...IS_IO_CMD_LED_SET_STATE) !" , MB_ICONEXCLAMATION, 0);
		PostQuitMessage(0);
	}
}


INT CUEyeIODlg::InitCamera (HIDS *hCam, HWND hWnd)
{
    INT nRet = is_InitCamera (hCam, hWnd);	
    /************************************************************************************************/
    /*                                                                                              */
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*                                                                                              */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */
    /*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
    /*                                                                                              */
    /************************************************************************************************/
    if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
    {
        // Time for the firmware upload = 25 seconds by default
        INT nUploadTime = 25000;
        is_GetDuration (*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);
    
        CString Str1, Str2, Str3;
        Str1 = "This camera requires a new firmware. The upload will take about";
        Str2 = "seconds. Please wait ...";
        Str3.Format (L"%s %d %s", Str1, nUploadTime / 1000, Str2);
        AfxMessageBox (Str3, MB_ICONWARNING);
    
        // Set mouse to hourglass
	    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS) (((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
        nRet = is_InitCamera (hCam, hWnd);   
    }
    
    return nRet;
}


void CUEyeIODlg::OnBnClickedRadioLedMode()
{
	INT nCommand = 0;

	if (((CButton*)GetDlgItem(IDC_RADIO_NORMAL))->GetCheck() == BST_CHECKED)
	{
		GetDlgItem(IDC_BUTTON_LED)->EnableWindow(TRUE);

		nCommand = IO_LED_BLINK_DISABLE;
		is_IO(m_hCam, IS_IO_CMD_LED_SET_STATE, &nCommand, sizeof(nCommand));

		if (m_nLed == IO_LED_ENABLE)
		{
			nCommand = IO_LED_ENABLE;
			LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_ON, CLed::LED_ROUND, CLed::LED_BIG);
		}
		else
		{
			nCommand = IO_LED_DISABLE;
			LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
		}

		m_nNumberOfBlinks = -1;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_BLINK))->GetCheck() == BST_CHECKED)
	{
		GetDlgItem(IDC_BUTTON_LED)->EnableWindow(FALSE);

		LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
		nCommand = IO_LED_BLINK_ENABLE;

		m_nNumberOfBlinks = 11;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_5_TIMES))->GetCheck() == BST_CHECKED)
	{
		GetDlgItem(IDC_BUTTON_LED)->EnableWindow(FALSE);

		LedLed.SetLed(CLed::LED_COLOR_GREEN, CLed::LED_OFF, CLed::LED_ROUND, CLed::LED_BIG);
		nCommand = IO_LED_BLINK_5_TIMES;

		m_nNumberOfBlinks = 10;

		m_nBlinkToggleState = m_nLed;
	}

	INT nRet = is_IO(m_hCam, IS_IO_CMD_LED_SET_STATE, &nCommand, sizeof(nCommand));
}
