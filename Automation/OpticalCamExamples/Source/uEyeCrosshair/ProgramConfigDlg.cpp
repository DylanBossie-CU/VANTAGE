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
#include "ProgramConfigDlg.h"


IMPLEMENT_DYNAMIC (CProgramConfigDlg, CPropertyPage)


CProgramConfigDlg::CProgramConfigDlg (/*=NULL*/)
  : CPropertyPage (CProgramConfigDlg::IDD)
  , m_nPosCrossX(0)
  , m_nPosCrossY(0)
  , m_nPicturePosX(0)
  , m_nPicturePosY(0)
  , m_nCircleRadius(0)
  , m_strPicturePath(_T(""))
  , m_bShowPicture(FALSE)
  , m_bStartFullscreen(FALSE)
  , m_nCanvasSizeX(0)
  , m_nCanvasSizeY(0)
{
}


CProgramConfigDlg::~CProgramConfigDlg ()
{
}


void CProgramConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMB_WIDTH, m_ctrlPencilWidth);
    DDX_Text(pDX, IDC_EDIT_CROSS_POSX, m_nPosCrossX);
    DDX_Text(pDX, IDC_EDIT_CROSS_POSY, m_nPosCrossY);
    DDX_Text(pDX, IDC_EDIT_POSITION_X, m_nPicturePosX);
    DDX_Text(pDX, IDC_EDIT_POSITION_Y, m_nPicturePosY);
    DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS, m_nCircleRadius);
    DDX_Text(pDX, IDC_EDIT_FOLDER, m_strPicturePath);
    DDX_Check(pDX, IDC_CHK_SHOW_PICTURE, m_bShowPicture);
    DDX_Check(pDX, IDC_CHK_STARTFULLSCREEN, m_bStartFullscreen);
}


BEGIN_MESSAGE_MAP(CProgramConfigDlg, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_CHOOSE_COLOR, &CProgramConfigDlg::OnBnClickedBtnChooseColor)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_CHK_STARTFULLSCREEN, &CProgramConfigDlg::OnBnClickedChkStartfullscreen)
    ON_CBN_SELCHANGE(IDC_CMB_WIDTH, &CProgramConfigDlg::OnCbnSelchangeCmbWidth)
    ON_EN_CHANGE(IDC_EDIT_CIRCLE_RADIUS, &CProgramConfigDlg::OnEnChangeEditCircleRadius)
    ON_BN_CLICKED(IDC_BTN_FOLDER, &CProgramConfigDlg::OnBnClickedBtnFolder)
    ON_BN_CLICKED(IDC_CHK_SHOW_PICTURE, &CProgramConfigDlg::OnBnClickedChkShowPicture)
    ON_BN_CLICKED(IDC_BTN_CHOOSE_KEYCOLOR, &CProgramConfigDlg::OnBnClickedBtnChooseKeycolor)
    ON_EN_CHANGE(IDC_EDIT_POSITION_Y, &CProgramConfigDlg::OnEnChangeEditnPicturePosY)
    ON_EN_CHANGE(IDC_EDIT_POSITION_X, &CProgramConfigDlg::OnEnChangeEditnPicturePosX)
    ON_EN_CHANGE(IDC_EDIT_CROSS_POSX, &CProgramConfigDlg::OnEnChangeEditCrossPosx)
    ON_EN_CHANGE(IDC_EDIT_CROSS_POSY, &CProgramConfigDlg::OnEnChangeEditCrossPosy)
    ON_BN_CLICKED(IDC_BUTTON_RESET_POS, &CProgramConfigDlg::OnBnClickedButtonResetPos)
    ON_EN_CHANGE(IDC_EDIT_FOLDER, &CProgramConfigDlg::OnEnChangeEditFolder)
END_MESSAGE_MAP()


void CProgramConfigDlg::SetParent (CDialog* parent, ProgramOptions* pOptions)
{
    dlg = parent;
    m_Options = pOptions;
}


BOOL CProgramConfigDlg::OnInitDialog ()
{
    CDialog::OnInitDialog ();

    // check the ctrls
    if (m_Options != NULL)
    {
        m_bStartFullscreen = m_Options->bStartFullscreen;
        m_bShowPicture = m_Options->bShowPicture;
        m_strPicturePath = m_Options->strPicturePath;
        m_nPosCrossX = m_Options->nCrossPosX;
        m_nPosCrossY = m_Options->nCrossPosY;
        m_nPicturePosX = m_Options->nPicturePosX;
        m_nPicturePosY = m_Options->nPicturePosY;
        m_nCircleRadius = m_Options->nCircleRadius;
        m_nCanvasSizeX = m_Options->m_nCanvasSizeX;
        m_nCanvasSizeY = m_Options->m_nCanvasSizeY;
 
        CString strTemp;
        strTemp.Format(L"%d",m_Options->nPencilWidth);
        m_ctrlPencilWidth.SetCurSel (m_ctrlPencilWidth.FindStringExact(0,strTemp));

        UpdateData(false);
    }

    if (dlg != NULL)
    {
        CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem (IDC_SPIN_POSITION_X);

        if (pSpin != NULL)
        {
            pSpin->SetRange32 (0, ((uEyeCrosshairDlg*) dlg)->m_nSizeX);
            pSpin->SetBuddy   (GetDlgItem (IDC_EDIT_POSITION_X));
        }

        pSpin = (CSpinButtonCtrl*)GetDlgItem (IDC_SPIN_POSITION_Y);

        if (pSpin != NULL)
        {
            pSpin->SetRange32 (0, ((uEyeCrosshairDlg*) dlg)->m_nSizeY);
            pSpin->SetBuddy   (GetDlgItem (IDC_EDIT_POSITION_Y));
        }

        pSpin = (CSpinButtonCtrl*)GetDlgItem (IDC_SPIN_CROSS_POSX);

        if (pSpin != NULL)
        {
            pSpin->SetRange32 (0, ((uEyeCrosshairDlg*) dlg)->m_nSizeX);
            pSpin->SetBuddy   (GetDlgItem (IDC_EDIT_CROSS_POSX));
        }

        pSpin = (CSpinButtonCtrl*)GetDlgItem (IDC_SPIN_CROSS_POSY);

        if (pSpin != NULL)
        {
            pSpin->SetRange32 (0, ((uEyeCrosshairDlg*) dlg)->m_nSizeY);
            pSpin->SetBuddy   (GetDlgItem (IDC_EDIT_CROSS_POSY));
        }
    }

    return TRUE;
}


void CProgramConfigDlg::OnBnClickedBtnChooseColor ()
{
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16]; 
    
    // Array of custom colors     
    ZeroMemory (acrCustClr, 16 * sizeof (DWORD)); 

    // Fill choosecolor structure
    ZeroMemory (&cc, sizeof (cc));
    cc.lStructSize  = sizeof (cc);
    cc.hwndOwner    = this->GetSafeHwnd ();
    cc.lpCustColors = (LPDWORD) acrCustClr;
    cc.rgbResult    = m_Options->DrawColor;
    cc.Flags        = CC_RGBINIT;

    // show the coosecolor dialog
    if ((m_Options != NULL) && ChooseColor (&cc))
    {
		//dirty may change
		if(!cc.rgbResult)
        {
			m_Options->DrawColor = RGB(0,0,1);
        }
		else
        {
			m_Options->DrawColor = cc.rgbResult;
        }

        if (dlg != NULL)
        {
            ((uEyeCrosshairDlg*) dlg)->DrawObjects ();
        }

	    if(GetDlgItem(IDC_BTN_CHOOSE_COLOR) != NULL)
        {
		    GetDlgItem(IDC_BTN_CHOOSE_COLOR)->Invalidate();
        }
	}
}


HBRUSH CProgramConfigDlg::OnCtlColor (CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = NULL;

    if (pDC != NULL)
    {
        // draw the button in the chosen color
        hbr = CPropertyPage::OnCtlColor (pDC, pWnd, nCtlColor);

        CBrush brBK, *oldBrush;
        RECT   rcBounds;
      
        if ((pWnd != NULL) && (m_Options != NULL))
        {
            // Test if its the right control
            if (pWnd->GetDlgCtrlID () == IDC_BTN_CHOOSE_COLOR)
            {
                if (pWnd->IsWindowEnabled ())
                {
                    brBK.CreateSolidBrush (m_Options->DrawColor);
                }
                else
                {
                    brBK.CreateHatchBrush (HS_DIAGCROSS, m_Options->DrawColor);
                }

                pWnd->GetClientRect (&rcBounds);
                oldBrush = pDC->SelectObject(&brBK);
                pDC->FillRect(&rcBounds, &brBK);        
                pDC->SelectObject(oldBrush);
            }
    
            if (pWnd->GetDlgCtrlID() == IDC_BTN_CHOOSE_KEYCOLOR)
            {
                if (pWnd->IsWindowEnabled ())
                {
                    brBK.CreateSolidBrush (m_Options->KeyColor);
                }
                else
                {
                    brBK.CreateHatchBrush(HS_DIAGCROSS, m_Options->KeyColor);
                }

                pWnd->GetClientRect (&rcBounds);
                oldBrush = pDC->SelectObject(&brBK);
                pDC->FillRect (&rcBounds, &brBK);        
                pDC->SelectObject (oldBrush);
            }
        }
    }

    return hbr;
}


void CProgramConfigDlg::OnBnClickedChkStartfullscreen ()
{
    // set the option to start in fullscreen
    if (m_Options != NULL)
    {
        CButton* pBtn = (CButton*) GetDlgItem (IDC_CHK_STARTFULLSCREEN);

        if (pBtn != NULL)
        {
            m_Options->bStartFullscreen = (pBtn->GetCheck () != 0);
        }
    }
}


void CProgramConfigDlg::OnCbnSelchangeCmbWidth ()
{
    if (m_Options != NULL)
    {
        // set the width of the pencil
        CString strTemp;
        m_ctrlPencilWidth.GetLBText (m_ctrlPencilWidth.GetCurSel (), strTemp);
      
        m_Options->nPencilWidth = _tstoi (strTemp);

        if (dlg != NULL)
        {
            ((uEyeCrosshairDlg*) dlg)->DrawObjects ();
        }
    }
}


void CProgramConfigDlg::OnEnChangeEditCircleRadius()
{
    if (m_Options != NULL)
    {
        // set the radius of the circle
        if (GetDlgItemInt (IDC_EDIT_CIRCLE_RADIUS) > 100000)
        {
            SetDlgItemInt (IDC_EDIT_CIRCLE_RADIUS, 100000);
            m_Options->nCircleRadius = GetDlgItemInt (IDC_EDIT_CIRCLE_RADIUS);
        }
        else
        {
            m_Options->nCircleRadius = GetDlgItemInt (IDC_EDIT_CIRCLE_RADIUS);
        }

        // redraw
        if (dlg != NULL)
        {
            ((uEyeCrosshairDlg*) dlg)->DrawObjects ();
        }
    }
}


void CProgramConfigDlg::OnBnClickedBtnFolder ()
{
}


void CProgramConfigDlg::OnBnClickedChkShowPicture ()
{
    if (m_Options != NULL)
    {
        CButton* pBtn = (CButton*) GetDlgItem (IDC_CHK_SHOW_PICTURE);

        // show the picture?
        m_Options->bShowPicture = (pBtn != NULL) ? (pBtn->GetCheck () != 0) : false;
     
        // redraw
        if (dlg != NULL)
        {
            ((uEyeCrosshairDlg*) dlg)->DrawObjects ();
        }
    }
}


void CProgramConfigDlg::OnBnClickedBtnChooseKeycolor ()
{
    if (m_Options != NULL)
    {
        CHOOSECOLOR cc;
        static COLORREF acrCustClr[16]; 
      
        // Array of custom colors     
        ZeroMemory (acrCustClr, 16 * sizeof (DWORD)); 

        // Fill choosecolor structure
        ZeroMemory (&cc, sizeof (cc));
      
        cc.lStructSize  = sizeof (cc);
        cc.hwndOwner    = this->GetSafeHwnd ();
        cc.lpCustColors = (LPDWORD) acrCustClr;
        cc.rgbResult    = m_Options->KeyColor;
        cc.Flags        = CC_RGBINIT;

        // show the coosecolor dialog
        if(ChooseColor (&cc))
        {
            m_Options->KeyColor = cc.rgbResult;

            if (dlg != NULL)
            {
                ((uEyeCrosshairDlg*) dlg)->DrawObjects ();
            }

		    if(GetDlgItem(IDC_BTN_CHOOSE_KEYCOLOR))
            {
			    GetDlgItem(IDC_BTN_CHOOSE_KEYCOLOR)->Invalidate();
            }
        }
    }
}


void CProgramConfigDlg::OnEnChangeEditnPicturePosY ()
{
    if ((dlg != NULL) && (m_Options != NULL) && (GetDlgItem (IDC_EDIT_POSITION_Y) != NULL))
    {
        // check if input is not to high
        if (GetDlgItemInt (IDC_EDIT_POSITION_Y) > ((uEyeCrosshairDlg*) dlg)->m_nSizeY)
        {
            m_Options->nPicturePosY = ((uEyeCrosshairDlg*) dlg)->m_nSizeY;
            SetDlgItemInt (IDC_EDIT_POSITION_Y, ((uEyeCrosshairDlg*) dlg)->m_nSizeY);
        }
        else
        {
            m_Options->nPicturePosY = GetDlgItemInt (IDC_EDIT_POSITION_Y);
        }

	    //redraw
	    ((uEyeCrosshairDlg*)dlg)->DrawObjects();
    }
}


void CProgramConfigDlg::OnEnChangeEditnPicturePosX ()
{
    if ((dlg != NULL) && (m_Options != NULL) && (GetDlgItem (IDC_EDIT_POSITION_X) != NULL))
    {
        // check if input is not to high
        if (GetDlgItemInt (IDC_EDIT_POSITION_X) > ((uEyeCrosshairDlg*) dlg)->m_nSizeX)
        {
            m_Options->nPicturePosX = ((uEyeCrosshairDlg*) dlg)->m_nSizeX;
            SetDlgItemInt (IDC_EDIT_POSITION_X, ((uEyeCrosshairDlg*) dlg)->m_nSizeX);
        }
        else
        {
            m_Options->nPicturePosX = GetDlgItemInt (IDC_EDIT_POSITION_X);
        }

	    //redraw
	    ((uEyeCrosshairDlg*)dlg)->DrawObjects();
    }
}


void CProgramConfigDlg::OnOK()
{
	if(m_Options != NULL)
	{
		if(GetDlgItem(IDC_EDIT_FOLDER) != NULL)
        {
			GetDlgItemText(IDC_EDIT_FOLDER, m_Options->strPicturePath);
        }
	}
}


void CProgramConfigDlg::OnEnChangeEditCrossPosx()
{
    if ((dlg != NULL) && (m_Options != NULL) && (GetDlgItem (IDC_EDIT_CROSS_POSX) != NULL))
    {
        // check if input is not to high
        if (GetDlgItemInt (IDC_EDIT_CROSS_POSX) > ((uEyeCrosshairDlg*) dlg)->m_nSizeX)
        {
            m_Options->nCrossPosX = ((uEyeCrosshairDlg*) dlg)->m_nSizeX;
            SetDlgItemInt (IDC_EDIT_CROSS_POSX, ((uEyeCrosshairDlg*) dlg)->m_nSizeX);
        }
        else
        {
            m_Options->nCrossPosX = GetDlgItemInt (IDC_EDIT_CROSS_POSX);
        }

	    //redraw
	    ((uEyeCrosshairDlg*)dlg)->DrawObjects();
    }
}


void CProgramConfigDlg::OnEnChangeEditCrossPosy()
{
    if ((dlg != NULL) && (m_Options != NULL) && (GetDlgItem (IDC_EDIT_CROSS_POSY) != NULL))
    {
        // check if input is not to high
        if (GetDlgItemInt (IDC_EDIT_CROSS_POSY) > ((uEyeCrosshairDlg*) dlg)->m_nSizeY)
        {
            m_Options->nCrossPosY = ((uEyeCrosshairDlg*) dlg)->m_nSizeY;
            SetDlgItemInt (IDC_EDIT_CROSS_POSY, ((uEyeCrosshairDlg*) dlg)->m_nSizeY);
        }
        else
        {
            m_Options->nCrossPosY = GetDlgItemInt (IDC_EDIT_CROSS_POSY);
        }

	    //redraw
	    ((uEyeCrosshairDlg*)dlg)->DrawObjects();
    }
}


void CProgramConfigDlg::OnBnClickedButtonResetPos()
{
    if ((dlg != NULL) && m_Options != NULL)
    {
        if (GetDlgItem (IDC_EDIT_CROSS_POSY) != NULL)
        {
            SetDlgItemInt (IDC_EDIT_CROSS_POSY, m_nCanvasSizeY / 2);
        }

        if (GetDlgItem (IDC_EDIT_CROSS_POSX) != NULL)
        {
            SetDlgItemInt (IDC_EDIT_CROSS_POSX, m_nCanvasSizeX / 2);
        }
    }
}


void CProgramConfigDlg::OnEnChangeEditFolder()
{
    if (m_Options != NULL)
    {
        if (((CButton*) GetDlgItem (IDC_CHK_SHOW_PICTURE))->GetCheck())
        {
            // redraw
            if (dlg != NULL)
            {
                CString strPath;
                GetDlgItemText(IDC_EDIT_FOLDER,strPath);
                m_Options->strPicturePath = strPath;
                ((uEyeCrosshairDlg*) dlg)->DrawObjects ();
            }
        }
        else
        {
            CString strPath;
            GetDlgItemText(IDC_EDIT_FOLDER,strPath);
            m_Options->strPicturePath = strPath;
        }
    }
}
