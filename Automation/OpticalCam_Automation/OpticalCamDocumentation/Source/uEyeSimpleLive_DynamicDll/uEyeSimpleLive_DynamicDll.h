#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

class CuEyeSimpleLive_DynamicDllApp : public CWinApp
{
public:
	CuEyeSimpleLive_DynamicDllApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CuEyeSimpleLive_DynamicDllApp theApp;