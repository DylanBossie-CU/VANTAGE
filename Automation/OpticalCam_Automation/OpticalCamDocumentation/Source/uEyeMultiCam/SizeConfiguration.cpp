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

// SizeConfiguration.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "Multi_Cam.h"
#include "SizeConfiguration.h"


// CSizeConfiguration-Dialogfeld

IMPLEMENT_DYNAMIC(CSizeConfiguration, CPropertyPage)

CSizeConfiguration::CSizeConfiguration() : 
CPropertyPage(CSizeConfiguration::IDD),
m_pCam(NULL)
{

}

CSizeConfiguration::~CSizeConfiguration()
{

}

void CSizeConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDER_WIDTH, m_sliderWidth);
	DDX_Control(pDX, IDC_SLIDER_HIGH, m_sliderHeight);
	DDX_Control(pDX, IDC_SLIDER_LEFT, m_sliderLeft);
	DDX_Control(pDX, IDC_SLIDER_TOP, m_sliderTop);
}


BEGIN_MESSAGE_MAP(CSizeConfiguration, CPropertyPage)

	ON_WM_HSCROLL()

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WIDTH, &CSizeConfiguration::OnDeltaposSpinWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HEIGHT, &CSizeConfiguration::OnDeltaposSpinHeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LEFT, &CSizeConfiguration::OnDeltaposSpinLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TOP, &CSizeConfiguration::OnDeltaposSpinTop)

	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CSizeConfiguration::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_CHECK_WINDOW, &CSizeConfiguration::OnBnClickedCheckWindow)

END_MESSAGE_MAP()

// CSizeConfiguration-Meldungshandler

BOOL CSizeConfiguration::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateControls();

    ((CButton *)GetDlgItem(IDC_CHECK_WINDOW))->SetCheck(m_pCam->RenderMode == IS_RENDER_FIT_TO_WINDOW ? 1 : 0);

	return TRUE;
}

void CSizeConfiguration::SetCameraHandle(Camera *pCamera)
{
    /* set the active camera handle */
    m_pCam = pCamera;
}

void CSizeConfiguration::UpdateControls()
{
	int nRet = 0;

    /* receive aoi values */
	IS_SIZE_2D sizeMin;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_SIZE_MIN, (void*) &sizeMin, sizeof (sizeMin));

	IS_SIZE_2D sizeMax;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_SIZE_MAX, (void*) &sizeMax, sizeof (sizeMax));

	IS_SIZE_2D sizeInc;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_SIZE_INC, (void*) &sizeInc, sizeof (sizeInc));

	IS_POINT_2D pointMin;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_POS_MIN, (void*) &pointMin, sizeof (pointMin));

	IS_POINT_2D pointMax;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_POS_MAX, (void*) &pointMax, sizeof (pointMax));

	IS_POINT_2D pointInc;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_POS_INC, (void*) &pointInc, sizeof (pointInc));

	/* Update step values */
	m_nWidthStep = sizeInc.s32Width;
	m_nHeightStep = sizeInc.s32Height;

	m_nXStep = pointInc.s32X;
	m_nYStep = pointInc.s32Y;

	/* Update min and max values */
	SetDlgItemInt(IDC_STATIC_BREITE_MIN, sizeMin.s32Width);
	SetDlgItemInt(IDC_STATIC_BREITE_MAX, sizeMax.s32Width);

	SetDlgItemInt(IDC_STATIC_HOEHE_MIN, sizeMin.s32Height);
	SetDlgItemInt(IDC_STATIC_HOEHE_MAX, sizeMax.s32Height);

	SetDlgItemInt(IDC_STATIC_LEFT_MIN, pointMin.s32X);
	SetDlgItemInt(IDC_STATIC_LEFT_MAX, pointMax.s32X);

	SetDlgItemInt(IDC_STATIC_TOP_MIN, pointMin.s32Y);
	SetDlgItemInt(IDC_STATIC_TOP_MAX, pointMax.s32Y);

	/* Update slider ranges */
	m_sliderWidth.SetRange(sizeMin.s32Width, sizeMax.s32Width, TRUE);
	m_sliderHeight.SetRange(sizeMin.s32Height, sizeMax.s32Height, TRUE);

	m_sliderLeft.SetRange(pointMin.s32X, pointMax.s32X, TRUE);
	m_sliderTop.SetRange(pointMin.s32Y, pointMax.s32Y, TRUE);

    /* Update slider positions */
    IS_RECT rectAOI;
    nRet = is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAOI, sizeof(IS_RECT));
    if (nRet == IS_SUCCESS)
    {
        /* set slider && spin control values */
        m_sliderWidth.SetPos(rectAOI.s32Width);
        SetDlgItemInt(IDC_EDIT_WIDTH, rectAOI.s32Width);

        m_sliderHeight.SetPos(rectAOI.s32Height);
        SetDlgItemInt(IDC_EDIT_HEIGHT, rectAOI.s32Height);

        m_sliderLeft.SetPos(rectAOI.s32X);
        SetDlgItemInt(IDC_EDIT_LEFT, rectAOI.s32X);

        m_sliderTop.SetPos(rectAOI.s32Y);
        SetDlgItemInt(IDC_EDIT_TOP, rectAOI.s32Y);
    }
}

void CSizeConfiguration::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_WIDTH:
		{
            int nPos = m_sliderWidth.GetPos();
            while (nPos % m_nWidthStep)
            {
                nPos--;
            }
            
            SetAoiWidth(nPos);
            ReallocMemory();
		}
		break;

	case IDC_SLIDER_HIGH:
		{
            int nPos = m_sliderHeight.GetPos();
            while (nPos % m_nHeightStep)
            {
                nPos--;
            }
            SetAoiHeight(nPos);
            ReallocMemory();
		}
        break;

	case IDC_SLIDER_LEFT:
		{
            int nPos = m_sliderLeft.GetPos();
            while (nPos % m_nXStep)
            {
                nPos--;
            }
            SetAoiLeft(nPos);
            ReallocMemory();
		}
		break;

	case IDC_SLIDER_TOP:
		{
            int nPos = m_sliderTop.GetPos();
            while (nPos % m_nYStep)
            {
                nPos--;
            }
            SetAoiTop(nPos);
            ReallocMemory();
		}
		break;

	default:
		break;
	}
}

void CSizeConfiguration::OnDeltaposSpinWidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_WIDTH);

	if (pNMUpDown->iDelta < 0)
	{
		do
		{
			nValue++;
		}
		while (nValue % m_nWidthStep);
	}
	else
	{
		do
		{
			nValue--;
		}
		while (nValue % m_nWidthStep);
	}

	SetAoiWidth(nValue);
    ReallocMemory();

	*pResult = 0;
}

void CSizeConfiguration::OnDeltaposSpinHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_HEIGHT);

	if (pNMUpDown->iDelta < 0)
	{
		do
		{
			nValue++;
		}
		while (nValue % m_nHeightStep);
	}
	else
	{
		do
		{
			nValue--;
		}
		while (nValue % m_nHeightStep);
	}

	SetAoiHeight(nValue);
    ReallocMemory();

	*pResult = 0;
}

void CSizeConfiguration::OnDeltaposSpinLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_LEFT);

	if (pNMUpDown->iDelta < 0)
	{
		do
		{
			nValue++;
		}
		while (nValue % m_nXStep);
	}
	else
	{
		do
		{
			nValue--;
		}
		while (nValue % m_nXStep);
	}

	SetAoiLeft(nValue);
    ReallocMemory();

	*pResult = 0;
}

void CSizeConfiguration::OnDeltaposSpinTop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nValue = GetDlgItemInt(IDC_EDIT_TOP);

	if (pNMUpDown->iDelta < 0)
	{
		do
		{
			nValue++;
		}
		while (nValue % m_nYStep);
	}
	else
	{
		do
		{
			nValue--;
		}
		while (nValue % m_nYStep);
	}

	SetAoiTop(nValue);
    ReallocMemory();

	*pResult = 0;
}

void CSizeConfiguration::SetAoiWidth(int nValue)
{
    /* receive current aoi */
    IS_RECT rect;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rect, sizeof(IS_RECT));

	rect.s32Width = nValue;

    /* set new aoi */
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rect, sizeof(IS_RECT));

	UpdateControls();
}

void CSizeConfiguration::SetAoiHeight(int nValue)
{
    /* receive current aoi */
    IS_RECT rect;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rect, sizeof(IS_RECT));

	rect.s32Height = nValue;

    /* set new aoi */
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rect, sizeof(IS_RECT));

	UpdateControls();
}

void CSizeConfiguration::SetAoiLeft(int nValue)
{
    /* receive current aoi */
    IS_RECT rect;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rect, sizeof(IS_RECT));

	rect.s32X = nValue;

    /* set new aoi */
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rect, sizeof(IS_RECT));

	UpdateControls();
}

void CSizeConfiguration::SetAoiTop(int nValue)
{
    /* receive current aoi */
    IS_RECT rect;
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rect, sizeof(IS_RECT));

	rect.s32Y = nValue;

    /* set new aoi */
	is_AOI(m_pCam->hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rect, sizeof(IS_RECT));

	UpdateControls();
}

BOOL CSizeConfiguration::PreTranslateMessage(MSG* pMsg)
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
			if (GetFocus() == GetDlgItem(IDC_EDIT_WIDTH))
			{
				// a value has changed --> assume it
				int nValue = GetDlgItemInt(IDC_EDIT_WIDTH);
				SetAoiWidth(nValue);
			}

			if (GetFocus() == GetDlgItem(IDC_EDIT_HEIGHT))
			{
				// a value has changed --> assume it
				int nValue = GetDlgItemInt(IDC_EDIT_HEIGHT);
				SetAoiHeight(nValue);
			}

			if (GetFocus() == GetDlgItem(IDC_EDIT_LEFT))
			{
				// a value has changed --> assume it
				int nValue = GetDlgItemInt(IDC_EDIT_LEFT);
				SetAoiLeft(nValue);
			}

			if (GetFocus() == GetDlgItem(IDC_EDIT_TOP))
			{
				// a value has changed --> assume it
				int nValue = GetDlgItemInt(IDC_EDIT_TOP);
				SetAoiTop(nValue);
			}

            // avoid close
            return 0;
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CSizeConfiguration::OnBnClickedButtonDefault()
{	
    /* restore aoi */
    IS_RECT rectAOI;
    rectAOI.s32X = 0;
    rectAOI.s32Y = 0;
    rectAOI.s32Width = m_pCam->SensorInfo.nMaxWidth;
    rectAOI.s32Height = m_pCam->SensorInfo.nMaxHeight;

    is_AOI(m_pCam->hCam, IS_AOI_IMAGE_SET_AOI, (void*)&rectAOI, sizeof(IS_RECT));

    /* reallocate and update the controls */
	ReallocMemory();
    UpdateControls();

    /* restore render mode */
    CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_WINDOW);
	pButton->SetCheck(true);
	OnBnClickedCheckWindow();
}

void CSizeConfiguration::OnBnClickedCheckWindow()
{
	CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_WINDOW);

    /* set new render mode */
    m_pCam->RenderMode = pButton->GetCheck() ? IS_RENDER_FIT_TO_WINDOW : IS_RENDER_NORMAL;
}

void CSizeConfiguration::SetView(CView *pView)
{
	m_pView = pView;
}

void CSizeConfiguration::ReallocMemory()
{
    int nRet = 0;

    /* stop live video */
    if (m_pCam->bLive)
    {
        nRet = is_StopLiveVideo(m_pCam->hCam, IS_FORCE_VIDEO_STOP);
    }

    m_pCam->ClearSequence();
    /* free previous allocated image data */
    m_pCam->FreeImageMems();

    /* receive current aoi size */
    IS_RECT rect;
    is_AOI(m_pCam->hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rect, sizeof(rect));

    /* allocate new image buffer */
    nRet = m_pCam->AllocImageMems(rect.s32Width, rect.s32Height, 32);
    nRet = m_pCam->InitSequence();

    /* start live video */
    if (m_pCam->bLive)
    {
        nRet = is_CaptureVideo(m_pCam->hCam, IS_WAIT);
    }

    /* redraw */
	m_pView->Invalidate(1);
}