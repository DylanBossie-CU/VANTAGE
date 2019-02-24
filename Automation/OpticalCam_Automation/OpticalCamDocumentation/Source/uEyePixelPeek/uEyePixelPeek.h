// uEyePixelPeek.h : main header file for the UEYEPIXELPEEK application
//

#if !defined(AFX_UEYEPIXELPEEK_H__C928F381_0973_4AF9_8E08_A1CAD4A5008C__INCLUDED_)
#define AFX_UEYEPIXELPEEK_H__C928F381_0973_4AF9_8E08_A1CAD4A5008C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUEyePixelPeekApp:
// See uEyePixelPeek.cpp for the implementation of this class
//

class CUEyePixelPeekApp : public CWinApp
{
public:
	CUEyePixelPeekApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUEyePixelPeekApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUEyePixelPeekApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UEYEPIXELPEEK_H__C928F381_0973_4AF9_8E08_A1CAD4A5008C__INCLUDED_)
