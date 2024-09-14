#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture 
	: public CComponent
{
private :
	explicit CTexture(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CTexture(const CTexture& _rhs);
	virtual ~CTexture();

public :
	// ¿¬¿í
	_uint Get_TextureCount() { return m_vecTexture.size(); }

public :
	static CTexture* Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _tchar* _pPath, TEXTUREID _eType, const _uint& _iCnt = 1);

public :
	HRESULT	Ready_Texture(const _tchar* _pPath, TEXTUREID _eType, const int& _iCnt);
	void Set_Texture(const _uint& _iIndex = 0);

public :
	virtual CComponent* Clone();

private :
	virtual void Free();

private :
	vector<IDirect3DBaseTexture9*> m_vecTexture;
};

END