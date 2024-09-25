#include "..\..\Header\Renderer.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
	// 초기화 관련 추가
	Clear_RenderGroup();

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matOrtho, (_float)WINCX, (_float)WINCY, 0.f, 1.f);
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID _eType, CGameObject* _pGameObject)
{
	if ((_uint)RENDERID::RENDER_END <= (_uint)_eType || _pGameObject == nullptr)
		return;

	m_RenderGroup[(_uint)_eType].push_back(_pGameObject);
	_pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	Render_Priority(_pGraphicDev);
	Render_NonAlpha(_pGraphicDev);
	Render_Alpha(_pGraphicDev);
	Render_Collider(_pGraphicDev);
	Render_Orthogonal(_pGraphicDev);
	Render_UI(_pGraphicDev);

	Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < (_uint)RENDERID::RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}

	Engine::Clear_Collider();
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	for (auto& pGameObject : m_RenderGroup[(_uint)RENDERID::RENDER_PRIORITY])
		pGameObject->Render_GameObject();
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	for (auto& pGameObject : m_RenderGroup[(_uint)RENDERID::RENDER_NONALPHA])
		pGameObject->Render_GameObject();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_RenderGroup[(_uint)RENDERID::RENDER_ALPHA].sort
	(
		[](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		}
	);

	for (auto& pGameObject : m_RenderGroup[(_uint)RENDERID::RENDER_ALPHA])
		pGameObject->Render_GameObject();

	_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_Collider(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	if (Engine::Get_ColliderRender())
	{
		DWORD PreState;
		_pGraphicDev->GetRenderState(D3DRS_FILLMODE, &PreState);
		_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

		Engine::Render_Collider();

		_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		_pGraphicDev->SetRenderState(D3DRS_FILLMODE, PreState);
	}

	Engine::Clear_Collider();
}

void CRenderer::Render_Orthogonal(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	if (Engine::Get_ControllerID() != CONTROLLERID::CONTROL_PLAYER)
		return;

	// alphablending + orthogonal + alpha sorting(not yet) 
	_matrix matOldProjection, matOldView, matIdentity, matOrthogonal;
	_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProjection);
	_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	D3DXMatrixIdentity(&matIdentity);
	_pGraphicDev->SetTransform(D3DTS_VIEW, &matIdentity);

	D3DXMatrixOrthoLH(&matOrthogonal, WINCX, WINCY, 0.1f, 1000.f);
	_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOrthogonal);

	_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_RenderGroup[(_uint)RENDERID::RENDER_ORTHOGONAL].sort
	(
		[](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		}
	);

	for (auto& pGameObject : m_RenderGroup[(_uint)RENDERID::RENDER_ORTHOGONAL])
		pGameObject->Render_GameObject();

	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProjection);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	CUIManager::GetInstance()->Render_UI(_pGraphicDev);
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}