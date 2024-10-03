#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CFloorTex;
class CTransform;
class CTexture;
class CCollider;

END
class CFloor : public CGameObject
{
private:
	explicit CFloor(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CFloor();

public:
	static CFloor* Create_Pos(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
	static CFloor* Create_Info(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _tchar* _pName);
	static CFloor* Create_InfoTest(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, const _tchar* _pName);
	static CFloor* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	const _tchar* Get_FloorName() { return m_pName; }
	_vec3 Get_VecPos() { return m_vecPos; }
	_vec3 Get_VecRot() { return m_vecRot; }

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
private:
	HRESULT Add_Component();
	HRESULT Setup_Material();
	void Setup_Position(_vec3 _vecPos);
	void Setup_Rotation(_vec3 _vecRot);
	void Setup_ImageName(const _tchar* _pName);

private:
	virtual void Free();

private:
	Engine::CFloorTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pColliderCom;

	const _tchar* m_pName;
	_vec3 m_vecPos;
	_vec3 m_vecRot;


};