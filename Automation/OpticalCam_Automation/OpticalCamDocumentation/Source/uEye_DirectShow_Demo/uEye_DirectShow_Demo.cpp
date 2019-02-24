// uEye_DirectShow_Demo.cpp : Definiert das Klassenverhalten für die Anwendung.
//

#include "stdafx.h"
#include "uEye_DirectShow_Demo.h"
#include "uEye_DirectShow_Demo_Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// uEye_DirectShow_Demo_App

BEGIN_MESSAGE_MAP(uEye_DirectShow_Demo_App, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// uEye_DirectShow_Demo_App-Erstellung

uEye_DirectShow_Demo_App::uEye_DirectShow_Demo_App()
{
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


// Das einzige uEye_DirectShow_Demo_App-Objekt

uEye_DirectShow_Demo_App theApp;


// uEye_DirectShow_Demo_App Initialisierung

BOOL uEye_DirectShow_Demo_App::InitInstance()
{
	// InitCommonControls() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	InitCommonControls();

	CWinApp::InitInstance();

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

	uEye_DirectShow_Demo_Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über OK zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über "Abbrechen" zu steuern
	}

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, so dass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
