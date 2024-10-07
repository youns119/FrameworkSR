#pragma once
#include "TileContorl.h"

BEGIN(Engine)

class CFloorTex;
class CTransform;
class CTexture;
class CCollider;

END
class CFloor : public CTileContorl
{
private:
	explicit CFloor(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CFloor();

public:
	static CFloor* Create_InfoNumberTrigger(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber, const _int& _iTrigger);
	static CFloor* Create_InfoNumberTrigger2(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, const _int& _iNumber, const _int& _iTrigger);

public:
	_vec3 Get_VecPos() { return m_vecPos; }
	_vec3 Get_VecRot() { return m_vecRot; }
	_int Get_Number() { return m_iNumber; }
	_int Get_Trigger() { return m_iTriggerNumber; }//10.06
	void Set_Number(const _int& _iNumber) { m_iNumber = _iNumber; }
	void Set_Trigger(const _int& _iTrigger) { m_iTriggerNumber = _iTrigger; }//10.06

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual void Set_IsRender(const _bool& _bool); //For Trigger(Tile, Monster)
private:
	virtual HRESULT Add_Component();
	virtual HRESULT Setup_Material();
	void Setup_Position(_vec3 _vecPos);
	void Setup_Rotation(_vec3 _vecRot);

private:
	virtual void Free();

private:
	Engine::CFloorTex* m_pBufferCom;
	Engine::CCollider* m_pColliderCom;

	_vec3 m_vecRot;
	_float m_fDamage;
};