#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;

END
class CTileContorl : public CGameObject
{
protected:
	explicit CTileContorl(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CTileContorl();


public:
	virtual HRESULT	Ready_GameObject() PURE;
	virtual _int Update_GameObject(const _float& _fTimeDelta)PURE;
	virtual void LateUpdate_GameObject()PURE;
	virtual void Render_GameObject()PURE;

	virtual _vec3 Get_VecPos() { return m_vecPos; }
	virtual _int Get_Number() { return m_iNumber; }
	virtual _int Get_Number_Type() { return m_iNumber_Type; }
	virtual _int Get_Trigger() { return m_iTriggerNumber; }

protected:
	virtual HRESULT Add_Component()PURE;
	virtual HRESULT Setup_Material()PURE;

protected:
	virtual void Free();

protected:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_vec3 m_vecPos;
	_int m_iNumber; //이게 몇번째 WallTB이냐는 정보
	_int m_iNumber_Type; //이게 WallTB이라는 정보

	_int m_iTriggerNumber; //몇번째 트리거에 넣을거냐 10.06

};

