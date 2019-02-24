// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "stdafx.h"
#include "Multi_Cam.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	// Statusleistenanzeige
	ID_SEPARATOR,           
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};


// CMainFrame-Erstellung/Zerstörung

CMainFrame::CMainFrame()
{
	// TODO: Hier Code für die Memberinitialisierung einfügen
}

CMainFrame::~CMainFrame()
{

}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0,0,0,0), IDR_MAINFRAME_8BIT_DEPTH_DUMMY) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME_8BIT_DEPTH_DUMMY))
	{
		TRACE0("Fehler beim Erstellen der Symbolleiste.\n");
		return -1;      // Fehler beim Erstellen
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Fehler beim Erstellen der Statusleiste.\n");
		return -1;      // Fehler beim Erstellen
	}

	CBitmap bitmapToolbar, bitmapToolbarHot, bitmapToolbarDisable;
	CImageList imageListToolbar, imageListToolbarHot, imageListToolbarDisable;
	COLORREF colorTransparent = RGB(217,213,206);

	bitmapToolbar.LoadBitmap(IDR_MAINFRAME);
	bitmapToolbarHot.LoadBitmap(IDB_BITMAP2);
	bitmapToolbarDisable.LoadBitmap(IDB_BITMAP3);

	imageListToolbar.Create(36, 32, ILC_COLOR24 | ILC_MASK, 0, 0);
	imageListToolbar.Add(&bitmapToolbar, colorTransparent);

	imageListToolbarHot.Create(36, 32, ILC_COLOR24 | ILC_MASK, 0, 0);
	imageListToolbarHot.Add(&bitmapToolbarHot, colorTransparent);

	imageListToolbarDisable.Create(36, 32, ILC_COLOR24 | ILC_MASK, 0, 0);
	imageListToolbarDisable.Add(&bitmapToolbarDisable, colorTransparent);

	m_wndToolBar.GetToolBarCtrl().SetImageList(&imageListToolbar);
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&imageListToolbarHot);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&imageListToolbarDisable);

	imageListToolbar.Detach(); 
	imageListToolbarHot.Detach();
	imageListToolbarDisable.Detach();

	// TODO: Löschen Sie diese drei Zeilen, wenn Sie nicht möchten, dass die Systemleiste andockbar ist
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Ändern Sie hier die Fensterklasse oder die Darstellung, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return TRUE;
}

// CMainFrame-Diagnose

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame-Meldungshandler
