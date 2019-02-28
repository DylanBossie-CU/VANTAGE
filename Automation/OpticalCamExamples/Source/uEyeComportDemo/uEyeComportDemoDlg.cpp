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
#include "uEyeComportDemo.h"
#include "uEyeComportDemoDlg.h"
#include ".\ueyecomportdemodlg.h"
#include "version.h"
#include "Strsafe.h"

#define TransceiveBufferSize 128 // Buffer for terminal

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning (disable : 4996)

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

protected:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV-Unterstützung


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



CuEyeComportDemoDlg::CuEyeComportDemoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CuEyeComportDemoDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nGpioDirection = -1;
}


void CuEyeComportDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_BAUD_VIRT, ComboBaudVirt);
    DDX_Control(pDX, IDC_COMBO_BAUD_COM1, ComboBaudCOM1);
    DDX_Control(pDX, IDC_BTN_CONNECT_VIRT, BtnConnectVirt);
    DDX_Control(pDX, IDC_BTN_CONNECT_COM1, BtnConnectCOM1);
    DDX_Control(pDX, IDC_COMBO_DIRECTION, ComboDirection);
    DDX_Control(pDX, IDC_EDIT_SOURCE_FILE, EditSourceFile);
    DDX_Control(pDX, IDC_EDIT_DEST_FILE, EditDestFile);
    DDX_Control(pDX, IDC_PROGRESSBAR, ProgressBar);
    DDX_Control(pDX, IDC_SIZE_SOURCE2, LblSourceBytes);
    DDX_Control(pDX, IDC_SIZE_DEST2, LblDestBytes);
    DDX_Control(pDX, IDC_BTN_SEND_FILE, BtnSend);
    DDX_Control(pDX, IDC_COMBO_CHAR_VIRT, ComboAdditionalCharVirt);
    DDX_Control(pDX, IDC_COMBO_CHAR_COM1, ComboAdditionalCharCOM1);
    DDX_Control(pDX, IDC_CHECK_BROADCAST, CheckboxBroadcastComport);
    DDX_Radio  (pDX, IDC_RADIO_GPIO1_TX, m_nGpioDirection);
}


BEGIN_MESSAGE_MAP(CuEyeComportDemoDlg, CDialog)
    ON_WM_CTLCOLOR()
    ON_WM_DESTROY()
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_SEND_VIRT, OnBnClickedBtnSendVirt)
    ON_BN_CLICKED(IDC_BTN_CLEAR_VIRT, OnBnClickedBtnClearVirt)
    ON_BN_CLICKED(IDC_BTN_CONNECT_VIRT, OnBnClickedBtnConnectVirt)
    ON_BN_CLICKED(IDC_BTN_CONNECT_COM1, OnBnClickedBtnConnectCom1)
    ON_BN_CLICKED(IDC_BTN_SEND_COM1, OnBnClickedBtnSendCom1)
    ON_BN_CLICKED(IDC_BTN_CLEAR_COM1, OnBnClickedBtnClearCom1)
    ON_CBN_SELCHANGE(IDC_COMBO_BAUD_VIRT, OnCbnSelchangeComboBaudVirt)
    ON_CBN_SELCHANGE(IDC_COMBO_BAUD_COM1, OnCbnSelchangeComboBaudCom1)
    ON_BN_CLICKED(IDC_CHECK_BROADCAST, OnBnClickedCheckBroadcast)
    ON_BN_CLICKED(IDC_BTN_SOURCE_FILE, OnBnClickedBtnSourceFile)
    ON_BN_CLICKED(IDC_BTN_DEST_FILE, OnBnClickedBtnDestFile)
    ON_BN_CLICKED(IDC_BTN_SEND_FILE, OnBnClickedBtnSendFile)
    ON_BN_CLICKED(IDC_RADIO_GPIO1_TX, &CuEyeComportDemoDlg::OnBnClickedRadioGpioDirection)
    ON_BN_CLICKED(IDC_RADIO_GPIO1_RX, &CuEyeComportDemoDlg::OnBnClickedRadioGpioDirection)
    ON_BN_CLICKED(IDC_BTN_CONFIGURE, &CuEyeComportDemoDlg::OnBnClickedBtnConfigure)
END_MESSAGE_MAP()


BOOL CuEyeComportDemoDlg::OnInitDialog()
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

    // Initialize the handles for the COM ports
    hCOM1 = INVALID_HANDLE_VALUE;
    hVirt = INVALID_HANDLE_VALUE;

    // Initialize the threads
    m_ThreadVirt = NULL;
    m_ThreadCOM1 = NULL;
    m_ThreadSendCOM1ToVirt = NULL;
    m_ThreadSendVirtToCOM1 = NULL;

    // Initialize the bool variables that indicate the open COM-ports
    m_bVirtOpen = false;
    m_bCOM1Open = false;

    // Initialize the bool variables for the lifetime of the threads
    m_bThreadAliveVirt = false;
    m_bThreadAliveCOM1 = false;
    m_bThreadAliveSendCOM1ToVirt = false;
    m_bThreadAliveSendVirtToCOM1 = false;

    m_bDialogAlive = true;

    // Open the first uEye camera in the camera list
    if(OpenCamera())
    {
        INT Type = is_GetCameraType(m_hCam);

        BOOL bETH  = (Type == IS_CAMERA_TYPE_UEYE_ETH_HE);
        BOOL bUSB3 = ((Type & IS_CAMERA_TYPE_UEYE_USB3_CP) == IS_CAMERA_TYPE_UEYE_USB3_CP);

        if (bETH || bUSB3)
        {
            // Get the comport number saved in the camera EEPROM
            INT ret = is_GetComportNumber(m_hCam, &ComportNumber);
            if (ret == IS_SUCCESS)
            {
                // Set the title of the group box
                CString Str1, Str2, Str3;
                Str2.Format(L" (COM%d)", ComportNumber);
                GetDlgItem(IDC_GRP_COMPORT_VIRT)->GetWindowText(StringTitleVirtualComport);
                Str1.Format(L"%s%s", StringTitleVirtualComport, Str2);
                GetDlgItem(IDC_GRP_COMPORT_VIRT)->SetWindowText(Str1);

                // Get the broadcast COM-port number (by default 255)
                m_nBroadcastComportNumber = GetBroadcastComportNumber();

                // Insert items into the combo boxes
                FillComboComport();
                FillComboDirection();
                FillComboAdditionalChar();
            }
            else
            {
                CString Str;
                Str.LoadString(IDS_COMPORT_ERROR);
                AfxMessageBox(Str, MB_ICONWARNING);
                OnOK();
            }
        }
        // Camera type not supported
        else
        {
            CString Str;
            Str.LoadString(IDS_NO_CAMERA);
            AfxMessageBox(Str, MB_ICONWARNING);
            OnOK();
        }

        GetDlgItem(IDC_BTN_SEND_FILE)->EnableWindow(FALSE);

        if (bETH)
        {
            GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(FALSE);
            GetDlgItem(IDC_RADIO_GPIO1_TX)->EnableWindow(FALSE);
            GetDlgItem(IDC_RADIO_GPIO1_RX)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_CONNECT_VIRT)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_SEND_FILE)->EnableWindow(TRUE);
        }
        else if (bUSB3)
        {
            /* These commands are needed because GPIO1-RX, GPIO2-TX is not supported yet */
            GetDlgItem(IDC_RADIO_GPIO1_RX)->EnableWindow(FALSE);
            m_nGpioDirection = 0;
            UpdateData(FALSE);
            OnBnClickedBtnConfigure();
        }
    }
    // Camera can not be opened
    else
    {
        CString Str;
        Str.LoadString(IDS_NO_CAMERA_OPENED);
        AfxMessageBox(Str, MB_ICONWARNING);
        OnOK();
    }

    return TRUE;
}


HBRUSH CuEyeComportDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    // background of the controls = white
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor); 

    if((pWnd->GetDlgCtrlID() == IDC_EDIT_RECEIVE_VIRT)
        || (pWnd->GetDlgCtrlID() == IDC_EDIT_RECEIVE_COM1)
        || (pWnd->GetDlgCtrlID() == IDC_EDIT_SOURCE_FILE)
        || (pWnd->GetDlgCtrlID() == IDC_EDIT_DEST_FILE))
    {
        hbr = CreateSolidBrush(RGB(255,255,255));
        pDC->SetBkMode(TRANSPARENT);
        DeleteObject(hbr);
    }

    return hbr;
}


void CuEyeComportDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CuEyeComportDemoDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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


HCURSOR CuEyeComportDemoDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CuEyeComportDemoDlg::OnBnClickedBtnSendVirt()
{
    SendMessageVirt();
}


void CuEyeComportDemoDlg::OnBnClickedBtnSendCom1()
{
    SendMessageCOM1();
}


void CuEyeComportDemoDlg::OnBnClickedBtnClearVirt()
{
    GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->SetWindowText(L"");
}


void CuEyeComportDemoDlg::OnBnClickedBtnClearCom1()
{
    GetDlgItem(IDC_EDIT_RECEIVE_COM1)->SetWindowText(L"");
}


void CuEyeComportDemoDlg::OnBnClickedBtnConnectVirt()
{
    CString Str1, Str2, Str3, Str4;

    BtnConnectVirt.GetWindowText(Str1);
    Str2.LoadString(IDS_CONNECT);
    Str3.LoadString(IDS_DISCONNECT);

    // Disconnect
    if(Str1 == Str3)
    {
        if(CloseVirt() == true)
        {
            BtnConnectVirt.SetFocus();
            BtnConnectVirt.SetWindowText(Str2);

            // Enable the send controls again (only if COM1 closed)
            if(m_bCOM1Open == false)
                UpdateSendControls(true);

            CheckboxBroadcastComport.EnableWindow(TRUE);

            m_bVirtOpen = false;
        }

        else
        {
            BtnConnectVirt.SetFocus();
            m_bVirtOpen = true;
            Str4.LoadString(IDS_COMPORT_NOT_CLOSED);
            AfxMessageBox(Str4, MB_ICONERROR);
        }
    }

    // Connect
    else if(Str1 == Str2)
    {
        if(OpenVirt() == true)
        {
            GetDlgItem(IDC_EDIT_SEND_VIRT)->SetFocus();
            BtnConnectVirt.SetWindowText(Str3);
            UpdateSendControls(false);
            CheckboxBroadcastComport.EnableWindow(FALSE);
            m_bVirtOpen = true;
        }

        else
        {
            BtnConnectVirt.SetFocus();
            m_bVirtOpen = false;
            Str4.LoadString(IDS_COMPORT_NOT_OPENED);
            AfxMessageBox(Str4, MB_ICONERROR);
        }
    }
}

void CuEyeComportDemoDlg::OnBnClickedBtnConnectCom1()
{
    CString Str1, Str2, Str3, Str4;

    BtnConnectCOM1.GetWindowText(Str1);
    Str2.LoadString(IDS_CONNECT);
    Str3.LoadString(IDS_DISCONNECT);

    // Disconnect
    if(Str1 == Str3)
    {	
        if(CloseCOM1() == true)
        {
            BtnConnectCOM1.SetFocus();
            BtnConnectCOM1.SetWindowText(Str2);

            // Enable the send controls again (only if virtual COM-port closed)
            if(m_bVirtOpen == false)
                UpdateSendControls(true);

            m_bCOM1Open = false;
        }

        else
        {
            BtnConnectCOM1.SetFocus();
            m_bCOM1Open = true;
            Str4.LoadString(IDS_COMPORT_NOT_CLOSED);
            AfxMessageBox(Str4, MB_ICONERROR);
        }
    }

    // Connect
    else if(Str1 == Str2)
    {
        if(OpenCOM1() == true)
        {
            GetDlgItem(IDC_EDIT_SEND_COM1)->SetFocus();
            BtnConnectCOM1.SetWindowText(Str3);
            UpdateSendControls(false);
            m_bCOM1Open = true;
        }

        else
        {
            BtnConnectCOM1.SetFocus();
            m_bCOM1Open = false;
            Str4.LoadString(IDS_COMPORT_NOT_OPENED);
            AfxMessageBox(Str4, MB_ICONERROR);
        }
    }
}


void CuEyeComportDemoDlg::OnCbnSelchangeComboBaudVirt()
{
    CString Str1, Str2;

    Str1.LoadString(IDS_CONNECT);
    Str2.LoadString(IDS_COMPORT_NOT_CLOSED);

    if(m_bVirtOpen == true)
    {
        if(CloseVirt() == true)
        {
            BtnConnectVirt.SetWindowText(Str1);
            m_bVirtOpen = false;
        }

        else
        {
            m_bVirtOpen = true;
            AfxMessageBox(Str2, MB_ICONERROR);
        }
    }
}


void CuEyeComportDemoDlg::OnCbnSelchangeComboBaudCom1()
{
    CString Str1, Str2;

    Str1.LoadString(IDS_CONNECT);
    Str2.LoadString(IDS_COMPORT_NOT_CLOSED);

    if(m_bCOM1Open == true)
    {
        if(CloseCOM1() == true)
        {
            BtnConnectCOM1.SetWindowText(Str1);
            m_bCOM1Open = false;
        }

        else
        {
            m_bCOM1Open = true;
            AfxMessageBox(Str2, MB_ICONERROR);
        }
    }
}


bool CuEyeComportDemoDlg::OpenCamera()
{
    bool ret = false;

    CloseCamera();

    // Init camera (open next available camera)
    m_hCam = (HIDS) 0;
    if (InitCamera(&m_hCam, NULL) == IS_SUCCESS)
    {
        ret = true;
    }

    return ret;
}


void CuEyeComportDemoDlg::CloseCamera()
{
    if( m_hCam != 0 )
    {
        // Close camera
        is_ExitCamera( m_hCam );
        m_hCam = NULL;
    }
}


// Open the virtual COM-port
bool CuEyeComportDemoDlg::OpenVirt()
{
    bool opened = false;
    DCB dcb;
    //char port[20], settings[100];
    CString port, settings, baud;
    int nCnt = 0;

    // Get the baud rate from the combo box
    ComboBaudVirt.GetWindowText(baud);

    // If the broadcast checkbox is checked -> Ignore the COM-port in the EEPROM
    // and take the broadcast COM-port
    if(CheckboxBroadcastComport.GetCheck() == BST_CHECKED)
    {
        port.Format(L"\\\\.\\COM%d", m_nBroadcastComportNumber);
    }
    // Take the COM-port from the camera EEPROM
    else
    {
        port.Format(L"\\\\.\\COM%d", ComportNumber);
    }
    settings.Format(L"baud=%s parity=N data=8 stop=1\0", baud);

    // Get a COM-port handle
    hVirt = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

    if(hVirt != INVALID_HANDLE_VALUE)
    {
        // Get the timeout settings
        COMMTIMEOUTS lpTimeOuts;
        GetCommTimeouts (hVirt, &m_lpOldTimeOutsVirt);

        // Save the timeout settings
        lpTimeOuts = m_lpOldTimeOutsVirt;

        // Set new settings
        lpTimeOuts.ReadIntervalTimeout         = MAXDWORD;
        lpTimeOuts.ReadTotalTimeoutMultiplier  = 0;
        lpTimeOuts.ReadTotalTimeoutConstant    = 0;
        lpTimeOuts.WriteTotalTimeoutMultiplier = 0;
        lpTimeOuts.WriteTotalTimeoutConstant   = 0;

        // Set the timeout settings
        if (SetCommTimeouts (hVirt, &lpTimeOuts))
        {
            // Set the event mask. We are waiting for RXCHAR events (receive)
            m_EventMask = EV_RXCHAR;
            if(SetCommMask(hVirt, m_EventMask) != 0)
            {
                // Get the current settings of the COM-port
                if (GetCommState (hVirt, &m_dcbOldVirt))
                {
                    ZeroMemory(&dcb, sizeof(DCB));
                    dcb.DCBlength = sizeof(DCB);

                    // Define the I/O control settings for the COM-port
                    if(BuildCommDCB(settings, &dcb) != 0)
                    { 
                        // Set the I/O control settings
                        if(SetCommState(hVirt, &dcb) != 0)
                        {
                            opened = true;
                        }
                    }
                }
            }
        }
    }

    if((!opened) && (hVirt != INVALID_HANDLE_VALUE))
    { 
        CloseHandle(hVirt);
        hVirt = INVALID_HANDLE_VALUE;
    }

    else
    {
        // Stop already running thread
        while (m_bThreadAliveVirt && (nCnt < 50))
        {
            SetEvent (m_hShutdownEventVirt);
            Sleep (10);
            nCnt++;
        }

        // Kill thread (no other way)
        if (m_bThreadAliveVirt && (m_ThreadVirt != NULL))
            TerminateThread (m_ThreadVirt->m_hThread, 1);

        // Initialize events
        m_hShutdownEventVirt = NULL;

        m_ovReadVirt.Offset      = 0;
        m_ovReadVirt.OffsetHigh  = 0;
        m_ovReadVirt.hEvent      = NULL;

        m_ovWriteVirt.Offset     = 0;
        m_ovWriteVirt.OffsetHigh = 0;
        m_ovWriteVirt.hEvent     = NULL;

        // Shutdown event to terminate thread
        if(m_hShutdownEventVirt != NULL)
            ResetEvent (m_hShutdownEventVirt);

        // Read event
        if (m_ovReadVirt.hEvent != NULL)
            ResetEvent (m_ovReadVirt.hEvent);

        // Write event
        if (m_ovWriteVirt.hEvent != NULL)
            ResetEvent (m_ovWriteVirt.hEvent);

        m_hShutdownEventVirt = CreateEvent (NULL, TRUE, FALSE, NULL);
        m_ovReadVirt.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
        // we don't need the write event

        m_hEventArrayVirt[0] = m_hShutdownEventVirt;
        m_hEventArrayVirt[1] = m_ovReadVirt.hEvent;

        // Start thread for the virtual COM-port
        m_ThreadVirt = AfxBeginThread (ThreadVirt, this);
    }

    return opened;
}


// Open COM1
bool CuEyeComportDemoDlg::OpenCOM1()
{
    bool opened = false;
    DCB dcb;
    CString baud, port, settings;
    int nCnt = 0;

    // Get the baud rate from the combo box
    ComboBaudCOM1.GetWindowText(baud);
    port.Format(L"\\\\.\\COM1");
    settings.Format(L"baud=%s parity=N data=8 stop=1\0", baud);

    // Get a COM-port handle
    hCOM1 = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

    if(hCOM1 != INVALID_HANDLE_VALUE)
    {
        // Get the timeout settings
        COMMTIMEOUTS lpTimeOuts;
        GetCommTimeouts (hCOM1, &m_lpOldTimeOutsCOM1);

        // Save the timeout settings
        lpTimeOuts = m_lpOldTimeOutsCOM1;

        // Set new settings
        lpTimeOuts.ReadIntervalTimeout         = MAXDWORD;
        lpTimeOuts.ReadTotalTimeoutMultiplier  = 0;
        lpTimeOuts.ReadTotalTimeoutConstant    = 0;
        lpTimeOuts.WriteTotalTimeoutMultiplier = 0;
        lpTimeOuts.WriteTotalTimeoutConstant   = 0;

        // Set the timeout settings
        if (SetCommTimeouts (hCOM1, &lpTimeOuts))
        {
            // Set the event mask. We are waiting for RXCHAR events (receive)
            m_EventMask = EV_RXCHAR;
            if(SetCommMask(hCOM1, m_EventMask) != 0)
            {
                // Get the current settings of the COM-port
                if (GetCommState (hCOM1, &m_dcbOldCOM1))
                {
                    ZeroMemory(&dcb, sizeof(DCB));
                    dcb.DCBlength = sizeof(DCB);

                    // Define the I/O control settings for the COM-port
                    if(BuildCommDCB(settings, &dcb) != 0)
                    { 
                        // Set the I/O control settings
                        if(SetCommState(hCOM1, &dcb) != 0)
                        {
                            opened = true;
                        }
                    }
                }
            }
        }
    }

    if((!opened) && (hCOM1 != INVALID_HANDLE_VALUE))
    { 
        CloseHandle(hCOM1);
        hCOM1 = INVALID_HANDLE_VALUE;
    }
    else
    {
        // Stop already running thread
        while (m_bThreadAliveCOM1 && (nCnt < 50))
        {
            SetEvent (m_hShutdownEventCOM1);
            Sleep    (10);
            nCnt++;
        }

        // Kill thread (no other way)
        if (m_bThreadAliveCOM1 && (m_ThreadCOM1 != NULL))
            TerminateThread (m_ThreadCOM1->m_hThread, 1);

        // Initialize events
        m_hShutdownEventCOM1  = NULL;

        m_ovReadCOM1.Offset      = 0;
        m_ovReadCOM1.OffsetHigh  = 0;
        m_ovReadCOM1.hEvent      = NULL;

        m_ovWriteCOM1.Offset     = 0;
        m_ovWriteCOM1.OffsetHigh = 0;
        m_ovWriteCOM1.hEvent     = NULL;

        // Shutdown event to terminate thread
        if(m_hShutdownEventCOM1 != NULL)
            ResetEvent (m_hShutdownEventCOM1);

        // Read event
        if (m_ovReadCOM1.hEvent != NULL)
            ResetEvent (m_ovReadCOM1.hEvent);

        // Write event
        if (m_ovWriteCOM1.hEvent != NULL)
            ResetEvent (m_ovWriteCOM1.hEvent);

        m_hShutdownEventCOM1 = CreateEvent (NULL, TRUE, FALSE, NULL);
        m_ovReadCOM1.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
        // we don't need the write event

        m_hEventArrayCOM1[0] = m_hShutdownEventCOM1;
        m_hEventArrayCOM1[1] = m_ovReadCOM1.hEvent;

        // Start the thread for COM1
        m_ThreadCOM1 = AfxBeginThread (ThreadCOM1, this);
    }

    return opened;
}


// Close the virtual COM-port
bool CuEyeComportDemoDlg::CloseVirt()
{
    bool closed = false;
    int nCnt = 0;

    if(CloseHandle(hVirt) != 0)
    {
        // Terminate the thread
        while (m_bThreadAliveVirt && (nCnt < 50))
        {
            SetEvent (m_hShutdownEventVirt);
            Sleep (10);
            nCnt++;
        }

        if (m_bThreadAliveVirt && (m_ThreadVirt != NULL))
            TerminateThread (m_ThreadVirt->m_hThread, 1);

        ResetEvent (m_hShutdownEventVirt);

        // Set the old timeout values back
        SetCommTimeouts (hVirt, &m_lpOldTimeOutsVirt);

        // Set the old I/O control settings back
        if (m_dcbOldVirt.DCBlength == 28)
            SetCommState (hVirt, &m_dcbOldVirt);

        hVirt = INVALID_HANDLE_VALUE;

        // Close the event handles
        CloseHandle(m_hShutdownEventVirt);
        CloseHandle(m_ovReadVirt.hEvent);
        CloseHandle(m_ovWriteVirt.hEvent);

        m_hShutdownEventVirt = INVALID_HANDLE_VALUE;
        m_ovReadVirt.hEvent = INVALID_HANDLE_VALUE;
        m_ovWriteVirt.hEvent = INVALID_HANDLE_VALUE;

        m_hEventArrayVirt[0] = INVALID_HANDLE_VALUE;
        m_hEventArrayVirt[1] = INVALID_HANDLE_VALUE;

        closed = true;
    }

    return closed;
}


// Close COM1
bool CuEyeComportDemoDlg::CloseCOM1()
{
    bool closed = false;
    int nCnt = 0;

    if(CloseHandle(hCOM1) != 0)
    {
        // Terminate the thread
        while (m_bThreadAliveCOM1 && (nCnt < 50))
        {
            SetEvent (m_hShutdownEventCOM1);
            Sleep (10);
            nCnt++;
        }

        if (m_bThreadAliveCOM1 && (m_ThreadCOM1 != NULL))
            TerminateThread (m_ThreadCOM1->m_hThread, 1);

        ResetEvent (m_hShutdownEventCOM1);

        // Set the old timeout values back
        SetCommTimeouts (hCOM1, &m_lpOldTimeOutsCOM1);

        // Set the old I/O control settings back
        if (m_dcbOldCOM1.DCBlength == 28)
            SetCommState (hCOM1, &m_dcbOldCOM1);

        hCOM1 = INVALID_HANDLE_VALUE;

        // Close the event handles
        CloseHandle(m_hShutdownEventCOM1);
        CloseHandle(m_ovReadCOM1.hEvent);
        CloseHandle(m_ovWriteCOM1.hEvent);

        m_hShutdownEventCOM1 = INVALID_HANDLE_VALUE;
        m_ovReadCOM1.hEvent = INVALID_HANDLE_VALUE;
        m_ovWriteCOM1.hEvent = INVALID_HANDLE_VALUE;

        m_hEventArrayCOM1[0] = INVALID_HANDLE_VALUE;
        m_hEventArrayCOM1[1] = INVALID_HANDLE_VALUE;

        closed = true;
    }

    return closed;
}


// Fill the combo boxes
void CuEyeComportDemoDlg::FillComboComport()
{
    INT nIndex = 0;

    INT Type = is_GetCameraType(m_hCam);
    BOOL bETH  = (Type == IS_CAMERA_TYPE_UEYE_ETH_HE);

    if (bETH)
    {
        ComboBaudVirt.InsertString(nIndex++, L"1200");
        ComboBaudVirt.InsertString(nIndex++, L"2400");
        ComboBaudVirt.InsertString(nIndex++, L"4800");
    }

    ComboBaudVirt.InsertString(nIndex++, L"9600");

    if (bETH)
    {
        ComboBaudVirt.InsertString(nIndex++, L"14400");
    }

    ComboBaudVirt.InsertString(nIndex++, L"19200");

    if (bETH)
    {
        ComboBaudVirt.InsertString(nIndex++, L"28800");
    }

    ComboBaudVirt.InsertString(nIndex++, L"38400");

    if (bETH)
    {
        ComboBaudVirt.InsertString(nIndex++, L"56000");
    }

    ComboBaudVirt.InsertString(nIndex++, L"57600");
    ComboBaudVirt.InsertString(nIndex, L"115200");
    ComboBaudVirt.SetCurSel(nIndex);

    nIndex = 0;

    if (bETH)
    {
        ComboBaudCOM1.InsertString(nIndex++, L"1200");
        ComboBaudCOM1.InsertString(nIndex++, L"2400");
        ComboBaudCOM1.InsertString(nIndex++, L"4800");
    }

    ComboBaudCOM1.InsertString(nIndex++, L"9600");

    if (bETH)
    {
        ComboBaudCOM1.InsertString(nIndex++, L"14400");
    }

    ComboBaudCOM1.InsertString(nIndex++, L"19200");

    if (bETH)
    {
        ComboBaudCOM1.InsertString(nIndex++, L"28800");
    }

    ComboBaudCOM1.InsertString(nIndex++, L"38400");

    if (bETH)
    {
        ComboBaudCOM1.InsertString(nIndex++, L"56000");
    }

    ComboBaudCOM1.InsertString(nIndex++, L"57600");
    ComboBaudCOM1.InsertString(nIndex, L"115200");
    ComboBaudCOM1.SetCurSel(nIndex);
}


// Called when the dialog is destroyed
void CuEyeComportDemoDlg::OnDestroy()
{
    m_bDialogAlive = false;

    if(m_bThreadAliveSendCOM1ToVirt)
    {
        m_bThreadAliveSendCOM1ToVirt = false;

        // Wait until the COM1_Virt thread has ended
        WaitForSingleObject(m_ThreadSendCOM1ToVirt->m_hThread, 2000);
    }

    if(m_bThreadAliveSendVirtToCOM1)
    {
        m_bThreadAliveSendVirtToCOM1 = false;

        // Wait until the Virt_COM1 thread has ended
        WaitForSingleObject(m_ThreadSendVirtToCOM1->m_hThread, 2000);
    }

    if(m_bVirtOpen)
    {
        CloseVirt();
    }

    if(m_bCOM1Open)
    {
        CloseCOM1();
    }

    CDialog::OnDestroy();
}


BOOL CuEyeComportDemoDlg::PreTranslateMessage(MSG *pMsg)
{
    // If 'return' was pressed
    if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
    {
        // get the current focus
        CWnd* pCurrWnd = GetFocus();

        // If the focus is on the edit control of the virtual COM-port send function
        if(pCurrWnd == GetDlgItem(IDC_EDIT_SEND_VIRT))
        {
            // Send a the string to the virtual COM-port
            SendMessageVirt();
        }

        // If the focus is on the edit control of the COM1 send function
        else if(pCurrWnd == GetDlgItem(IDC_EDIT_SEND_COM1))
        {
            // Send a the string to the virtual COM-port
            SendMessageCOM1();
        }

        // Ignore original message
        return 1;
    }

    // Standard
    else
    {
        return 0;
    }
}


// Send a message over the virtual COM-port
void CuEyeComportDemoDlg::SendMessageVirt()
{
    CString Str1, Str2;
    GetDlgItem(IDC_EDIT_SEND_VIRT)->GetWindowText(Str1);

    DWORD NumberOfBytesWritten;
    char buffer[TransceiveBufferSize];
    ZeroMemory(buffer, sizeof(buffer));
    CStringA multiByteString(Str1);

    // Copy the maximum number of characters (without CR and LF)
    StringCchCopyA(buffer, sizeof(buffer) - 2, multiByteString);

    size_t length = strlen(buffer);

    // Add CR
    if(ComboAdditionalCharVirt.GetCurSel() == 1)
    {
        buffer[length] = 13;
    }

    // Add LF
    if(ComboAdditionalCharVirt.GetCurSel() == 2)
    {
        buffer[length] = 10; 
    }

    // Add CR and LF
    if(ComboAdditionalCharVirt.GetCurSel() == 3)
    {
        buffer[length] = 13;
        buffer[length + 1] = 10;
    }

    if(m_bVirtOpen)
    {
        // Send the buffer to the virtual COM-port
        if(WriteFile(hVirt, buffer, (DWORD)strlen(buffer), &NumberOfBytesWritten, &m_ovWriteVirt) == 0)
        {
            if(GetOverlappedResult (hVirt, &m_ovWriteVirt, &NumberOfBytesWritten, FALSE) == 0)
            {
                Str2.LoadString(IDS_COMPORT_SEND_ERROR);
                AfxMessageBox(Str2, MB_ICONERROR);
            }
        }
    }

    else
    {
        Str2.LoadString(IDS_COMPORT_CONNECT_FIRST);
        AfxMessageBox(Str2, MB_ICONERROR);
    }
}


// Send a message over the COM1
void CuEyeComportDemoDlg::SendMessageCOM1()
{
    CString Str1, Str2;
    GetDlgItem(IDC_EDIT_SEND_COM1)->GetWindowText(Str1);

    DWORD NumberOfBytesWritten;
    char buffer[TransceiveBufferSize];
    ZeroMemory(buffer, sizeof(buffer));
    CStringA multiByteString(Str1);

    // Copy the maximum number of characters (without CR and LF)
    StringCchCopyA(buffer, sizeof(buffer) - 2, multiByteString);

    size_t length = strlen(buffer);

    // Add CR
    if(ComboAdditionalCharCOM1.GetCurSel() == 1)
    {
        buffer[length] = 13;
    }

    // Add LF
    if(ComboAdditionalCharCOM1.GetCurSel() == 2)
    {
        buffer[length] = 10;
    }

    // Add CR and LF
    if(ComboAdditionalCharCOM1.GetCurSel() == 3)
    {
        buffer[length] = 13;
        buffer[length + 1] = 10;
    }

    if(m_bCOM1Open)
    {
        // Send the buffer to the COM1
        if(WriteFile(hCOM1, buffer, (DWORD)strlen(buffer), &NumberOfBytesWritten, &m_ovWriteCOM1) == 0)
        {
            if(GetOverlappedResult (hCOM1, &m_ovWriteCOM1, &NumberOfBytesWritten, TRUE) == 0)
            {
                Str2.LoadString(IDS_COMPORT_SEND_ERROR);
                AfxMessageBox(Str2, MB_ICONERROR);
            }
        }
    }

    else
    {
        Str2.LoadString(IDS_COMPORT_CONNECT_FIRST);
        AfxMessageBox(Str2, MB_ICONERROR);
    }
}


// Receive a message at the virtual COM-port
void CuEyeComportDemoDlg::ReceiveMessageVirt()
{
    CString Str1, Str2;

    DWORD NumberOfBytesRead = 0;
    char buffer[TransceiveBufferSize + 1];
    DWORD BufferLength = TransceiveBufferSize;

    ZeroMemory(buffer, sizeof(buffer));

    if(m_bVirtOpen)
    {
        // Receive message
        if(ReadFile(hVirt, buffer, BufferLength, &NumberOfBytesRead, &m_ovReadVirt) == 0)
        {
            if(GetOverlappedResult (hVirt, &m_ovReadVirt, &NumberOfBytesRead, FALSE) != 0)
            {
                GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->GetWindowText(Str1);
                Str2.Format(L"%s%s", Str1, buffer);
                GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->SetWindowText(Str2);
            }

            else
            {
                Str2.LoadString(IDS_COMPORT_RECEIVE_ERROR);
                AfxMessageBox(Str2, MB_ICONERROR);
            }
        }

        else
        {
            GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->GetWindowText(Str1);
            Str2.Format(L"%s%s", Str1, buffer);
            GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->SetWindowText(Str2);
        }
    }

    else
    {
        Str2.LoadString(IDS_COMPORT_CONNECT_FIRST);
        AfxMessageBox(Str2, MB_ICONERROR);
    }
}


// Receive a message at COM1
void CuEyeComportDemoDlg::ReceiveMessageCOM1()
{
    CString Str1, Str2;

    DWORD NumberOfBytesRead = 0;
    char buffer[TransceiveBufferSize + 1];
    DWORD BufferLength = TransceiveBufferSize;

    ZeroMemory(buffer, sizeof(buffer));

    if(m_bCOM1Open)
    {
        // Receive message
        if(ReadFile(hCOM1, buffer, BufferLength, &NumberOfBytesRead, &m_ovReadCOM1) == 0)
        {
            if(GetOverlappedResult (hCOM1, &m_ovReadCOM1, &NumberOfBytesRead, TRUE) != 0)
            {
                GetDlgItem(IDC_EDIT_RECEIVE_COM1)->GetWindowText(Str1);
                Str2.Format(L"%s%s", Str1, buffer);
                GetDlgItem(IDC_EDIT_RECEIVE_COM1)->SetWindowText(Str2);
            }
            else
            {
                Str2.LoadString(IDS_COMPORT_RECEIVE_ERROR);
                AfxMessageBox(Str2, MB_ICONERROR);
            }
        }
        else
        {
            GetDlgItem(IDC_EDIT_RECEIVE_COM1)->GetWindowText(Str1);
            Str2.Format(L"%s%s", Str1, buffer);
            GetDlgItem(IDC_EDIT_RECEIVE_COM1)->SetWindowText(Str2);
        }
    }
    else
    {
        Str2.LoadString(IDS_COMPORT_CONNECT_FIRST);
        AfxMessageBox(Str2, MB_ICONERROR);
    }
}


// This thread is waiting for a RXCHAR event at the virtual COM-port
UINT CuEyeComportDemoDlg::ThreadVirt (LPVOID pParam)
{
    CuEyeComportDemoDlg* pCom = (CuEyeComportDemoDlg*) pParam;

    if (pCom != NULL)
    {
        DWORD Event     = 0;
        DWORD CommEvent = 0;
        DWORD dwError   = 0;
        BOOL  bResult   = TRUE;

        COMSTAT comstat;
        comstat.cbInQue  = 0;
        comstat.cbOutQue = 0;

        pCom->m_bThreadAliveVirt = true;

        if (pCom->hVirt != NULL)
        {	
            PurgeComm (pCom->hVirt, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
        }

        for (;;)
        {
            // Wait for a com event
            bResult = WaitCommEvent (pCom->hVirt, &Event, &pCom->m_ovReadVirt);
            Event = WaitForMultipleObjects (2, pCom->m_hEventArrayVirt, FALSE, INFINITE);

            // If thread close
            if(Event == 0)
            {
                pCom->m_bThreadAliveVirt = false;
                AfxEndThread (100);
            }

            // If it is the RXCHAR event
            else if(Event == 1)
            {
                GetCommMask (pCom->hVirt, &CommEvent);
                if ((CommEvent & EV_RXCHAR) == EV_RXCHAR)
                {
                    pCom->ReceiveMessageVirt();
                }
            }
        }
    }

    return 1;
}


// This thread is waiting for a RXCHAR event at COM1
UINT CuEyeComportDemoDlg::ThreadCOM1 (LPVOID pParam)
{
    CuEyeComportDemoDlg* pCom = (CuEyeComportDemoDlg*) pParam;

    if (pCom != NULL)
    {
        DWORD Event     = 0;
        DWORD CommEvent = 0;
        DWORD dwError   = 0;
        BOOL  bResult   = TRUE;

        COMSTAT comstat;
        comstat.cbInQue  = 0;
        comstat.cbOutQue = 0;

        pCom->m_bThreadAliveCOM1 = true;

        if (pCom->hCOM1 != NULL)
        {
            PurgeComm (pCom->hCOM1, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
        }

        for (;;)
        {
            // Wait for a com event
            bResult = WaitCommEvent (pCom->hCOM1, &Event, &pCom->m_ovReadCOM1);
            Event = WaitForMultipleObjects (2, pCom->m_hEventArrayCOM1, FALSE, INFINITE);

            // If thread close
            if(Event == 0)
            {
                pCom->m_bThreadAliveCOM1 = false;
                AfxEndThread (100);
            }

            // If it is the RXCHAR event
            else if(Event == 1)
            {
                GetCommMask (pCom->hCOM1, &CommEvent);
                if ((CommEvent & EV_RXCHAR) == EV_RXCHAR)
                {
                    pCom->ReceiveMessageCOM1();
                }
            }
        }
    }

    return 1;
}


// Update the file send controls
void CuEyeComportDemoDlg::UpdateSendControls(bool Enable)
{
    ComboDirection.EnableWindow(Enable);
    BtnSend.EnableWindow(Enable);
    EditSourceFile.EnableWindow(Enable);
    EditDestFile.EnableWindow(Enable);
    ProgressBar.EnableWindow(Enable);
    LblSourceBytes.EnableWindow(Enable);
    LblDestBytes.EnableWindow(Enable);
    GetDlgItem(IDC_PROGRESS)->EnableWindow(Enable);
    GetDlgItem(IDC_BTN_SOURCE_FILE)->EnableWindow(Enable);
    GetDlgItem(IDC_BTN_DEST_FILE)->EnableWindow(Enable);
}


// Fill the 'direction' combo box
void CuEyeComportDemoDlg::FillComboDirection()
{
    CString Str1, Str2, Str3;

    Str1.LoadString(IDS_SOURCE_FILE);
    Str2.LoadString(IDS_DESTINATION_FILE);

    Str3.Format(L"%s  ->  COM1  ->  COM%d  ->  %s", Str1, ComportNumber, Str2);
    ComboDirection.InsertString(0, Str3);

    Str3.Format(L"%s  ->  COM%d  ->  COM1  ->  %s", Str1, ComportNumber, Str2);
    ComboDirection.InsertString(1, Str3);

    ComboDirection.SetCurSel(0);
}


// Fill the 'Additional char' combo box
void CuEyeComportDemoDlg::FillComboAdditionalChar()
{
    CString Str1;

    Str1.LoadString(IDS_NO_CHAR);
    ComboAdditionalCharVirt.InsertString(0, Str1);
    ComboAdditionalCharCOM1.InsertString(0, Str1);

    Str1.LoadString(IDS_CR);
    ComboAdditionalCharVirt.InsertString(1, Str1);
    ComboAdditionalCharCOM1.InsertString(1, Str1);

    Str1.LoadString(IDS_LF);
    ComboAdditionalCharVirt.InsertString(2, Str1);
    ComboAdditionalCharCOM1.InsertString(2, Str1);

    Str1.LoadString(IDS_CR_LF);
    ComboAdditionalCharVirt.InsertString(3, Str1);
    ComboAdditionalCharCOM1.InsertString(3, Str1);

    ComboAdditionalCharVirt.SetCurSel(0);
    ComboAdditionalCharCOM1.SetCurSel(0);
}


// Get the broadcast comport number: If a special key is not found in the registry,
// the broadcast comport is 255.
int CuEyeComportDemoDlg::GetBroadcastComportNumber()
{
    HKEY Key;
    DWORD dwType = REG_SZ;
    DWORD dwSize = MAX_REG_LEN;
    char Value[MAX_REG_LEN];

    int ComportNumber = 255;

    if(RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\ueye\\Parameters", &Key) == ERROR_SUCCESS)
    {
        if(RegQueryValueEx(Key, L"BroadcastComport" , NULL, &dwType, (LPBYTE)Value, &dwSize) == ERROR_SUCCESS)
        {
            ComportNumber = atoi(Value);
            if(ComportNumber == 0)
                ComportNumber = 255;
        }
    }

    return ComportNumber;
}


void CuEyeComportDemoDlg::OnBnClickedCheckBroadcast()
{
    if(CheckboxBroadcastComport.GetCheck() == BST_CHECKED)
    {
        // Set the title of the group box
        CString Str1, Str2;
        Str2.Format(L" (COM%d)", m_nBroadcastComportNumber);
        Str1.Format(L"%s%s", StringTitleVirtualComport, Str2);
        GetDlgItem(IDC_GRP_COMPORT_VIRT)->SetWindowText(Str1);
    }

    else
    {
        // Set the title of the group box
        CString Str1, Str2;
        Str2.Format(L" (COM%d)", ComportNumber);
        Str1.Format(L"%s%s", StringTitleVirtualComport, Str2);
        GetDlgItem(IDC_GRP_COMPORT_VIRT)->SetWindowText(Str1);
    }
}


void CuEyeComportDemoDlg::OnBnClickedBtnSourceFile()
{
    CString SourceName;

    // Open a file dialog to select a source file
    CFileDialog FileDlg(TRUE, NULL, NULL, 0, NULL);
    if(FileDlg.DoModal() == IDOK)
    {
        SourceName = FileDlg.GetPathName();
        GetDlgItem(IDC_EDIT_SOURCE_FILE)->SetWindowText(SourceName);

        // Get the file size
        HANDLE file = CreateFile(SourceName, GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
        if(file)
        {
            DWORD dummy;
            FileSize = GetFileSize(file, &dummy);

            CString Str1, Str2;
            Str1.LoadString(IDS_BYTES);
            Str2.Format(L"%d %s", FileSize, Str1); 

            LblSourceBytes.SetWindowText(Str2);

            CloseHandle(file);
        }
    }
}


void CuEyeComportDemoDlg::OnBnClickedBtnDestFile()
{
    CString DestName;

    // Open a file dialog to select a destination file
    CFileDialog FileDlg(TRUE, NULL, NULL, 0, NULL);
    if(FileDlg.DoModal() == IDOK)
    {
        DestName = FileDlg.GetPathName();
        GetDlgItem(IDC_EDIT_DEST_FILE)->SetWindowText(DestName);

        LblDestBytes.SetWindowText(L"0");
        ProgressBar.SetPos(0);
    }
}


void CuEyeComportDemoDlg::OnBnClickedBtnSendFile()
{
    CString SourceName, DestName, Str;

    // Check if source and destination files are selected
    EditSourceFile.GetWindowText(SourceName);
    EditDestFile.GetWindowText(DestName);

    if((SourceName != "") && (DestName != ""))
    {
        if(ComboDirection.GetCurSel() == 0)
        {
            // If the thread is not running
            if(m_bThreadAliveSendCOM1ToVirt == false)
            {
                if(ComboBaudCOM1.GetCurSel() == ComboBaudVirt.GetCurSel())
                {
                    CString Str;
                    Str.LoadString(IDS_STOP);
                    BtnSend.SetWindowText(Str);
                    m_bThreadAliveSendCOM1ToVirt = true;
                    m_ThreadSendCOM1ToVirt = AfxBeginThread (ThreadSendCOM1ToVirt, this);
                }

                else
                {
                    CString Str;
                    Str.LoadString(IDS_BAUDRATES_DIFFERENT);
                    AfxMessageBox(Str, MB_ICONSTOP);
                }
            }

            else
            {
                m_bThreadAliveSendCOM1ToVirt = false;

                CString Str;
                Str.LoadString(IDS_SEND_FILE);
                BtnSend.SetWindowText(Str);
            }
        }

        else if(ComboDirection.GetCurSel() == 1)
        {
            // If the thread is not running
            if(m_bThreadAliveSendVirtToCOM1 == false)
            {
                if(ComboBaudCOM1.GetCurSel() == ComboBaudVirt.GetCurSel())
                {
                    CString Str;
                    Str.LoadString(IDS_STOP);
                    BtnSend.SetWindowText(Str);
                    m_bThreadAliveSendVirtToCOM1 = true;
                    m_ThreadSendVirtToCOM1 = AfxBeginThread (ThreadSendVirtToCOM1, this);
                }

                else
                {
                    CString Str;
                    Str.LoadString(IDS_BAUDRATES_DIFFERENT);
                    AfxMessageBox(Str, MB_ICONSTOP);
                }
            }

            else
            {
                m_bThreadAliveSendVirtToCOM1 = false;

                CString Str;
                Str.LoadString(IDS_SEND_FILE);
                BtnSend.SetWindowText(Str);
            }
        }
    }

    else
    {
        Str.LoadString(IDS_SELECT_FILES_FIRST);
        AfxMessageBox(Str, MB_ICONEXCLAMATION);
    }
}


// send a file over COM1 to the virtual COM-port
UINT CuEyeComportDemoDlg::ThreadSendCOM1ToVirt (LPVOID pParam)
{
    CuEyeComportDemoDlg* pCom = (CuEyeComportDemoDlg*) pParam;

    if (pCom != NULL)
    {      
        CString SourceName, DestName, Str1, Str2;

        // Check if source and destination files are selected
        pCom->EditSourceFile.GetWindowText(SourceName);
        pCom->EditDestFile.GetWindowText(DestName);

        if((SourceName != "") && (DestName != ""))
        {
            // Pointer to files
            FILE *pSourceFile, *pDestFile;

            // Position if the progress bar
            int pos = 0, oldpos = -1;

            // Bytes read from file, COM1 and virtual COM-port, shown on screen
            long ReadTotalFile = 0;
            long ReadTotalVirt = 0;

            // Byte counter
            int count = 0;

            Str1.LoadString(IDS_BYTES);

            // Set the range of the progress bar to 0..100 and the step to 1
            pCom->ProgressBar.SetRange(0, 100);
            pCom->ProgressBar.SetStep(1);

            // Number of bytes read and written
            DWORD NumberOfBytesReadFile = 0;
            DWORD NumberOfBytesWrittenCOM1 = 0;
            DWORD NumberOfBytesReadVirt = 0;
            DWORD NumberOfBytes = 0;

            // Initialize Send/Receive buffer
            char *buffer1, *buffer2;
            DWORD BufferLength;

            int BufferSize;

            // We choose the buffer size depending on the baudrate
            // With small baud rates and large buffers the timeouts are long
            // -> So we choose a small buffer. The maximum buffer size is 10000!
            int SelectedBaudRate = pCom->ComboBaudCOM1.GetCurSel();

            // 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400
            if(SelectedBaudRate <= 7)
            {
                BufferSize = 128;
            }

            // 56000, 115200
            else
            {
                BufferSize = 1024;
            }

            buffer1 = new char[BufferSize + 1];
            buffer2 = new char[BufferSize + 1];
            BufferLength = BufferSize;

            //DWORD BufferLength = TransceiveBufferSize;
            ZeroMemory(buffer1, sizeof(buffer1));
            ZeroMemory(buffer2, sizeof(buffer2));

            // Inititalize COM-ports
            bool OkCOM1 = false;
            bool OkVirt = false;

            HANDLE hCOM1, hVirt;

            DCB dcb;
            CString baud, port, settings;
            int nCnt = 0;

            // Get the Selection of the 'Baud' combo box (COM1)
            pCom->ComboBaudCOM1.GetWindowText(baud);

            // Initialize COM1
            port.Format(L"\\\\.\\COM1");
            settings.Format(L"baud=%s parity=N data=8 stop=1\0", baud);

            hCOM1 = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
            if(hCOM1 != INVALID_HANDLE_VALUE)
            {
                if (GetCommState (hCOM1, &pCom->m_dcbOldCOM1))
                {
                    ZeroMemory(&dcb, sizeof(DCB));
                    dcb.DCBlength = sizeof(DCB);

                    if(BuildCommDCB(settings, &dcb) != 0)
                    { 
                        if(SetCommState(hCOM1, &dcb) != 0)
                        {
                            OkCOM1 = true;
                        }
                    }
                }
            }

            if((!OkCOM1) && (hCOM1 != INVALID_HANDLE_VALUE))
            { 
                CloseHandle(hCOM1);
                hCOM1 = INVALID_HANDLE_VALUE;
            }

            // Get the Selection of the 'Baud' combo box (virtual COM-port)
            pCom->ComboBaudVirt.GetWindowText(baud);

            // Initialize virtual COM-port
            port.Format(L"\\\\.\\COM%d", pCom->ComportNumber);
            settings.Format(L"baud=%s parity=N data=8 stop=1\0", baud);

            hVirt = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
            if(hVirt != INVALID_HANDLE_VALUE)
            {
                if (GetCommState (hVirt, &pCom->m_dcbOldVirt))
                {
                    ZeroMemory(&dcb, sizeof(DCB));
                    dcb.DCBlength = sizeof(DCB);

                    if(BuildCommDCB(settings, &dcb) != 0)
                    { 
                        if(SetCommState(hVirt, &dcb) != 0)
                        {
                            OkVirt = true;
                        }
                    }
                }
            }

            if((!OkVirt) && (hVirt != INVALID_HANDLE_VALUE))
            {
                CloseHandle(hVirt);
                hVirt = INVALID_HANDLE_VALUE;
            }

            // Start of send / receive loop
            if(OkCOM1)
            {
                PurgeComm (hCOM1, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

                if(OkVirt)
                {
                    PurgeComm (hVirt, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

                    // Open source file
                    pSourceFile = _tfopen(SourceName, L"rb");
                    if(pSourceFile != NULL)
                    {
                        // Open destination file
                        pDestFile = _tfopen(DestName, L"wb");
                        if(pDestFile != NULL)
                        {
                            // Read data from source file
                            NumberOfBytesReadFile = (DWORD)fread(buffer1, sizeof(char), BufferLength, pSourceFile);
                            while((NumberOfBytesReadFile != 0) && pCom->m_bThreadAliveSendCOM1ToVirt)
                            {
                                ReadTotalFile += NumberOfBytesReadFile;

                                // Calculate the position of the progress bar
                                pos = (int)(((double)ReadTotalFile) / ((double)pCom->FileSize) * 100.0);
                                if(pos != oldpos)
                                {
                                    if(pCom->m_bDialogAlive)
                                    {
                                        pCom->ProgressBar.SetPos(pos);
                                        pCom->ProgressBar.Invalidate();
                                    }
                                }

                                // Send data to COM1
                                if(WriteFile(hCOM1, buffer1, NumberOfBytesReadFile, &NumberOfBytesWrittenCOM1, NULL) != 0)
                                {	
                                    NumberOfBytesReadVirt = 0;
                                    NumberOfBytes = 0;

                                    // Read data at virtual COM-port
                                    while((NumberOfBytesReadVirt != NumberOfBytesWrittenCOM1) && pCom->m_bThreadAliveSendCOM1ToVirt)
                                    {
                                        ReadFile(hVirt, buffer2, NumberOfBytesWrittenCOM1, &NumberOfBytes, NULL);
                                        if(NumberOfBytes != 0)
                                        {
                                            // Write data to destination file
                                            fwrite(buffer2, sizeof(char), NumberOfBytes, pDestFile);
                                            NumberOfBytesReadVirt += NumberOfBytes;
                                        }
                                    }

                                    ReadTotalVirt += NumberOfBytesReadVirt;

                                    // Update display
                                    if(pCom->m_bDialogAlive)
                                    {
                                        Str2.Format(L"%d %s", ReadTotalVirt, Str1);
                                        pCom->LblDestBytes.SetWindowText(Str2);
                                        pCom->LblDestBytes.Invalidate();
                                    }
                                }

                                // Read new data from file
                                NumberOfBytesReadFile = (DWORD)fread(buffer1, sizeof(char), BufferLength, pSourceFile);

                                // Save old position of the progress bar
                                oldpos = pos;
                            }

                            fclose(pDestFile);
                        }

                        fclose(pSourceFile);
                    }

                    if (pCom->m_dcbOldVirt.DCBlength == 28)
                    {
                        SetCommState (hVirt, &pCom->m_dcbOldVirt);
                    }

                    CloseHandle(hVirt);
                    hVirt = INVALID_HANDLE_VALUE;
                }

                if (pCom->m_dcbOldCOM1.DCBlength == 28)
                    SetCommState (hCOM1, &pCom->m_dcbOldCOM1);

                CloseHandle(hCOM1);
                hCOM1 = INVALID_HANDLE_VALUE;
            }

            if(pCom->m_bDialogAlive)
            {	
                if(ReadTotalVirt == (long)pCom->FileSize)
                {
                    Str2.LoadString(IDS_FILE_TRANSFER_COMPLETE);
                    AfxMessageBox(Str2, MB_ICONINFORMATION);
                }

                else
                {
                    Str2.LoadString(IDS_FILE_TRANSFER_INCOMPLETE);
                    AfxMessageBox(Str2, MB_ICONSTOP);
                }

                pCom->ProgressBar.SetPos(0);
                pCom->ProgressBar.Invalidate();
                pCom->LblDestBytes.SetWindowText(L"0");
                pCom->LblDestBytes.Invalidate();

                Str2.LoadString(IDS_SEND_FILE);
                pCom->BtnSend.SetWindowText(Str2);
            }

            delete buffer1;
            delete buffer2;
        }

        // Thread is finished
        pCom->m_bThreadAliveSendCOM1ToVirt = false;
    }

    return 1;
}


// send a file over the virtual COM-port to COM1
UINT CuEyeComportDemoDlg::ThreadSendVirtToCOM1 (LPVOID pParam)
{
    CuEyeComportDemoDlg* pCom = (CuEyeComportDemoDlg*) pParam;

    if (pCom != NULL)
    {      
        CString SourceName, DestName, Str1, Str2;

        // Check if source and destination files are selected
        pCom->EditSourceFile.GetWindowText(SourceName);
        pCom->EditDestFile.GetWindowText(DestName);

        if((SourceName != "") && (DestName != ""))
        {
            // Pointer to files
            FILE *pSourceFile, *pDestFile;

            // Position if the progress bar
            int pos = 0, oldpos = -1;

            // Bytes read from file, COM1 and virtual COM-port, shown on screen
            long ReadTotalFile = 0;
            long ReadTotalCOM1 = 0;

            // Byte counter
            int count = 0;

            Str1.LoadString(IDS_BYTES);

            // Set the range of the progress bar to 0..100 and the step to 1
            pCom->ProgressBar.SetRange(0, 100);
            pCom->ProgressBar.SetStep(1);

            // Number of bytes read and written
            DWORD NumberOfBytesReadFile = 0;
            DWORD NumberOfBytesReadCOM1 = 0;
            DWORD NumberOfBytesWrittenVirt = 0;
            DWORD NumberOfBytes = 0;

            // Initialize Send/Receive buffer
            char *buffer1, *buffer2;
            DWORD BufferLength;

            int BufferSize;

            // We choose the buffer size depending on the baudrate
            // With small baud rates and large buffers the timeouts are long
            // -> So we choose a small buffer. The maximum buffer size is 10000!
            int SelectedBaudRate = pCom->ComboBaudCOM1.GetCurSel();

            // 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400
            if(SelectedBaudRate <= 7)
            {
                BufferSize = 128;
            }

            // 56000, 115200
            else
            {
                BufferSize = 1024;
            }

            buffer1 = new char[BufferSize + 1];
            buffer2 = new char[BufferSize + 1];
            BufferLength = BufferSize;

            //DWORD BufferLength = TransceiveBufferSize;
            ZeroMemory(buffer1, sizeof(buffer1));
            ZeroMemory(buffer2, sizeof(buffer2));

            // Inititalize COM-ports
            bool OkCOM1 = false;
            bool OkVirt = false;

            HANDLE hCOM1, hVirt;

            DCB dcb;
            CString baud, port, settings;
            int nCnt = 0;

            // Get the Selection of the 'Baud' combo box (COM1)
            pCom->ComboBaudCOM1.GetWindowText(baud);

            // Initialize COM1
            port.Format(L"\\\\.\\COM1");
            settings.Format(L"baud=%s parity=N data=8 stop=1\0", baud);

            hCOM1 = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
            if(hCOM1 != INVALID_HANDLE_VALUE)
            {
                if (GetCommState (hCOM1, &pCom->m_dcbOldCOM1))
                {
                    ZeroMemory(&dcb, sizeof(DCB));
                    dcb.DCBlength = sizeof(DCB);

                    if(BuildCommDCB(settings, &dcb) != 0)
                    { 
                        if(SetCommState(hCOM1, &dcb) != 0)
                        {
                            OkCOM1 = true;
                        }
                    }
                }
            }

            if((!OkCOM1) && (hCOM1 != INVALID_HANDLE_VALUE))
            { 
                CloseHandle(hCOM1);
                hCOM1 = INVALID_HANDLE_VALUE;
            }

            // Get the Selection of the 'Baud' combo box (virtual COM-port)
            pCom->ComboBaudVirt.GetWindowText(baud);

            // Initialize virtual COM-port
            port.Format(L"\\\\.\\COM%d", pCom->ComportNumber);
            settings.Format(L"baud=%s parity=N data=8 stop=1\0", baud);

            hVirt = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL); 
            if(hVirt != INVALID_HANDLE_VALUE)
            {
                if (GetCommState (hVirt, &pCom->m_dcbOldVirt))
                {
                    ZeroMemory(&dcb, sizeof(DCB));
                    dcb.DCBlength = sizeof(DCB);

                    if(BuildCommDCB(settings, &dcb) != 0)
                    { 
                        if(SetCommState(hVirt, &dcb) != 0)
                        {
                            OkVirt = true;
                        }
                    }
                }
            }

            if((!OkVirt) && (hVirt != INVALID_HANDLE_VALUE))
            { 
                CloseHandle(hVirt);
                hVirt = INVALID_HANDLE_VALUE;
            }

            // Start of send / receive loop (polling)
            if(OkCOM1)
            {
                PurgeComm (hCOM1, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

                if(OkVirt)
                {
                    PurgeComm (hVirt, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

                    // Open source file
                    pSourceFile = _tfopen(SourceName, L"rb");
                    if(pSourceFile != NULL)
                    {
                        // Open destination file
                        pDestFile = _tfopen(DestName, L"wb");
                        if(pDestFile != NULL)
                        {
                            // Read data from source file
                            NumberOfBytesReadFile = (DWORD)fread(buffer1, sizeof(char), BufferLength, pSourceFile);
                            while((NumberOfBytesReadFile != 0) && pCom->m_bThreadAliveSendVirtToCOM1)
                            {
                                ReadTotalFile += NumberOfBytesReadFile;

                                // Calculate the position of the progress bar
                                pos = (int)(((double)ReadTotalFile) / ((double)pCom->FileSize) * 100.0);
                                if(pos != oldpos)
                                {
                                    if(pCom->m_bDialogAlive)
                                    {
                                        pCom->ProgressBar.SetPos(pos);
                                        pCom->ProgressBar.Invalidate();
                                    }
                                }

                                // Send data to Virtual COM-port
                                if(WriteFile(hVirt, buffer1, NumberOfBytesReadFile, &NumberOfBytesWrittenVirt, NULL) != 0)
                                {	
                                    NumberOfBytesReadCOM1 = 0;
                                    NumberOfBytes = 0;

                                    // Read data at COM1
                                    while((NumberOfBytesReadCOM1 != NumberOfBytesWrittenVirt) && pCom->m_bThreadAliveSendVirtToCOM1)
                                    {
                                        ReadFile(hCOM1, buffer2, NumberOfBytesWrittenVirt, &NumberOfBytes, NULL);
                                        if(NumberOfBytes != 0)
                                        {
                                            // Write data to destination file
                                            fwrite(buffer2, sizeof(char), NumberOfBytes, pDestFile);
                                            NumberOfBytesReadCOM1 += NumberOfBytes;
                                        }
                                    }

                                    ReadTotalCOM1 += NumberOfBytesReadCOM1;

                                    // Update display
                                    if(pCom->m_bDialogAlive)
                                    {
                                        Str2.Format(L"%d %s", ReadTotalCOM1, Str1);
                                        pCom->LblDestBytes.SetWindowText(Str2);
                                        pCom->LblDestBytes.Invalidate();
                                    }
                                }

                                // Read new data from file
                                NumberOfBytesReadFile = (DWORD)fread(buffer1, sizeof(char), BufferLength, pSourceFile);

                                // Save old position of the progress bar
                                oldpos = pos;
                            }

                            fclose(pDestFile);
                        }

                        fclose(pSourceFile);
                    }

                    if (pCom->m_dcbOldVirt.DCBlength == 28)
                        SetCommState (hVirt, &pCom->m_dcbOldVirt);

                    CloseHandle(hVirt);
                    hVirt = INVALID_HANDLE_VALUE;
                }

                if (pCom->m_dcbOldCOM1.DCBlength == 28)
                    SetCommState (hCOM1, &pCom->m_dcbOldCOM1);

                CloseHandle(hCOM1);
                hCOM1 = INVALID_HANDLE_VALUE;
            }

            if(pCom->m_bDialogAlive)
            {	
                if(ReadTotalCOM1 == (long)pCom->FileSize) 
                {
                    Str2.LoadString(IDS_FILE_TRANSFER_COMPLETE);
                    AfxMessageBox(Str2, MB_ICONINFORMATION);
                }

                else
                {
                    Str2.LoadString(IDS_FILE_TRANSFER_INCOMPLETE);
                    AfxMessageBox(Str2, MB_ICONSTOP);
                }

                pCom->ProgressBar.SetPos(0);
                pCom->ProgressBar.Invalidate();
                pCom->LblDestBytes.SetWindowText(L"0");
                pCom->LblDestBytes.Invalidate();

                Str2.LoadString(IDS_SEND_FILE);
                pCom->BtnSend.SetWindowText(Str2);
            }

            delete buffer1;
            delete buffer2;
        }

        // Thread is finished
        pCom->m_bThreadAliveSendVirtToCOM1 = false;
    }

    return 1;
}


INT CuEyeComportDemoDlg::InitCamera (HIDS *hCam, HWND hWnd)
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


void CuEyeComportDemoDlg::OnBnClickedRadioGpioDirection()
{
    UpdateData(TRUE);

    if ((m_nGpioDirection == 0) ||
        (m_nGpioDirection == 1))
    {
        GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(FALSE);
    }
}


void CuEyeComportDemoDlg::OnBnClickedBtnConfigure()
{
    IO_GPIO_CONFIGURATION gpioConfig;
    gpioConfig.u32Gpio = IO_GPIO_1;

    // Configure COM-Port (GPIO1 is TX)
    if (m_nGpioDirection == 0)
    {
        gpioConfig.u32Configuration = IS_GPIO_COMPORT_TX;
    }
    /*
    // Configure COM-Port (GPIO1 is RX)
    else if (m_nGpioDirection == 1)
    {
    gpioConfig.u32Configuration = IS_GPIO_COMPORT_RX;
    }
    */

    CString Str1, Str2, Str3, Str4;
    /*
    Str1 = "The software configuration of the GPIO pins must match the electric connection of the input/output lines!";
    Str2 = "An inverted connection can cause the destruction of the GPIO pins!";
    Str3 = "Are you sure this is the correct configuration?";
    Str4.Format("%s\n\n%s\n\n%s", Str1, Str2, Str3);
    INT nRet = AfxMessageBox(Str4, MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2);
    */
    INT nRet = IDYES;

    if (nRet == IDYES)
    {
        nRet = is_IO(m_hCam,
            IS_IO_CMD_GPIOS_SET_CONFIGURATION,
            (void*)&gpioConfig,
            sizeof(gpioConfig));

        if (nRet != IS_SUCCESS)
        {
            Str1 = "The GPIO pins can not be configured!";
            AfxMessageBox(Str1, MB_ICONERROR);
        }
        else
        {
            GetDlgItem(IDC_BTN_CONFIGURE)->EnableWindow(FALSE);
            GetDlgItem(IDC_RADIO_GPIO1_TX)->EnableWindow(FALSE);
            GetDlgItem(IDC_RADIO_GPIO1_RX)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_CONNECT_VIRT)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_SEND_FILE)->EnableWindow(TRUE);
        }
    }
}



#pragma warning (default : 4996)


