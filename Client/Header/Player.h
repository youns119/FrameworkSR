#pragma once

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CPlayer 
	: public Engine::CGameObject
{
private :
	explicit CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CPlayer();

public :
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual	HRESULT Ready_GameObject();
	virtual	_int Update_GameObject(const _float& _fTimeDelta);
	virtual	void LateUpdate_GameObject();
	virtual	void Render_GameObject();

private :
	HRESULT	Add_Component();
	void Key_Input(const _float& _fTimeDelta);
	_vec3 Picking_OnTerrain();

private:
	virtual void Free();

private :
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;
};