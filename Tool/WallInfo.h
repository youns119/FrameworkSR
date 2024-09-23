#pragma once
#include "GameObject.h"
#include "Engine_Define.h"


BEGIN(Engine)

class CWallTex;
class CTransform;
class CTexture;

END

class CWallInfo : public Engine::CGameObject
{

protected:
	explicit CWallInfo(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CWallInfo();

public:
	virtual HRESULT	Ready_GameObject()PURE;
	virtual _int Update_GameObject(const _float& _fTimeDelta)PURE;
	virtual void LateUpdate_GameObject()PURE;
	virtual void Render_GameObject()PURE;
	virtual void Free()PURE;

protected:
	Engine::CWallTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;


};
