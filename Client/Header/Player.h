#pragma once

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CComponentCamera;

END

class CPlayer
	: public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CPlayer();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual	HRESULT Ready_GameObject();
	virtual	_int Update_GameObject(const _float& _fTimeDelta);
	virtual	void LateUpdate_GameObject();
	virtual	void Render_GameObject();

private:
	HRESULT	Add_Component();
	void Key_Input(const _float& _fTimeDelta);
	void Mouse_Move();
	void Mouse_Fix();
	_vec3 Picking_OnTerrain();

private:
	virtual void Free();

private:
	Engine::CCalculator* m_pCalculatorCom;

	//Beomseung
	Engine::CRcTex* m_pRight_BufferCom;
	Engine::CRcTex* m_pLeft_BufferCom;

	Engine::CTransform* m_pBody_TransformCom;

	Engine::CTransform* m_pRight_TransformCom;
	Engine::CTransform* m_pLeft_TransformCom;

	Engine::CTexture* m_pRight_TextureCom;
	Engine::CTexture* m_pLeft_TextureCom;

	Engine::CComponentCamera* m_pCComponentCamera;

};