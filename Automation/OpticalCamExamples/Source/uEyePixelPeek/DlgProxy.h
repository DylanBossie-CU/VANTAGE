// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__BA69ACD7_5D5D_4D16_AEF0_ED8B628AE0DC__INCLUDED_)
#define AFX_DLGPROXY_H__BA69ACD7_5D5D_4D16_AEF0_ED8B628AE0DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUEyePixelPeekDlg;

/////////////////////////////////////////////////////////////////////////////
// CUEyePixelPeekDlgAutoProxy command target

class CUEyePixelPeekDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CUEyePixelPeekDlgAutoProxy)

	CUEyePixelPeekDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CUEyePixelPeekDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUEyePixelPeekDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CUEyePixelPeekDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CUEyePixelPeekDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CUEyePixelPeekDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CUEyePixelPeekDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__BA69ACD7_5D5D_4D16_AEF0_ED8B628AE0DC__INCLUDED_)
