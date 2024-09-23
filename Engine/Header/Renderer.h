#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer
	: public CBase
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void Add_RenderGroup(RENDERID _eType, CGameObject* _pGameObject);
	void Render_GameObject(LPDIRECT3DDEVICE9& _pGraphicDev);
	void Clear_RenderGroup();

private:
	void Render_Priority(LPDIRECT3DDEVICE9& _pGraphicDev);
	void Render_NonAlpha(LPDIRECT3DDEVICE9& _pGraphicDev);
	void Render_Alpha(LPDIRECT3DDEVICE9& _pGraphicDev);
	void Render_Collider(LPDIRECT3DDEVICE9& _pGraphicDev);
	void Render_Orthogonal(LPDIRECT3DDEVICE9& _pGraphicDev);
	void Render_UI(LPDIRECT3DDEVICE9& _pGraphicDev);

private:
	virtual void Free();

private:
	list<CGameObject*> m_RenderGroup[(_uint)RENDERID::RENDER_END];

	// 직교 투영 관련 변수 추가
	_matrix m_matView, m_matOrtho;
};

END