#pragma once
#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CFloorTex;
class CTransform;
class CTexture;

END

class CFloorInfo : public Engine::CGameObject
{
protected:
	explicit CFloorInfo(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CFloorInfo();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual _int Update_GameObject(const _float& _fTimeDelta)PURE;
	virtual void LateUpdate_GameObject()PURE;
	virtual void Render_GameObject()PURE;
	virtual void Free()PURE;

protected:
	Engine::CFloorTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;


};
