//==========================================================================//
//                                                                          //
//  Copyright (C) 2009 - 2018                                               //
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
#include "uEyeDirectRenderer.h"
#include "uEyeDirectRendererDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP (CUEyeDirectRendererApp, CWinApp)
	ON_COMMAND (ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP ()


CUEyeDirectRendererApp::CUEyeDirectRendererApp ()
{
}

CUEyeDirectRendererApp theApp;


BOOL CUEyeDirectRendererApp::InitInstance ()
{
	InitCommonControls();

// if Visual Studio 6.0
#if _MSC_VER < 1300					
	#ifdef _AFXDLL
		Enable3dControls ();			// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic ();	    // Call this when linking to MFC statically
	#endif
#endif

	CUEyeDirectRendererDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal ();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
