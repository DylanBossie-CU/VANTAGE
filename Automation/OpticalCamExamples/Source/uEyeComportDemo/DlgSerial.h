#pragma once

class DlgSerial : public CDialog
{
	DECLARE_DYNAMIC(DlgSerial)

private:

	Camera *Cam;
	HANDLE hVirt, hCOM1;
	CComboBox ComboBaudVirt, ComboDataVirt, ComboStopVirt, ComboParityVirt;
	CComboBox ComboBaudCOM1, ComboDataCOM1, ComboStopCOM1, ComboParityCOM1;
	CComboBox ComboDirection, ComboAdditionalCharVirt, ComboAdditionalCharCOM1;
	CButton BtnConnectVirt, BtnConnectCOM1, BtnSend;
	CEdit EditSourceFile, EditDestFile;
	CStatic LblSourceBytes, LblDestBytes;
	CProgressCtrl ProgressBar;
	CButton CheckboxBroadcastComport;
	CString StringTitleVirtualComport;

	int				m_BroadcastComportNumber;

	bool			m_bDialogAlive;

	ULONG			FileSize;

	bool			m_bVirtOpen, m_bCOM1Open;
	bool			m_bThreadAliveVirt, m_bThreadAliveCOM1;
	bool			m_bThreadAliveSend, m_bThreadAliveSendCOM1ToVirt, m_bThreadAliveSendVirtToCOM1;

	DWORD			m_EventMask;
	
	HANDLE			m_hEventArrayVirt[2], m_hEventArrayCOM1[2];;
	HANDLE			m_hShutdownEventVirt, m_hShutdownEventCOM1;

	CWinThread		*m_ThreadVirt, *m_ThreadCOM1;
	CWinThread		*m_ThreadSend, *m_ThreadSendCOM1ToVirt, *m_ThreadSendVirtToCOM1;
	
	COMMTIMEOUTS	m_lpOldTimeOutsVirt, m_lpOldTimeOutsCOM1;
	DCB				m_dcbOldVirt, m_dcbOldCOM1;
    OVERLAPPED		m_ovReadVirt, m_ovReadCOM1;
	OVERLAPPED      m_ovWriteVirt, m_ovWriteCOM1;

	static UINT ThreadVirt (LPVOID pParam);
	static UINT ThreadCOM1 (LPVOID pParam);
	static UINT ThreadSend (LPVOID pParam);
	static UINT ThreadSendCOM1ToVirt (LPVOID pParam);
	static UINT ThreadSendVirtToCOM1 (LPVOID pParam);

	// Regular Expression
	CAtlRegExp<> RegularExpression;
	CAtlREMatchContext<> MatchedContext;
	const CAtlREMatchContext<>::RECHAR* MatchedContextStart;
	const CAtlREMatchContext<>::RECHAR* MatchedContextEnd;

	bool OpenVirt();
	bool CloseVirt();
	void SendMessageVirt();
	void ReceiveMessageVirt();
	
	bool OpenCOM1();
	bool CloseCOM1();
	void SendMessageCOM1();
	void ReceiveMessageCOM1();

	void FillComboComport();
	void FillComboDirection();
	void FillComboAdditionalChar();

	void UpdateSendControls(bool Enable);

	int  GetBroadcastComportNumber();

	virtual BOOL OnInitDialog();

public:
	DlgSerial(CWnd* pParent = NULL, Camera* C = NULL);   // Standardkonstruktor
	virtual ~DlgSerial();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()

	enum { IDD = IDD_DLGSERIAL };
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg BOOL PreTranslateMessage(MSG *pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnConnectVirt();
	afx_msg void OnBnClickedBtnSendVirt();
	afx_msg void OnBnClickedBtnClearVirt();
	afx_msg void OnCbnSelchangeComboBaudVirt();
	afx_msg void OnBnClickedBtnConnectCom1();
	afx_msg void OnCbnSelchangeComboBaudCom1();
	afx_msg void OnBnClickedBtnSendCom1();
	afx_msg void OnBnClickedBtnClearCom1();
	afx_msg void OnBnClickedBtnSourceFile();
	afx_msg void OnBnClickedBtnDestFile();
	afx_msg void OnBnClickedBtnSendFile();
	afx_msg void OnBnClickedCheckBroadcast();
};
