#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CFloorTex;
class CTransform;
class CTexture;

END
class CFloor : public CGameObject
{
private:
	explicit CFloor(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CFloor();

public:
	static CFloor* Create_Pos(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
	static CFloor* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	Engine::CFloorTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
};