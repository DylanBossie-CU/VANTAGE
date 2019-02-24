// uEyeEvent.h : main header file for the UEYEEVENT application
//
#pragma warning(disable: 4996)

#if !defined(AFX_UEYEEVENT_H__1FD4ECB6_DBDE_4BC0_8E82_5A61664A2152__INCLUDED_)
#define AFX_UEYEEVENT_H__1FD4ECB6_DBDE_4BC0_8E82_5A61664A2152__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CuEyeEventApp:
// See uEyeEvent.cpp for the implementation of this class
class CuEyeEventApp : public CWinApp
{
 public:
	CuEyeEventApp();

	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_UEYEEVENT_H__1FD4ECB6_DBDE_4BC0_8E82_5A61664A2152__INCLUDED_)
