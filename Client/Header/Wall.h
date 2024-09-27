#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CWallTex;
class CTransform;
class CTexture;

END
class CWall : public CGameObject
{
private:
	explicit CWall(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CWall();

public:
	static CWall* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	static CWall* Create_Pos(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT Add_Component();
	HRESULT Setup_Material();
	void Setup_Position(_vec3 _vecPos);

private:
	virtual void Free();

private:
	Engine::CWallTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
};

