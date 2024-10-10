#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTransform;
class CTexture;

END

class CSkyBox 
	: public Engine::CGameObject
{
private :
	explicit CSkyBox(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CSkyBox(LPDIRECT3DDEVICE9 _pGraphicDev, int _iTextureNumber);//배경 텍스쳐 넘버 추가
	virtual ~CSkyBox();

public :
	static CSkyBox* Create(LPDIRECT3DDEVICE9 _pGraphicDev, int _iTextureNumber);

public :
	virtual	HRESULT	Ready_GameObject();
	virtual	_int Update_GameObject(const _float& _fTimeDelta);
	virtual	void LateUpdate_GameObject();
	virtual	void Render_GameObject();

private :
	HRESULT	Add_Component();

private :
	virtual void Free();

private :
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	_int m_iTextureNumber;
};