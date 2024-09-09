#pragma once

#include "GameObject.h"

BEGIN(Engine)

//Jonghan Monster Change Start
class CTriCol;
class CRcTex;
class CTexture;
class CCalculator;
//Jonghan Monster Change End
class CTransform;

END

class CMonster 
	: public Engine::CGameObject
{
private :
	explicit CMonster(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMonster();

public :
	static CMonster* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private :
	HRESULT	Add_Component();

private :
	virtual void Free();

private :
	//Jonghan Monster Change Start

	//Engine::CTriCol* m_pBufferCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

	_float m_fFrame;

	//Jonghan Monster Change End
	Engine::CTransform* m_pTransformCom;
};