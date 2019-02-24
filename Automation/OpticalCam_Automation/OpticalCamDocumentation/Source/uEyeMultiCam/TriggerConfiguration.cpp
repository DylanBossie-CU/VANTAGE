//==========================================================================//
//                                                                          //
//  Copyright (C) 2012 - 2018                                               //
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

// TriggerConfiguration.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Multi_Cam.h"
#include "TriggerConfiguration.h"

// CTriggerConfiguration-Dialogfeld

IMPLEMENT_DYNAMIC(CTriggerConfiguration, CPropertyPage)

CTriggerConfiguration::CTriggerConfiguration() :
CPropertyPage(CTriggerConfiguration::IDD),
m_pCam(NULL)
{

}

CTriggerConfiguration::~CTriggerConfiguration()
{

}

void CTriggerConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	// slider
	DDX_Control(pDX, IDC_SLIDER_TRIGGER_DELAY,   m_sliderTriggerDelay);
	DDX_Control(pDX, IDC_SLIDER_TRIGGER_TIMEOUT, m_sliderTriggerTimeout);

}


BEGIN_MESSAGE_MAP(CTriggerConfiguration, CPropertyPage)

	ON_WM_HSCROLL()

	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CTriggerConfiguration::OnBnClickedButtonDefault)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TRIGGER_DELAY, &CTriggerConfiguration::OnDeltaposSpinTriggerDelay)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TRIGGER_TIMEOUT, &CTriggerConfiguration::OnDeltaposSpinTriggerTimeout)

	ON_BN_CLICKED(IDC_RADIO_SOFTWARE, &CTriggerConfiguration::OnBnClickedRadioSoftware)
	ON_BN_CLICKED(IDC_RADIO_FALLING, &CTriggerConfiguration::OnBnClickedRadioFalling)
	ON_BN_CLICKED(IDC_RADIO_RISING, &CTriggerConfiguration::OnBnClickedRadioRising)

	ON_BN_CLICKED(IDC_CHECK_TRIGGER_DELAY, &CTriggerConfiguration::OnBnClickedCheckTriggerDelay)
END_MESSAGE_MAP()


void CTriggerConfiguration::SetCameraHandle(Camera *pCam)
{
    /* update the active camera handle */
	m_pCam = pCam;
}

// CTriggerConfiguration-Meldungshandler

BOOL CTriggerConfiguration::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControls();
	UpdateControls();

	return TRUE;
}

void CTriggerConfiguration::InitControls()
{
	// Helper variable
	CString strHelper;

	int nDelayMin;
	int nDelayMax;

	// set minimum delay
	nDelayMin = is_SetTriggerDelay(m_pCam->hCam, IS_GET_MIN_TRIGGER_DELAY);
	strHelper.Format(L"%d µs", nDelayMin);
	SetDlgItemText(IDC_STATIC_MIN_DELAY, strHelper);

	// set maximum delay
	nDelayMax = is_SetTriggerDelay(m_pCam->hCam, IS_GET_MAX_TRIGGER_DELAY);
	strHelper.Format(L"%d µs", nDelayMax);
	SetDlgItemText(IDC_STATIC_MAX_DELAY, strHelper);

	// Slider Range
	m_sliderTriggerDelay.SetRange(nDelayMin, nDelayMax);
	m_sliderTriggerTimeout.SetRange(100, 3600000);

	// Fill Edit
	int nDelay = m_sliderTriggerDelay.GetPos();
	SetDlgItemInt(IDC_EDIT_TRIGGER_DELAY, nDelay);

	int nTimeout = m_sliderTriggerTimeout.GetPos();
	SetDlgItemInt(IDC_EDIT_TRIGGER_TIMEOUT, nTimeout);

    /* get supported trigger modes */
    int nSupportedTriggerMode = is_SetExternalTrigger(m_pCam->hCam, IS_GET_SUPPORTED_TRIGGER_MODE);
    CButton *pButton = NULL;

    /* enable or disable trigger controls */
    pButton = (CButton*)GetDlgItem(IDC_RADIO_SOFTWARE);
    pButton->EnableWindow((nSupportedTriggerMode & IS_SET_TRIGGER_SOFTWARE) == IS_SET_TRIGGER_SOFTWARE);

    pButton = (CButton*)GetDlgItem(IDC_RADIO_FALLING);
    pButton->EnableWindow((nSupportedTriggerMode & IS_SET_TRIGGER_HI_LO) == IS_SET_TRIGGER_HI_LO);

    pButton = (CButton*)GetDlgItem(IDC_RADIO_RISING);
    pButton->EnableWindow((nSupportedTriggerMode & IS_SET_TRIGGER_LO_HI) == IS_SET_TRIGGER_LO_HI);
}

void CTriggerConfiguration::UpdateControls()
{
	int  nMode;
	UINT nTimeout;

	is_GetTimeout(m_pCam->hCam, IS_TRIGGER_TIMEOUT, &nTimeout);

    /* standard value */
    if (nTimeout == 0)
    {
        nTimeout = 100;
    }

    SetDlgItemInt(IDC_EDIT_TRIGGER_TIMEOUT, nTimeout);
    m_sliderTriggerTimeout.SetPos(nTimeout);
	
    /* update active trigger control */
	((CButton*)GetDlgItem(IDC_RADIO_SOFTWARE))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_RISING))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_FALLING))->SetCheck(false);

    /* get current trigger mode */
	nMode = is_SetExternalTrigger(m_pCam->hCam, IS_GET_EXTERNALTRIGGER);

	if ((nMode & IS_SET_TRIGGER_SOFTWARE) == IS_SET_TRIGGER_SOFTWARE)
	{
		((CButton*)GetDlgItem(IDC_RADIO_SOFTWARE))->SetCheck(true);
	}
	if ((nMode & IS_SET_TRIGGER_LO_HI) == IS_SET_TRIGGER_LO_HI)
	{
		((CButton*)GetDlgItem(IDC_RADIO_RISING))->SetCheck(true);
	}
	if ((nMode & IS_SET_TRIGGER_HI_LO) == IS_SET_TRIGGER_HI_LO)
	{
		((CButton*)GetDlgItem(IDC_RADIO_FALLING))->SetCheck(true);
	}

    /* receive trigger delay */
    int nTriggerDelay = is_SetTriggerDelay(m_pCam->hCam, IS_GET_TRIGGER_DELAY);

    /* update the trigger delay controls */
    CButton* pButtonTriggerDelay = (CButton*)GetDlgItem(IDC_CHECK_TRIGGER_DELAY);
    pButtonTriggerDelay->SetCheck(nTriggerDelay != 0);

    m_sliderTriggerDelay.EnableWindow(nTriggerDelay != 0);
    GetDlgItem(IDC_EDIT_TRIGGER_DELAY)->EnableWindow(nTriggerDelay != 0);
    GetDlgItem(IDC_SPIN_TRIGGER_DELAY)->EnableWindow(nTriggerDelay != 0);
}

void CTriggerConfiguration::OnBnClickedButtonDefault()
{
	// Default Settings
	int nDelayMin;
	int nRet = 0;

	// set minimum delay
	nDelayMin = is_SetTriggerDelay(m_pCam->hCam, IS_GET_MIN_TRIGGER_DELAY);
	SetDlgItemInt(IDC_EDIT_TRIGGER_DELAY, nDelayMin);
	m_sliderTriggerDelay.SetPos(nDelayMin);

	nRet = is_SetTriggerDelay(m_pCam->hCam, nDelayMin);

	// standard Timeout
	m_sliderTriggerTimeout.SetPos(100);
	SetDlgItemInt(IDC_EDIT_TRIGGER_TIMEOUT, 100);

	nRet = is_SetTimeout(m_pCam->hCam, IS_TRIGGER_TIMEOUT, 100);

	CButton* pButtonRising = (CButton*)GetDlgItem(IDC_RADIO_RISING);
	pButtonRising->SetCheck(false);

	CButton* pButtonFalling = (CButton*)GetDlgItem(IDC_RADIO_FALLING);
	pButtonFalling->SetCheck(false);

    // set software trigger active
	OnBnClickedRadioSoftware();

	// CheckBoxes
	CButton* pButtonTriggerDelay = (CButton*)GetDlgItem(IDC_CHECK_TRIGGER_DELAY);
	pButtonTriggerDelay->SetCheck(false);

	m_sliderTriggerDelay.EnableWindow(false);
	GetDlgItem(IDC_EDIT_TRIGGER_DELAY)->EnableWindow(false);
	GetDlgItem(IDC_SPIN_TRIGGER_DELAY)->EnableWindow(false);

	m_nDelay = 0;
	is_SetTriggerDelay(m_pCam->hCam, m_nDelay);

	UpdateControls();
}

void CTriggerConfiguration::OnDeltaposSpinTriggerDelay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    /* receive current delay */
	int nValue = GetDlgItemInt(IDC_EDIT_TRIGGER_DELAY);

    /* receive min/max delay */
	int nDelayMin = is_SetTriggerDelay(m_pCam->hCam, IS_GET_MIN_TRIGGER_DELAY);
	int nDelayMax = is_SetTriggerDelay(m_pCam->hCam, IS_GET_MAX_TRIGGER_DELAY);

	if (pNMUpDown->iDelta < 0)
	{
		if(nValue < nDelayMax)
		{
			++nValue;
		}
	}
	else
	{
		if(nValue > nDelayMin)
		{
			--nValue;
		}
	}

    /* set new value */
	int nRet = is_SetTriggerDelay(m_pCam->hCam, nValue);
	if (nRet == IS_SUCCESS)
	{
        /* update controls */
		m_sliderTriggerDelay.SetPos(nValue);
	    SetDlgItemInt(IDC_EDIT_TRIGGER_DELAY, nValue);
	}

	*pResult = 0;
}

void CTriggerConfiguration::OnDeltaposSpinTriggerTimeout(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    /* receive current timeout */
	int nValue = GetDlgItemInt(IDC_EDIT_TRIGGER_TIMEOUT);

	if (pNMUpDown->iDelta < 0)
	{
		if(nValue < 3600000)
		{
			nValue += 10;
		}

		if(nValue > 3600000)
		{
			nValue = 3600000;
		}
	}
	else
	{
		if(nValue > 100)
		{
			nValue -= 10;
			m_sliderTriggerTimeout.SetPos(nValue);
		}

		if(nValue < 100)
		{
			nValue = 100;
		}
	}

    /* set new value */
	int nRet = is_SetTimeout(m_pCam->hCam, IS_TRIGGER_TIMEOUT, nValue);
	if (nRet == IS_SUCCESS)
	{
        /* update controls */
		m_sliderTriggerTimeout.SetPos(nValue);
	    SetDlgItemInt(IDC_EDIT_TRIGGER_TIMEOUT, nValue);
	}

	*pResult = 0;
}

void CTriggerConfiguration::OnBnClickedRadioSoftware()
{
	int nRet = 0;

    /* force video stop */
    is_StopLiveVideo(m_pCam->hCam, IS_FORCE_VIDEO_STOP);

    /* set trigger mode */
    nRet = is_SetExternalTrigger(m_pCam->hCam, IS_SET_TRIGGER_SOFTWARE);
    if (nRet != IS_SUCCESS)
    {
        CString strMessage;
        strMessage.Format(L"Setting software trigger failed with: %d", nRet);
        AfxMessageBox(strMessage, MB_ICONERROR);
    }

    if (m_pCam->bLive)
    {
        is_CaptureVideo(m_pCam->hCam, IS_DONT_WAIT);
    }
}

void CTriggerConfiguration::OnBnClickedRadioFalling()
{
	int nRet = 0;

    /* force video stop */
    is_StopLiveVideo(m_pCam->hCam, IS_FORCE_VIDEO_STOP);

    /* set trigger mode */
    nRet = is_SetExternalTrigger(m_pCam->hCam, IS_SET_TRIGGER_HI_LO);
    if (nRet != IS_SUCCESS)
    {
        CString strMessage;
        strMessage.Format(L"Setting falling trigger failed with: %d", nRet);
        AfxMessageBox(strMessage, MB_ICONERROR);
    }

    if (m_pCam->bLive)
    {
        is_CaptureVideo(m_pCam->hCam, IS_DONT_WAIT);
    }
}

void CTriggerConfiguration::OnBnClickedRadioRising()
{
    int nRet = 0;

    /* force video stop */
    is_StopLiveVideo(m_pCam->hCam, IS_FORCE_VIDEO_STOP);

    /* set trigger mode */
    nRet = is_SetExternalTrigger(m_pCam->hCam, IS_SET_TRIGGER_LO_HI);
    if (nRet != IS_SUCCESS)
    {
        CString strMessage;
        strMessage.Format(L"Setting rising trigger failed with: %d", nRet);
        AfxMessageBox(strMessage, MB_ICONERROR);
    }

    if (m_pCam->bLive)
    {
        is_CaptureVideo(m_pCam->hCam, IS_DONT_WAIT);
    }
}

void CTriggerConfiguration::OnBnClickedCheckTriggerDelay()
{
    CButton* pButtonTriggerDelay = (CButton*)GetDlgItem(IDC_CHECK_TRIGGER_DELAY);

    BOOL bEnable = pButtonTriggerDelay->GetCheck();

    /* enable trigger delay controls */
    m_sliderTriggerDelay.EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_TRIGGER_DELAY)->EnableWindow(bEnable);
    GetDlgItem(IDC_SPIN_TRIGGER_DELAY)->EnableWindow(bEnable);

    /* set trigger delay value */
    is_SetTriggerDelay(m_pCam->hCam, bEnable == TRUE ? m_sliderTriggerDelay.GetPos() : 0);

    UpdateControls();
}

void CTriggerConfiguration::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (pScrollBar->GetDlgCtrlID())
	{	
	case IDC_SLIDER_TRIGGER_DELAY:
		{
			int nDelay = m_sliderTriggerDelay.GetPos();

			int nRet = is_SetTriggerDelay(m_pCam->hCam, nDelay);
			if (nRet == IS_SUCCESS)
			{
                /* update spin box */
				SetDlgItemInt(IDC_EDIT_TRIGGER_DELAY, nDelay);
			}
		}
		break;

	case IDC_SLIDER_TRIGGER_TIMEOUT:
		{
			int nTimeout = m_sliderTriggerTimeout.GetPos();

			int nRet = is_SetTimeout(m_pCam->hCam, IS_TRIGGER_TIMEOUT, nTimeout);
			if (nRet == IS_SUCCESS)
			{
                /* update spin box */
				SetDlgItemInt(IDC_EDIT_TRIGGER_TIMEOUT, nTimeout);
			}
		}
		break;
	}
}

BOOL CTriggerConfiguration::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_ESCAPE))
		{
			// avoid close on escape press
			return 0;
		}
		if ((pMsg->wParam == VK_RETURN))
		{
			if (GetFocus() == GetDlgItem(IDC_EDIT_TRIGGER_DELAY))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditDelay();

			}
			if (GetFocus() == GetDlgItem(IDC_EDIT_TRIGGER_TIMEOUT))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditTimeout();

			}

			// avoid close on return press
			return 1;
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CTriggerConfiguration::OnEnKillfocusEditDelay()
{
	int nRet = 0;
	int nDelayMin;
	int nDelayMax;

	nDelayMin = is_SetTriggerDelay(m_pCam->hCam, IS_GET_MIN_TRIGGER_DELAY);
	nDelayMax = is_SetTriggerDelay(m_pCam->hCam, IS_GET_MAX_TRIGGER_DELAY);

	int nValue = GetDlgItemInt(IDC_EDIT_TRIGGER_DELAY);
	
    /* check ranges */
	if (nValue > nDelayMax)
	{
		nValue = nDelayMax;
	}

	if (nValue < nDelayMin)
	{
		nValue = nDelayMin;
	}

    /* set new value */
	nRet = is_SetTriggerDelay(m_pCam->hCam, nValue);
	if (nRet != IS_SUCCESS)
	{
        /* update controls */
		m_sliderTriggerDelay.SetPos(nValue);
	    SetDlgItemInt(IDC_EDIT_TRIGGER_DELAY, nValue);
	}
}

void CTriggerConfiguration::OnEnKillfocusEditTimeout()
{
	int nRet = 0;

	int nValue = GetDlgItemInt(IDC_EDIT_TRIGGER_TIMEOUT);
	
    /* check ranges */
	if (nValue > 3600000)
	{
		nValue = 3600000;
	}

	if (nValue < 100)
	{
		nValue = 100;
	}

    /* set new value */
	nRet = is_SetTimeout(m_pCam->hCam, IS_TRIGGER_TIMEOUT, nValue);
	if (nRet != IS_SUCCESS)
	{
        /* update controls */
		SetDlgItemInt(IDC_EDIT_TRIGGER_TIMEOUT, nValue);
        m_sliderTriggerTimeout.SetPos(nValue);
	}
}
