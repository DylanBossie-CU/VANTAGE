// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "uEyePixelPeek.h"
#include "DlgProxy.h"
#include "uEyePixelPeekDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUEyePixelPeekDlgAutoProxy

IMPLEMENT_DYNCREATE(CUEyePixelPeekDlgAutoProxy, CCmdTarget)

CUEyePixelPeekDlgAutoProxy::CUEyePixelPeekDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CUEyePixelPeekDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CUEyePixelPeekDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CUEyePixelPeekDlgAutoProxy::~CUEyePixelPeekDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CUEyePixelPeekDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CUEyePixelPeekDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CUEyePixelPeekDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUEyePixelPeekDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CUEyePixelPeekDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IUEyePixelPeek to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {AE872FB9-0D15-430E-892C-A61849D31633}
static const IID IID_IUEyePixelPeek =
{ 0xae872fb9, 0xd15, 0x430e, { 0x89, 0x2c, 0xa6, 0x18, 0x49, 0xd3, 0x16, 0x33 } };

BEGIN_INTERFACE_MAP(CUEyePixelPeekDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CUEyePixelPeekDlgAutoProxy, IID_IUEyePixelPeek, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {B2061A39-5F07-411C-96EE-EAAFF303FC69}
IMPLEMENT_OLECREATE2(CUEyePixelPeekDlgAutoProxy, "UEyePixelPeek.Application", 0xb2061a39, 0x5f07, 0x411c, 0x96, 0xee, 0xea, 0xaf, 0xf3, 0x3, 0xfc, 0x69)

/////////////////////////////////////////////////////////////////////////////
// CUEyePixelPeekDlgAutoProxy message handlers
