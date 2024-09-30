#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;

END

class CFilterFundo
	: public CGameObject
{
private:
	explicit CFilterFundo(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CFilterFundo();

public:
	static CFilterFundo* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& _fTimeDelta) override;
	void LateUpdate_GameObject() override;
	void Render_GameObject() override;

private:
	HRESULT	Add_Component();

private:
	void Free() override;

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;

};

