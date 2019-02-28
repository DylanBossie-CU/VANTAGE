// uEyeComportDemo.h : Hauptheaderdatei für die uEyeComportDemo-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error 'stdafx.h' muss vor dieser Datei in PCH eingeschlossen werden.
#endif

#include "resource.h"		// Hauptsymbole


// CuEyeComportDemoApp:
// Siehe uEyeComportDemo.cpp für die Implementierung dieser Klasse
//

class CuEyeComportDemoApp : public CWinApp
{
public:
	CuEyeComportDemoApp();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CuEyeComportDemoApp theApp;
