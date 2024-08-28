#include "..\..\Header\Renderer.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
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
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	for (auto& pGameObject : m_RenderGroup[(_uint)RENDERID::RENDER_PRIORITY])
		pGameObject->Render_GameObject();
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	for (auto& pGameObject : m_RenderGroup[(_uint)RENDERID::RENDER_NONALPHA])
		pGameObject->Render_GameObject();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	for (auto& pGameObject : m_RenderGroup[(_uint)RENDERID::RENDER_ALPHA])
		pGameObject->Render_GameObject();

	_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& _pGraphicDev)
{
	for (auto& pGameObject : m_RenderGroup[(_uint)RENDERID::RENDER_UI])
		pGameObject->Render_GameObject();
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}