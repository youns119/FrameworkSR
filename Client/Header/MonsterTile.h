#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END
class CMonsterTile : public CGameObject
{
private:
	explicit CMonsterTile(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonsterTile();

public:
	static CMonsterTile* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	static CMonsterTile* Create_Pos(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
	static CMonsterTile* Create_Rot(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot);
	static CMonsterTile* Create_Info(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _tchar* _pName);

	const _tchar* Get_MonsterName() { return m_pName; }
	_vec3 Get_VecPos() { return m_vecPos; }
	_int Get_Number() { return m_iNumber; }
public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	void Setup_ImageName(const _tchar* _pName);
	void Setup_ImageNumber(const _int& _iNumber) { m_iNumber = _iNumber; }

private:
	HRESULT Add_Component();
	HRESULT Setup_Material();
	void Setup_Position(_vec3 _vecPos);
	void Setup_Angle(_vec3 _vecRot);
	const _tchar* m_pName;
	_int m_iNumber;

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	_vec3 m_vecPos;
};
