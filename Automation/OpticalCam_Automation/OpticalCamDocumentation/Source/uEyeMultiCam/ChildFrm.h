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

	// Vorg�nge
public:

	// �berschreibungen
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementierung
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generierte Funktionen f�r die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
};
