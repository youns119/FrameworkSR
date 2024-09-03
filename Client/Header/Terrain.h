#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CTexture;

END

class CTerrain 
	: public Engine::CGameObject
{
private :
	explicit CTerrain(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CTerrain();

public :
	static CTerrain* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private :
	HRESULT Add_Component();
	HRESULT Setup_Material();

private :
	virtual void Free();

private :
	Engine::CTerrainTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
};