#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CGuideTex;
class CTransform;
//class CTexture;

END

class CGuideTerrain : public CGameObject

{
private:
	explicit CGuideTerrain(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CGuideTerrain();

public:
	static CGuideTerrain* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	void Set_Position(_vec3 _vecPos) { m_vecPos = _vecPos; }


public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT Add_Component();
	HRESULT Setup_Material();

private:
	virtual void Free();

private:
	Engine::CGuideTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	_vec3 m_vecPos;
	//Engine::CTexture* m_pTextureCom;
};

