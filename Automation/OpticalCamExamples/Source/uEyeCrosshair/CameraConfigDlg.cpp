//==========================================================================//
//																			//
//	Copyright (C) 2009 - 2018												//
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
//																			//
//==========================================================================//

#include "stdafx.h"
#include "uEyeCrosshair.h"
#include "CameraConfigDlg.h"


IMPLEMENT_DYNAMIC(CCameraConfigDlg, CPropertyPage)

CCameraConfigDlg::CCameraConfigDlg(/*=NULL*/)
  : CPropertyPage(CCameraConfigDlg::IDD)
{

}


CCameraConfigDlg::~CCameraConfigDlg()
{
}


void CCameraConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLD_GAIN2, m_ctrlContrast);
    DDX_Control(pDX, IDC_SLD_EXPOSURE2, m_ctrlLum);
    DDX_Control(pDX, IDC_SLD_EXPOSURE, m_ctrlExposure);
    DDX_Control(pDX, IDC_SLD_GAIN, m_ctrlGain);
}


BEGIN_MESSAGE_MAP(CCameraConfigDlg, CPropertyPage)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_GAIN2, &CCameraConfigDlg::OnNMCustomdrawSldContrast)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_EXPOSURE2, &CCameraConfigDlg::OnNMCustomdrawSldLum)
    ON_BN_CLICKED(IDC_CHK_AWB, &CCameraConfigDlg::OnBnClickedChkAwb)
    ON_BN_CLICKED(IDC_CHK_AES, &CCameraConfigDlg::OnBnClickedChkAes)
    ON_BN_CLICKED(IDC_CHK_AGC, &CCameraConfigDlg::OnBnClickedChkAgc)
    ON_BN_CLICKED(IDC_RD_COLOR, &CCameraConfigDlg::OnBnClickedRdColor)
    ON_BN_CLICKED(IDC_RD_BLACKWHITE, &CCameraConfigDlg::OnBnClickedRdBlackwhite)
    ON_BN_CLICKED(IDC_CHK_VERTICAL, &CCameraConfigDlg::OnBnClickedChkVertical)
    ON_BN_CLICKED(IDC_CHK_HORIZONTAL, &CCameraConfigDlg::OnBnClickedChkHorizontal)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_EXPOSURE, &CCameraConfigDlg::OnNMCustomdrawSldExposure)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLD_GAIN, &CCameraConfigDlg::OnNMCustomdrawSldGain)
END_MESSAGE_MAP()


void CCameraConfigDlg::SetParent(CDialog *parent, CameraOptions *pOptions)
{
    dlg = parent;
    m_Options = pOptions;
}


BOOL CCameraConfigDlg::OnInitDialog()
{
  CDialog::OnInitDialog ();

  // set the settings of the brightness slider
  if ((dlg != NULL) && (m_Options != NULL))
  {
        double dmin, dmax, dintervall;
        double dblRange[3];

        INT nRet = is_Exposure(((uEyeCrosshairDlg*) dlg)->m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE,
                              (void*)dblRange, sizeof(dblRange));

        if (nRet == IS_SUCCESS)
        {
            dmin = dblRange[0];
            dmax = dblRange[1];
            dintervall = dblRange[2];
        }
        if(GetDlgItem (IDC_LBL_MIN_LUM) != 0)
        {
            SetDlgItemInt (IDC_LBL_MIN_LUM, 1);
        }
        if (GetDlgItem (IDC_LBL_MAX_LUM) != 0)
        {
            SetDlgItemInt (IDC_LBL_MAX_LUM, (unsigned int) dmax);
        }

        m_ctrlLum.SetRange   (1, (unsigned int) dmax);
        m_ctrlLum.SetTicFreq (10);

        double dEnable = 1;
        is_SetAutoParameter(((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, 0);

        m_ctrlLum.SetPos (m_Options->nLum);
    
        // set the settings of the contrast slider
        m_ctrlContrast.SetRange   (0, 100);
        m_ctrlContrast.SetTicFreq (20);
        m_ctrlContrast.SetPos (m_Options->nContrast);
    }

    // check the ctrls
    if (m_Options != NULL)
    {
        CButton* pBtn;
        pBtn = (CButton*) GetDlgItem (IDC_CHK_VERTICAL);
    
        if(pBtn != NULL)
        {
            pBtn->SetCheck (m_Options->bMirrorVer);
        }

        pBtn = (CButton*) GetDlgItem (IDC_CHK_HORIZONTAL);
    
        if (pBtn != NULL)
        {
            pBtn->SetCheck (m_Options->bMirrorHor);
        }

        pBtn = (CButton*) GetDlgItem (IDC_CHK_AWB);
    
        if (pBtn != NULL)
        {
            pBtn->SetCheck (m_Options->bAWB);
        }

        pBtn = (CButton*) GetDlgItem (IDC_CHK_AGC);
    
        if (pBtn != NULL)
        {
            pBtn->SetCheck (m_Options->bAGC);
        }

        pBtn = (CButton*) GetDlgItem (IDC_CHK_AES);
    
        if (pBtn != NULL)
        {
            pBtn->SetCheck (m_Options->bAES);
        }

        if (!m_Options->nColorMode != 0)
        {
            pBtn = (CButton*) GetDlgItem (IDC_RD_COLOR);

            if (pBtn != NULL)
            {
                pBtn->SetCheck (true);
            }
        }
        else
        {
            pBtn = (CButton*) GetDlgItem (IDC_RD_BLACKWHITE);

            if (pBtn != NULL)
            {
                pBtn->SetCheck (true);
            }
        }
    }
  
    // set range of gain and exposure slider and position
    m_ctrlGain.SetRange (0, 10);
    m_ctrlExposure.SetRange (0, 10);

    if (m_Options != NULL)
    {
        m_ctrlGain.SetPos (m_Options->nAGCskipFrame);
        m_ctrlExposure.SetPos (m_Options->nAESskipFrame);

        //disable gain and exposure slider
        if ((m_Options->bAES) || (m_Options->bAGC))
        {
            m_ctrlLum.EnableWindow (false);
        	m_ctrlContrast.EnableWindow (false);
		}
    }

    return TRUE;
}


void CCameraConfigDlg::OnNMCustomdrawSldContrast (NMHDR* pNMHDR, LRESULT* pResult)
{
    // set the new contrast
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW> (pNMHDR);

    // if aes activated leave
    if ((m_Options != NULL) && !m_Options->bAES && !m_Options->bAGC)
    {
        if (dlg != NULL)
        {
			is_SetHardwareGain(((uEyeCrosshairDlg*)dlg)->m_hCam,
							   	m_ctrlContrast.GetPos(),
								0, 0, 0);
        }

        SetDlgItemInt (IDC_EDIT_CONTRAST, m_ctrlContrast.GetPos ());
        m_Options->nContrast = m_ctrlContrast.GetPos();
    }

    if (pResult != NULL)
    {   
        *pResult = 0;
    }
}


void CCameraConfigDlg::OnNMCustomdrawSldLum(NMHDR* pNMHDR, LRESULT* pResult)
{
    //set the new exposure time
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

    // if aes activated leave
    if ((m_Options != NULL) && !m_Options->bAES && !m_Options->bAGC)
    {
        if (dlg != NULL)
        {
            double dExposure = 0.0;
            dExposure = m_ctrlLum.GetPos();
            
            is_Exposure(((uEyeCrosshairDlg*) dlg)->m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE,
                       (void*)&dExposure, sizeof(dExposure));
        }
  
       SetDlgItemInt (IDC_EDIT_LUM, m_ctrlLum.GetPos ());
       m_Options->nLum = m_ctrlLum.GetPos ();
    }

    if (pResult != NULL)
    {
        *pResult = 0;
    }
}


void CCameraConfigDlg::OnBnClickedChkAwb ()
{
    // Enable Whitebalance
    if (m_Options != NULL)
    {
        CButton* pBtn = (CButton*) GetDlgItem (IDC_CHK_AWB);

        if (pBtn != NULL)
        {
            if (pBtn->GetCheck ())
            {
                if (dlg != NULL)
                {
                    double dbl = 1.0;
                    is_SetAutoParameter (((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dbl, 0);
                }

                m_Options->bAWB = true;
            }
            else
            {
                if (dlg != NULL)
                {
                    double dbl = 0.0;
                    is_SetAutoParameter (((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dbl, 0);
                }
            
                m_Options->bAWB = false;
            }
        }
    }
}


void CCameraConfigDlg::OnBnClickedChkAes ()
{
    // set skip frames
    double dSkipFrame;
    dSkipFrame = m_ctrlExposure.GetPos();
    is_SetAutoParameter(((uEyeCrosshairDlg*)dlg)->m_hCam,IS_SET_AUTO_SKIPFRAMES, &dSkipFrame, 0);
  
    // Enable AES
    if(m_Options != NULL)
    {
        CButton* pBtn1 = (CButton*) GetDlgItem (IDC_CHK_AES);
		CButton* pBtn2 = (CButton*) GetDlgItem (IDC_CHK_AGC);

        if(pBtn1 != NULL)
        {
            if(pBtn1->GetCheck())
            {
                m_ctrlLum.EnableWindow(false);
				m_ctrlContrast.EnableWindow(false);
                
                if(dlg != NULL)
                {
                    double dEnable = 1;
					is_SetAutoParameter(((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, 0);
                }

                m_Options->bAES = true;
            }
            else
            {
                m_Options->bAES = false;

				if (dlg != NULL)
                {
                    double dEnable = 0;
					is_SetAutoParameter(((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, 0);
                }

				if (!pBtn1->GetCheck() && !pBtn2->GetCheck())
				{
					m_ctrlLum.EnableWindow(true);
					m_ctrlContrast.EnableWindow(true);
				}
            }
        }
    }
}


void CCameraConfigDlg::OnBnClickedChkAgc()
{
    // set skip frames
    double dSkipFrame;
    dSkipFrame = m_ctrlGain.GetPos();
    is_SetAutoParameter(((uEyeCrosshairDlg*)dlg)->m_hCam,IS_SET_AUTO_SKIPFRAMES, &dSkipFrame, 0);

    // Enable AGC
    if(m_Options != NULL)
    {
        CButton* pBtn1 = (CButton*) GetDlgItem (IDC_CHK_AGC);
		CButton* pBtn2 = (CButton*) GetDlgItem (IDC_CHK_AES);
    
        if (pBtn1 != NULL)
        {
            if (pBtn1->GetCheck ())
            {
				m_ctrlLum.EnableWindow(false);
				m_ctrlContrast.EnableWindow(false);

                if (dlg != NULL)
                {
                    double dbl = 1.0;
                    is_SetAutoParameter (((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_ENABLE_AUTO_GAIN, &dbl, 0);
                }

                m_Options->bAGC = true;	
            }
            else
            {
				m_Options->bAGC = false;

                if (dlg != NULL)
                {
                    double dbl = 0.0;
                    is_SetAutoParameter (((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_ENABLE_AUTO_GAIN, &dbl, 0);
                }
   
				if (!pBtn1->GetCheck() && !pBtn2->GetCheck())
				{
					m_ctrlLum.EnableWindow(true);
					m_ctrlContrast.EnableWindow(true);
				}
            }
        }
    }
}


void CCameraConfigDlg::OnBnClickedRdColor()
{
    // set the display to color
    if(m_Options != NULL)
    {
        if(dlg != NULL)
        {
            is_SetDisplayMode (((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_DM_DIRECT3D);
        }

        m_Options->nColorMode = 0;
      
        if (m_Options->bAWB)
        {
            double dbl = 1.0;
            is_SetAutoParameter (((uEyeCrosshairDlg*) dlg)->m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dbl, 0);
        }

        if (dlg != NULL)
        {
            ((uEyeCrosshairDlg*)dlg)->DrawObjects();
        }
    }
}


void CCameraConfigDlg::OnBnClickedRdBlackwhite()
{
    // set the display to black/white
    if(m_Options != NULL)
    {
        is_SetDisplayMode (((uEyeCrosshairDlg*)dlg)->m_hCam, IS_SET_DM_DIRECT3D | IS_SET_DM_MONO);
        m_Options->nColorMode = 1;

        if(dlg != NULL)
        {
            ((uEyeCrosshairDlg*)dlg)->DrawObjects();
        }
    }
}


void CCameraConfigDlg::OnBnClickedChkVertical()
{
    // disable or enable vertical mirror
    if(m_Options != NULL || dlg != NULL)
    {
        CButton* pBtn = (CButton*) GetDlgItem (IDC_CHK_VERTICAL);
        if(pBtn != NULL)
        {
            if(pBtn->GetCheck())
            {
                is_SetRopEffect(((uEyeCrosshairDlg*)dlg)->m_hCam,IS_SET_ROP_MIRROR_UPDOWN,1,0);
                m_Options->bMirrorVer = true;
            }
            else
            {
                is_SetRopEffect(((uEyeCrosshairDlg*)dlg)->m_hCam,IS_SET_ROP_MIRROR_UPDOWN,0,0);
                m_Options->bMirrorVer = false;
            }
        }
    }
}


void CCameraConfigDlg::OnBnClickedChkHorizontal()
{
    // disable or enable horizontal mirror
    if(m_Options != NULL && dlg != NULL)
    {
        CButton* pBtn = (CButton*) GetDlgItem (IDC_CHK_HORIZONTAL);
        if(pBtn != NULL)
        {
            if(pBtn->GetCheck())
            {
                is_SetRopEffect(((uEyeCrosshairDlg*)dlg)->m_hCam,IS_SET_ROP_MIRROR_LEFTRIGHT,1,0);
                m_Options->bMirrorHor = true;
            }
            else
            {
                is_SetRopEffect(((uEyeCrosshairDlg*)dlg)->m_hCam,IS_SET_ROP_MIRROR_LEFTRIGHT,0,0);
                m_Options->bMirrorHor = false;
            }
        }
    }
}


void CCameraConfigDlg::OnNMCustomdrawSldExposure(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
  
    if(GetDlgItem(IDC_EDIT_EXPOSURE) != NULL)
    {
        SetDlgItemInt(IDC_EDIT_EXPOSURE,m_ctrlExposure.GetPos());
        if(m_Options != 0)
        {
            m_Options->nAESskipFrame = m_ctrlExposure.GetPos();
        }
    }

    *pResult = 0;
}


void CCameraConfigDlg::OnNMCustomdrawSldGain(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    if(GetDlgItem(IDC_EDIT_GAIN) != NULL)
    {
        SetDlgItemInt(IDC_EDIT_GAIN, m_ctrlGain.GetPos());
        if(m_Options != 0)
        {
            m_Options->nAGCskipFrame = m_ctrlGain.GetPos();
        }
    }
    
    *pResult = 0;
}
