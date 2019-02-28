// IdsSimpleAcquire.h : main header file for the IDSSIMPLEACQUIRE application
//

#if !defined(AFX_IDSSIMPLEACQUIRE_H__37478BE3_A3F6_4D4B_8CD0_BE0712A58367__INCLUDED_)
#define AFX_IDSSIMPLEACQUIRE_H__37478BE3_A3F6_4D4B_8CD0_BE0712A58367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifdef _WIN32_WCE
    #include "resource_ce.h"		// main symbols
#else
    #include "resource.h"		// main symbols
#endif

/////////////////////////////////////////////////////////////////////////////
// CIdsSimpleAcquireApp:
// See IdsSimpleAcquire.cpp for the implementation of this class
//

class CIdsSimpleAcquireApp : public CWinApp
{
public:
	CIdsSimpleAcquireApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIdsSimpleAcquireApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIdsSimpleAcquireApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDSSIMPLEACQUIRE_H__37478BE3_A3F6_4D4B_8CD0_BE0712A58367__INCLUDED_)
