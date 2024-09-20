
// ToolView.h: CToolView 클래스의 인터페이스
//

#pragma once
#include "Device.h"
#include "TextureMgr.h"
#include "FloorCreate.h"
#include "../Reference/Header/Export_System.h"
#include "../Reference/Header/Export_Utility.h"

BEGIN(Engine)
class CGraphicDev;

END

class CMainSetting;
class CToolDoc;
class CToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// 특성입니다.
public:
	CToolDoc* GetDocument() const;

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
//private:
//	HRESULT	SetUp_DefaultSetting(LPDIRECT3DDEVICE9* _ppGraphicDev);
//	HRESULT	Ready_Scene(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::CManagement** _ppManagement);
public:
	CFloorCreate* m_pFloor;
private:
	CGraphicDev* m_pDeviceClass;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CMainSetting* m_pMainSetting;

	CManagement* m_pManagementClass;

private:
	void Free();

public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif

