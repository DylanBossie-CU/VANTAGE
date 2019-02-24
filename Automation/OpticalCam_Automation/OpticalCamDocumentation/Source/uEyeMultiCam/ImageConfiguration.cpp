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

// ImageConfiguration.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Multi_Cam.h"
#include "ImageConfiguration.h"

// CImageConfiguration-Dialogfeld

IMPLEMENT_DYNAMIC(CImageConfiguration, CPropertyPage)

CImageConfiguration::CImageConfiguration(): 
CPropertyPage(CImageConfiguration::IDD),
m_pCam(NULL)
{

}

CImageConfiguration::~CImageConfiguration()
{

}

void CImageConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	// slider
	DDX_Control(pDX, IDC_SLIDER_GAIN_MASTER, m_sliderGain);
	DDX_Control(pDX, IDC_SLIDER_BL_OFFSET,	 m_sliderBlackLevel);
	DDX_Control(pDX, IDC_SLIDER_GAIN_RED,	 m_sliderRed);
	DDX_Control(pDX, IDC_SLIDER_GAIN_GREEN,  m_sliderGreen);
	DDX_Control(pDX, IDC_SLIDER_GAIN_BLUE,	 m_sliderBlue);
	DDX_Control(pDX, IDC_SLIDER_GAMMA,		 m_sliderGamma);

}

BEGIN_MESSAGE_MAP(CImageConfiguration, CPropertyPage)

	ON_WM_HSCROLL()

	ON_BN_CLICKED(IDC_GAINBOOST, &CImageConfiguration::OnBnClickedGainboost)
	ON_BN_CLICKED(IDC_CHECK_GAMMA, &CImageConfiguration::OnBnClickedCheckGamma)

	ON_NOTIFY(UDN_DELTAPOS, IDC_Gain,		&CImageConfiguration::OnDeltaposGain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_BlackLevel,	&CImageConfiguration::OnDeltaposBlacklevel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RedGain,	&CImageConfiguration::OnDeltaposRedgain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_GreenGain,	&CImageConfiguration::OnDeltaposGreengain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_BlueGain,	&CImageConfiguration::OnDeltaposBluegain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Gamma,		&CImageConfiguration::OnDeltaposGamma)

	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CImageConfiguration::OnBnClickedButtonDefault)

	ON_EN_KILLFOCUS(IDC_EDIT_MASTER, &CImageConfiguration::OnEnKillfocusEditMaster)
	ON_EN_KILLFOCUS(IDC_EDIT_BL_OFFSET, &CImageConfiguration::OnEnKillfocusEditBlOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_RED, &CImageConfiguration::OnEnKillfocusEditRed)
	ON_EN_KILLFOCUS(IDC_EDIT_GREEN, &CImageConfiguration::OnEnKillfocusEditGreen)
	ON_EN_KILLFOCUS(IDC_EDIT_BLUE, &CImageConfiguration::OnEnKillfocusEditBlue)
	ON_EN_KILLFOCUS(IDC_EDIT_GAMMA, &CImageConfiguration::OnEnKillfocusEditGamma)

END_MESSAGE_MAP()

BOOL CImageConfiguration::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControls();
	UpdateControls();

	return TRUE;
}

// CImageConfiguration-Meldungshandler
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void CImageConfiguration::SetCameraHandle(Camera *pCam)
{
	m_pCam = pCam;
}

void CImageConfiguration::InitControls()
{
	int nRet = 0;
	int nBlacklevelCaps;

	CButton* pButtonGamma = (CButton*)GetDlgItem(IDC_CHECK_GAMMA);

	INT nGamma = 100;
	is_Gamma(m_pCam->hCam, IS_GAMMA_CMD_GET, (void*)&nGamma, sizeof(nGamma));
	if (nGamma == 100)
	{
		pButtonGamma->SetCheck(FALSE);
	}
	else
	{
		pButtonGamma->SetCheck(TRUE);
	}

	m_sliderGamma.EnableWindow(pButtonGamma->GetCheck());
	GetDlgItem(IDC_EDIT_GAMMA)->EnableWindow(pButtonGamma->GetCheck());


	m_bBlackLevel = false;

	// Slider Range
	m_sliderGamma.SetRange(100, 220);
	m_sliderBlackLevel.SetRange(0 , 255);

	/* Enable/Disable gain controls */
    if(m_pCam->SensorInfo.bMasterGain != TRUE)
    {
        m_sliderGain.EnableWindow(0);
        GetDlgItem(IDC_EDIT_MASTER)->EnableWindow(0);
    }

    if(m_pCam->SensorInfo.bBGain != TRUE || m_pCam->SensorInfo.bGGain != TRUE || m_pCam->SensorInfo.bRGain != TRUE)
    {
        m_sliderBlue.EnableWindow(0);
        m_sliderGreen.EnableWindow(0);
        m_sliderRed.EnableWindow(0);

        GetDlgItem(IDC_EDIT_BLUE)->EnableWindow(0);
        GetDlgItem(IDC_EDIT_GREEN)->EnableWindow(0);
        GetDlgItem(IDC_EDIT_RED)->EnableWindow(0);
    }

    /* Enable/Disable blacklevel control */
	nRet = is_Blacklevel(m_pCam->hCam, IS_BLACKLEVEL_CMD_GET_CAPS, (void*) &nBlacklevelCaps, sizeof (nBlacklevelCaps));
	if(nRet == IS_SUCCESS)
	{
		BOOL bSetBlacklevelOffset = (nBlacklevelCaps & IS_BLACKLEVEL_CAP_SET_OFFSET) != 0;

		if(bSetBlacklevelOffset == 0)
		{
			m_sliderBlackLevel.EnableWindow(0);
			GetDlgItem(IDC_EDIT_BL_OFFSET)->EnableWindow(0);

			m_bBlackLevel = true;
		}
	}

    /* Enable/Disable gain boost control */
	nRet = is_SetGainBoost(m_pCam->hCam, IS_GET_SUPPORTED_GAINBOOST);
	if(nRet == IS_SUCCESS)
	{
		CButton* pButtonGainBoost = (CButton*)GetDlgItem(IDC_GAINBOOST);
		pButtonGainBoost->EnableWindow(false);
	}
}

void CImageConfiguration::UpdateControls()
{
	UpdateGainControls();
	UpdateGammaControls();
}

void CImageConfiguration::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Slider features
	switch (pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_GAIN_MASTER:
		{
			int nRet = 0;

			nRet = is_SetHardwareGain(m_pCam->hCam, m_sliderGain.GetPos(), IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
			if (nRet == IS_SUCCESS)
			{
				SetDlgItemInt(IDC_EDIT_MASTER, m_sliderGain.GetPos());
			}
		}
		break;

    case IDC_SLIDER_BL_OFFSET:
        {
            int nRet = 0;
            int nValue = m_sliderBlackLevel.GetPos();

            is_Blacklevel(m_pCam->hCam, IS_BLACKLEVEL_CMD_SET_OFFSET, (void*)&nValue, sizeof(nValue));

            if (nRet == IS_SUCCESS)
            {
                SetDlgItemInt(IDC_EDIT_BL_OFFSET, nValue);
            }
		}
		break;

	case IDC_SLIDER_GAIN_RED:
		{
			int nRet = 0;

			nRet = is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, m_sliderRed.GetPos(), IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
			if (nRet == IS_SUCCESS)
			{
				SetDlgItemInt(IDC_EDIT_RED,m_sliderRed.GetPos());
			}
		}
		break;

	case IDC_SLIDER_GAIN_GREEN:
		{
			int nRet = 0;

			nRet = is_SetHardwareGain(m_pCam->hCam,IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, m_sliderGreen.GetPos(), IS_IGNORE_PARAMETER);
			if (nRet == IS_SUCCESS)
			{
				SetDlgItemInt(IDC_EDIT_GREEN, m_sliderGreen.GetPos());
			}
		}
		break;

	case IDC_SLIDER_GAIN_BLUE:
		{
			int nRet = 0;

			nRet = is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, m_sliderBlue.GetPos());
			if (nRet == IS_SUCCESS)
			{
				SetDlgItemInt(IDC_EDIT_BLUE, m_sliderBlue.GetPos());
			}
		}
		break;

	case IDC_SLIDER_GAMMA:
		{
			int nRet = 0;

			INT nGamma = m_sliderGamma.GetPos();
			nRet = is_Gamma(m_pCam->hCam, IS_GAMMA_CMD_SET, (void*)&nGamma, sizeof(nGamma));
			if (nRet == IS_SUCCESS)
			{
				CString str;
				str.Format(L"%.2f", m_sliderGamma.GetPos() / 100.);
				SetDlgItemText(IDC_EDIT_GAMMA, str);
			}
		}
		break;

	default:
		break;
	}
}

void CImageConfiguration::OnBnClickedGainboost()
{
	int nRet = 0;

	CButton* pButtonGainBoost = (CButton*)GetDlgItem(IDC_GAINBOOST);
	int nMode = pButtonGainBoost->GetCheck() ? IS_SET_GAINBOOST_ON : IS_SET_GAINBOOST_OFF;

	nRet = is_SetGainBoost(m_pCam->hCam, nMode);
	if (nRet != IS_SUCCESS)
	{
		CString strMessage;
        strMessage.Format(L"Setting gain boost failed with: %d", nRet);
		AfxMessageBox(strMessage, MB_ICONERROR);
	}
}

void CImageConfiguration::OnBnClickedCheckGamma()
{
	CButton* pButtonGamma = (CButton*)GetDlgItem(IDC_CHECK_GAMMA);

    /* update controls */
    m_sliderGamma.EnableWindow(pButtonGamma->GetCheck());
	GetDlgItem(IDC_EDIT_GAMMA)->EnableWindow(pButtonGamma->GetCheck());

    /* enable == current slider position, disable == 100 */
	INT nGamma = pButtonGamma->GetCheck() ? m_sliderGamma.GetPos() : 100;
    is_Gamma(m_pCam->hCam, IS_GAMMA_CMD_SET, (void*)&nGamma, sizeof(nGamma));
}

void CImageConfiguration::UpdateGainControls()
{
	int nValue = 0;

	// Master Gain   updated slider
	nValue = is_SetHardwareGain(m_pCam->hCam, IS_GET_MASTER_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
	m_sliderGain.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_MASTER, nValue);

	// Red Gain   updated slider
	nValue = is_SetHardwareGain(m_pCam->hCam, IS_GET_RED_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
	m_sliderRed.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_RED, nValue);

	// Green Gain   updated slider
	nValue = is_SetHardwareGain(m_pCam->hCam, IS_GET_GREEN_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
	m_sliderGreen.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_GREEN, nValue);

	// Blue Gain   updated slider
	nValue = is_SetHardwareGain(m_pCam->hCam, IS_GET_BLUE_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
	m_sliderBlue.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_BLUE, nValue);

    // Black Level   updated slider
	is_Blacklevel(m_pCam->hCam, IS_BLACKLEVEL_CMD_GET_OFFSET, (void*)&nValue, sizeof(nValue));
	m_sliderBlackLevel.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_BL_OFFSET, nValue);
}

void CImageConfiguration::UpdateGammaControls()
{
    /* receive gamma value */
    INT nGamma;
	is_Gamma(m_pCam->hCam, IS_GAMMA_CMD_GET, (void*)&nGamma, sizeof(nGamma));

    /* update controls */
    m_sliderGamma.SetPos(nGamma);

	CString str;
	str.Format(L"%.2f", nGamma / 100.);
	SetDlgItemText(IDC_EDIT_GAMMA, str);
}

/////////////////////////////////////////////////////////////////////////////////////
//SpinBox Function
/////////////////////////////////////////////////////////////////////////////////////
void CImageConfiguration::OnDeltaposGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_MASTER);

	if (pNMUpDown->iDelta < 0)
	{
		if (nValue < 100)
		{
			++nValue;
		}
	}
	else
	{
		if(nValue > 0)
		{
			--nValue;
		}
	}

    /* set new gain */
    is_SetHardwareGain(m_pCam->hCam, nValue, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);

    /* update controls */
    m_sliderGain.SetPos(nValue);
    SetDlgItemInt(IDC_EDIT_MASTER, nValue);

	*pResult = 0;
}

void CImageConfiguration::OnDeltaposBlacklevel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_BL_OFFSET);

	if (pNMUpDown->iDelta < 0)
	{
		if(nValue < 255)
		{
			++nValue;
		}
	}
	else
	{
		if(nValue > 0)
		{
			--nValue;
		}
	}

    /* set new blacklevel */
	is_Blacklevel(m_pCam->hCam, IS_BLACKLEVEL_CMD_SET_OFFSET, (void*)&nValue, sizeof(nValue));

    /* update controls */
    m_sliderBlackLevel.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_BL_OFFSET, nValue);

	*pResult = 0;
}

void CImageConfiguration::OnDeltaposRedgain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_RED);

	if (pNMUpDown->iDelta < 0)
	{
		if(nValue < 100)
		{
			++nValue;
		}
	}
	else
	{
		if(nValue > 0)
		{
			--nValue;
		}
	}

    /* set new gain */
	is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, nValue, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);

    /* update controls */
    m_sliderRed.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_RED, nValue);

	*pResult = 0;
}

void CImageConfiguration::OnDeltaposGreengain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_GREEN);

	if (pNMUpDown->iDelta < 0)
	{
		if(nValue < 100)
		{
			++nValue;
		}
	}
	else
	{
		if(nValue > 0)
		{
			--nValue;
		}
	}

    /* update gain */
	is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, nValue, IS_IGNORE_PARAMETER);

    /* update controls */
    m_sliderGreen.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_GREEN, nValue);

	*pResult = 0;
}

void CImageConfiguration::OnDeltaposBluegain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_BLUE);

	if (pNMUpDown->iDelta < 0)
	{
		if(nValue < 100)
		{
			++nValue;
		}
	}
	else
	{
		if(nValue > 0)
		{
			--nValue;
		}
	}

    /* set new gain */
	is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, nValue);
	
    /* update controls */
    m_sliderBlue.SetPos(nValue);
	SetDlgItemInt(IDC_EDIT_BLUE, nValue);

	*pResult = 0;
}

void CImageConfiguration::OnDeltaposGamma(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    CString strGamma;
    GetDlgItemText(IDC_EDIT_GAMMA, strGamma);

    double dValue = wcstod(strGamma, NULL);

	if (pNMUpDown->iDelta < 0)
	{
		if(dValue < 2.2)
		{
			dValue += 0.01;
		}
	}
	else
	{
		if(dValue > 1.0)
		{
			dValue -= 0.01;
		}
	}

    /* update gamma */
	INT nGamma = (int)(dValue * 100.0);
    is_Gamma(m_pCam->hCam, IS_GAMMA_CMD_SET, (void*)&nGamma, sizeof(nGamma));

    /* update controls */
	strGamma.Format(L"%.2f", dValue);
	SetDlgItemText(IDC_EDIT_GAMMA, strGamma);

    m_sliderGamma.SetPos((int)(dValue * 100.0));

	*pResult = 0;
}

// Default Button, Setzt alle Einstellungen auf den Standart Wert zurück
void CImageConfiguration::OnBnClickedButtonDefault()
{
	// variable for error messages
	int nRet = 0;

	// Variable
	int nDefaultMasterGain	= 0;
	int nDefaultRedGain		= 0;
	int nDefaultGreenGain	= 0;
	int nDefaultBlueGain	= 0;
	int nDefaultBlackLevel	= 0;
	double dDefaultGamma	= 0;

	if (m_pCam->SensorInfo.bMasterGain == TRUE)
	{
		/* receive the default */
		nDefaultMasterGain = is_SetHardwareGain(m_pCam->hCam, IS_GET_DEFAULT_MASTER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);

        /* update slider */
		m_sliderGain.SetPos(nDefaultMasterGain);
		SetDlgItemInt(IDC_EDIT_MASTER, nDefaultMasterGain);

        /* set default */
		is_SetHardwareGain(m_pCam->hCam, nDefaultMasterGain, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
	}

	// Get the default value for the black level
	if(m_bBlackLevel == false)
	{
        nDefaultBlackLevel = 0;
		nRet = is_Blacklevel(m_pCam->hCam, IS_BLACKLEVEL_CMD_SET_OFFSET, (void*)&nDefaultBlackLevel, sizeof(nDefaultBlackLevel));
		if (nRet == IS_SUCCESS)
		{
		    SetDlgItemInt(IDC_EDIT_BL_OFFSET, nDefaultBlackLevel);
		    m_sliderBlackLevel.SetPos(nDefaultBlackLevel);	
		}
	}

	/* Disable gain boost */
    CButton* pButtonGainBoost = (CButton*)GetDlgItem(IDC_GAINBOOST);
	pButtonGainBoost->SetCheck(FALSE);

	is_SetGainBoost(m_pCam->hCam, IS_SET_GAINBOOST_OFF);

	// Get the default value for the red gian
	if (!(m_pCam->SensorInfo.bBGain != TRUE || m_pCam->SensorInfo.bGGain != TRUE || m_pCam->SensorInfo.bRGain != TRUE))
	{
        /* get default red gain */
		nDefaultRedGain = is_SetHardwareGain(m_pCam->hCam, IS_GET_DEFAULT_RED, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
		
		nRet = is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, nDefaultRedGain, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
		if (nRet == IS_SUCCESS)
		{
            /* update controls */
			m_sliderRed.SetPos(nDefaultRedGain);
		    SetDlgItemInt(IDC_EDIT_RED, nDefaultRedGain);
		}

		/* get default green gain */
		nDefaultGreenGain = is_SetHardwareGain(m_pCam->hCam, IS_GET_DEFAULT_GREEN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
		
		nRet = is_SetHardwareGain(m_pCam->hCam,IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, nDefaultGreenGain, IS_IGNORE_PARAMETER);
		if (nRet == IS_SUCCESS)
		{
            /* update controls */
			m_sliderGreen.SetPos(nDefaultGreenGain);
		    SetDlgItemInt(IDC_EDIT_GREEN, nDefaultGreenGain);
		}

		/* get default blue gain */
		nDefaultBlueGain = is_SetHardwareGain(m_pCam->hCam, IS_GET_DEFAULT_BLUE, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
		
		nRet = is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, nDefaultBlueGain);
		if (nRet == IS_SUCCESS)
		{
			/* update controls */
            m_sliderBlue.SetPos(nDefaultBlueGain);
		    SetDlgItemInt(IDC_EDIT_BLUE, nDefaultBlueGain);
		}
	}

	/* default gamma = 100 */
	INT nGamma = 100;
    nRet = is_Gamma(m_pCam->hCam, IS_GAMMA_CMD_SET, (void*)&nGamma, sizeof(nGamma));

    if (nRet == IS_SUCCESS)
    {
        /* update controls */
        CString strGamma;
        strGamma.Format(L"%.2f", 1.0);
        SetDlgItemText(IDC_EDIT_GAMMA, strGamma);

        m_sliderGamma.SetPos(100);

        /* enable controls */
        m_sliderGamma.EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_GAMMA)->EnableWindow(TRUE);

        CButton* pButtonGamma = (CButton*)GetDlgItem(IDC_CHECK_GAMMA);
        pButtonGamma->SetCheck(TRUE);
    }
}

void CImageConfiguration::OnEnKillfocusEditMaster()
{
	/* receive value */
    int nValue= GetDlgItemInt(IDC_EDIT_MASTER);

    /* check range */
    nValue = nValue > 100 ? 100 : nValue < 0 ? 0 : nValue;

    /* set new value */
    int nRet = is_SetHardwareGain(m_pCam->hCam, nValue, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    if (nRet == IS_SUCCESS)
    {
        /* update controls */
        m_sliderGain.SetPos(nValue);
        SetDlgItemInt(IDC_EDIT_MASTER, nValue);
    }
}

void CImageConfiguration::OnEnKillfocusEditBlOffset()
{
    /* receive value */
	int nValue= GetDlgItemInt(IDC_EDIT_BL_OFFSET);

    /* check range */
    nValue = nValue > 255 ? 255 : nValue < 0 ? 0 : nValue;

    /* set new value */
    int nRet = is_Blacklevel(m_pCam->hCam, IS_BLACKLEVEL_CMD_SET_OFFSET, (void*)&nValue, sizeof(nValue));
    if (nRet == IS_SUCCESS)
    {
        /* update controls */
        m_sliderBlackLevel.SetPos(nValue);
        SetDlgItemInt(IDC_EDIT_BL_OFFSET, nValue);
    }
}

void CImageConfiguration::OnEnKillfocusEditRed()
{
    /* receive value */
	int nValue= GetDlgItemInt(IDC_EDIT_RED);

    /* check range */
    nValue = nValue > 100 ? 100 : nValue < 0 ? 0 : nValue;

    /* set new value */
    int nRet = is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, nValue, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    if (nRet == IS_SUCCESS)
    {
        /* update controls */
        m_sliderRed.SetPos(nValue);
        SetDlgItemInt(IDC_EDIT_RED, nValue);
    }
}

void CImageConfiguration::OnEnKillfocusEditGreen()
{
    /* receive value */
    int nValue= GetDlgItemInt(IDC_EDIT_GREEN);

    /* check range */
    nValue = nValue > 100 ? 100 : nValue < 0 ? 0 : nValue;

    /* set new value */
    int nRet = is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, nValue, IS_IGNORE_PARAMETER);
    if (nRet == IS_SUCCESS)
    {
        /* update controls */
        m_sliderGreen.SetPos(nValue);
        SetDlgItemInt(IDC_EDIT_GREEN, nValue);
    }
}

void CImageConfiguration::OnEnKillfocusEditBlue()
{
    /* receive value */
    int nValue = GetDlgItemInt(IDC_EDIT_BLUE);

    /* check range */
    nValue = nValue > 100 ? 100 : nValue < 0 ? 0 : nValue;

    /* set new value */
    int nRet = is_SetHardwareGain(m_pCam->hCam, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, nValue);
    if (nRet == IS_SUCCESS)
    {
        /* update controls */
        m_sliderBlue.SetPos(nValue);
        SetDlgItemInt(IDC_EDIT_BLUE, nValue);
    }
}

void CImageConfiguration::OnEnKillfocusEditGamma()
{
	CString strGamma;
	GetDlgItemText(IDC_EDIT_GAMMA, strGamma);

	INT nValue = (INT)(wcstod(strGamma, NULL) * 100.0);

    /* check range */
    nValue = nValue > 220 ? 220 : nValue < 100 ? 100 : nValue;

	int nRet = is_Gamma(m_pCam->hCam, IS_GAMMA_CMD_SET, (void*)&nValue, sizeof(nValue));
    if (nRet == IS_SUCCESS)
    {
        /* update controls */
	    m_sliderGamma.SetPos(nValue);
        strGamma.Format(L"%.2f", nValue / 100);
    }
}


BOOL CImageConfiguration::PreTranslateMessage(MSG* pMsg)
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

			if (GetFocus() == GetDlgItem(IDC_EDIT_MASTER))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditMaster();
			}

			if (GetFocus() == GetDlgItem(IDC_EDIT_BL_OFFSET))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditBlOffset();
			}

			if (GetFocus() == GetDlgItem(IDC_EDIT_RED))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditRed();
			}

			if (GetFocus() == GetDlgItem(IDC_EDIT_GREEN))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditGreen();
			}

			if (GetFocus() == GetDlgItem(IDC_EDIT_BLUE))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditBlue();
			}

			if (GetFocus() == GetDlgItem(IDC_EDIT_GAMMA))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditGamma();
			}

			// avoid close on return press
			return 1;
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}