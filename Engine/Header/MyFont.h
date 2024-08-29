#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMyFont 
	: public CBase
{
private :
	explicit CMyFont(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMyFont();

public :
	static CMyFont* Create
	(
		LPDIRECT3DDEVICE9 _pGraphicDev,
		const _tchar* _pFontType,
		const _uint& _iWidth,
		const _uint& _iHeight,
		const _uint& _iWeight
	);

public :
	HRESULT	Ready_Font
	(
		const _tchar* _pFontType,
		const _uint& _iWidth,
		const _uint& _iHeight,
		const _uint& _iWeight
	);
	void Render_Font
	(
		const _tchar* _pString,
		const _vec2* _pPos,
		D3DXCOLOR _tColor
	);

private :
	virtual void Free();

private :
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT m_pFont;
};

END