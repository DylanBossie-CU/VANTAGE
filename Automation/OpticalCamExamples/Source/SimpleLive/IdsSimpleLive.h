// IdsSimpleLive.h : main header file for the IDSSIMPLELIVE application
//

#if !defined(AFX_IDSSIMPLELIVE_H__3066A7A2_C7D9_4E0E_AF60_510BE2961B11__INCLUDED_)
#define AFX_IDSSIMPLELIVE_H__3066A7A2_C7D9_4E0E_AF60_510BE2961B11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CIdsSimpleLiveApp:
// See IdsSimpleLive.cpp for the implementation of this class
//

class CIdsSimpleLiveApp : public CWinApp
{
public:
	CIdsSimpleLiveApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIdsSimpleLiveApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIdsSimpleLiveApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDSSIMPLELIVE_H__3066A7A2_C7D9_4E0E_AF60_510BE2961B11__INCLUDED_)
