#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CWallTBTex;
class CTransform;
class CTexture;
class CCollider;

END
class CWallTB : public CGameObject
{
private:
	explicit CWallTB(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CWallTB();

public:
	static CWallTB* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	static CWallTB* Create_Pos(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
	static CWallTB* Create_Rot(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot);

	_vec3 Get_VecPos() { return m_vecPos; }
public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT Add_Component();
	HRESULT Setup_Material();
	void Setup_Position(_vec3 _vecPos);
	void Setup_Angle(_vec3 _vecRot);

private:
	virtual void Free();

private:
	Engine::CWallTBTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pColliderCom;
	_vec3 m_vecPos;
};

