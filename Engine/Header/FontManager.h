#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "MyFont.h"

BEGIN(Engine)

class ENGINE_DLL CFontManager 
	: public CBase
{
	DECLARE_SINGLETON(CFontManager)

private :
	explicit CFontManager();
	virtual ~CFontManager();

public :
	HRESULT	Ready_Font
	(
		LPDIRECT3DDEVICE9 _pGraphicDev,
		const _tchar* _pFontTag,
		const _tchar* _pFontType,
		const _uint& _iWidth,
		const _uint& _iHeight,
		const _uint& _iWeight
	);
	void Render_Font
	(
		const _tchar* _pFontTag,
		const _tchar* _pString,
		const _vec2* _pPos,
		D3DXCOLOR _tColor
	);

private :
	CMyFont* Find_Font(const _tchar* _pFontTag);

private :
	virtual void Free();

private :
	map<const _tchar*, CMyFont*> m_mapFont;
};

END