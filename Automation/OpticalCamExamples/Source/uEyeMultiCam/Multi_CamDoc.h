// Multi_CamDoc.h : Schnittstelle der Klasse CMulti_CamDoc
//
#include "ueye.h"

#pragma once


class CMulti_CamDoc : public CDocument
{
protected: // Nur aus Serialisierung erstellen
	CMulti_CamDoc();
	DECLARE_DYNCREATE(CMulti_CamDoc)

// Attribute
public:

// Vorg�nge
public:

// �berschreibungen
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementierung
public:
	virtual ~CMulti_CamDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    int m_nCamDevID;

protected:

// Generierte Funktionen f�r die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
};


