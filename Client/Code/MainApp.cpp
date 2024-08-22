#include "pch.h"
#include "..\Header\MainApp.h"

const DWORD Vertex::m_FVF = D3DFVF_XYZ;

CMainApp::CMainApp()
	: m_pDeviceClass(nullptr)
	, m_pGraphicDev(nullptr)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
{
}

CMainApp::~CMainApp()
{
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if (FAILED(pMainApp->Ready_MainApp()))
	{
		delete pMainApp;
		pMainApp = nullptr;

		return nullptr;
	}

	return pMainApp;
}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	SetUp();

	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
}

void CMainApp::Render_MainApp()
{
	if (m_pGraphicDev)
	{
		D3DXMATRIX Rx, Ry;

		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);

		static float y = 0.f;
		D3DXMatrixRotationY(&Ry, y);
		y += CTimerManager::GetInstance()->Get_TimeDelta(L"Timer_FPS60");

		if (y >= 6.28f)
			y = 0.0f;

		D3DXMATRIX p = Rx * Ry;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &p);

		Engine::Render_Begin(D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		m_pGraphicDev-> SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pGraphicDev->SetFVF(Vertex::m_FVF);
		m_pGraphicDev->SetIndices(m_pIB);
		m_pGraphicDev->DrawIndexedPrimitive
		(
			D3DPT_TRIANGLELIST,
			0, 0, 8, 0, 12
		);

		Engine::Render_End();
	}
}

void CMainApp::Free()
{
	m_pVB->Release();
	m_pIB->Release();

	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);

	Engine::Release_System();
}

bool CMainApp::SetUp()
{
	m_pGraphicDev->CreateVertexBuffer
	(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::m_FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		0
	);

	m_pGraphicDev->CreateIndexBuffer
	(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		0
	);

	Vertex* vertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(1.0f, -1.0f, 1.0f);

	m_pVB->Unlock();

	WORD* indices = nullptr;
	m_pIB->Lock(0, 0, (void**)&indices, 0);

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	indices[12] = 4; indices[13] = 5; indices[14] = 2;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	m_pIB->Unlock();

	D3DXVECTOR3 position(0.f, 0.f, -5.f);
	D3DXVECTOR3 target(0.f, 0.f, 0.f);
	D3DXVECTOR3 up(0.f, 1.f, 0.f);
	D3DXMATRIX V;

	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &V);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH
	(
		&proj,
		D3DX_PI * 0.5f,
		(float)WINCX / (float)WINCY,
		1.0f,
		1000.0f
	);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &proj);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}