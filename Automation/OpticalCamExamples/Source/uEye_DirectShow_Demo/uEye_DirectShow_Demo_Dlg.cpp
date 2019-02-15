//==========================================================================//
//                                                                          //
//  Copyright (C) 2010 - 2018                                               //
//  IDS - Imaging Development Systems GmbH                                  //
//  Dimbacherstr. 6-8                                                       //
//  D-74182 Obersulm-Willsbach                                              //
//                                                                          //
//  The information in this document is subject to change without           //
//  notice and should not be construed as a commitment by IDS Imaging       //
//  Development Systems GmbH.                                               //
//  IDS Imaging Development Systems GmbH does not assume any responsibility //
//  for any errors that may appear in this document.                        //
//                                                                          //
//  This document, or source code, is provided solely as an example         //
//  of how to utilize IDS software libraries in a sample application.       //
//  IDS Imaging Development Systems GmbH does not assume any responsibility //
//  for the use or reliability of any portion of this document or the       //
//  described software.                                                     //
//                                                                          //
//  General permission to copy or modify, but not for profit, is hereby     //
//  granted,  provided that the above copyright notice is included and      //
//  included and reference made to the fact that reproduction privileges    //
//  were granted by IDS Imaging Development Systems GmbH.                   //
//                                                                          //
//  IDS cannot assume any responsibility for the use, or misuse, of any     //
//  portion or misuse, of any portion of this software for other than its   //
//  intended diagnostic purpose in calibrating and testing IDS manufactured //
//  image processing boards and software.                                   //
//                                                                          //
//==========================================================================//


// uEye_DirectShow_Demo_Dlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "uEye_DirectShow_Demo.h"
#include "uEye_DirectShow_Demo_Dlg.h"
#include "version.h"

// include DirectShow interface
#include <dshow.h>

// include uEye interface
#include "uEyeCaptureInterface.h"
#include ".\ueye_directshow_demo_dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg-Dialogfeld für Anwendungsbefehl 'Info'

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString Str;
	GetDlgItem(IDC_STATIC_COPYRIGHT)->GetWindowText(Str);
	Str.Append(_T(CURRENT_YEAR));
	GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(Str);

	return TRUE;
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// uEye_DirectShow_Demo_Dlg Dialogfeld



uEye_DirectShow_Demo_Dlg::uEye_DirectShow_Demo_Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(uEye_DirectShow_Demo_Dlg::IDD, pParent)
    , m_pActiveVideoSource(NULL)
    , m_pActiveFilterGraphManager(NULL)
    , m_iGraphState(-1)
    , m_devListener(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_devListener = new DeviceListener(this);
}

uEye_DirectShow_Demo_Dlg::~uEye_DirectShow_Demo_Dlg()
{
    if(m_devListener != NULL)
    {
        delete m_devListener;
        m_devListener = NULL;
    }
}

void uEye_DirectShow_Demo_Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_VIDEOSOURCES, m_comboVideoSources);
    DDX_Control(pDX, IDC_BUTTON_DUMP_GRAPH, m_bnDumpGraph);
    DDX_Control(pDX, IDC_SLIDER_EXPOSURETIME, m_sliderExposureTime);
    DDX_Control(pDX, IDC_STATIC_MINEXPOSURETIME, m_labelMinExposureTime);
    DDX_Control(pDX, IDC_STATIC_MAXEXPOSURETIME, m_labelMaxExposureTime);
    DDX_Control(pDX, IDC_STATIC_CURRENTEXPOSURETIME, m_labelCurrentExposureTime);
    DDX_Control(pDX, IDC_STATIC_GRAPHSTATE, m_labelGraphState);
    DDX_Control(pDX, IDC_CHECK_AUTO_EXPOSURE_CONTROL_ACTIVE, m_chkAutoExposureControlActive);
    DDX_Control(pDX, IDC_BUTTON_FORCE_TRIGGER, m_bnForceTrigger);
    DDX_Control(pDX, IDC_STATIC_CURRENT_FRAMERATE, m_labelCurrentFramerate);
    DDX_Control(pDX, IDC_BUTTON_STARTGRAPH, m_bnGraphStart);
    DDX_Control(pDX, IDC_BUTTON_STOPGRAPH, m_bnGraphStop);
    DDX_Control(pDX, IDC_BUTTON_PAUSEGRAPH, m_bnGraphPause);
}

BEGIN_MESSAGE_MAP(uEye_DirectShow_Demo_Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_COMBO_VIDEOSOURCES, OnCbnSelchangeComboVideosources)
    ON_BN_CLICKED(IDC_BUTTON_STARTGRAPH, OnBnClickedButtonStartgraph)
    ON_BN_CLICKED(IDC_BUTTON_PAUSEGRAPH, OnBnClickedButtonPausegraph)
    ON_BN_CLICKED(IDC_BUTTON_STOPGRAPH, OnBnClickedButtonStopgraph)
    ON_BN_CLICKED(IDC_BUTTON_PIN_PROPERTIES, OnBnClickedButtonPinProperties)
    ON_BN_CLICKED(IDC_BUTTON_FILTER_PROPERTIES, OnBnClickedButtonFilterProperties)
    ON_BN_CLICKED(IDC_BUTTON_DUMP_GRAPH, OnBnClickedButtonDumpGraph)
    ON_BN_CLICKED(IDC_BUTTON_EXIT, OnBnClickedButtonExit)
    ON_BN_CLICKED(IDC_CHECK_AUTO_EXPOSURE_CONTROL_ACTIVE, OnBnClickedCheckAutoExposureControlActive)
    ON_BN_CLICKED(IDC_BUTTON_FORCE_TRIGGER, OnBnClickedButtonForceTrigger)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_EXPOSURETIME, OnNMReleasedcaptureSliderExposuretime)
	ON_WM_HSCROLL()
    ON_WM_TIMER()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// uEye_DirectShow_Demo_Dlg Meldungshandler

BOOL uEye_DirectShow_Demo_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden



    // clear exposure control displays
    m_labelMinExposureTime.SetWindowText(L"");
    m_labelMaxExposureTime.SetWindowText(L"");
    m_labelCurrentExposureTime.SetWindowText(L"");
    m_labelCurrentFramerate.SetWindowText(L"");

    // DirectShow intitialization
    DirectShow_Init();

    // scan for video sources and fill the list
    VideoSourcesList_Fill();

    // update the graph state view
    UpdateGraphState( -1);

    // disable 'force trigger' button as long as the graph is not running
    m_bnForceTrigger.EnableWindow( FALSE);
    // disable 'dump graph' button as long as no graph present
    m_bnDumpGraph.EnableWindow( FALSE);

    // disable all graph controls
    m_bnGraphStart.EnableWindow(FALSE);
    m_bnGraphPause.EnableWindow(FALSE);
    m_bnGraphStop.EnableWindow(FALSE);

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void uEye_DirectShow_Demo_Dlg::OnDestroy()
{
    CDialog::OnDestroy();

    // stop the filter graph, if any
    FilterGraph_Stop();

    // destroy the filter graph, if any
    FilterGraph_Destroy();

    // clear the video capture sources list and release the monikers, if any
    VideoSourcesList_Clear();

    // DirectShow deinitialization
    DirectShow_Deinit();
}

void uEye_DirectShow_Demo_Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void uEye_DirectShow_Demo_Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR uEye_DirectShow_Demo_Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void uEye_DirectShow_Demo_Dlg::OnCbnSelchangeComboVideosources()
{
    // disable the exposure control
    ExposureControl_Deinit();

    // stop the graph, if any
    FilterGraph_Stop();

    FilterGraphPins_Connect();

    // delete the graph, if any
    FilterGraph_Destroy();

    // get the currently selected list item
    int cbsel= m_comboVideoSources.GetCurSel();
    if( cbsel == CB_ERR)
    {
        MessageBoxEx( NULL, L"No item selected!", _T(__FUNCTION__), MB_ICONERROR, 0);

        // select first list entry
        m_comboVideoSources.SetCurSel( 0);

        return;
    }

    // get the moniker associated to the selected list item
    IMoniker* pMoniker= (IMoniker*)m_comboVideoSources.GetItemDataPtr(cbsel);
    if( pMoniker == NULL)
    {
        // first list item is dummy entry ("[no device selected]")
        if( cbsel != 0)
        {
            MessageBoxEx( NULL, L"No moniker for the selected entry!", _T(__FUNCTION__), MB_ICONERROR, 0);

            // select first list entry
            m_comboVideoSources.SetCurSel( 0);
        }
        m_bnGraphStart.EnableWindow(FALSE);
    }
    else
    {
        if( !FilterGraph_Create( pMoniker))
        {
            // select first list entry
            m_comboVideoSources.SetCurSel( 0);

            return;
        }
        ExposureControl_Init();
        m_bnGraphStart.EnableWindow(TRUE);
    }

    if(m_comboVideoSources.GetCurSel() == 0)
    {
        m_currentDeviceName = "";
    }
    else
    {
        m_comboVideoSources.GetLBText(m_comboVideoSources.GetCurSel(), m_currentDeviceName);
    }
    VideoSourcesList_Refill();
}


void uEye_DirectShow_Demo_Dlg::OnBnClickedButtonStartgraph()
{
    if( FilterGraph_Start())
    {
        // enable 'force trigger' button for the time the graph is running
        m_bnForceTrigger.EnableWindow( TRUE);
    }
}

void uEye_DirectShow_Demo_Dlg::OnBnClickedButtonPausegraph()
{
    if( FilterGraph_Pause())
    {
        // disable 'force trigger' button for the time the graph is not running
        m_bnForceTrigger.EnableWindow( FALSE);
    }
}

void uEye_DirectShow_Demo_Dlg::OnBnClickedButtonStopgraph()
{
    if( FilterGraph_Stop())
    {
        // disable 'force trigger' button for the time the graph is not running
        m_bnForceTrigger.EnableWindow( FALSE);
    }
}

void uEye_DirectShow_Demo_Dlg::OnBnClickedButtonPinProperties()
{
    // start the timer to reflect changes made via the property page at the slider control.
    // ExposureControl_Init() does care for the timer after closing the property page.
    ExposureControl_StartAutoTimer();

    ShowPinProperties();

    // reinit the exposure control setter to adopt the changes possibly made via the property page 'timing'.
    ExposureControl_Init();
}

void uEye_DirectShow_Demo_Dlg::OnBnClickedButtonFilterProperties()
{
    // start the timer to reflect changes made via the property page at the slider control.
    // ExposureControl_Init() does care for the timer after closing the property page.
    ExposureControl_StartAutoTimer();

    ShowFilterProperties();

    // reinit the exposure control setter to adopt the changes possibly made via the property page 'timing'.
    ExposureControl_Init();
}

void uEye_DirectShow_Demo_Dlg::OnBnClickedButtonDumpGraph()
{
    EnumerateFilters();
}

void uEye_DirectShow_Demo_Dlg::OnBnClickedButtonExit()
{
    FilterGraph_Stop();
    EndDialog( 0);
}

void uEye_DirectShow_Demo_Dlg::OnBnClickedCheckAutoExposureControlActive()
{
    if( !ExposureControl_SetAutoControlState( m_chkAutoExposureControlActive.GetCheck() == BST_CHECKED))
    {
        MessageBoxEx( NULL, L"(De)Activating auto exposure control failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
    }

    // reinit the exposure control setter to apply the change.
    ExposureControl_Init();
}

void uEye_DirectShow_Demo_Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_bKeyDown == false)
	{
		ExposureControl_UpdateFilter();
	}
	m_bKeyDown = false;
}

BOOL uEye_DirectShow_Demo_Dlg::PreTranslateMessage(MSG *pMsg)
{
    // If a key is pressed
    if (pMsg->message == WM_KEYDOWN)
    {
        // If it is up, down
        if ((pMsg->wParam == VK_UP) || (pMsg->wParam == VK_DOWN))        
        {
			m_bKeyDown = true;
		}
		else
		{
			ExposureControl_UpdateFilter();
		}
	}
    else if(pMsg->message == WM_DST_DEVICELIST_CHANGED)
    {
        int cbsel= m_comboVideoSources.GetCurSel();
        if( cbsel == CB_ERR)
        {
            // select first list entry
            m_comboVideoSources.SetCurSel( 0);
        }

        VideoSourcesList_Refill();
    }
	else
	{
		//ExposureControl_UpdateFilter();
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void uEye_DirectShow_Demo_Dlg::OnNMReleasedcaptureSliderExposuretime(NMHDR *pNMHDR, LRESULT *pResult)
{
    ExposureControl_UpdateFilter();

    *pResult = 0;
}

void
uEye_DirectShow_Demo_Dlg::OnBnClickedButtonForceTrigger()
{
    HwTrigger_ForceTrigger();
}

void uEye_DirectShow_Demo_Dlg::OnTimer(UINT_PTR nIDEvent)
{
    if( nIDEvent == 1)
    {
        ExposureControl_OnAutoTimer();
    }

    CDialog::OnTimer(nIDEvent);
}


bool
uEye_DirectShow_Demo_Dlg::DirectShow_Init()
{
    // initialize the COM library on the current thread
    HRESULT err= CoInitialize(NULL);

    if( FAILED(err))
    {
        MessageBoxEx( NULL, L"Initializing COM library failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
    }

    return err == S_OK;
}

bool
uEye_DirectShow_Demo_Dlg::DirectShow_Deinit()
{
    // close the COM library on the current thread
    CoUninitialize();

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::VideoSourcesList_Fill()
{
    HRESULT status= S_OK;

    // create System Device Enumerator
    ICreateDevEnum *pSystemDeviceEnumerator= NULL;
    status= CoCreateInstance(  CLSID_SystemDeviceEnum,
                                NULL,
                                CLSCTX_INPROC,
                                IID_ICreateDevEnum,
                                (void**)&pSystemDeviceEnumerator);
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Creating System Device Enumerator failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // create Class Enumerator that lists alls video input devices among the system devices
    IEnumMoniker *pVideoInputDeviceEnumerator= NULL;
    status= pSystemDeviceEnumerator->CreateClassEnumerator( CLSID_VideoInputDeviceCategory,
                                                            &pVideoInputDeviceEnumerator,
                                                            0);

    // release the System Device Enumerator which is not needed anymore
    pSystemDeviceEnumerator->Release();
    pSystemDeviceEnumerator= NULL;

    if( status == S_OK)
    {
        // add entry '[no device selected]' to list
        m_comboVideoSources.AddString( L"[no device selected]");
        m_comboVideoSources.SetItemDataPtr( 0, NULL);

        // for each enumerated video input device: add it to the list
        IMoniker *pMoniker= NULL;
        while( pVideoInputDeviceEnumerator->Next( 1, &pMoniker, NULL) == S_OK )
        {
            VARIANT var;
            VariantInit(&var);

            // make filters properties accessible
            IPropertyBag *pPropBag= NULL;
            status= pMoniker->BindToStorage( 0, 0, IID_IPropertyBag, (void**)&pPropBag);
            if( FAILED(status))
            {
                pPropBag= NULL;
                MessageBoxEx( NULL, L"Accessing filter properties failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
                // continue with the next filter
            }
            else
            {
                // add a reference to the storage object
                pPropBag->AddRef();

                // get the name of this filter
                status= pPropBag->Read( L"FriendlyName", &var, 0);
                if( FAILED(status))
                {
                    MessageBoxEx( NULL, L"Reading filter name failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
                    // continue with the next filter
                }
                else
                {
                    // if uEye Capture Device:
                    // add filtername to the list and link the moniker pointer to the list entry
                    CString sTemp(var.bstrVal);
#if (0) /* jma [04/08/2010] add devices named UI... too */
                    if( sTemp.Find( "uEye Capture Device", 0) != -1)
#endif
                        int index = m_comboVideoSources.AddString( sTemp);
                    // dont forget to release the moniker later!
                    m_comboVideoSources.SetItemDataPtr( index, pMoniker);
                }

                // release the reference to the storage object
                pPropBag->Release();
                pPropBag= NULL;
            }

            VariantClear(&var);
        }

        // release the class enumerator
        pVideoInputDeviceEnumerator->Release();
        pVideoInputDeviceEnumerator= NULL;
    }
    else
    {
        if(m_currentDeviceName == "")
        {
            m_comboVideoSources.AddString( L"[no devices found]");
            m_comboVideoSources.SetItemDataPtr( 0, NULL);
        }
        else
        {
            m_comboVideoSources.AddString( L"[no device selected]");
            m_comboVideoSources.SetItemDataPtr( 0, NULL);
        }
    }

    // select first list entry
    m_comboVideoSources.SetCurSel( 0);

    // search for camera which is not connected but has a filtergraph
    bool found = false;
    for(int i = 0; i < m_comboVideoSources.GetCount(); i++)
    {
        CString str;
        m_comboVideoSources.GetLBText(i, str);
        if(str == m_currentDeviceName)
        {
            found = true;
            break;
        }
    }
    if(!found && m_currentDeviceName != "")
    {
        m_comboVideoSources.AddString(m_currentDeviceName);
    }

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::VideoSourcesList_Refill()
{
    if(!VideoSourcesList_Clear())
    {
        return false;
    }
    if(!VideoSourcesList_Fill())
    {
        return false;
    }

    for(int i = 0; i < m_comboVideoSources.GetCount(); i++)
    {
        CString str;
        m_comboVideoSources.GetLBText(i, str);
        if(m_currentDeviceName == str)
        {
            m_comboVideoSources.SetCurSel(i);
            break;
        }
    }

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::VideoSourcesList_Clear()
{
    // walk the list, release monikers, if any, and finally clear the list.

    IMoniker* pMoniker= NULL;
    int i= 0;
    int count= m_comboVideoSources.GetCount();
    for( i= 0; i < count; i++)
    {
        pMoniker= (IMoniker*)m_comboVideoSources.GetItemDataPtr( i);
        if( pMoniker != NULL)
        {
            pMoniker->Release();
            pMoniker= NULL;
        }
    }

    m_comboVideoSources.ResetContent();

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::FilterGraph_Create( IMoniker* pMoniker)
{
    HRESULT status= S_OK;

    // some sanity checks
    if( pMoniker == NULL)
    {
        MessageBoxEx( NULL, L"Bad input: pMoniker==NULL!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }
    if( m_pActiveVideoSource != NULL)
    {
        MessageBoxEx( NULL, L"Bad state: m_pActiveVideoSource==NULL!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // create the capture filter for the selected device
    status= pMoniker->BindToObject( 0, 0, IID_IBaseFilter, (void**)&m_pActiveVideoSource);
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Creating capture filter failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // create a Capture Graph Builder object 
    ICaptureGraphBuilder2* pCaptureGraphBuilder= NULL;
    status= CoCreateInstance(   CLSID_CaptureGraphBuilder2,
                                NULL,
                                CLSCTX_INPROC,
                                IID_ICaptureGraphBuilder2,
                                (void**)&pCaptureGraphBuilder);
    if( FAILED(status))
    {
        // cleanup

        // release the capture filter object
        m_pActiveVideoSource->Release();
        m_pActiveVideoSource= NULL;

        MessageBoxEx( NULL, L"Creating capture graph builder failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // create the Filter Graph Manager
    status= CoCreateInstance(   CLSID_FilterGraph,
                                NULL,
                                CLSCTX_INPROC,
                                IID_IGraphBuilder,
                                (void **)&m_pActiveFilterGraphManager);
    if( FAILED(status))
    {
        // cleanup

        // release the capture filter object
        m_pActiveVideoSource->Release();
        m_pActiveVideoSource= NULL;

        // release the capture graph builder object
        pCaptureGraphBuilder->Release();
        pCaptureGraphBuilder= NULL;

        MessageBoxEx( NULL, L"Creating filter graph manager failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // initialize the Capture Graph Builder object
    // i.e. connect the graph with the builder
    status= pCaptureGraphBuilder->SetFiltergraph( m_pActiveFilterGraphManager);
    if( FAILED(status))
    {
        // cleanup

        // release the capture filter object
        m_pActiveVideoSource->Release();
        m_pActiveVideoSource= NULL;

        // release the capture graph builder object
        pCaptureGraphBuilder->Release();
        pCaptureGraphBuilder= NULL;

        // release the filter graph manager object
        m_pActiveFilterGraphManager->Release();
        m_pActiveFilterGraphManager= NULL;

        MessageBoxEx( NULL, L"Initializing capture graph builder failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // add the capture filter to the graph
    status= m_pActiveFilterGraphManager->AddFilter( m_pActiveVideoSource, L"Video Capture");
    if( FAILED(status))
    {
        // cleanup

        // release the capture filter object
        m_pActiveVideoSource->Release();
        m_pActiveVideoSource= NULL;

        // release the capture graph builder object
        pCaptureGraphBuilder->Release();
        pCaptureGraphBuilder= NULL;

        // release the filter graph manager object
        m_pActiveFilterGraphManager->Release();
        m_pActiveFilterGraphManager= NULL;

        MessageBoxEx( NULL, L"Adding capture filter to graph failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // render the video capture pin, connect the capture filter with the (default) renderer

    status= pCaptureGraphBuilder->RenderStream( &PIN_CATEGORY_CAPTURE, 
                                                &MEDIATYPE_Video,
                                                m_pActiveVideoSource, 
                                                NULL,
                                                NULL);
    if( FAILED(status))
    {
        // cleanup

        // remove the filter from the graph
        // (dont care for the return value here)
        m_pActiveFilterGraphManager->RemoveFilter( m_pActiveVideoSource);

        // release the capture filter object
        m_pActiveVideoSource->Release();
        m_pActiveVideoSource= NULL;

        // release the capture graph builder object
        pCaptureGraphBuilder->Release();
        pCaptureGraphBuilder= NULL;

        // release the filter graph manager object
        m_pActiveFilterGraphManager->Release();
        m_pActiveFilterGraphManager= NULL;

        MessageBoxEx( NULL, L"Adding capture filter to graph failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // query the VideoWindow interface of the filter graph manager
    IVideoWindow* pVideoWindow= NULL;
    status= m_pActiveFilterGraphManager->QueryInterface( IID_IVideoWindow, (void**)&pVideoWindow);
    if( FAILED(status) || pVideoWindow==NULL)
    {
        // cleanup

        // remove the filter from the graph
        // (dont care for the return value here)
        m_pActiveFilterGraphManager->RemoveFilter( m_pActiveVideoSource);

        // release the capture filter object
        m_pActiveVideoSource->Release();
        m_pActiveVideoSource= NULL;

        // release the capture graph builder object
        pCaptureGraphBuilder->Release();
        pCaptureGraphBuilder= NULL;

        // release the filter graph manager object
        m_pActiveFilterGraphManager->Release();
        m_pActiveFilterGraphManager= NULL;

        MessageBoxEx( NULL, L"Querying VideoWindow interface of the filter graph manager failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // connect the dialogs video view to the graph
    CWnd* pwnd= GetDlgItem( IDC_STATIC_VIDEOVIEW);
    pVideoWindow->put_Owner( (OAHWND)pwnd->m_hWnd);  // put_Owner always returns NOERROR

    // we dont want a title bar on our video view
    long pWindowStyle = 0;
    pVideoWindow->get_WindowStyle(&pWindowStyle);
    pWindowStyle &= ~WS_CAPTION;
    pVideoWindow->put_WindowStyle(pWindowStyle);

    // adjust graphs video geometry
    CRect rc( 0, 0, 0, 0);
    pwnd->GetClientRect( &rc);
    pVideoWindow->SetWindowPosition( rc.left, rc.top, rc.Width(), rc.Height());

    // release the VideoWindow interface object, we do not need it anymore
    pVideoWindow->Release();
    pVideoWindow= NULL;

    // release the capture graph builder object, we do not need it anymore
    pCaptureGraphBuilder->Release();
    pCaptureGraphBuilder= NULL;

    // enable 'dump graph' button for the time a graph is present
    m_bnDumpGraph.EnableWindow( TRUE);

    // enable start control
    m_bnGraphStart.EnableWindow(TRUE);
    m_bnGraphPause.EnableWindow(FALSE);
    m_bnGraphStop.EnableWindow(FALSE);

    // update the graph state view
    UpdateGraphState( State_Stopped);

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::FilterGraph_Destroy()
{
    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return true;
    }

    if( !FilterGraph_Stop())
    {
        return false;
    }

    // remove the pins in the connection list because they will be invalidated after the graph is destroyed
    if(!m_connections.empty())
    {
        while(!m_connections.empty())
        {
            m_connections.pop();
        }
    }

    // disable 'dump graph' button as long as no graph present
    m_bnDumpGraph.EnableWindow( FALSE);

    ULONG refcount = 0;

    // delete the capture filter
    refcount = m_pActiveVideoSource->Release();
    m_pActiveVideoSource= NULL;

    // delete the graph
    refcount = m_pActiveFilterGraphManager->Release();
    m_pActiveFilterGraphManager= NULL;

    // update the graph state view
    UpdateGraphState( -1);

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::FilterGraph_Start()
{
    // return true if graph is already running
    if(m_iGraphState == State_Running)
    {
        return true;
    }

    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return true;
    }

    if(!FilterGraphPins_Connect())
    {
        MessageBoxEx( NULL, L"Connecting pins failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
    }

    HRESULT status= S_OK;

    // get the MediaControl interface of the graph
    IMediaControl* pMediaControl= NULL;
    status= m_pActiveFilterGraphManager->QueryInterface( IID_IMediaControl, (void**)&pMediaControl);
    if( FAILED(status) || pMediaControl == NULL)
    {
        MessageBoxEx( NULL, L"Querying MediaControl interface of the filter graph manager failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    //// check for graph to be stopped before allowing to start

    //OAFilterState filterState= 0;   // OAFilterState is actually a long
    //status= pMediaControl->GetState( 100, &filterState);
    //if( FAILED(status))
    //{
    //    // cleanup

    //    // release the MediaControl interface object
    //    pMediaControl->Release();
    //    pMediaControl= NULL;

    //    MessageBoxEx( NULL, L"Querying graph state failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
    //    return false;
    //}

    //if( filterState == State_Stopped)
    {
        // start the execution of the graph
        status= pMediaControl->Run();
        if( FAILED(status))
        {
            // cleanup

            // release the MediaControl interface object
            pMediaControl->Release();
            pMediaControl= NULL;

            MessageBoxEx( NULL, L"Starting the graph failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
            return false;
        }

        // update the graph state view
        UpdateGraphState( State_Running);
    }

    // release the MediaControl interface object
    pMediaControl->Release();
    pMediaControl= NULL;

    m_bnGraphStart.EnableWindow(FALSE);
    m_bnGraphPause.EnableWindow(TRUE);
    m_bnGraphStop.EnableWindow(TRUE);
    return true;
}

bool
uEye_DirectShow_Demo_Dlg::FilterGraph_Pause()
{
    // return true if graph is already paused
    if(m_iGraphState == State_Paused)
    {
        return true;
    }

    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return true;
    }

    HRESULT status= S_OK;

    // get the MediaControl interface of the graph
    IMediaControl* pMediaControl= NULL;
    status= m_pActiveFilterGraphManager->QueryInterface( IID_IMediaControl, (void**)&pMediaControl);
    if( FAILED(status) || pMediaControl == NULL)
    {
        MessageBoxEx( NULL, L"Querying MediaControl interface of the filter graph manager failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    //// check for graph to be executing before allowing to pause

    //OAFilterState filterState= 0;   // OAFilterState is actually a long
    //status= pMediaControl->GetState( 100, &filterState);
    //if( FAILED(status))
    //{
    //    // cleanup

    //    // release the MediaControl interface object
    //    pMediaControl->Release();
    //    pMediaControl= NULL;

    //    MessageBoxEx( NULL, L"Querying graph state failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
    //    return false;
    //}

    //if( filterState == State_Running)
    {
        // pause the execution of the graph
        status= pMediaControl->Pause();
        if( FAILED(status))
        {
            // cleanup

            // release the MediaControl interface object
            pMediaControl->Release();
            pMediaControl= NULL;

            MessageBoxEx( NULL, L"Pausing the graph failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
            return false;
        }

        // update the graph state view
        UpdateGraphState( State_Paused);
   }

    // release the MediaControl interface object
    pMediaControl->Release();
    pMediaControl= NULL;

    m_bnGraphStart.EnableWindow(TRUE);
    m_bnGraphPause.EnableWindow(FALSE);
    m_bnGraphStop.EnableWindow(TRUE);
    return true;
}

bool
uEye_DirectShow_Demo_Dlg::FilterGraph_Stop()
{
    Invalidate();

    // return if graph is already stopped
    if(m_iGraphState == State_Stopped)
    {
        return true;
    }

    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return true;
    }

    HRESULT status= S_OK;

    // get the MediaControl interface of the graph
    IMediaControl* pMediaControl= NULL;
    status= m_pActiveFilterGraphManager->QueryInterface( IID_IMediaControl, (void**)&pMediaControl);
    if( FAILED(status) || pMediaControl == NULL)
    {
        MessageBoxEx( NULL, L"Querying MediaControl interface of the filter graph manager failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    //// check for graph to be executing before allowing to stop

    //OAFilterState filterState= 0;   // OAFilterState is actually a long
    //status= pMediaControl->GetState( 100, &filterState);
    //if( FAILED(status))
    //{
    //    // cleanup

    //    // release the MediaControl interface object
    //    pMediaControl->Release();
    //    pMediaControl= NULL;

    //    MessageBoxEx( NULL, L"Querying graph state failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
    //    return false;
    //}

    //if( filterState != State_Stopped)
    {
        // stop the execution of the graph
        status= pMediaControl->Stop();
        if( FAILED(status))
        {
            // cleanup

            // release the MediaControl interface object
            pMediaControl->Release();
            pMediaControl= NULL;

            MessageBoxEx( NULL, L"Stopping the graph failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
            return false;
        }

        // update the graph state view
        UpdateGraphState( State_Stopped);
    }

    // release the MediaControl interface object
    pMediaControl->Release();
    pMediaControl= NULL;

    if(!FilterGraphPins_Disconnect())
    {
        MessageBoxEx( NULL, L"Disconnecting pins failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
    }

    m_bnGraphStart.EnableWindow(TRUE);
    m_bnGraphPause.EnableWindow(FALSE);
    m_bnGraphStop.EnableWindow(FALSE);
    return true;
}

bool
uEye_DirectShow_Demo_Dlg::FilterGraphPins_Connect()
{
    HRESULT hr = S_OK;
    while(!m_connections.empty())
    {
        std::pair<CComPtr<IPin>, CComPtr<IPin> > connection(m_connections.top());
        hr = m_pActiveFilterGraphManager->Connect(connection.first, connection.second);
        if(FAILED(hr))
        {
            while(!m_connections.empty())
            {
                m_connections.pop();
            }
            MessageBoxEx( NULL, L"Connecting pins failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
            return false;
        }
        m_connections.pop();
    }
    return true;
}

bool
uEye_DirectShow_Demo_Dlg::FilterGraphPins_Disconnect()
{
    if(!m_connections.empty())
    {
        while(!m_connections.empty())
        {
            m_connections.pop();
        }
    }

    HRESULT hr = S_OK;
    // proceed only if filter graph builder object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return false;
    }

    CComPtr<IEnumFilters> pEnum = NULL;
    CComPtr<IBaseFilter> pFilter = NULL;

    hr = m_pActiveFilterGraphManager->EnumFilters(&pEnum);
    if (FAILED(hr))
    {
        MessageBoxEx( NULL, L"Enumerating filters failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    while(pEnum->Next(1, &pFilter, NULL) == S_OK)
    {
        CComPtr<IEnumPins> pEnumPins = NULL;
        hr = pFilter->EnumPins(&pEnumPins);
        if(FAILED(hr))
        {
            while(!m_connections.empty())
            {
                m_connections.pop();
            }
            MessageBoxEx( NULL, L"Enumerating pins failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
            return false;
        }

        CComPtr<IPin> pCurrentPin = NULL;
        while(pEnumPins->Next(1, &pCurrentPin, NULL) == S_OK)
        {
            PIN_DIRECTION pinDir;
            hr = pCurrentPin->QueryDirection(&pinDir);
            if(FAILED(hr))
            {
                while(!m_connections.empty())
                {
                    m_connections.pop();
                }
                MessageBoxEx( NULL, L"Querying pin direction failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
                return false;
            }

            if(pinDir == PINDIR_INPUT)
            {
                CComPtr<IPin> pConnection = NULL;
                hr = pCurrentPin->ConnectedTo(&pConnection);
                if(FAILED(hr))
                {
                    while(!m_connections.empty())
                    {
                        m_connections.pop();
                    }
                    MessageBoxEx( NULL, L"Querying connected pin failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
                    return false;
                }

                if(pConnection != NULL)
                {
                    // write a connection to list
                    m_connections.push(std::make_pair(pConnection, pCurrentPin));
                    hr = pCurrentPin->Disconnect();
                    if(FAILED(hr))
                    {
                        while(!m_connections.empty())
                        {
                            m_connections.pop();
                        }
                        MessageBoxEx( NULL, L"Disconnecting pins failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
                        return false;
                    }

                    hr = pConnection->Disconnect();
                    if(FAILED(hr))
                    {
                        while(!m_connections.empty())
                        {
                            m_connections.pop();
                        }
                        MessageBoxEx( NULL, L"Disconnecting pins failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
                        return false;
                    }
                    pCurrentPin = NULL;
                    continue;
                }
            }
            pCurrentPin = NULL;
        }
        pFilter = NULL;
    }
    return true;
}


bool
uEye_DirectShow_Demo_Dlg::ExposureControl_Init()
{
    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return true;
    }

    // query the auto exposure control state
    bool bAutoExposureControlActive= false;
    if( !ExposureControl_AcquireAutoControlState( bAutoExposureControlActive))
    {
        MessageBoxEx( NULL, L"Querying current auto exposure control state failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // enable/disable the slider according to auto exposure control state.
    m_sliderExposureTime.EnableWindow( bAutoExposureControlActive ? FALSE : TRUE);

    // enable 'auto' check box
    m_chkAutoExposureControlActive.EnableWindow( TRUE);

    // update the widgets
    if( !ExposureControl_UpdateWidgets())
    {
        MessageBoxEx( NULL, L"Updating exposure control slider failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // start/stop the timer according to auto exposure control state.
    // the timer keeps the slider position up to date when auto exposure control is enabled.
    if( bAutoExposureControlActive)
    {
        if( !ExposureControl_StartAutoTimer())
        {
            MessageBoxEx( NULL, L"Starting auto exposure control timer failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
            return false;
        }
    }
    else
    {
        if( !ExposureControl_StopAutoTimer())
        {
            MessageBoxEx( NULL, L"Stopping auto exposure control timer failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
            return false;
        }
    }

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ExposureControl_Deinit()
{
    // disable the slider
    m_sliderExposureTime.EnableWindow( FALSE);

    // disable the 'auto' check box
    m_chkAutoExposureControlActive.EnableWindow( FALSE);

    // clear exposure control displays
    m_labelMinExposureTime.SetWindowText(L"");
    m_labelMaxExposureTime.SetWindowText(L"");
    m_labelCurrentExposureTime.SetWindowText(L"");
    m_labelCurrentFramerate.SetWindowText(L"");

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ExposureControl_UpdateFilter()
{
    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return true;
    }

    HRESULT status= S_OK;

    // get the uEyeCapturePin interface of the source filter
    IuEyeCapturePin* pCapturePin= NULL;
    status= m_pActiveVideoSource->QueryInterface( IID_IuEyeCapturePin, (void**)&pCapturePin);
    if( FAILED(status) || pCapturePin == NULL)
    {
        MessageBoxEx( NULL, L"Querying uEyeCapturePin interface of the filter graph manager failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // read the new exposure time from the slider
    long lExp= m_sliderExposureTime.GetPos();

    // set the new exposure time to the filter
    status= pCapturePin->SetExposureTime( lExp);
    if( FAILED(status))
    {
        // cleanup

        // release the uEyeCapturePin interface object
        pCapturePin->Release();
        pCapturePin= NULL;

        MessageBoxEx( NULL, L"Setting exposure time to uEyeCapturePin failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // update exposure time display
    CString s(L"");
    s.Format( L"%d", lExp);
    m_labelCurrentExposureTime.SetWindowText(s.GetBuffer());

    // release the uEyeCapturePin interface object
    pCapturePin->Release();
    pCapturePin= NULL;

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ExposureControl_UpdateWidgets()
{
    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return true;
    }

    HRESULT status= S_OK;


    // query the auto exposure control state
    bool bAutoExposureControlActive= false;
    if( !ExposureControl_AcquireAutoControlState( bAutoExposureControlActive))
    {
        MessageBoxEx( NULL, L"Querying current auto exposure control state failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // check/uncheck the 'auto' checkbox according to auto exposure control state.
    m_chkAutoExposureControlActive.SetCheck( bAutoExposureControlActive ? BST_CHECKED : BST_UNCHECKED);


    // get the uEyeCapturePin interface of the graph
    IuEyeCapturePin* pCapturePin= NULL;
    status= m_pActiveVideoSource->QueryInterface( IID_IuEyeCapturePin, (void**)&pCapturePin);
    if( FAILED(status) || pCapturePin == NULL)
    {
        MessageBoxEx( NULL, L"Querying uEyeCapturePin interface of the filter graph manager failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    long lMinExp= 0;
    long lMaxExp= 0;
    long lInterval= 0;
    status= pCapturePin->GetExposureRange( &lMinExp, &lMaxExp, &lInterval);
    if( FAILED(status))
    {
        // cleanup

        // release the uEyeCapturePin interface object
        pCapturePin->Release();
        pCapturePin= NULL;

        MessageBoxEx( NULL, L"Querying exposure range from uEyeCapturePin failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    long lExp= 0;
    status= pCapturePin->GetExposureTime( &lExp);
    if( FAILED(status))
    {
        // cleanup

        // release the uEyeCapturePin interface object
        pCapturePin->Release();
        pCapturePin= NULL;

        MessageBoxEx( NULL, L"Querying current exposure time from uEyeCapturePin failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // update exposure control displays
    CString s(L"");
    s.Format( L"%d", lMinExp);
    m_labelMinExposureTime.SetWindowText(s.GetBuffer());
    s.Format( L"%d", lMaxExp);
    m_labelMaxExposureTime.SetWindowText(s.GetBuffer());
    s.Format( L"%d", lExp);
    m_labelCurrentExposureTime.SetWindowText(s.GetBuffer());

    // update slider
    m_sliderExposureTime.SetRange( lMinExp, lMaxExp, TRUE);
    m_sliderExposureTime.SetLineSize( lInterval);
    m_sliderExposureTime.SetPos( lExp);

    // release the uEyeCapturePin interface object
    pCapturePin->Release();
    pCapturePin= NULL;

    // query camera's framerate
    IuEyeAutoFramerate* puEyeAutoFramerate = NULL;
    status = m_pActiveVideoSource->QueryInterface(IID_IuEyeAutoFramerate, reinterpret_cast<void**>(&puEyeAutoFramerate)); 
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Querying filter's IID_IueyeAutoFramerate interface failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }
    double dblFps = 0.01;
    status = puEyeAutoFramerate->AutoFramerate_GetFramerate(&dblFps);
    if( FAILED(status))
    {
        // cleanup

        // release the uEyeCapturePin interface object
        puEyeAutoFramerate->Release();
        puEyeAutoFramerate= NULL;

        MessageBoxEx( NULL, L"Querying current frame rate from uEyeAutoFramerate failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }
    s.Format(L"%.2f", dblFps);
    m_labelCurrentFramerate.SetWindowText(s.GetBuffer());

    puEyeAutoFramerate->Release();
    puEyeAutoFramerate = NULL;

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ExposureControl_SetAutoControlState( bool bAutoControlActive )
{
    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return false;
    }

    HRESULT status= S_OK;

    // get the IID_IAMCameraControl interface of the source filter
    IAMCameraControl* pCamCtrl= NULL;
    status= m_pActiveVideoSource->QueryInterface( IID_IAMCameraControl, (void**)&pCamCtrl);
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Querying filter's IID_IAMCameraControl interface failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // write the auto control state to the filter
    long lValue= 0;
    long lFlags= 0;
    status = pCamCtrl->Set( CameraControl_Exposure, 0, bAutoControlActive ? CameraControl_Flags_Auto : 0);
    if( FAILED(status))
    {
        // cleanup

        // release the CameraControl interface object
        pCamCtrl->Release();
        pCamCtrl= NULL;

        MessageBoxEx( NULL, L"Querying filter's auto exposure control state failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // release the CameraControl interface object
    pCamCtrl->Release();
    pCamCtrl= NULL;

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ExposureControl_AcquireAutoControlState( bool& rbAutoControlActive )
{
    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return false;
    }

    HRESULT status= S_OK;

    // get the IID_IAMCameraControl interface of the source filter
    IAMCameraControl* pCamCtrl= NULL;
    status= m_pActiveVideoSource->QueryInterface( IID_IAMCameraControl, (void**)&pCamCtrl);
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Querying filter's IID_IAMCameraControl interface failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // read the auto control state from the filter
    long lValue= 0;
    long lFlags= 0;
    status = pCamCtrl->Get( CameraControl_Exposure, &lValue, &lFlags);
    if( FAILED(status))
    {
        // cleanup

        // release the CameraControl interface object
        pCamCtrl->Release();
        pCamCtrl= NULL;

        MessageBoxEx( NULL, L"Querying filter's auto exposure control state failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    rbAutoControlActive= (lFlags == CameraControl_Flags_Auto);

    // release the CameraControl interface object
    pCamCtrl->Release();
    pCamCtrl= NULL;

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ExposureControl_StartAutoTimer()
{
    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return false;
    }

    SetTimer( 1, 250, NULL);

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ExposureControl_StopAutoTimer()
{
    // proceed only if filter graph manager object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return false;
    }

    KillTimer( 1);

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ExposureControl_OnAutoTimer()
{
    return ExposureControl_UpdateWidgets();
}

bool
uEye_DirectShow_Demo_Dlg::HwTrigger_ForceTrigger()
{
    // proceed only if video source (filter) object present
    if( m_pActiveVideoSource == NULL)
    {
        return false;
    }

    // make sure graph is running at this point
    if( m_iGraphState != State_Running)
    {
        return false;
    }

    HRESULT status= S_OK;

    // get the uEyeCapturePin interface of the source filter
    IAMVideoControl* pVideoControl= NULL;
    status= m_pActiveVideoSource->QueryInterface( IID_IAMVideoControl, (void**)&pVideoControl);
    if( FAILED(status) || pVideoControl == NULL)
    {
        MessageBoxEx( NULL, L"Querying video control interface of the filter graph manager failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    bool bExternalTriggermodeWasEnabled= false;
    bool bReturn= true;
    long lMode= 0;

    // get the current video control mode
    status= pVideoControl->GetMode( NULL, &lMode );
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Querying video control mode failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        bReturn= false;
        goto exit_HwTrigger_ForceTrigger;
    }

    // remember if hw trigger mode is set at this point. we then have to unset it before leaving this procedure.
    bExternalTriggermodeWasEnabled= ((lMode & VideoControlFlag_ExternalTriggerEnable) == VideoControlFlag_ExternalTriggerEnable);

    // enable hw trigger mode, if not set already
    if( !bExternalTriggermodeWasEnabled)
    {
        // note: keep the change to lMode. otherwise we would unset the trigger mode with the next call to SetMode().
        // note: in a multithreaded application environment it is advisable to fetch the current mode before each call to SetMode().
        lMode|= VideoControlFlag_ExternalTriggerEnable;
        status= pVideoControl->SetMode(NULL, (lMode));
        if( FAILED(status))
        {
            MessageBoxEx( NULL, L"Setting video control mode failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
            bReturn= false;
            goto exit_HwTrigger_ForceTrigger;
        }

        // sleep for a second to make the triggered image noticeable in the live stream
        Sleep( 1000);
    }

    // force trigger
    // note: do not keep the trigger flag in lMode value. this would result in triggering once again with a possibly following SetMode() call.
    status= pVideoControl->SetMode(NULL, lMode | VideoControlFlag_Trigger);
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Forcing trigger failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        bReturn= false;
        goto exit_HwTrigger_ForceTrigger;
    }

exit_HwTrigger_ForceTrigger:
    // cleanup
    if( pVideoControl != NULL)
    {
        // unset hw trigger mode if it was not set at the beginning of this procedure
        if( !bExternalTriggermodeWasEnabled)
        {
            // sleep for a second to make the triggered image noticeable in the live stream
            Sleep( 1000);

            lMode&= ~VideoControlFlag_ExternalTriggerEnable;
            // note: in a multithreaded application environment it is advisable to fetch the current mode before each call to SetMode().
            status= pVideoControl->SetMode(NULL, (lMode));
            if( FAILED(status))
            {
                MessageBoxEx( NULL, L"Setting video control mode failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
                bReturn= false;
            }
        }

        // release the video control object
        pVideoControl->Release();
        pVideoControl= NULL;
    }

    return bReturn;
}

// caller to release the pin
IPin*
uEye_DirectShow_Demo_Dlg::GetPin(   IBaseFilter*    pFilter, 
                                    PIN_DIRECTION   pinDirection,               // Direction PINDIR_INPUT or PINDIR_OUTPUT
                                    const GUID*     pMajorMediaType/*= NULL*/,  // NULL or optional major media type of the stream: MEDIATYPE_Audio, MEDIATYPE_Video, MEDIATYPE_Midi, ...
                                    const GUID*     pPinCategory/*= NULL*/,     // NULL or optional pin category
                                    LPCSTR          szFiltername/*= NULL*/)     // NULL or optional name of the output pin (visible in GraphEdit)
{
    HRESULT status= S_OK;

    bool bFound= false;
    int iFoundCount= 0;

    IEnumPins*  pEnum= NULL;
    IPin*       pPin= NULL;

    CString sFiltername(szFiltername);
    sFiltername.MakeLower();

    //count all pins from the filter
    status= pFilter->EnumPins( &pEnum);
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Enumerating pins failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return NULL;
    }

    while( pEnum->Next( 1, &pPin, 0) == S_OK)
    {
        //find out the direction
        PIN_DIRECTION pinDirection_Temp;
        pPin->QueryDirection( &pinDirection_Temp);
        if( pinDirection != pinDirection_Temp)
        {
            continue;
        }

        //find out the name    
        if( szFiltername != NULL)
        {
            PIN_INFO pinInfo;
            pPin->QueryPinInfo( &pinInfo);

            CString sPinName(pinInfo.achName);
            sPinName.MakeLower();

            if( sPinName != sFiltername)
            {
                continue;
            }
        }

        //find out the media type
        if( pMajorMediaType != NULL)
        {
            //Check the Pin Category
            bool bPinCategoryMatch= true;

            if( pPinCategory != NULL)
            {
                bPinCategoryMatch= false;

                IKsPropertySet* pKsPropertySet= NULL;

                status= pPin->QueryInterface( IID_IKsPropertySet, (void**)&pKsPropertySet);
                if( FAILED(status) || pKsPropertySet != NULL)
                {
                    continue;
                }

                // Try to retrieve the pin category.
                DWORD cbReturned;
                GUID guidPinCategory= *pPinCategory;
                status= pKsPropertySet->Get(   AMPROPSETID_Pin,
                                    AMPROPERTY_PIN_CATEGORY,
                                    NULL,
                                    0,
                                    (void*)&guidPinCategory,
                                    sizeof(GUID),
                                    &cbReturned);
                // If this succeeded, pPinCategory now contains the category GUID.

                pKsPropertySet->Release();
                pKsPropertySet= NULL;

                if( FAILED(status))
                {
                    continue;
                }
            }

            if( bPinCategoryMatch)
            {
                AM_MEDIA_TYPE* pMediaType= NULL;
                IEnumMediaTypes* pEnumMediaTypes= NULL;

                status= pPin->EnumMediaTypes( &pEnumMediaTypes);

                if( FAILED(status))
                {
                    pEnum->Release();
                    pEnum= NULL;

                    pPin->Release();
                    pPin= NULL;

                    MessageBoxEx( NULL, L"Enumerating pins failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
                    return NULL;
                }

                bool bMediaTypeFound= false;

                while( pEnumMediaTypes->Next( 1, &pMediaType, 0) == S_OK)
                {
                    if( pMediaType->majortype == *pMajorMediaType)
                    {
                        bMediaTypeFound= true;
                    }

                    // free the media type
                    if( pMediaType->cbFormat != 0)
                    {
                        CoTaskMemFree( (PVOID)pMediaType->pbFormat);
                        pMediaType->cbFormat= 0;
                        pMediaType->pbFormat= NULL;
                    }
                    if( pMediaType->pUnk != NULL)
                    {
                        // Unecessary because pUnk should not be used, but safest.
                        pMediaType->pUnk->Release();
                        pMediaType->pUnk= NULL;
                    }
                    CoTaskMemFree( pMediaType);
                    pMediaType= NULL;
                }

                pEnumMediaTypes->Release();
                pEnumMediaTypes= NULL;

                if( !bMediaTypeFound)
                {
                    continue;
                }
            }
        }

        // if we get here, the filter was found
        break;

        pPin->Release();
        pPin= NULL;
    }

    pEnum->Release();
    pEnum= NULL;
    
    return pPin;  
}

bool
uEye_DirectShow_Demo_Dlg::ShowPinProperties()
{
    // proceed only if video source object present
    if( m_pActiveVideoSource == NULL)
    {
        return true;
    }

    HRESULT status= S_OK;

    IPin* pPin= GetPin( m_pActiveVideoSource, PINDIR_OUTPUT, &MEDIATYPE_Video, &PIN_CATEGORY_CAPTURE);
    if( pPin == NULL)
    {
        MessageBoxEx( NULL, L"Pin not available!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    IUnknown* pFilterUnk= NULL;
    status= pPin->QueryInterface( IID_IUnknown, (void**)&pFilterUnk);
    if( FAILED(status))
    {
        // cleanup

        pPin->Release();
        pPin= NULL;

        MessageBoxEx( NULL, L"Querying pin's IAMStreamConfig interface failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    ISpecifyPropertyPages* pSpecifyPropertyPages= NULL;
    status= pFilterUnk->QueryInterface( IID_ISpecifyPropertyPages, (void**)&pSpecifyPropertyPages);
    if( FAILED(status))
    {
        // cleanup

        pFilterUnk->Release();
        pFilterUnk= NULL;

        pPin->Release();
        pPin= NULL;

        MessageBoxEx( NULL, L"Querying pin's ISpecifyPropertyPages interface failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    CAUUID cauuid= { 0, NULL};
    status= pSpecifyPropertyPages->GetPages( &cauuid);
    if( FAILED(status))
    {
        // cleanup

        pSpecifyPropertyPages->Release();
        pSpecifyPropertyPages->Release();
        pSpecifyPropertyPages= NULL;

        pFilterUnk->Release();
        pFilterUnk= NULL;

        pPin->Release();
        pPin= NULL;

        MessageBoxEx( NULL, L"Querying pin's ISpecifyPropertyPages interface failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }
    pSpecifyPropertyPages->Release();
    pSpecifyPropertyPages->Release();
    pSpecifyPropertyPages= NULL;

    status= OleCreatePropertyFrame( GetSafeHwnd(),//*this,
                                    0,
                                    0,
                                    OLESTR("uEye Capture Device Pin"),
                                    1,
                                    (IUnknown**)&pFilterUnk,
                                    cauuid.cElems,
                                    (GUID*)cauuid.pElems,
                                    0,
                                    0,
                                    NULL);

    if( FAILED(status))
    {
        // cleanup

        CoTaskMemFree( cauuid.pElems);
        cauuid.pElems= NULL;
        cauuid.cElems= 0;

        pFilterUnk->Release();
        pFilterUnk= NULL;

        pPin->Release();
        pPin= NULL;

        MessageBoxEx( NULL, L"OleCreatePropertyFrame failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // cleanup

    CoTaskMemFree( cauuid.pElems);
    cauuid.pElems= NULL;
    cauuid.cElems= 0;

    pFilterUnk->Release();
    pFilterUnk= NULL;

    pPin->Release();
    pPin= NULL;

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::ShowFilterProperties()
{
    // proceed only if video source object present
    if( m_pActiveVideoSource == NULL)
    {
        return true;
    }

    HRESULT status= S_OK;

    ISpecifyPropertyPages* pSpecifyPropertyPages= NULL;
    status= m_pActiveVideoSource->QueryInterface( IID_ISpecifyPropertyPages, (void**)&pSpecifyPropertyPages);
    if( FAILED(status))
    {
        MessageBoxEx( NULL, L"Querying filter's ISpecifyPropertyPages interface failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    CAUUID cauuid= { 0, NULL};
    status= pSpecifyPropertyPages->GetPages( &cauuid);
    if( FAILED(status))
    {
        // cleanup

        pSpecifyPropertyPages->Release();
        pSpecifyPropertyPages= NULL;

        MessageBoxEx( NULL, L"Querying filter's ISpecifyPropertyPages interface failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }
    pSpecifyPropertyPages->Release();
    pSpecifyPropertyPages= NULL;

    status= OleCreatePropertyFrame( GetSafeHwnd(),//*this,
                                    0,
                                    0,
                                    OLESTR("uEye Capture Device Filter"),
                                    1,
                                    (IUnknown**)&m_pActiveVideoSource,
                                    cauuid.cElems,
                                    (GUID*)cauuid.pElems,
                                    0,
                                    0,
                                    NULL);

    if( FAILED(status))
    {
        // cleanup

        CoTaskMemFree( cauuid.pElems);
        cauuid.pElems= NULL;
        cauuid.cElems= 0;

        MessageBoxEx( NULL, L"OleCreatePropertyFrame failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    // cleanup

    CoTaskMemFree( cauuid.pElems);
    cauuid.pElems= NULL;
    cauuid.cElems= 0;

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::EnumerateFilters() 
{
    // proceed only if filter graph builder object present
    if( m_pActiveFilterGraphManager == NULL)
    {
        return false;
    }

    IEnumFilters *pEnum = NULL;
    IBaseFilter *pFilter;
    ULONG cFetched;

    HRESULT hr = m_pActiveFilterGraphManager->EnumFilters(&pEnum);
    if (FAILED(hr))
    {
        MessageBoxEx( NULL, L"Enumerating filters failed!", _T(__FUNCTION__), MB_ICONERROR, 0);
        return false;
    }

    CString sInfo( L"Filters in the Graph:\n\n");
    int i= 0;

    while(pEnum->Next(1, &pFilter, &cFetched) == S_OK)
    {
        i++;

        FILTER_INFO FilterInfo;
        hr = pFilter->QueryFilterInfo(&FilterInfo);
        if (FAILED(hr))
        {
            MessageBoxEx( NULL, L"Could not get the filter info", _T(__FUNCTION__), MB_ICONERROR, 0);
            continue;  // Maybe the next one will work.
        }

        LPWSTR pVendorInfo= NULL;
        hr= pFilter->QueryVendorInfo( &pVendorInfo);
        if( FAILED(hr))
        {
            pVendorInfo= NULL;
        }


        if( pVendorInfo != NULL)
        {
            sInfo.AppendFormat( L"%d: %s (by %s)\n", i, FilterInfo.achName, pVendorInfo);
            CoTaskMemFree( pVendorInfo);
            pVendorInfo= NULL;
        }
        else
        {
            sInfo.AppendFormat( L"%d: %s\n", i, FilterInfo.achName);
        }

        // The FILTER_INFO structure holds a pointer to the Filter Graph
        // Manager, with a reference count that must be released.
        if (FilterInfo.pGraph != NULL)
        {
            FilterInfo.pGraph->Release();
        }
        pFilter->Release();
    }

    pEnum->Release();

    MessageBoxEx( NULL, sInfo.GetBuffer(), _T(__FUNCTION__), MB_OK, 0);

    return true;
}

bool
uEye_DirectShow_Demo_Dlg::UpdateGraphState( int iGraphState)
{
    if( m_pActiveFilterGraphManager == NULL)
    {
        m_labelGraphState.SetWindowText( L"No graph present.");
    }
    else
    {
        switch( iGraphState )
        {
        case State_Stopped:
            m_labelGraphState.SetWindowText( L"Graph state: STOPPED");
            break;

        case State_Paused:
            m_labelGraphState.SetWindowText( L"Graph state: PAUSED");
            break;

        case State_Running:
            m_labelGraphState.SetWindowText( L"Graph state: RUNNING");
            break;

        default:
            m_labelGraphState.SetWindowText( L"Graph state: !INVALID!");
            break;
        }
    }

    m_iGraphState= iGraphState;

    return true;
}
