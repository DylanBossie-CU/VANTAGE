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

// CameraConfiguration.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Multi_Cam.h"
#include "CameraConfiguration.h"

// CCameraConfiguration-Dialogfeld

IMPLEMENT_DYNAMIC(CCameraConfiguration, CPropertyPage)

CCameraConfiguration::CCameraConfiguration(): 
CPropertyPage(CCameraConfiguration::IDD),
m_pCam(NULL)
{

}

CCameraConfiguration::~CCameraConfiguration()
{

}

void CCameraConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	// Slider
	DDX_Control(pDX, IDC_SLIDER_PIXELCLOCK, m_sliderPixelClock);
	DDX_Control(pDX, IDC_SLIDER_FRAMEDELAY, m_sliderFrame);
	DDX_Control(pDX, IDC_SLIDER_EXPOSURE, m_sliderExpousure);
}

BEGIN_MESSAGE_MAP(CCameraConfiguration, CPropertyPage)

	ON_WM_HSCROLL()
	// SpinBox
	ON_NOTIFY(UDN_DELTAPOS, IDC_PixelClock, &CCameraConfiguration::OnDeltaposPixelclock)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FrameRate, &CCameraConfiguration::OnDeltaposFramerate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Time, &CCameraConfiguration::OnDeltaposTime)

	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CCameraConfiguration::OnBnClickedButtonDefault)

	ON_EN_KILLFOCUS(IDC_EDIT_PIXELCLOCK, &CCameraConfiguration::OnEnKillfocusEditPixelclock)
	ON_EN_KILLFOCUS(IDC_EDIT_FRAMERATE, &CCameraConfiguration::OnEnKillfocusEditFramerate)
	ON_EN_KILLFOCUS(IDC_EDIT_EXPOSURE, &CCameraConfiguration::OnEnKillfocusEditExposure)

END_MESSAGE_MAP()

BOOL CCameraConfiguration::OnInitDialog()
{
	CDialog::OnInitDialog();

    memset(&m_nPixelClockList, 0, sizeof(m_nPixelClockList));

    /* receive the pixelclock number count */
    is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_GET_NUMBER, (void*)&m_nNumberOfSupportedPixelClocks, sizeof(m_nNumberOfSupportedPixelClocks));

    /* receive the pixelclock list */
    is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_GET_LIST, (void*)m_nPixelClockList, 
        m_nNumberOfSupportedPixelClocks * sizeof(UINT));

    /* update the controls */
	UpdateControls();

	return TRUE;
}

void CCameraConfiguration::SetCameraHandle(Camera *pCam)
{
	/* set the active camera handle */
	m_pCam = pCam;
}

void CCameraConfiguration::UpdateControls()
{
	/* update pixel clock */
	UpdatePixelClock();

    /*  UpdatePixelClock() calls --> UpdateFramerate
     *  UpdateFramerate() calls --> UpdateExposure 
     */
}

void CCameraConfiguration::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nRet = 0;

	switch (pScrollBar->GetDlgCtrlID())
	{	
	case IDC_SLIDER_FRAMEDELAY:
		{
            /* update the framerate */
			double dFps = 0, dTemp = 0;
			dFps = m_sliderFrame.GetPos();
			dFps /= 100;

            /* set new framerate */
			nRet = is_SetFrameRate(m_pCam->hCam, dFps, &dTemp);
			if (nRet == IS_SUCCESS)
			{
			    UpdateFramerate();
			}
		}
		break;

	case IDC_SLIDER_PIXELCLOCK:
		{
            /* update pixel clock */
			m_nPixelClock = m_sliderPixelClock.GetPos();
			UpdateNextPixelclock();

            /* set new pixel clock */
			nRet = is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_SET, (void*)&m_nPixelClock, sizeof(m_nPixelClock));
			if (nRet == IS_SUCCESS)
			{
				UpdatePixelClock();
			}
		}
		break;

	case IDC_SLIDER_EXPOSURE:
		{
            /* calcualte new exposure value */
			int nRange = m_sliderExpousure.GetRangeMax();
			double dblExp = m_dblMinExp + (((double)m_sliderExpousure.GetPos() / (double)(nRange)) * (m_dblMaxExp - m_dblMinExp));

            /* set new exposure */
			nRet = is_Exposure(m_pCam->hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dblExp, sizeof(dblExp));
			if (nRet == IS_SUCCESS)
			{
                UpdateExposure();
			}
		}
		break;

	default:

		break;
	}
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCameraConfiguration::UpdatePixelClock()
{
	int nRet = 0;

    /* receive pixel clock range */
	int pixelClockRange[3];
	nRet = is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)pixelClockRange, sizeof(pixelClockRange));

    m_nMinPixelclock = pixelClockRange[0];
	m_nMaxPixelclock = pixelClockRange[1];

    /* set slider range */
	m_sliderPixelClock.SetRange(m_nMinPixelclock, m_nMaxPixelclock);

    CString strHelper;

	/* set minimum */
	strHelper.Format(L"%d Mhz",m_nMinPixelclock);
	SetDlgItemText(IDC_PIXEL_CLOCK_MIN, strHelper);

	/* set maximum */
	strHelper.Format(L"%d Mhz", m_nMaxPixelclock);
	SetDlgItemText(IDC_PIXEL_CLOCK_MAX, strHelper);

    /* get current pixel clock */
	nRet = is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_GET, (void*)&m_nPixelClock, sizeof(m_nPixelClock));
	if (nRet == IS_SUCCESS)
	{
        /* update controls */
		m_sliderPixelClock.SetPos(m_nPixelClock);
        SetDlgItemInt(IDC_EDIT_PIXELCLOCK, m_nPixelClock);
	}

    /* update framerate */
	UpdateFramerate();
}

void CCameraConfiguration::UpdateFramerate()
{	
	double dFpsMin = 0.0;
	double dFpsMax = 0.0;
	double dFpsInc = 0.0;

	int nRet = 0;

    /* get framerate range */
	nRet = is_GetFrameTimeRange(m_pCam->hCam, &dFpsMin, &dFpsMax, &dFpsInc);
	if (nRet == IS_SUCCESS)
	{
		m_sliderFrame.SetRange((int)(1 / dFpsMax * 100 ) , (int)(1 / dFpsMin * 100), TRUE);

        CString strTemp;
		/* set minimum */
		strTemp.Format(L"%.2f", 1 / dFpsMax);
		SetDlgItemText(IDC_STATIC_FRAMERATE_MIN, strTemp);

		/* set maximum */
		strTemp.Format(L"%.2f", 1 / dFpsMin);
		SetDlgItemText(IDC_STATIC_FRAMERATE_MAX, strTemp);

		/* get the current frametime */
		double dFps = 0;

		nRet = is_SetFrameRate(m_pCam->hCam, IS_GET_FRAMERATE, &dFps);
        if (nRet == IS_SUCCESS)
        {
            /* update controls */
            m_sliderFrame.SetPos((int)(dFps * 100));

            strTemp.Format(L"%.2f", dFps);
            SetDlgItemText(IDC_EDIT_FRAMERATE, strTemp);
        }

        /* update exposure */
	    UpdateExposure();	
	}
}

void CCameraConfiguration::UpdateExposure()
{   
    int nRet = 0;

    /* get exposure range */
    double dExpsoureRange[3];
    nRet = is_Exposure(m_pCam->hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE, (void*)&dExpsoureRange, sizeof(dExpsoureRange));
    if (nRet == IS_SUCCESS)
    {
        /* min, max, inc */
        m_dblMinExp = dExpsoureRange[0];
        m_dblMaxExp = dExpsoureRange[1];
        m_dblIncExp = dExpsoureRange[2];

        int nMaxSteps = (int)((m_dblMaxExp - m_dblMinExp) / m_dblIncExp + 0.0005);

        /* set range */
        m_sliderExpousure.SetRange((int)m_dblMinExp, nMaxSteps, TRUE);

        CString strTemp;
        /* set minimum */
        strTemp.Format(L"%.2f", m_dblMinExp);
        GetDlgItem(IDC_STATIC_EXPOSURE_MIN)->SetWindowText(strTemp);

        /* set maximum */
        strTemp.Format(L"%.2f", m_dblMaxExp);
        GetDlgItem(IDC_STATIC_EXPOSURE_MAX)->SetWindowText(strTemp);

        /* get current exposure */
        double dExposure;
        nRet = is_Exposure(m_pCam->hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&dExposure, sizeof(dExposure));
        if (nRet == IS_SUCCESS)
        {
            /* update controls */
            int value = (int)(((dExposure - m_dblMinExp) / m_dblIncExp) + 0.5);
            m_sliderExpousure.SetPos(value);

            CString strExposure;
            strExposure.Format(L"%.2f", dExposure);
            GetDlgItem(IDC_EDIT_EXPOSURE)->SetWindowText(strExposure);
        }
    }
}

void CCameraConfiguration::OnDeltaposPixelclock(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nRet = 0;

    /* get the pixel clock range */
    int nPixelClockRange[3];
	nRet = is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)nPixelClockRange, sizeof(nPixelClockRange));

    /* min,max */
    int nPixelClockMin = nPixelClockRange[0];
    int nPixelClockMax = nPixelClockRange[1];

	int nPixelClock = GetDlgItemInt(IDC_EDIT_PIXELCLOCK);

	if (pNMUpDown->iDelta < 0)
	{
        // increment
        m_nPixelClock = nPixelClock + 1 < nPixelClockMax ? nPixelClock + 1 : nPixelClockMax;
        UpdateNextPixelclock();
	}
	else
	{
        // decrement
		m_nPixelClock = nPixelClock - 1 > nPixelClockMin ? nPixelClock - 1 : nPixelClockMin;
        UpdatePrevPixelclock();
	}

    // set new pixelclock
	nRet = is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_SET, (void*)&m_nPixelClock, sizeof(m_nPixelClock));

    // update pixelclock controls
	UpdatePixelClock();

	*pResult = 0;
}

void CCameraConfiguration::OnDeltaposFramerate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Variable dTemp = useless
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	double dFpsMin = 0.0;
	double dFpsMax = 0.0;
	double dFpsInc = 0.0;
	double dDummy = 0;
	
	int nRet = 0;

    /* receive framerate range */
	nRet = is_GetFrameTimeRange(m_pCam->hCam, &dFpsMin, &dFpsMax, &dFpsInc);

    /* receive new framerate */
    CString strTemp;
	GetDlgItemText(IDC_EDIT_FRAMERATE, strTemp);
	double dFrameRate = wcstod(strTemp, NULL);

	if (pNMUpDown->iDelta < 0)
	{
		// icrement
        dFrameRate = dFrameRate + 1 < 1 / dFpsMin ? dFrameRate + 1 : 1 / dFpsMin;
	}
	else
	{
        // decrement
		dFrameRate = dFrameRate - 1 > 1 / dFpsMax ? dFrameRate - 1 : 1 / dFpsMax;
	}

    /* set new framerate */
	nRet = is_SetFrameRate(m_pCam->hCam, dFrameRate, &dDummy);

    /* update framerate controls */
	UpdateFramerate();

	*pResult = 0;
}

void CCameraConfiguration::OnDeltaposTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    CString strTemp;
    GetDlgItemText(IDC_EDIT_EXPOSURE, strTemp);

	double dExposure = wcstod(strTemp, NULL);

	if (pNMUpDown->iDelta < 0)
	{
        // icrement
        dExposure = dExposure + 1 < m_dblMaxExp ? dExposure + 1: m_dblMaxExp;
	}
	else
	{
        // decrement
		dExposure = (dExposure - 1) > m_dblMinExp ? dExposure - 1: m_dblMinExp;
	}

    /* set new exposure value */
	int nRet = is_Exposure(m_pCam->hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dExposure, sizeof(dExposure));

    /* update exposure controls */
	UpdateExposure();

	*pResult = 0;
}

void CCameraConfiguration::OnBnClickedButtonDefault()
{
	int nRet = 0;

	/* Get default pixel clock */
    int	nPixelClockDefault = 0;
	nRet = is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_GET_DEFAULT, (void*)&nPixelClockDefault, sizeof(nPixelClockDefault));
	if (nRet == IS_SUCCESS)
	{
		/* restore to default pixelclock */
		is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_SET, (void*)&nPixelClockDefault, sizeof(nPixelClockDefault));
	}

	/* get default framerate */
    double dFrameRateDefault = 0;
	nRet = is_SetFrameRate(m_pCam->hCam, IS_GET_DEFAULT_FRAMERATE, &dFrameRateDefault);
	if (nRet == IS_SUCCESS)
	{
		/* restore to default framerate */
        double dDummy;
		is_SetFrameRate(m_pCam->hCam, dFrameRateDefault, &dDummy);
	}

	/* get the default expousure time */
	double dExpousureTimeDefault = 0;
    nRet = is_Exposure(m_pCam->hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_DEFAULT, (void*)&dExpousureTimeDefault, sizeof(dExpousureTimeDefault));
	if (nRet != IS_SUCCESS)
	{
        /* restore to default exposure time */
		nRet = is_Exposure(m_pCam->hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dExpousureTimeDefault, sizeof(dExpousureTimeDefault));
	}

    /* update our controls */
    UpdateControls();
}

void CCameraConfiguration::OnEnKillfocusEditPixelclock()
{
	/* update pixel clock value */
	GetDlgItemInt(IDC_EDIT_PIXELCLOCK, &m_nPixelClock);

    /* check ranges */
	if (m_nPixelClock > m_nMaxPixelclock)
	{
		m_nPixelClock = m_nMaxPixelclock;
	}

	if (m_nPixelClock < m_nMinPixelclock)
	{
		m_nPixelClock = m_nMinPixelclock;
	}

    /* compare with pixelclock list */
	UpdateNextPixelclock();

	/* Set new PixelClock */
	int nRet = is_PixelClock(m_pCam->hCam, IS_PIXELCLOCK_CMD_SET, (void*)&m_nPixelClock, sizeof(m_nPixelClock));
	if (nRet != IS_SUCCESS)
	{
		CString strMessage;
        strMessage.Format(L"Setting pixel clock failed with: %d", nRet);
		AfxMessageBox(strMessage, MB_ICONERROR);
	}

    /* update controls */
	UpdatePixelClock();
}

void CCameraConfiguration::OnEnKillfocusEditFramerate()
{
	/* get current framerate */
    CString strFramerate;
	GetDlgItemText(IDC_EDIT_FRAMERATE, strFramerate);
    double dFrameRate = wcstol(strFramerate, NULL, 10);
    
    double dDummy = 0;

    /* set new framerate */
	int nRet = is_SetFrameRate(m_pCam->hCam, dFrameRate, &dDummy);
	if (nRet != IS_SUCCESS)
	{
		CString Str;
        Str.Format(L"Setting frame rate failed with: %d", nRet);
		AfxMessageBox(Str, MB_ICONERROR);
	}

    /* update controls */
	UpdateFramerate();
}

void CCameraConfiguration::OnEnKillfocusEditExposure()
{
    /* get current exposure */
    CString strExposure;
	GetDlgItemText(IDC_EDIT_EXPOSURE, strExposure);
	double dExposure = wcstod(strExposure, NULL);

    /* set new exposure */
	int nRet = is_Exposure(m_pCam->hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&dExposure, sizeof(dExposure));
	if (nRet != IS_SUCCESS)
	{
		CString Str;
        Str.Format(L"Setting exposure failed with: %d", nRet);
		AfxMessageBox(Str, MB_ICONERROR);
	}

    /* update controls */
	UpdateExposure();
}

BOOL CCameraConfiguration::PreTranslateMessage(MSG* pMsg)
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
			if (GetFocus() == GetDlgItem(IDC_EDIT_PIXELCLOCK))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditPixelclock();
			}
			if (GetFocus() == GetDlgItem(IDC_EDIT_FRAMERATE))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditFramerate();
			}
			if (GetFocus() == GetDlgItem(IDC_EDIT_EXPOSURE))
			{
				// a value has changed --> assume it
				OnEnKillfocusEditExposure();	
			}

			// avoid close on return press
			return 1;
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}


void CCameraConfiguration::UpdateNextPixelclock()
{
    /* returns the equal or higher pixel clock from the pixelclock list */
    for (int i = 0; i < m_nNumberOfSupportedPixelClocks; i++)
    {
        if (m_nPixelClockList[i] >= m_nPixelClock)
        {
            m_nPixelClock = m_nPixelClockList[i];
            break;
        }
    }
}

void CCameraConfiguration::UpdatePrevPixelclock()
{
    /* returns the equal or lower pixel clock from the pixelclock list */
   for (int i = m_nNumberOfSupportedPixelClocks - 1; i >=  0; i--)
    {
        if (m_nPixelClockList[i] <= m_nPixelClock)
        {
            m_nPixelClock = m_nPixelClockList[i];
            break;
        }
    }
}
