
#include "stdafx.h"
#include "Multi_Cam.h"
#include "Multi_CamDoc.h"
#include "Multi_CamView.h"
#include "DlgSelectCamera.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMulti_CamView, CView)

BEGIN_MESSAGE_MAP(CMulti_CamView, CView)

	ON_WM_TIMER()
	ON_WM_DESTROY()

	ON_COMMAND(ID_FILE_SETTINGS, OnProperties)
	ON_COMMAND(ID_BUTTON_TRIGGER_FREEZE, OnTriggerFreeze)
	ON_COMMAND(ID_BUTTON_TRIGGER_CAPTURE, OnTriggerCapture)
	ON_COMMAND(ID_BUTTON_SAVE, OnSave)
	ON_COMMAND(ID_BUTTON_PARAMETER, OnLoadParam)

	ON_UPDATE_COMMAND_UI(ID_BUTTON_TRIGGER_CAPTURE, OnUpdateCommandUi)

	ON_MESSAGE(IS_UEYE_MESSAGE, OnUEyeMessage)

END_MESSAGE_MAP()

CMulti_CamView::CMulti_CamView()
{
}

CMulti_CamView::~CMulti_CamView()
{

}

BOOL CMulti_CamView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CMulti_CamView::OnDraw(CDC* pDC)
{
    int nRet = 0;

    /* set window icon */
    HICON hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
    ((CMDIFrameWnd*) AfxGetApp()->m_pMainWnd)->MDIGetActive()->SetIcon(hIcon, false);

    CRect rect;
    GetClientRect(&rect);
    pDC->FillSolidRect(&rect, RGB(0, 0, 0));

    char* pLast = NULL;
    INT lMemoryId = 0;
    INT lSequenceId = 0;

    if (IS_SUCCESS == m_Camera.GetLastMem(&pLast, lMemoryId, lSequenceId))
    {
        INT nRet = is_LockSeqBuf(m_Camera.hCam, IS_IGNORE_PARAMETER, pLast);

        if (IS_SUCCESS == nRet)
        {
            nRet = is_RenderBitmap(m_Camera.hCam, lMemoryId, m_hWnd, m_Camera.RenderMode);

            OnDrawTimestamp();
            is_UnlockSeqBuf(m_Camera.hCam, IS_IGNORE_PARAMETER, pLast);
        }
    }
}

#ifdef _DEBUG
void CMulti_CamView::AssertValid() const
{
	CView::AssertValid();
}

void CMulti_CamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMulti_CamDoc* CMulti_CamView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMulti_CamDoc)));
	return (CMulti_CamDoc*)m_pDocument;
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
void CMulti_CamView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
    OpenCamera();
}

int  CMulti_CamView::InitCamera(HIDS *hCam, HWND hWnd)
{
    INT nRet = is_InitCamera (hCam, hWnd);	
    /************************************************************************************************/
    /*                                                                                              */
    /*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
    /*  is necessary. This upload can take several seconds. We recommend to check the required      */
    /*  time with the function is_GetDuration().                                                    */
    /*                                                                                              */
    /*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */ 
    /*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
    /*                                                                                              */                        
    /************************************************************************************************/
    if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
    {
        // Time for the firmware upload = 25 seconds by default
        INT nUploadTime = 25000;
        is_GetDuration (*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);

        CString Str1, Str2, Str3;
        Str1 = L"This camera requires a new firmware. The upload will take about";
        Str2 = L"seconds. Please wait ...";
        Str3.Format(L"%s %d %s", Str1, nUploadTime / 1000, Str2);
        AfxMessageBox(Str3, MB_ICONWARNING);

        // Set mouse to hourglass
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

        // Try again to open the camera. This time we allow the automatic upload of the firmware by
        // specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
        *hCam = (HIDS) (((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD); 
        nRet = is_InitCamera (hCam, hWnd);   
    }

    return nRet;
}

void CMulti_CamView::OpenCamera()
{
	CDlgSelectCamera dlgSelectCamera;
    int nRet = 0;

    m_Camera.hCam = ((HIDS)(GetDocument()->m_nCamDevID | IS_USE_DEVICE_ID)); 
    nRet = InitCamera(&m_Camera.hCam, m_hWnd);

    if (nRet == IS_SUCCESS)
    {
        m_Camera.bOpened = true;
        bool bFound = false;

        std::vector<void*> &vecTemp = ((CMainFrame*)AfxGetMainWnd())->m_handleVector;

        for (unsigned int i = 0; i < vecTemp.size(); ++i)
        {
            CMulti_CamView *view = ((CMulti_CamView*)vecTemp[i]);

            if (view == this)
            {
                bFound = true;
            }
        }

        if (bFound == false)
        {
            ((CMainFrame*)AfxGetMainWnd())->m_handleVector.push_back(this);
        }

        nRet = is_SetExternalTrigger(m_Camera.hCam, IS_SET_TRIGGER_SOFTWARE);
        nRet = is_EnableMessage(m_Camera.hCam, IS_FRAME, m_hWnd);

        nRet = is_GetSensorInfo(m_Camera.hCam, &m_Camera.SensorInfo);
        nRet = is_GetCameraInfo(m_Camera.hCam, &m_Camera.CamInfo);

        nRet = is_SetColorMode(m_Camera.hCam, IS_CM_BGRA8_PACKED);

        IS_RECT rectAoi = { 0 };

        nRet = is_AOI(m_Camera.hCam, IS_AOI_IMAGE_GET_AOI, (void*)&rectAoi, sizeof(rectAoi));
        nRet = m_Camera.AllocImageMems(rectAoi.s32Width, rectAoi.s32Height,
                                       m_Camera.nBitsPerPixel);

        nRet = m_Camera.InitSequence();
        nRet = is_CaptureVideo(m_Camera.hCam, IS_WAIT); 
        m_Camera.bLive = true;

        // Window - Name
        WCHAR temp[32];
        mbstowcs(temp, m_Camera.SensorInfo.strSensorName, 32);
        m_strName.Format(L"%s", temp);

        WCHAR temp1[20];
        mbstowcs(temp1, m_Camera.CamInfo.ID, 20);
        m_strBordID.Format(L"%s", temp1);

        WCHAR temp2[12];
        mbstowcs(temp2, m_Camera.CamInfo.SerNo, 12);
        m_strSerNo.Format(L"%s", temp2);

        /* create our window title */
        CString strWindowTitle = L"Camera: " + m_strName + L" ID: " + m_strBordID + L" Ser. No.: " + m_strSerNo;

        GetDocument()->SetTitle(strWindowTitle);
    }
    else
    {
        CString strMessage;
        strMessage.Format(L"Opening the camera failed with: %d", nRet);
        AfxMessageBox(strMessage);

        GetParentFrame()->PostMessage(WM_CLOSE);
    }
}

void CMulti_CamView::OnDestroy()
{
	int nRet = is_ExitCamera(m_Camera.hCam);
    m_Camera.bOpened = false;

	std::vector<void*> &vecTemp = ((CMainFrame*)AfxGetMainWnd())->m_handleVector;

	for (std::vector<void*>::iterator iter = vecTemp.begin(); iter != vecTemp.end(); iter++)
	{
		CMulti_CamView *pView = (CMulti_CamView*)*iter;

		if (pView == this)
		{
            vecTemp.erase(iter);
			break;
		}
	}

    if (vecTemp.empty())
    {
        // uncheck button
        CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
        pFrame->m_wndToolBar.GetToolBarCtrl().CheckButton(ID_BUTTON_TRIGGER_CAPTURE, FALSE);
    }

	CView::OnDestroy();
}

LRESULT CMulti_CamView::OnUEyeMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IS_FRAME:
		{
            char* pLast = NULL;
            INT lMemoryId = 0;
            INT lSequenceId = 0;

            if (IS_SUCCESS == m_Camera.GetLastMem(&pLast, lMemoryId, lSequenceId))
            {
                INT nRet = is_LockSeqBuf(m_Camera.hCam, IS_IGNORE_PARAMETER, pLast);

                if (IS_SUCCESS == nRet)
                {
                    nRet = is_RenderBitmap(m_Camera.hCam, lMemoryId, m_hWnd, m_Camera.RenderMode);

                    if (nRet == IS_SUCCESS)
                    {
                        /* receive imageinfo for timestamp */
                        UEYEIMAGEINFO ImageInfo;
                        nRet = is_GetImageInfo(m_Camera.hCam, lMemoryId, &ImageInfo, sizeof(ImageInfo));

                        if (nRet == IS_SUCCESS)
                        {
                            // Timestamp of system
                            m_strTimestamp.Format(L"%02d.%02d.%04d, %02d:%02d:%02d:%03d",
                                                  ImageInfo.TimestampSystem.wDay,
                                                  ImageInfo.TimestampSystem.wMonth,
                                                  ImageInfo.TimestampSystem.wYear,
                                                  ImageInfo.TimestampSystem.wHour,
                                                  ImageInfo.TimestampSystem.wMinute,
                                                  ImageInfo.TimestampSystem.wSecond,
                                                  ImageInfo.TimestampSystem.wMilliseconds);

                            OnDrawTimestamp();
                        }
                    }

                    is_UnlockSeqBuf(m_Camera.hCam, IS_IGNORE_PARAMETER, pLast);
                }
            }
		}

		break;

	}

	return 0;
}

void CMulti_CamView::OnProperties()
{
    if (m_Camera.SensorInfo.SensorID != IS_SENSOR_XS &&
        m_Camera.SensorInfo.SensorID != IS_SENSOR_UI1005_C)
    {
        // set camera handle
        m_cameraConfiguration.SetCameraHandle(&m_Camera);
        m_imageConfiguration.SetCameraHandle(&m_Camera);
        m_triggerConfiguration.SetCameraHandle(&m_Camera);

        // set size configuration
        m_sizeConfiguration.SetCameraHandle(&m_Camera);
        m_sizeConfiguration.SetView(this);

        // Property Page
        CPropertySheet dlgSheet (L"Settings - Camera: " + m_strName + L" Ser. No.: " + m_strSerNo);

        // Add pages to our property page
        dlgSheet.AddPage(&m_cameraConfiguration);
        dlgSheet.AddPage(&m_imageConfiguration);
        dlgSheet.AddPage(&m_sizeConfiguration);
        dlgSheet.AddPage(&m_triggerConfiguration);

        dlgSheet.m_psh.dwFlags &= ~(PSH_HASHELP); 
        dlgSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;

        // show the properties dialog
        dlgSheet.DoModal();
    }
    else
    {
        AfxMessageBox(L"Settings are not available for XS.");
    }

	Invalidate(1);
}

void CMulti_CamView::OnTriggerFreeze()
{
	// Freeze Videos
	std::vector<void*> &vecTemp = ((CMainFrame*)AfxGetMainWnd())->m_handleVector;
	// Trigger all Windows
	for (unsigned int i = 0; i < vecTemp.size(); ++i)
	{
		CMulti_CamView *pView = ((CMulti_CamView*)vecTemp[i]);
		is_FreezeVideo(pView->m_Camera.hCam, IS_DONT_WAIT);

		pView->m_Camera.bLive = false;
	}
}

void CMulti_CamView::OnTriggerCapture()
{
	int nRet = 0;
	// Capture Video
	std::vector<void*> &vecTemp = ((CMainFrame*)AfxGetMainWnd())->m_handleVector;

    for (unsigned int i = 0; i < vecTemp.size(); ++i)
    {
        CMulti_CamView *pView = ((CMulti_CamView*)vecTemp[i]);
        if (!pView->m_Camera.bLive)
        {
            nRet = is_CaptureVideo(pView->m_Camera.hCam, IS_DONT_WAIT);

            if (nRet != IS_SUCCESS)
            {
                CString Str;
                Str.Format(L"Capture video failed with: %d", nRet);
                AfxMessageBox(Str, MB_ICONERROR);
            }
            else
            {
                pView->m_Camera.bLive = true;
            }
        }
    }
}

void CMulti_CamView::OnDrawTimestamp()
{
	// show Text on window
	CDC *pDC = GetDC();
	CRect rect2(0, 0, 200, 200);

	pDC->DrawText(m_strTimestamp, &rect2, 0);

	ReleaseDC(pDC);
}

void CMulti_CamView::OnSave()
{
	// Save a Picture 
	int nRet = 0;

	IMAGE_FILE_PARAMS ImageFileParams;

    memset(&ImageFileParams, 0, sizeof(ImageFileParams));
	ImageFileParams.pwchFileName = NULL;
	ImageFileParams.pnImageID = NULL;
	ImageFileParams.ppcImageMem = NULL;
	ImageFileParams.nQuality = 0;

	nRet = is_ImageFile(m_Camera.hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams, sizeof(ImageFileParams));
    if (nRet != IS_SUCCESS)
    {
        CString strMessage;
        strMessage.Format(L"Could not save image. Error: %d", nRet);
        AfxMessageBox(strMessage);
    }
}

void CMulti_CamView::OnLoadParam()
{
	// Load Parameters
	if (m_Camera.hCam != 0 )
	{
		if (is_ParameterSet(m_Camera.hCam, IS_PARAMETERSET_CMD_LOAD_FILE, NULL, NULL) == IS_SUCCESS)
		{
			// stop live video
		    is_StopLiveVideo(m_Camera.hCam, IS_FORCE_VIDEO_STOP);  

            m_Camera.ClearSequence();
			// realloc image mem with actual sizes and depth.
            m_Camera.FreeImageMems();

			IS_SIZE_2D imageSize;
			is_AOI(m_Camera.hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

			INT nAllocSizeX = 0;
			INT nAllocSizeY = 0;

			nAllocSizeX = imageSize.s32Width;
			nAllocSizeY = imageSize.s32Height;

			UINT nAbsPosX = 0;
			UINT nAbsPosY = 0;

			// absolute pos?
			is_AOI(m_Camera.hCam, IS_AOI_IMAGE_GET_POS_X_ABS, (void*)&nAbsPosX , sizeof(nAbsPosX));
			is_AOI(m_Camera.hCam, IS_AOI_IMAGE_GET_POS_Y_ABS, (void*)&nAbsPosY , sizeof(nAbsPosY));

			if (nAbsPosX)
			{
				nAllocSizeX = m_Camera.SensorInfo.nMaxWidth;
			}
			if (nAbsPosY)
			{
				nAllocSizeY = m_Camera.SensorInfo.nMaxHeight;
			}

            // use always the same colormode
            is_SetColorMode(m_Camera.hCam, IS_CM_RGBA8_PACKED);
			
			// memory initialization
            m_Camera.AllocImageMems(nAllocSizeX, nAllocSizeY, m_Camera.nBitsPerPixel);
            m_Camera.InitSequence();

			// run live image again
            if (m_Camera.bLive)
            {
				is_CaptureVideo(m_Camera.hCam, IS_DONT_WAIT);
            }
		}
	}  
}

void CMulti_CamView::OnUpdateCommandUi(CCmdUI *pCmdUI)
{
	if(m_Camera.bLive == true && m_Camera.bOpened)
	{
		pCmdUI->SetCheck(true);
	}
	else
	{
		pCmdUI->SetCheck(false);
	}
}
