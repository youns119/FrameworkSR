
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND	g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)

	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() : m_pFloor(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	// 스크롤 삭제하기

	//SetScrollSizes 스크롤 바의 사이즈를 지정하는 함수
	//MM_TEXT: 픽셀 단위로 크기를 조정

	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY * TILEY / 2));


	// AfxGetMainWnd : 현재의 메인 윈도우를 반환하는 전역 함수
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : 현재 윈도우의 Rect 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// 지정한 매개 변수 값으로 렉트를 설정하는 함수
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	// rcWnd.right -> 창의 가로 길이
	// rcWnd.bottom -> 창의 세로 길이
	RECT	rcMainView{};

	// GetClientRect : 현재 view 창의 RECT 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWnd.right - rcMainView.right);
	float fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : 매개 변수대로 새롭게 윈도우의 위치와 크기를 조정하는 함수

	//SetWindowPos(배치할 윈도우의 Z순서에 대한 포인터, X좌표, Y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기 및 위치 지정 옵션)
	//nullptr : 순서 변경 안함
	//SWP_NOZORDER : 현재 정렬 순서 유지

	pMainFrm->SetWindowPos(nullptr,
		0, 0,
		int(WINCX + fRowFrm),
		int(WINCY + fColFrm),
		SWP_NOZORDER);


	g_hWnd = m_hWnd;

	


	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"Init_Device Failed");
		return;
	}


	//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Client/Bin/Resource/MFCtest/Cube.png", TEX_SINGLE, L"Cube")))
	//{
	//	AfxMessageBox(L"Cube Texture Failed");
	//	return;
	//}

	m_pFloor = new CFloorCreate;
	if (FAILED(m_pFloor->Initialize()))
	{
		return;
	}

	m_pFloor->Set_MainView(this);
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CDevice::Get_Instance()->Render_Begin();

	m_pFloor->Render();

	CDevice::Get_Instance()->Render_End();
}
void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	//Safe_Delete(m_pFloor);

	CTextureMgr::Get_Instance()->Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CScrollView::OnLButtonDown(nFlags, point);
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

