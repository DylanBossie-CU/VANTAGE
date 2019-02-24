// ChildFrm.cpp : Implementierung der Klasse CChildFrame
//
#include "stdafx.h"
#include "Multi_Cam.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CChildFrame-Erstellung/Zerstörung

CChildFrame::CChildFrame()
{

}

CChildFrame::~CChildFrame()
{

}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Ändern Sie die Fensterklasse oder die Stile hier, indem Sie CREATESTRUCT ändern
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame-Diagnose

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame-Meldungshandler
