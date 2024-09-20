#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CFloorTex;
class CTransform;
class CTexture;

END


class CGuideLine
	: public Engine::CGameObject
{
private:
	explicit CGuideLine(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CGuideLine();

public:
	static CGuideLine* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT Add_Component();
	HRESULT Setup_Material();

private:
	virtual void Free();

private:
	Engine::CFloorTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
};

