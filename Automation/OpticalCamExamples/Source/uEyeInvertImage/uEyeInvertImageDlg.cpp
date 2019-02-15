//==========================================================================//
//                                                                          //
//  Copyright (C) 2004 - 2018                                               //
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


// uEyeInvertImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uEyeInvertImage.h"
#include "uEyeInvertImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuEyeInvertImageDlg dialog




CuEyeInvertImageDlg::CuEyeInvertImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CuEyeInvertImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CuEyeInvertImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CuEyeInvertImageDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CuEyeInvertImageDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_SNAP, &CuEyeInvertImageDlg::OnBnClickedButtonSnap)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CuEyeInvertImageDlg message handlers

BOOL CuEyeInvertImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// open next available camera 
	CamOpen();		  


	return TRUE;  // return TRUE  unless you set the focus to a control
}



// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CuEyeInvertImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		DisplayImageBuffers();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CuEyeInvertImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CuEyeInvertImageDlg::OnCancel() 
{
  // nop
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::CamOpen()
//
// DESCRIPTION: Opens a handle to a connected camera
//
///////////////////////////////////////////////////////////////////////////////
bool CuEyeInvertImageDlg::CamOpen()
{

	// init camera
	INT Ret;
	m_hCam = (HIDS) 0;  // open next camera
	Ret = InitCamera(&m_hCam, NULL);		
	if( Ret == IS_SUCCESS )
	{
		// enable the dialog based error report
		//Ret = is_SetErrorReport(m_hCam, IS_ENABLE_ERR_REP); // or IS_DISABLE_ERR_REP);
		//if( Ret != IS_SUCCESS )
		//{
		//	AfxMessageBox( _T("ERROR: Can not enable the automatic uEye error report!") , MB_ICONEXCLAMATION, 0 );
		//	return false;
		//}

        // query camera information
        SENSORINFO SensorInfo;
        is_GetSensorInfo(m_hCam, &SensorInfo );
    
        GetMaxImageSize(&m_nSizeX, &m_nSizeY);

        // setup image size
        IS_SIZE_2D imageSize;
        imageSize.s32Width = m_nSizeX;
        imageSize.s32Height = m_nSizeY;

        is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

        // use color depth according to monochrome or color camera
        if( SensorInfo.nColorMode == IS_COLORMODE_MONOCHROME )
        {
			// monochrome camera
			m_nBitsPerPixel =  8;
            is_SetColorMode(m_hCam, IS_CM_MONO8 );
        }
        else
        {
            // color camera
            m_nBitsPerPixel =  24;
            is_SetColorMode( m_hCam, IS_CM_BGR8_PACKED );

			// enable auto whitebalance	
			UEYE_AUTO_INFO autoInfo;
            double dblVal = 1.0;
            if (is_GetAutoInfo(m_hCam, &autoInfo) == IS_SUCCESS)
            {
                // sensor whitebalance is supported
                if (autoInfo.AutoAbility & AC_SENSOR_WB)
                {
                    is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &dblVal, NULL);
                }
                // sensor whitebalance is not supported
                else
                {
                    if (autoInfo.AutoAbility & AC_WHITEBAL)
                    {
                        // Try to activate software whitebalance
                        is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dblVal, NULL);
                    }
                }
            } 
        }


		// display initialization
        m_hWndDispOrg = GetDlgItem( IDC_ORIGINAL_IMAGE )->GetSafeHwnd(); 
        m_hWndDispInv = GetDlgItem( IDC_INVERTED_IMAGE )->GetSafeHwnd(); 
		is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);


        // allocate image buffer memory
        is_AllocImageMem( m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pcMemOrg, &m_lMemIdOrg);
        is_AllocImageMem( m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pcMemInv, &m_lMemIdInv);
		is_SetImageMem( m_hCam, m_pcMemOrg, m_lMemIdOrg ); // grab into this buffer
	

		// initial image acquisition
		GrabAndInvert();

    }
    else
    {	
		// terminate application
	    AfxMessageBox( _T("ERROR: Cannot open uEye camera!") , MB_ICONEXCLAMATION, 0 );
	    PostQuitMessage( 0 );
    }
  
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::OnBnClickedButtonExit()
//
// DESCRIPTION:		- exit the camera
//				    - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeInvertImageDlg::OnBnClickedButtonExit()
{  
	// exit camera 
	if( m_hCam )
	{
		// stop capture
		is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP);

		// memory and events are automatically released
		is_ExitCamera( m_hCam );
	}
    	
    // terminate
    PostQuitMessage( 0 );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::OnClose()
//
// DESCRIPTION:		- exit the camera
//				    - quit application
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeInvertImageDlg::OnClose()
{
    CDialog::OnClose();

    // exit camera 
	if( m_hCam )
	{
		// stop capture
		is_StopLiveVideo( m_hCam, IS_FORCE_VIDEO_STOP);

		// memory and events are automatically released
		is_ExitCamera( m_hCam );
	}

    // terminate
    PostQuitMessage( 0 ); 
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::OnBnClickedButtonSnap()
//
// DESCRIPTION:		- call grab and invert and image
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeInvertImageDlg::OnBnClickedButtonSnap()
{
		GrabAndInvert();
}



///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::GrabAndInvert()
//
// DESCRIPTION:		- grab an image
//					- call image inverter
//				    - call buffer display
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeInvertImageDlg::GrabAndInvert()
{
	is_FreezeVideo( m_hCam, IS_WAIT );
	
	InvertImageBuffer();
	DisplayImageBuffers();
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::InvertImageBuffer()
//
// DESCRIPTION:		- perform inversion of image buffer
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeInvertImageDlg::InvertImageBuffer()
{
	// variables
	bool bRet = true;
	unsigned char cPixelValueRead;	// pixel read from source buffer
	unsigned char cPixelValueWrite;	// pixel for writing intodestination buffer
	unsigned long nImageSize;		// image buffer size in bytes
	
	// calculate image size
	nImageSize = m_nSizeX *  m_nSizeY * m_nBitsPerPixel / 8;


	// perform pixel manipulation (invert)
	try
	{
		// loop over all pixels
		for( unsigned long lOffset = 0 ; lOffset<nImageSize ; lOffset++ )
		{
			// read	pixel from source buffer
			cPixelValueRead = *(reinterpret_cast<unsigned char*>(m_pcMemOrg + lOffset) );

			// invert pixel
			cPixelValueWrite = ~cPixelValueRead;

			// write pixel into destination buffer
			*(reinterpret_cast<unsigned char*>(m_pcMemInv + lOffset) ) = cPixelValueWrite;
		}
	}
	catch( ... )
	{
		// too bad - memory access violation
	}
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::DisplayImageBuffers()
//
// DESCRIPTION:		- perform image buffer display
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeInvertImageDlg::DisplayImageBuffers()
{
	is_RenderBitmap( m_hCam, m_lMemIdOrg,  m_hWndDispOrg, IS_RENDER_FIT_TO_WINDOW );
	is_RenderBitmap( m_hCam, m_lMemIdInv,  m_hWndDispInv, IS_RENDER_FIT_TO_WINDOW );
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::InitCamera
//
// DESCRIPTION:		- Open camera
//
///////////////////////////////////////////////////////////////////////////////
INT CuEyeInvertImageDlg::InitCamera (HIDS *hCam, HWND hWnd)
{
    INT nRet = is_InitCamera (hCam, hWnd);	
    /************************************************************************************************/
    /*                                                                                              */
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*                                                                                              */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */ 
    /*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
    /*                                                                                              */                        
    /************************************************************************************************/
    if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
    {
        // Time for the firmware upload = 25 seconds by default
        INT nUploadTime = 25000;
        is_GetDuration (*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);
    
        CString Str1, Str2, Str3;
        Str1 = L"This camera requires a new firmware. The upload will take about";
        Str2 = L"seconds. Please wait ...";
        Str3.Format (L"%s %d %s", Str1, nUploadTime / 1000, Str2);
        AfxMessageBox (Str3, MB_ICONWARNING);
    
        // Set mouse to hourglass
	    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS) (((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
        nRet = is_InitCamera (hCam, hWnd);   
    }
    
    return nRet;
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD CuEyeInvertImageDlg::GetMaxImageSize
//
// DESCRIPTION:		- Get the maximum image size
//
///////////////////////////////////////////////////////////////////////////////
void CuEyeInvertImageDlg::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
    // Check if the camera supports an arbitrary AOI
    // Only the ueye xs does not support an arbitrary AOI
    INT nAOISupported = 0;
    BOOL bAOISupported = TRUE;
    if (is_ImageFormat(m_hCam,
                       IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED, 
                       (void*)&nAOISupported, 
                       sizeof(nAOISupported)) == IS_SUCCESS)
    {
        bAOISupported = (nAOISupported != 0);
    }

    if (bAOISupported)
    {  
        // All other sensors
        // Get maximum image size
	    SENSORINFO sInfo;
	    is_GetSensorInfo (m_hCam, &sInfo);
	    *pnSizeX = sInfo.nMaxWidth;
	    *pnSizeY = sInfo.nMaxHeight;
    }
    else
    {
        // Only ueye xs
		// Get image size of the current format
		IS_SIZE_2D imageSize;
		is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

		*pnSizeX = imageSize.s32Width;
		*pnSizeY = imageSize.s32Height;
    }
}