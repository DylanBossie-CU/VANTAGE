
#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei f�r PCH einschlie�en"
#endif

#include "resource.h"

class uEyeCrosshairApp : public CWinApp
{
public:
	uEyeCrosshairApp();

	public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern uEyeCrosshairApp theApp;