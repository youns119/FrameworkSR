#include "..\..\Header\FontManager.h"

IMPLEMENT_SINGLETON(CFontManager)

CFontManager::CFontManager()
{
	m_mapFont.clear();
}

CFontManager::~CFontManager()
{
	Free();
}

HRESULT CFontManager::Ready_Font(LPDIRECT3DDEVICE9 _pGraphicDev, const _tchar* _pFontTag, const _tchar* _pFontType, const _uint& _iWidth, const _uint& _iHeight, const _uint& _iWeight)
{
	CMyFont* pMyFont = Find_Font(_pFontTag);

	if (nullptr != pMyFont)
		return E_FAIL;

	pMyFont = CMyFont::Create(_pGraphicDev, _pFontType, _iWidth, _iHeight, _iWeight);
	NULL_CHECK_RETURN(pMyFont, E_FAIL);

	m_mapFont.insert({ _pFontTag, pMyFont });

	return S_OK;
}

void CFontManager::Render_Font(const _tchar* _pFontTag, const _tchar* _pString, const _vec2* _pPos, D3DXCOLOR _tColor)
{
	CMyFont* pMyFont = Find_Font(_pFontTag);
	NULL_CHECK(pMyFont);

	pMyFont->Render_Font(_pString, _pPos, _tColor);
}

CMyFont* CFontManager::Find_Font(const _tchar* _pFontTag)
{
	auto iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(_pFontTag));

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CFontManager::Free()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}