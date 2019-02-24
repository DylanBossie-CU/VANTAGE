#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"    

class CMulti_CamApp : public CWinApp
{
    
public:
	CMulti_CamApp();

	virtual BOOL InitInstance();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMulti_CamApp theApp;