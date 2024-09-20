
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"

#include "MainFrm.h"
#include "../Reference/Header/Export_Utility.h"
#include "../Reference/Header/Export_System.h"
#include "MainSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND	g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pDeviceClass(nullptr)
	, m_pGraphicDev(nullptr)
	, m_pManagementClass(nullptr)
	, m_pFloor(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_hWnd = m_hWnd;

	m_pMainSetting = CMainSetting::Create(g_hWnd);

	Engine::Ready_Timer(L"Timer_Immediate");
	Engine::Ready_Timer(L"Timer_FPS60");
	Engine::Ready_Frame(L"Frame60", 60.f);


#pragma region 기존 툴뷰에 있던 것


	//if (FAILED(CDevice::Get_Instance()->Init_Device()))
	//{
	//	AfxMessageBox(L"Init_Device Failed");
	//	return;
	//}


	//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Client/Bin/Resource/MFCtest/Cube.png", TEX_SINGLE, L"Cube")))
	//{
	//	AfxMessageBox(L"Cube Texture Failed");
	//	return;
	//}

	//m_pFloor = new CFloorCreate;
	//if (FAILED(m_pFloor->Initialize()))
	//{
	//	return;
	//}

	//m_pFloor->Set_MainView(this);

#pragma endregion 기존 툴뷰에 있던 것

}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	Engine::Set_TimeDelta(L"Timer_Immediate");
	_float fTimer_Immediate = Engine::Get_TimeDelta(L"Timer_Immediate");

	if (Engine::IsPermit_Call(L"Frame60", fTimer_Immediate))
	{
		Engine::Set_TimeDelta(L"Timer_FPS60");
		_float fTimer_FPS60 = Engine::Get_TimeDelta(L"Timer_FPS60");
	
	m_pMainSetting->Update_MainSetting(fTimer_FPS60);
	m_pMainSetting->LateUpdate_MainSetting();
	m_pMainSetting->Render_MainSetting();
	}

}
void CToolView::OnDestroy()
{
	CView::OnDestroy();

	
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CView::OnLButtonDown(nFlags, point);
}


// 여기 아래 함수 사용 안함(디버그는 사용)
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

