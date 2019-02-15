// Multi_CamDoc.cpp : Implementierung der Klasse CMulti_CamDoc
//

#include "stdafx.h"
#include "Multi_Cam.h"
#include "DlgSelectCamera.h"
#include "Multi_CamDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMulti_CamDoc

IMPLEMENT_DYNCREATE(CMulti_CamDoc, CDocument)

BEGIN_MESSAGE_MAP(CMulti_CamDoc, CDocument)
END_MESSAGE_MAP()


// CMulti_CamDoc-Erstellung/Zerstörung

CMulti_CamDoc::CMulti_CamDoc()
{
	// TODO: Hier Code für One-Time-Konstruktion einfügen

}

CMulti_CamDoc::~CMulti_CamDoc()
{
}

BOOL CMulti_CamDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    CDlgSelectCamera dlgSelectCamera;
    if (dlgSelectCamera.DoModal() != IDOK)
    {
        return FALSE;
    }

    m_nCamDevID = dlgSelectCamera.m_nDevID;

	return TRUE;
}


// CMulti_CamDoc-Serialisierung

void CMulti_CamDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: Hier Code zum Speichern einfügen
	}
	else
	{
		// TODO: Hier Code zum Laden einfügen
	}
}


// CMulti_CamDoc-Diagnose

#ifdef _DEBUG
void CMulti_CamDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMulti_CamDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMulti_CamDoc-Befehle
