// DlgSerial.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "uEye Manager.h"
#include "uEye Manager Dlg.h"
#include "DlgSerial.h"
#include ".\dlgserial.h"
#include <strsafe.h>
#include "GetVersionInfo.h"

#define MaxSize1				4096	// Send buffer for file
#define MaxSize2				10000	// Receive buffer for file (max 10000)
#define TransceiveBufferSize	128		// Buffer for terminal


IMPLEMENT_DYNAMIC(DlgSerial, CDialog)
DlgSerial::DlgSerial(CWnd* pParent, Camera *C)
	: CDialog(DlgSerial::IDD, pParent)
{
	Cam = C;
}

DlgSerial::~DlgSerial()
{
}

void DlgSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BAUD_VIRT, ComboBaudVirt);
	DDX_Control(pDX, IDC_COMBO_BAUD_COM1, ComboBaudCOM1);
	DDX_Control(pDX, IDC_COMBO_DATA_VIRT, ComboDataVirt);
	DDX_Control(pDX, IDC_COMBO_DATA_COM1, ComboDataCOM1);
	DDX_Control(pDX, IDC_COMBO_STOP_VIRT, ComboStopVirt);
	DDX_Control(pDX, IDC_COMBO_STOP_COM1, ComboStopCOM1);
	DDX_Control(pDX, IDC_COMBO_PARITY_VIRT, ComboParityVirt);
	DDX_Control(pDX, IDC_COMBO_PARITY_COM1, ComboParityCOM1);
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
}


BEGIN_MESSAGE_MAP(DlgSerial, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CONNECT_VIRT, OnBnClickedBtnConnectVirt)
	ON_BN_CLICKED(IDC_BTN_SEND_VIRT, OnBnClickedBtnSendVirt)
	ON_BN_CLICKED(IDC_BTN_CLEAR_VIRT, OnBnClickedBtnClearVirt)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD_VIRT, OnCbnSelchangeComboBaudVirt)
	ON_BN_CLICKED(IDC_BTN_CONNECT_COM1, OnBnClickedBtnConnectCom1)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD_COM1, OnCbnSelchangeComboBaudCom1)
	ON_BN_CLICKED(IDC_BTN_SEND_COM1, OnBnClickedBtnSendCom1)
	ON_BN_CLICKED(IDC_BTN_CLEAR_COM1, OnBnClickedBtnClearCom1)
	ON_BN_CLICKED(IDC_BTN_SOURCE_FILE, OnBnClickedBtnSourceFile)
	ON_BN_CLICKED(IDC_BTN_DEST_FILE, OnBnClickedBtnDestFile)
	ON_BN_CLICKED(IDC_BTN_SEND_FILE, OnBnClickedBtnSendFile)
	ON_BN_CLICKED(IDC_CHECK_BROADCAST, OnBnClickedCheckBroadcast)
END_MESSAGE_MAP()


BOOL DlgSerial::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the title of the group box
	CString Str1, Str2, Str3;
	Str2.Format(" (COM%d)", Cam->getDeviceComportOffset() + COMPORTBASE);
	GetDlgItem(IDC_GRP_COMPORT_VIRT)->GetWindowText(StringTitleVirtualComport);

	Str1.Format("%s%s", StringTitleVirtualComport, Str2);
	GetDlgItem(IDC_GRP_COMPORT_VIRT)->SetWindowText(Str1);

	m_BroadcastComportNumber = GetBroadcastComportNumber();

	FillComboComport();
	FillComboDirection();
	FillComboAdditionalChar();

	hCOM1 = INVALID_HANDLE_VALUE;
	hVirt = INVALID_HANDLE_VALUE;

	m_bVirtOpen							= false;
	m_bCOM1Open							= false; 

	m_bThreadAliveVirt					= false;
	m_ThreadVirt						= NULL;

	m_bThreadAliveCOM1					= false;
	m_ThreadCOM1						= NULL;

	m_bThreadAliveSend					= false;
	m_ThreadSend						= NULL;

	m_bThreadAliveSendCOM1ToVirt		= false;
	m_ThreadSendCOM1ToVirt				= NULL;

	m_bThreadAliveSendVirtToCOM1		= false;
	m_ThreadSendVirtToCOM1				= NULL;

	m_bDialogAlive						= true;

	return TRUE;
}


HBRUSH DlgSerial::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void DlgSerial::OnBnClickedBtnSendVirt()
{
	SendMessageVirt();
}


void DlgSerial::OnBnClickedBtnSendCom1()
{
	SendMessageCOM1();
}


void DlgSerial::OnBnClickedOk()
{
	OnOK();
}


void DlgSerial::OnBnClickedBtnClearVirt()
{
	GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->SetWindowText("");
}


void DlgSerial::OnBnClickedBtnClearCom1()
{
	GetDlgItem(IDC_EDIT_RECEIVE_COM1)->SetWindowText("");
}


void DlgSerial::OnBnClickedBtnConnectVirt()
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


void DlgSerial::OnBnClickedBtnConnectCom1()
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

void DlgSerial::OnCbnSelchangeComboBaudVirt()
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


void DlgSerial::OnCbnSelchangeComboBaudCom1()
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


bool DlgSerial::OpenVirt()
{
	bool opened = false;
	DCB dcb;
	char port[20], settings[100];
	CString baud;
	int nCnt = 0;
	
	ComboBaudVirt.GetWindowText(baud);

	// Take the broadcast COM-port
	if(CheckboxBroadcastComport.GetCheck() == BST_CHECKED)
	{
		StringCchPrintf(port, sizeof(port), "\\\\.\\COM%d", m_BroadcastComportNumber);
	}

	// Take the COM-port from the camera EEPROM
	else
	{
		StringCchPrintf(port, sizeof(port), "\\\\.\\COM%d", Cam->getDeviceComportOffset() + COMPORTBASE);
	}
	
	
	StringCchPrintf(settings, sizeof(settings), "baud=%s parity=N data=8 stop=1\0", baud);
	
	hVirt = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL); 
	
	if(hVirt != INVALID_HANDLE_VALUE)
	{
		COMMTIMEOUTS lpTimeOuts;
        GetCommTimeouts (hVirt, &m_lpOldTimeOutsVirt);
		lpTimeOuts = m_lpOldTimeOutsVirt;

		lpTimeOuts.ReadIntervalTimeout         = MAXDWORD;
        lpTimeOuts.ReadTotalTimeoutMultiplier  = 0;
        lpTimeOuts.ReadTotalTimeoutConstant    = 0;
        lpTimeOuts.WriteTotalTimeoutMultiplier = 0;
        lpTimeOuts.WriteTotalTimeoutConstant   = 0;

		if (SetCommTimeouts (hVirt, &lpTimeOuts))
        {
			m_EventMask = EV_RXCHAR;
			if(SetCommMask(hVirt, m_EventMask) != 0)
			{
				if (GetCommState (hVirt, &m_dcbOldVirt))
				{
					SecureZeroMemory(&dcb, sizeof(DCB));
					dcb.DCBlength = sizeof(DCB);

					if(BuildCommDCB((char*)&settings, &dcb) != 0) 
					{ 
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
			Sleep    (10);
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

		if(m_hShutdownEventVirt != NULL)
			ResetEvent (m_hShutdownEventVirt);

		if (m_ovReadVirt.hEvent != NULL)
			ResetEvent (m_ovReadVirt.hEvent);

		if (m_ovWriteVirt.hEvent != NULL)
			ResetEvent (m_ovWriteVirt.hEvent);
	    
		m_hShutdownEventVirt = CreateEvent (NULL, TRUE, FALSE, NULL);
		m_ovReadVirt.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
		// we don't need the write event

		m_hEventArrayVirt[0] = m_hShutdownEventVirt;
		m_hEventArrayVirt[1] = m_ovReadVirt.hEvent;

		// Start thread
		m_ThreadVirt         = AfxBeginThread (ThreadVirt, this);
	}

	return opened;
}


bool DlgSerial::OpenCOM1()
{
	bool opened = false;
	DCB dcb;
	char port[20], settings[100];
	CString baud;
	int nCnt = 0;
	
	ComboBaudCOM1.GetWindowText(baud);

	StringCchPrintf(port, sizeof(port), "\\\\.\\COM1");
	StringCchPrintf(settings, sizeof(settings), "baud=%s parity=N data=8 stop=1\0", baud);

	hCOM1 = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL); 
	
	if(hCOM1 != INVALID_HANDLE_VALUE)
	{
		COMMTIMEOUTS lpTimeOuts;
        GetCommTimeouts (hCOM1, &m_lpOldTimeOutsCOM1);
		lpTimeOuts = m_lpOldTimeOutsCOM1;

		lpTimeOuts.ReadIntervalTimeout         = MAXDWORD;
        lpTimeOuts.ReadTotalTimeoutMultiplier  = 0;
        lpTimeOuts.ReadTotalTimeoutConstant    = 0;
        lpTimeOuts.WriteTotalTimeoutMultiplier = 0;
        lpTimeOuts.WriteTotalTimeoutConstant   = 0;

		if (SetCommTimeouts (hCOM1, &lpTimeOuts))
        {
			m_EventMask = EV_RXCHAR;
			if(SetCommMask(hCOM1, m_EventMask) != 0)
			{
				if (GetCommState (hCOM1, &m_dcbOldCOM1))
				{
					SecureZeroMemory(&dcb, sizeof(DCB));
					dcb.DCBlength = sizeof(DCB);

					if(BuildCommDCB((char*)&settings, &dcb) != 0) 
					{ 
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

		if(m_hShutdownEventCOM1 != NULL)
			ResetEvent (m_hShutdownEventCOM1);

		if (m_ovReadCOM1.hEvent != NULL)
			ResetEvent (m_ovReadCOM1.hEvent);

		if (m_ovWriteCOM1.hEvent != NULL)
			ResetEvent (m_ovWriteCOM1.hEvent);
	    
		m_hShutdownEventCOM1 = CreateEvent (NULL, TRUE, FALSE, NULL);
		m_ovReadCOM1.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
		// we don't need the write event

		m_hEventArrayCOM1[0] = m_hShutdownEventCOM1;
		m_hEventArrayCOM1[1] = m_ovReadCOM1.hEvent;

		// Start thread
		m_ThreadCOM1         = AfxBeginThread (ThreadCOM1, this);
	}

	return opened;
}


bool DlgSerial::CloseVirt()
{
	bool closed = false;
	int nCnt = 0;

	if(CloseHandle(hVirt) != 0)
	{
		while (m_bThreadAliveVirt && (nCnt < 50))
		{
			SetEvent (m_hShutdownEventVirt);
			Sleep    (10);
			nCnt++;
		}

		if (m_bThreadAliveVirt && (m_ThreadVirt != NULL))
			TerminateThread (m_ThreadVirt->m_hThread, 1);
  
		ResetEvent (m_hShutdownEventVirt);

		// Set values back
		SetCommTimeouts (hVirt, &m_lpOldTimeOutsVirt); 
		
		if (m_dcbOldVirt.DCBlength == 28)
			SetCommState (hVirt, &m_dcbOldVirt);

		hVirt = INVALID_HANDLE_VALUE;

		CloseHandle(m_hShutdownEventVirt);
		CloseHandle(m_ovReadVirt.hEvent);
		CloseHandle(m_ovWriteVirt.hEvent);

		m_hShutdownEventVirt = INVALID_HANDLE_VALUE;
		m_ovReadVirt.hEvent		 = INVALID_HANDLE_VALUE;
		m_ovWriteVirt.hEvent	 = INVALID_HANDLE_VALUE;

		m_hEventArrayVirt[0] = INVALID_HANDLE_VALUE;
		m_hEventArrayVirt[1] = INVALID_HANDLE_VALUE;

		closed = true;
	}

	return closed;
}


bool DlgSerial::CloseCOM1()
{
	bool closed = false;
	int nCnt = 0;

	if(CloseHandle(hCOM1) != 0)
	{
		while (m_bThreadAliveCOM1 && (nCnt < 50))
		{
			SetEvent (m_hShutdownEventCOM1);
			Sleep    (10);
			nCnt++;
		}

		if (m_bThreadAliveCOM1 && (m_ThreadCOM1 != NULL))
			TerminateThread (m_ThreadCOM1->m_hThread, 1);
  
		ResetEvent (m_hShutdownEventCOM1);

		// Set values back
		SetCommTimeouts (hCOM1, &m_lpOldTimeOutsCOM1); 
		    
		if (m_dcbOldCOM1.DCBlength == 28)
			SetCommState (hCOM1, &m_dcbOldCOM1);

		hCOM1 = INVALID_HANDLE_VALUE;

		CloseHandle(m_hShutdownEventCOM1);
		CloseHandle(m_ovReadCOM1.hEvent);
		CloseHandle(m_ovWriteCOM1.hEvent);

		m_hShutdownEventCOM1 = INVALID_HANDLE_VALUE;
		m_ovReadCOM1.hEvent	 = INVALID_HANDLE_VALUE;
		m_ovWriteCOM1.hEvent = INVALID_HANDLE_VALUE;

		m_hEventArrayCOM1[0] = INVALID_HANDLE_VALUE;
		m_hEventArrayCOM1[1] = INVALID_HANDLE_VALUE;

		closed = true;
	}

	return closed;
}


void DlgSerial::FillComboComport()
{
	ComboBaudVirt.InsertString(0, "1200");
	ComboBaudVirt.InsertString(1, "2400");
	ComboBaudVirt.InsertString(2, "4800");
	ComboBaudVirt.InsertString(3, "9600");
	ComboBaudVirt.InsertString(4, "14400");
	ComboBaudVirt.InsertString(5, "19200");
	ComboBaudVirt.InsertString(6, "25600");
	ComboBaudVirt.InsertString(7, "28800");
	ComboBaudVirt.InsertString(8, "38400");
	ComboBaudVirt.InsertString(9, "56000");
	ComboBaudVirt.InsertString(10, "57600");
	ComboBaudVirt.InsertString(11, "115200");
	ComboBaudVirt.InsertString(12, "128000");
	ComboBaudVirt.SetCurSel(11);

	ComboDataVirt.InsertString(0, "8");
	ComboDataVirt.SetCurSel(0);

	ComboStopVirt.InsertString(0, "1");
	ComboStopVirt.SetCurSel(0);

	CString Str;
	Str.LoadString(IDS_PARITY_NONE);
	ComboParityVirt.InsertString(0, Str);
	ComboParityVirt.SetCurSel(0);


	ComboBaudCOM1.InsertString(0, "1200");
	ComboBaudCOM1.InsertString(1, "2400");
	ComboBaudCOM1.InsertString(2, "4800");
	ComboBaudCOM1.InsertString(3, "9600");
	ComboBaudCOM1.InsertString(4, "14400");
	ComboBaudCOM1.InsertString(5, "19200");
	ComboBaudCOM1.InsertString(6, "25600");
	ComboBaudCOM1.InsertString(7, "28800");
	ComboBaudCOM1.InsertString(8, "38400");
	ComboBaudCOM1.InsertString(9, "56000");
	ComboBaudCOM1.InsertString(10, "57600");
	ComboBaudCOM1.InsertString(11, "115200");
	ComboBaudCOM1.InsertString(12, "128000");
	ComboBaudCOM1.SetCurSel(11);

	ComboDataCOM1.InsertString(0, "8");
	ComboDataCOM1.SetCurSel(0);

	ComboStopCOM1.InsertString(0, "1");
	ComboStopCOM1.SetCurSel(0);

	Str.LoadString(IDS_PARITY_NONE);
	ComboParityCOM1.InsertString(0, Str);
	ComboParityCOM1.SetCurSel(0);
}


void DlgSerial::OnDestroy()
{
	m_bDialogAlive = false;

	if(m_bThreadAliveSend)
	{
		m_bThreadAliveSend = false;
		WaitForSingleObject(m_ThreadSend->m_hThread, 2000);
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


BOOL DlgSerial::PreTranslateMessage(MSG *pMsg)
{
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
    {
		CWnd* pCurrWnd = GetFocus();
		if(pCurrWnd == GetDlgItem(IDC_EDIT_SEND_VIRT))
		{
			// Send a the string to the virtual COM-port
			SendMessageVirt();
		}

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


void DlgSerial::SendMessageVirt()
{
	CString Str1, Str2;
	GetDlgItem(IDC_EDIT_SEND_VIRT)->GetWindowText(Str1);

	DWORD NumberOfBytesWritten;
	char buffer[TransceiveBufferSize];
	ZeroMemory(buffer, sizeof(buffer));

	// Copy the maximum number of characters (without CR and LF)
	StringCchCopy(buffer, sizeof(buffer) - 2, Str1);

	size_t length = 0;
	StringCchLength(buffer, TransceiveBufferSize, &length);
	
	// Add CR
    if(ComboAdditionalCharVirt.GetCurSel() == 1)
	{
		buffer[length] = 13;
	}

	if(ComboAdditionalCharVirt.GetCurSel() == 2)
	{
		buffer[length] = 10; 
	}

	if(ComboAdditionalCharVirt.GetCurSel() == 3)
	{
		buffer[length]		= 13; 
		buffer[length + 1]	= 10; 
	}

	if(m_bVirtOpen)
	{
		if(WriteFile(hVirt, buffer, (DWORD)strlen(buffer), &NumberOfBytesWritten, &m_ovWriteVirt) == 0)
		{
			if(GetOverlappedResult (hVirt, &m_ovWriteVirt, &NumberOfBytesWritten, TRUE) == 0)
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


void DlgSerial::SendMessageCOM1()
{
	CString Str1, Str2;
	GetDlgItem(IDC_EDIT_SEND_COM1)->GetWindowText(Str1);

	DWORD NumberOfBytesWritten;
	char buffer[TransceiveBufferSize];
	ZeroMemory(buffer, sizeof(buffer));

	// Copy the maximum number of characters (without CR and LF)
	StringCchCopy(buffer, sizeof(buffer) - 2, Str1);

	size_t length = 0;
	StringCchLength(buffer, TransceiveBufferSize, &length);
	
	// Add CR
    if(ComboAdditionalCharCOM1.GetCurSel() == 1)
	{
		buffer[length] = 13;
	}

	if(ComboAdditionalCharCOM1.GetCurSel() == 2)
	{
		buffer[length] = 10; 
	}

	if(ComboAdditionalCharCOM1.GetCurSel() == 3)
	{
		buffer[length]		= 13; 
		buffer[length + 1]	= 10; 
	}

	if(m_bCOM1Open)
	{
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


void DlgSerial::ReceiveMessageVirt()
{
	CString Str1, Str2;
		
	DWORD NumberOfBytesRead = 0;
	char buffer[TransceiveBufferSize + 1];
	DWORD BufferLength = TransceiveBufferSize;

	ZeroMemory(buffer, sizeof(buffer));
	
	if(m_bVirtOpen)
	{
		if(ReadFile(hVirt, buffer, BufferLength, &NumberOfBytesRead, &m_ovReadVirt) == 0)
		{
			if(GetOverlappedResult (hVirt, &m_ovReadVirt, &NumberOfBytesRead, TRUE) != 0)
			{
				GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->GetWindowText(Str1);
				Str1.Append(buffer);
				GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->SetWindowText(Str1);
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
			Str1.Append(buffer);
			GetDlgItem(IDC_EDIT_RECEIVE_VIRT)->SetWindowText(Str1);
		}
	}

	else
	{
		Str2.LoadString(IDS_COMPORT_CONNECT_FIRST);
		AfxMessageBox(Str2, MB_ICONERROR);
	}
}


void DlgSerial::ReceiveMessageCOM1()
{
	CString Str1, Str2;

	DWORD NumberOfBytesRead = 0;
	char buffer[TransceiveBufferSize + 1];
	DWORD BufferLength = TransceiveBufferSize;
			
	ZeroMemory(buffer, sizeof(buffer));
	
	if(m_bCOM1Open)
	{
		if(ReadFile(hCOM1, buffer, BufferLength, &NumberOfBytesRead, &m_ovReadCOM1) == 0)
		{
			if(GetOverlappedResult (hCOM1, &m_ovReadCOM1, &NumberOfBytesRead, TRUE) != 0)
			{
				GetDlgItem(IDC_EDIT_RECEIVE_COM1)->GetWindowText(Str1);
				Str1.Append(buffer);
				GetDlgItem(IDC_EDIT_RECEIVE_COM1)->SetWindowText(Str1);
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
			Str1.Append(buffer);
			GetDlgItem(IDC_EDIT_RECEIVE_COM1)->SetWindowText(Str1);
		}
	}

	else
	{
		Str2.LoadString(IDS_COMPORT_CONNECT_FIRST);
		AfxMessageBox(Str2, MB_ICONERROR);
	}
}


UINT DlgSerial::ThreadVirt (LPVOID pParam)
{
	DlgSerial* pCom = (DlgSerial*) pParam;

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
			
			// If read event
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


UINT DlgSerial::ThreadCOM1 (LPVOID pParam)
{
	DlgSerial* pCom = (DlgSerial*) pParam;

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
			
			// If read event
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


void DlgSerial::OnBnClickedBtnSourceFile()
{
	CString SourceName;
	
	CFileDialog FileDlg(TRUE, NULL, NULL, 0, NULL);
	if(FileDlg.DoModal() == IDOK)
	{
		SourceName = FileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_SOURCE_FILE)->SetWindowText(SourceName);

		HANDLE file = CreateFile(SourceName, GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);  
		if(file)
		{
			LARGE_INTEGER size;
			if(GetFileSizeEx(file, &size))
			{
				FileSize = size.LowPart;
				CString Str1, Str2;
				Str1.LoadString(IDS_BYTES);
				Str2.Format("%d %s", FileSize, Str1); 
				LblSourceBytes.SetWindowText(Str2);
			}

			CloseHandle(file);
		}
	}
}


void DlgSerial::OnBnClickedBtnDestFile()
{
	CString DestName;
	
	CFileDialog FileDlg(TRUE, NULL, NULL, 0, NULL);
	if(FileDlg.DoModal() == IDOK)
	{
		DestName = FileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_DEST_FILE)->SetWindowText(DestName);

		LblDestBytes.SetWindowText("0");
		ProgressBar.SetPos(0);
	}
}


void DlgSerial::OnBnClickedBtnSendFile()
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
				CString Str;
				Str.LoadString(IDS_STOP);
				BtnSend.SetWindowText(Str);
				m_bThreadAliveSendCOM1ToVirt = true;
				m_ThreadSendCOM1ToVirt = AfxBeginThread (ThreadSendCOM1ToVirt, this);
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
				CString Str;
				Str.LoadString(IDS_STOP);
				BtnSend.SetWindowText(Str);
				m_bThreadAliveSendVirtToCOM1 = true;
				m_ThreadSendVirtToCOM1 = AfxBeginThread (ThreadSendVirtToCOM1, this);
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

/*
UINT DlgSerial::ThreadSend (LPVOID pParam)
{
	DlgSerial* pCom = (DlgSerial*) pParam;

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
			long ReadTotalVirt = 0;
			
			// Byte counter
			int count = 0;

			// Get the selection of the 'direction' combo box
			int direction = pCom->ComboDirection.GetCurSel();

			Str1.LoadString(IDS_BYTES);

			// Set the range of the progress bar to 0..100 and the step to 1
			pCom->ProgressBar.SetRange(0, 100);
			pCom->ProgressBar.SetStep(1);

			// Number of bytes read and written
			DWORD NumberOfBytesReadFile = 0;
			DWORD NumberOfBytesWrittenCOM1 = 0;
			DWORD NumberOfBytesReadCOM1 = 0;
			DWORD NumberOfBytesReadVirt = 0;
			DWORD NumberOfBytesWrittenVirt = 0;
			DWORD NumberOfBytes = 0;

			// Initialize Send/Receive buffer
			char *buffer1, *buffer2;
			DWORD BufferLength;

			if(direction == 0)
			{
				buffer1 = new char[MaxSize2 + 1];
				buffer2 = new char[MaxSize2 + 1];
				BufferLength = MaxSize2;
			}
			else
			{
				buffer1 = new char[MaxSize1 + 1];
				buffer2 = new char[MaxSize1 + 1];
				BufferLength = MaxSize1;
			}

			//DWORD BufferLength = TransceiveBufferSize;	
			ZeroMemory(buffer1, sizeof(buffer1));
			ZeroMemory(buffer2, sizeof(buffer2));

			// Inititalize COM-ports
			bool OkCOM1 = false;
			bool OkVirt = false;

			HANDLE hCOM1, hVirt;

			DCB dcb;
			char port[20], settings[100];
			CString baud;
			int nCnt = 0;
	
			// Get the Selection of the 'Baud' combo box (COM1)
			pCom->ComboBaudCOM1.GetWindowText(baud);

			// Initialize COM1
			StringCchPrintf(port, sizeof(port), "\\\\.\\COM1");
			StringCchPrintf(settings, sizeof(settings), "baud=%s parity=N data=8 stop=1\0", baud);
			
			hCOM1 = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL); 
	
			if(hCOM1 != INVALID_HANDLE_VALUE)
			{
				//if(SetCommMask(hCOM1, EV_TXEMPTY) != 0)
				{
					if (GetCommState (hCOM1, &pCom->m_dcbOldCOM1))
					{
						SecureZeroMemory(&dcb, sizeof(DCB));
						dcb.DCBlength = sizeof(DCB);

						if(BuildCommDCB((char*)&settings, &dcb) != 0) 
						{ 
							if(SetCommState(hCOM1, &dcb) != 0)
							{
								OkCOM1 = true;
							}
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
			StringCchPrintf(port, sizeof(port), "\\\\.\\COM%d", pCom->Cam->getDeviceComportOffset() + COMPORTBASE);
			StringCchPrintf(settings, sizeof(settings), "baud=%s parity=N data=8 stop=1\0", baud);
	
			hVirt = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL); 
	
			if(hVirt != INVALID_HANDLE_VALUE)
			{
				//if(SetCommMask(hVirt, EV_TXEMPTY) != 0)
				{
					if (GetCommState (hVirt, &pCom->m_dcbOldVirt))
					{
						SecureZeroMemory(&dcb, sizeof(DCB));
						dcb.DCBlength = sizeof(DCB);

						if(BuildCommDCB((char*)&settings, &dcb) != 0) 
						{ 
							if(SetCommState(hVirt, &dcb) != 0)
							{
								OkVirt = true;
							}
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
					pSourceFile = fopen(SourceName, "rb");
					if(pSourceFile != NULL)
					{
						// Open destination file
						pDestFile = fopen(DestName, "wb");
						if(pDestFile != NULL)
						{
							// Read data from source file
							NumberOfBytesReadFile = (DWORD)fread(buffer1, sizeof(char), BufferLength, pSourceFile);
							while((NumberOfBytesReadFile != 0) && pCom->m_bThreadAliveSend)
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

								// direction == 0  ->  Write data to COM1, read from virtual COM-port
								if(direction == 0)
								{
									// Send data to COM1
									if(WriteFile(hCOM1, buffer1, NumberOfBytesReadFile, &NumberOfBytesWrittenCOM1, NULL) != 0)
									{	
										NumberOfBytesReadCOM1 = 0;
										NumberOfBytes = 0;
														
										// Read data at virtual COM-port
										while((NumberOfBytesReadCOM1 != NumberOfBytesWrittenCOM1) && pCom->m_bThreadAliveSend)
										{
											ReadFile(hVirt, buffer2, NumberOfBytesWrittenCOM1, &NumberOfBytes, NULL);
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
											Str2.Format("%d %s", ReadTotalCOM1, Str1);
											pCom->LblDestBytes.SetWindowText(Str2);
											pCom->LblDestBytes.Invalidate();
										}
									}
								}

								// direction == 1  ->  Write data to virtual COM-port, read from COM1
								else if(direction == 1)
								{
									// Send data to virtual COM-port
									if(WriteFile(hVirt, buffer1, NumberOfBytesReadFile, &NumberOfBytesWrittenVirt, NULL) != 0)
									{
										NumberOfBytesReadVirt = 0;
										NumberOfBytes = 0;
										
										// Read data at COM1
										while((NumberOfBytesReadVirt != NumberOfBytesWrittenVirt) && pCom->m_bThreadAliveSend)
										{
											ReadFile(hCOM1, buffer2, NumberOfBytesWrittenVirt, &NumberOfBytes, NULL);
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
											Str2.Format("%d %s", ReadTotalVirt, Str1);
											pCom->LblDestBytes.SetWindowText(Str2);
											pCom->LblDestBytes.Invalidate();
										}
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

					// Read data in buffer that was not read before (clean buffer) 
					NumberOfBytes = -1;
					while(NumberOfBytes != 0)
						ReadFile(hVirt, buffer2, sizeof(buffer2), &NumberOfBytes, NULL);
	
					if (pCom->m_dcbOldVirt.DCBlength == 28)
						SetCommState (hVirt, &pCom->m_dcbOldVirt);

					CloseHandle(hVirt);
					hVirt = INVALID_HANDLE_VALUE;
				}

				//NumberOfBytes = -1;
				//while(NumberOfBytes != 0)
				//	ReadFile(hCOM1, buffer2, sizeof(buffer2), &NumberOfBytes, NULL);
											
				if (pCom->m_dcbOldCOM1.DCBlength == 28)
					SetCommState (hCOM1, &pCom->m_dcbOldCOM1);

				CloseHandle(hCOM1);
				hCOM1 = INVALID_HANDLE_VALUE;
			}
		
			if(pCom->m_bDialogAlive)
			{	
				if(((direction == 0) && (ReadTotalCOM1 == pCom->FileSize)) ||
				   ((direction == 1) && (ReadTotalVirt == pCom->FileSize)))
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
				pCom->LblDestBytes.SetWindowText("0");
				pCom->LblDestBytes.Invalidate();

				Str2.LoadString(IDS_SEND_FILE);
				pCom->BtnSend.SetWindowText(Str2);
			}

			delete buffer1;
			delete buffer2;
		}
	
		// Thread is finished
		pCom->m_bThreadAliveSend = false;
	}

	return 1;
}
*/

void DlgSerial::UpdateSendControls(bool Enable)
{
	ComboDirection.EnableWindow(Enable);
	BtnSend.EnableWindow(Enable);
	EditSourceFile.EnableWindow(Enable);
	EditDestFile.EnableWindow(Enable);
	ProgressBar.EnableWindow(Enable);
	LblSourceBytes.EnableWindow(Enable);
	LblDestBytes.EnableWindow(Enable);
	GetDlgItem(IDC_LBL_PROGRESS)->EnableWindow(Enable);
	GetDlgItem(IDC_BTN_SOURCE_FILE)->EnableWindow(Enable);
	GetDlgItem(IDC_BTN_DEST_FILE)->EnableWindow(Enable);
}


void DlgSerial::FillComboDirection()
{
	CString Str1, Str2, Str3;

	Str1.LoadString(IDS_SOURCE_FILE);
	Str2.LoadString(IDS_DESTINATION_FILE);

	Str3.Format("%s  ->  COM1  ->  COM%d  ->  %s", Str1, Cam->getDeviceComportOffset() + COMPORTBASE, Str2);
	ComboDirection.InsertString(0, Str3);

	Str3.Format("%s  ->  COM%d  ->  COM1  ->  %s", Str1, Cam->getDeviceComportOffset() + COMPORTBASE, Str2);
	ComboDirection.InsertString(1, Str3);

	ComboDirection.SetCurSel(0);
}


void DlgSerial::FillComboAdditionalChar()
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


int DlgSerial::GetBroadcastComportNumber()
{
	HKEY Key;
	DWORD dwType = REG_SZ;
	DWORD dwSize = MAX_REG_LEN;
	char Value[MAX_REG_LEN];
		
	int ComportNumber = 255;

	if(RegOpenKey(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\ueye\\Parameters", &Key) == ERROR_SUCCESS)
	{
		if(RegQueryValueEx(Key, "BroadcastComport" , NULL, &dwType, (LPBYTE)Value, &dwSize) == ERROR_SUCCESS)
		{
			ComportNumber = atoi(Value);
			if(ComportNumber == 0)
				ComportNumber = 255;
		}
	}

	return ComportNumber;
}


void DlgSerial::OnBnClickedCheckBroadcast()
{
	if(CheckboxBroadcastComport.GetCheck() == BST_CHECKED)
	{
		// Set the title of the group box
		CString Str1, Str2;
		Str2.Format(" (COM%d)", m_BroadcastComportNumber);
		Str1.Format("%s%s", StringTitleVirtualComport, Str2);
		GetDlgItem(IDC_GRP_COMPORT_VIRT)->SetWindowText(Str1);
	}

	else
	{
		// Set the title of the group box
		CString Str1, Str2;
		Str2.Format(" (COM%d)", Cam->getDeviceComportOffset() + COMPORTBASE);
		Str1.Format("%s%s", StringTitleVirtualComport, Str2);
		GetDlgItem(IDC_GRP_COMPORT_VIRT)->SetWindowText(Str1);
	}
}



UINT DlgSerial::ThreadSendCOM1ToVirt (LPVOID pParam)
{
	DlgSerial* pCom = (DlgSerial*) pParam;

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

			buffer1 = new char[MaxSize2 + 1];
			buffer2 = new char[MaxSize2 + 1];
			BufferLength = MaxSize2;

			//DWORD BufferLength = TransceiveBufferSize;	
			ZeroMemory(buffer1, sizeof(buffer1));
			ZeroMemory(buffer2, sizeof(buffer2));

			// Inititalize COM-ports
			bool OkCOM1 = false;
			bool OkVirt = false;

			HANDLE hCOM1, hVirt;

			DCB dcb;
			char port[20], settings[100];
			CString baud;
			int nCnt = 0;
	
			// Get the Selection of the 'Baud' combo box (COM1)
			pCom->ComboBaudCOM1.GetWindowText(baud);

			// Initialize COM1
			StringCchPrintf(port, sizeof(port), "\\\\.\\COM1");
			StringCchPrintf(settings, sizeof(settings), "baud=%s parity=N data=8 stop=1\0", baud);
			
			hCOM1 = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL); 
			if(hCOM1 != INVALID_HANDLE_VALUE)
			{
				if (GetCommState (hCOM1, &pCom->m_dcbOldCOM1))
				{
					SecureZeroMemory(&dcb, sizeof(DCB));
					dcb.DCBlength = sizeof(DCB);

					if(BuildCommDCB((char*)&settings, &dcb) != 0) 
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
			StringCchPrintf(port, sizeof(port), "\\\\.\\COM%d", pCom->Cam->getDeviceComportOffset() + COMPORTBASE);
			StringCchPrintf(settings, sizeof(settings), "baud=%s parity=N data=8 stop=1\0", baud);
	
			hVirt = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL); 
			if(hVirt != INVALID_HANDLE_VALUE)
			{
				if (GetCommState (hVirt, &pCom->m_dcbOldVirt))
				{
					SecureZeroMemory(&dcb, sizeof(DCB));
					dcb.DCBlength = sizeof(DCB);

					if(BuildCommDCB((char*)&settings, &dcb) != 0) 
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
					pSourceFile = fopen(SourceName, "rb");
					if(pSourceFile != NULL)
					{
						// Open destination file
						pDestFile = fopen(DestName, "wb");
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
										Str2.Format("%d %s", ReadTotalVirt, Str1);
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

					// Read all remaining characters in the buffer
					NumberOfBytes = -1;
					while(NumberOfBytes != 0)
					{	
						ReadFile(hVirt, buffer2, sizeof(buffer2), &NumberOfBytes, NULL);
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
				if(ReadTotalVirt == pCom->FileSize) 
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
				pCom->LblDestBytes.SetWindowText("0");
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




UINT DlgSerial::ThreadSendVirtToCOM1 (LPVOID pParam)
{
	DlgSerial* pCom = (DlgSerial*) pParam;

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

			buffer1 = new char[MaxSize1 + 1];
			buffer2 = new char[MaxSize1 + 1];
			BufferLength = MaxSize1;

			//DWORD BufferLength = TransceiveBufferSize;	
			ZeroMemory(buffer1, sizeof(buffer1));
			ZeroMemory(buffer2, sizeof(buffer2));

			// Inititalize COM-ports
			bool OkCOM1 = false;
			bool OkVirt = false;

			HANDLE hCOM1, hVirt;

			DCB dcb;
			char port[20], settings[100];
			CString baud;
			int nCnt = 0;
	
			// Get the Selection of the 'Baud' combo box (COM1)
			pCom->ComboBaudCOM1.GetWindowText(baud);

			// Initialize COM1
			StringCchPrintf(port, sizeof(port), "\\\\.\\COM1");
			StringCchPrintf(settings, sizeof(settings), "baud=%s parity=N data=8 stop=1\0", baud);
			
			hCOM1 = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL); 
			if(hCOM1 != INVALID_HANDLE_VALUE)
			{
				if (GetCommState (hCOM1, &pCom->m_dcbOldCOM1))
				{
					SecureZeroMemory(&dcb, sizeof(DCB));
					dcb.DCBlength = sizeof(DCB);

					if(BuildCommDCB((char*)&settings, &dcb) != 0) 
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
			StringCchPrintf(port, sizeof(port), "\\\\.\\COM%d", pCom->Cam->getDeviceComportOffset() + COMPORTBASE);
			StringCchPrintf(settings, sizeof(settings), "baud=%s parity=N data=8 stop=1\0", baud);
	
			hVirt = CreateFile(port, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL); 
			if(hVirt != INVALID_HANDLE_VALUE)
			{
				if (GetCommState (hVirt, &pCom->m_dcbOldVirt))
				{
					SecureZeroMemory(&dcb, sizeof(DCB));
					dcb.DCBlength = sizeof(DCB);

					if(BuildCommDCB((char*)&settings, &dcb) != 0) 
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
					pSourceFile = fopen(SourceName, "rb");
					if(pSourceFile != NULL)
					{
						// Open destination file
						pDestFile = fopen(DestName, "wb");
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
										Str2.Format("%d %s", ReadTotalCOM1, Str1);
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
				if(ReadTotalCOM1 == pCom->FileSize) 
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
				pCom->LblDestBytes.SetWindowText("0");
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