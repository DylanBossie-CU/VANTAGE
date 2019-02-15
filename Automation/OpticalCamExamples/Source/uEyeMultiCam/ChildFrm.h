// ChildFrm.h : Schnittstelle der Klasse CChildFrame
//


#pragma once
#include <vector>

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

	// Attribute
public:

	// Vorgänge
public:

	// Überschreibungen
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementierung
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
};
