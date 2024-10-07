#pragma once
#include "TileContorl.h"
BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END
class CMonsterTile : public CTileContorl
{
private:
	explicit CMonsterTile(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonsterTile();

public:
	static CMonsterTile* Create_InfoNumber(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber);
	static CMonsterTile* Create_InfoNumberTrigger(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber, const _int& _iTriggerNumber);//10.06

	_vec3 Get_VecPos() { return m_vecPos; }
	_int Get_Number() { return m_iNumber; }
	_int Get_Trigger() { return m_iTriggerNumber; }//10.06

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	void Setup_ImageNumber(const _int& _iNumber) { m_iNumber = _iNumber; }
	void Setup_TriggerNumber(const _int& _iTrigger) { m_iTriggerNumber = _iTrigger; }//10.06

private:
	virtual HRESULT Add_Component();
	virtual HRESULT Setup_Material();
	void Setup_Position(_vec3 _vecPos);
	void Setup_Angle(_vec3 _vecRot);

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	_vec3 m_vecPos;
};
