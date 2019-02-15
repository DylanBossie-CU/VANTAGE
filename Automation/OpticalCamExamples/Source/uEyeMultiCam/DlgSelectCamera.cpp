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

#include "stdafx.h"
#include "Multi_Cam.h"
#include "DlgSelectCamera.h"


IMPLEMENT_DYNAMIC(CDlgSelectCamera, CDialog)
CDlgSelectCamera::CDlgSelectCamera(CWnd* pParent /*=NULL*/)
: CDialog(CDlgSelectCamera::IDD, pParent)
{
    m_nDevID = 0;
    m_bFirmwareUpload = FALSE;
}


CDlgSelectCamera::~CDlgSelectCamera()
{

}


void CDlgSelectCamera::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_CAMERAS, m_ctrlCameras);
}


BEGIN_MESSAGE_MAP(CDlgSelectCamera, CDialog)
    ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage )
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_CAMERAS, OnLvnItemActivateListCameras)
    ON_WM_CLOSE()
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CAMERAS, OnLvnColumnclickListCameras)
    ON_BN_CLICKED(IDOK, &CDlgSelectCamera::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSelectCamera-Meldungshandler


// --------------------------- OnInitDialog ------------------------------------
//
BOOL CDlgSelectCamera::OnInitDialog()
{
    CDialog::OnInitDialog();

    /* init list header */
    if (m_ctrlCameras.GetHeaderCtrl()->GetItemCount() == 0)
    {
        CString Str;
        int ix = 0;

        Str = L"Available";
        m_ctrlCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 65 );

        Str = L"Type";
        m_ctrlCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 40 );

        Str = L"Cam ID";
        m_ctrlCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 54 );

        Str = L"Dev ID";
        m_ctrlCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 50 );

        Str = L"Model";
        m_ctrlCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 90 );

        Str = L"SerNo";
        m_ctrlCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 90 );

        m_ctrlCameras.InsertColumn( ix++, L"",   LVCFMT_LEFT, 0 );

        m_ctrlCameras.SetExtendedStyle( LVS_EX_FULLROWSELECT );

        //TODO: Remove?
#if 0
        if ( !m_imgList.GetSafeHandle() )
        {
            CBitmap bitmap;
            bitmap.LoadBitmap(IDB_SYMBOLS);
            m_imgList.Create( 16, 16, ILC_COLOR16, 8, 0);
            m_imgList.Add(&bitmap, RGB(0,0,0));
            bitmap.DeleteObject();
            m_ctrlCameras.SetImageList( &m_imgList, LVSIL_SMALL );
        }
#endif
    }

    /* receive remove/connect message */
    is_EnableMessage (0, IS_DEVICE_REMOVAL, m_hWnd );
    is_EnableMessage (0, IS_NEW_DEVICE, m_hWnd );

    /* update listcontrol entries */
    RefreshList();

    /* if just one camera free use it if possible */
    if (m_ctrlCameras.GetItemCount() == 1 )
    {
        /* is camera available? */
        if (m_ctrlCameras.GetItemText(0, 0).Compare(L"Yes") == 0)
        {
            m_ctrlCameras.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
            OnOK();
        }
    }
    else if (m_ctrlCameras.GetItemCount() > 1)
    {
        if(((GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0) || ((GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0))
        {
            /* available ? */
            for(int i = 0; i < m_ctrlCameras.GetItemCount(); i++)
            {
                /* Get first available camera */
                if (m_ctrlCameras.GetItemText(i, 0).Compare(L"Yes") == 0)
                {
                    m_ctrlCameras.SetItemState( i, LVIS_SELECTED, LVIS_SELECTED );
                    OnOK();   
                }
            }
        }
    }
    else
    {
        /* no camera available */
        CDialog::OnOK();
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}



// --------------------------- OnUEyeMessage ------------------------------------
//
LRESULT CDlgSelectCamera::OnUEyeMessage( WPARAM wParam, LPARAM lParam )
{
    switch (wParam)
    {
    case IS_NEW_DEVICE:
        RefreshList();
        break;
    case IS_DEVICE_REMOVAL:
        RefreshList();
        break;
    }

    return 0;
}


// --------------------------- EmptyList ------------------------------------
//
void CDlgSelectCamera::EmptyList( int nIndex /*=-1*/ )
{
    m_ctrlCameras.DeleteAllItems();
}


// --------------------------- RefreshList ------------------------------------
//
void CDlgSelectCamera::RefreshList()
{
    CString strTemp;

    // Free all cameras
    EmptyList();

    // first request number of cameras to determine the array size	
    int nCameraCount = 0;

    // get number of cameras
    if (is_GetNumberOfCameras(&nCameraCount) == IS_SUCCESS)
    {
        // allocate the required list size
        PUEYE_CAMERA_LIST pucl = (PUEYE_CAMERA_LIST)new char[sizeof(DWORD) + nCameraCount * sizeof(UEYE_CAMERA_INFO)];
        pucl->dwCount = nCameraCount;

        // receive the camera list
        if(is_GetCameraList(pucl) == IS_SUCCESS)
        {
            for(int i = 0; i < nCameraCount; i++)
            {
                int ix = 1;
                int icon = 0;

                // Type = 0 -> USB, Type = 1 -> ETH
                int Type = (pucl->uci[i].dwDeviceID >= 1000);

                // If ETH camera
                if(Type == 1)
                {
                    // If device is not used 
                    if(pucl->uci[i].dwInUse == 0)
                    {
                        // Check if starter firmware is compatible to the driver (Only SE)
                        if((pucl->uci[i].dwStatus & DEVSTS_INCLUDED_STARTER_FIRMWARE_INCOMPATIBLE) != 0)
                        {
                            icon = 8;
                            strTemp = L"Yes";    
                        }
                        else
                        {                  
                            icon = 3;
                            strTemp = L"Yes";
                        }
                    }
                    else
                    {
                        icon = 4;
                        strTemp = L"No";
                    }

                    m_ctrlCameras.InsertItem( i, strTemp, icon);
                    m_ctrlCameras.SetItemText( i, ix++, L"ETH");
                }
                // If USB camera
                else if(Type == 0)
                {
                    bool FirmwareDownloadSupported = (pucl->uci[i].dwStatus & FIRMWARE_DOWNLOAD_NOT_SUPPORTED) == 0;
                    bool InterfaceSpeedSupported = (pucl->uci[i].dwStatus & INTERFACE_SPEED_NOT_SUPPORTED) == 0;

                    // If device is not used 
                    if(pucl->uci[i].dwInUse == 0)
                    {
                        if(FirmwareDownloadSupported && InterfaceSpeedSupported)
                        {
                            icon = 0;
                            strTemp = L"Yes";
                        }
                        else
                        {
                            icon = 1;
                            strTemp = L"No";
                        }
                    }
                    else
                    {
                        icon = 1;
                        strTemp = L"No";
                    }

                    m_ctrlCameras.InsertItem( i, strTemp, icon);
                    m_ctrlCameras.SetItemText( i, ix++, L"USB");
                }

                // Camera ID
                strTemp.Format(L"%d", pucl->uci[i].dwCameraID );
                m_ctrlCameras.SetItemText( i, ix++, strTemp);

                // Device ID
                strTemp.Format(L"%d", pucl->uci[i].dwDeviceID );
                m_ctrlCameras.SetItemText( i, ix++, strTemp);

                // Model
                WCHAR temp[100];
                mbstowcs(temp, pucl->uci[i].Model, 100);
                strTemp.Format(L"%s", temp);
                m_ctrlCameras.SetItemText( i, ix++, strTemp);

                // Serial Number
                mbstowcs(temp,  pucl->uci[i].SerNo, 100);
                strTemp.Format(L"%s", temp);
                m_ctrlCameras.SetItemText( i, ix++, strTemp);

                // Firmware compatibility
                if((pucl->uci[i].dwStatus & DEVSTS_INCLUDED_STARTER_FIRMWARE_INCOMPATIBLE) != 0)
                {
                    m_ctrlCameras.SetItemText(i, ix++, L"1");
                }
                else
                {
                    m_ctrlCameras.SetItemText(i, ix++, L"0");
                }

                // set data to the device ID
                m_ctrlCameras.SetItemData(i, pucl->uci[i].dwDeviceID);
            }
        }
        else
        {
            strTemp = L"Could not receive camera list";
            AfxMessageBox(strTemp);
        }

        /* free allocated memory */
        delete pucl;
    }
    else
    {
        strTemp = L"Could not receive camera count";
        AfxMessageBox(strTemp);
    }

    m_ctrlCameras.SortItems(SortListCtrl, (LPARAM) &m_ctrlCameras);
    m_ctrlCameras.SetRedraw(TRUE);
}

// --------------------------- OnOK ------------------------------------
//
void CDlgSelectCamera::OnOK()
{
    POSITION pos = m_ctrlCameras.GetFirstSelectedItemPosition();

    /* is a row selected? */
    if (!pos)
    {
        AfxMessageBox(L"No camera selected");
        return;
    }

    /* get selected item */
    int nItem = m_ctrlCameras.GetNextSelectedItem(pos);

    if ((m_ctrlCameras.GetItemText(nItem, 0).Compare(L"Yes")) != 0)
    {
        AfxMessageBox(L"Camera not available");
        return;
    }

    m_nDevID = (int)m_ctrlCameras.GetItemData(nItem);
    m_sModel = m_ctrlCameras.GetItemText(nItem, 4);

    EmptyList(nItem);
    CDialog::OnOK();
}


// --------------------------- OnLvnItemActivateListCameras ------------------------------------
//
void CDlgSelectCamera::OnLvnItemActivateListCameras(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    OnOK();
    *pResult = 0;
}


// --------------------------- OnClose ------------------------------------
//
void CDlgSelectCamera::OnClose()
{
    EmptyList();
    CDialog::OnClose();
}


// --------------------------- OnCancel ------------------------------------
//
void CDlgSelectCamera::OnCancel()
{
    EmptyList();
    CDialog::OnCancel();
}


// Sort function for the CListCtrl
static int CALLBACK SortListCtrl(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    // lParamSort contains a pointer to the list view control.
    CListCtrl* pListCtrl = (CListCtrl*) lParamSort;

    int Index;
    LVFINDINFO FindInfo;
    FindInfo.flags = LVFI_PARAM;
    FindInfo.lParam = lParam1;
    Index = pListCtrl->FindItem(&FindInfo);
    CString    strItem1 = pListCtrl->GetItemText(Index, SortColumn);

    FindInfo.lParam = lParam2;
    Index = pListCtrl->FindItem(&FindInfo);
    CString    strItem2 = pListCtrl->GetItemText(Index, SortColumn);

    int ret;

    // If the column value is a number
    if((SortColumn == 2) || (SortColumn == 3)) 
    {
        int a = _wtoi(strItem1);
        int b = _wtoi(strItem2);
        if(a < b) ret = -1;
        if(a == b) ret = 0;
        if(a > b) ret = 1;
    }

    // If the column value is text
    else
    {
        ret = wcscmp(strItem1, strItem2);
    }

    // Sort descending
    if(Direction == false)
        ret = ret * (-1);

    return ret;
}


void CDlgSelectCamera::OnLvnColumnclickListCameras(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // Sort the ListCtrl in the other direction next time
    Direction = !Direction;

    // Get the column
    SortColumn = pNMLV->iSubItem;

    // Sort the list
    m_ctrlCameras.SortItems(SortListCtrl, (LPARAM) &m_ctrlCameras);
    m_ctrlCameras.SetRedraw(TRUE);

    *pResult = 0;
}

void CDlgSelectCamera::OnBnClickedOk()
{
    // TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
    OnOK();
}

