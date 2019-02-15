
#pragma once

#include "ueye.h"
#include <vector>
#include "afxwin.h"

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

struct Camera
{
    Camera(void);

    INT AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel);
    INT FreeImageMems(void);
    INT InitSequence(void);
    INT ClearSequence(void);
    INT GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId);

    typedef std::vector<Memory> MemoryVector;
    MemoryVector m_vecMemory;
    const INT m_cnNumberOfSeqMemory;

    HIDS hCam;

    /* Sensor information */
    SENSORINFO sensorInfo;

    /* Camera information */
    CAMINFO cameraInfo;
};

class CuEyeCameraMemoryDlg : public CDialog
{
  private:
	Camera m_Camera;

	CComboBox m_CbIteration;
    CComboBox m_CbImage;
    
	HICON m_hIcon;

	RANGE_OF_VALUES_U32 m_rangeBurstSize;
	UINT m_u32BurstSize;

	UINT m_u32NumberImageBuffers;
	UINT m_u32NumberImageBuffersInUse;

	CWinThread* m_threadLoop;
	BOOL m_bThreadLoop;
	static UINT ThreadLoop(LPVOID pParam);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    void OpenCamera();
    void ExitCamera();

	/* Update */
    void UpdateCameraInformation();
    void UpdateImageRange();
	void UpdateIterations();
	void UpdateBurstSize();

	void EnableTriggerControls(BOOL bEnable);
	void EnableAllControlsExceptLoop(BOOL bEnable);

    LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);
    BOOL PreTranslateMessage(MSG* pMsg);

  public:

	CuEyeCameraMemoryDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_UEYECAMERAMEMORY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	/* Camera */
    afx_msg void OnClose();
	afx_msg void OnBnClickedBtnExit();
    afx_msg void OnBnClickedButtonStartCapture();

	/* Trigger mode */
	afx_msg void OnBnClickedRadioPreTriggerHiLo();
	afx_msg void OnBnClickedRadioPostTriggerHiLo();
	afx_msg void OnBnClickedRadioPostTriggerLoHi();
	afx_msg void OnBnClickedRadioPreTriggerLoHi();
	afx_msg void OnBnClickedButtonForceTrigger();

	/* Trigger burst */
	afx_msg void OnEnKillfocusEditTriggerBurst();

	/* Iteration */
	afx_msg void OnCbnSelchangeComboIteration();
	afx_msg void OnBnClickedButtonRelease();

	/* Image */
	afx_msg void OnCbnSelchangeComboImage();
	afx_msg void OnDeltaposSpinImageId(NMHDR *pNMHDR, LRESULT *pResult);	

	/* Loop */
	afx_msg void OnBnClickedBtnLoop();
};
