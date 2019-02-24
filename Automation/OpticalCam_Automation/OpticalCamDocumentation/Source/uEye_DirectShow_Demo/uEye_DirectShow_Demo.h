// uEye_DirectShow_Demo.h : Hauptheaderdatei für die uEye_DirectShow_Demo-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error 'stdafx.h' muss vor dieser Datei in PCH eingeschlossen werden.
#endif

#include "resource.h"		// Hauptsymbole


// uEye_DirectShow_Demo_App:
// Siehe uEye_DirectShow_Demo.cpp für die Implementierung dieser Klasse
//

class uEye_DirectShow_Demo_App : public CWinApp
{
public:
	uEye_DirectShow_Demo_App();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern uEye_DirectShow_Demo_App theApp;
