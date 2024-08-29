#include "MyFont.h"

CMyFont::CMyFont(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_pSprite(nullptr)
	, m_pFont(nullptr)
{
	m_pGraphicDev->AddRef();
}

CMyFont::~CMyFont()
{
}

CMyFont* CMyFont::Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _tchar* _pFontType, const _uint& _iWidth, const _uint& _iHeight, const _uint& _iWeight)
{
	CMyFont* pInstance = new CMyFont(_pGraphicDev);

	if (FAILED(pInstance->Ready_Font(_pFontType, _iWidth, _iHeight, _iWeight)))
	{
		Safe_Release(pInstance);
		MSG_BOX("폰트 문제");
		return nullptr;
	}

	return pInstance;
}

HRESULT CMyFont::Ready_Font(const _tchar* _pFontType, const _uint& _iWidth, const _uint& _iHeight, const _uint& _iWeight)
{
	D3DXFONT_DESC tFont_Desc;
	ZeroMemory(&tFont_Desc, sizeof(D3DXFONT_DESC));

	tFont_Desc.CharSet = HANGEUL_CHARSET;
	tFont_Desc.Width = _iWidth;
	tFont_Desc.Height = _iHeight;
	tFont_Desc.Weight = _iWeight;
	lstrcpy(tFont_Desc.FaceName, _pFontType);

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFont_Desc, &m_pFont)))
	{
		MSG_BOX("Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CMyFont::Render_Font(const _tchar* _pString, const _vec2* _pPos, D3DXCOLOR _tColor)
{
	RECT rc{ (_long)_pPos->x, (_long)_pPos->y };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, _pString, lstrlen(_pString), &rc, DT_NOCLIP, _tColor);

	m_pSprite->End();
}

void CMyFont::Free()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);

	Safe_Release(m_pGraphicDev);
}