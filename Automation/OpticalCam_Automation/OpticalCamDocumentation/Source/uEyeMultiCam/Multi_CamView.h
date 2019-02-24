
#pragma once

#include <vector>
#include "ueye.h"
#include "Camera.h"
#include "ImageConfiguration.h"
#include "CameraConfiguration.h"
#include "SizeConfiguration.h"
#include "TriggerConfiguration.h"
#include "ChildFrm.h"

class CMulti_CamDoc;

class CMulti_CamView : public CView
{
private: 
	CMulti_CamView();
	virtual ~CMulti_CamView();

	DECLARE_DYNCREATE(CMulti_CamView)

	Camera m_Camera;

	CString m_strName;
	CString m_strBordID;
	CString m_strSerNo;

	CString m_strTimestamp;

	CCameraConfiguration	m_cameraConfiguration;
	CImageConfiguration		m_imageConfiguration;
	CSizeConfiguration		m_sizeConfiguration;
	CTriggerConfiguration	m_triggerConfiguration;

public:
	std::vector<CChildFrame*> m_VecView;
	CMulti_CamDoc* GetDocument() const;

	virtual void OnDraw(CDC* pDC);	
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnDestroy();
	afx_msg void OnProperties();
	afx_msg void OnUpdateCommandUi(CCmdUI *pCmdUI);

	void OnTriggerFreeze();
	void OnTriggerCapture();
	void OnSingleTrigger();
	void OnDraw();
	void OnDrawTimestamp();
	void OnSave();
	void OnLoadParam();

    void OpenCamera();
    int  InitCamera(HIDS *hCam, HWND hWnd);

	LRESULT OnUEyeMessage(WPARAM wParam, LPARAM lParam);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  
inline CMulti_CamDoc* CMulti_CamView::GetDocument() const
{ return reinterpret_cast<CMulti_CamDoc*>(m_pDocument); }
#endif

