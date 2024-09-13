
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Tool.h"

#include "MainFrm.h"
#include "ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

//IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

//BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	// 나중에 삭제
	//theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
// CreateStatic(부모 윈도우 주소, 행의 수, 열의 수, 창 스타일 옵션, 자식 창 ID) : 창을 분할하는 함수
 m_MainSplitter.CreateStatic(this, 1, 2);

// CreateView : 분할된 창에 표시할 view를 생성하는 함수
// CreateView(배치할 행, 배치할 열, 배치할 view창을 새로 생성, 초기 크기, pContext);

m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);
m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);

m_MainSplitter.SetColumnInfo(1, 100, 10);


//m_MainSplitter.CreateStatic(this, 2, 2);
//
//// CreateView : 분할된 창에 표시할 view를 생성하는 함수
//// CreateView(배치할 행, 배치할 열, 배치할 view창을 새로 생성, 초기 크기, pContext);
//
//m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
//m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
//m_MainSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);
//m_MainSplitter.CreateView(1, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);

	//m_MainSplitter.CreateStatic(this, 1, 2);

	//m_MainSplitter.CreateView(1, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);

	//// WS_CHILD : 자식 창 형식 옵션
	//// WS_VISIBLE : 생성 후 바로 화면에 표시 옵션
	//// IdFromRowCol(행, 열) : 지정한 행과 열에 해당하는 창의 id값을 반환

	//m_SecondSplitter.CreateStatic(&m_MainSplitter, 0, 0, WS_CHILD | WS_VISIBLE, m_MainSplitter.IdFromRowCol(0, 0));
	//m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);

	//// SetColumnInfo(열 번호, 열의 크기 지정, 허용 가능한 최소 크기)
	//m_MainSplitter.SetColumnInfo(0, 300, 10);

	return TRUE;//CFrameWnd::OnCreateClient(lpcs, pContext);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//if( !CFrameWndEx::PreCreateWindow(cs) )
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	//CFrameWndEx::AssertValid();
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	//CFrameWndEx::Dump(dc);
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
