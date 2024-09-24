#include "..\..\Header\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
{
}

CTexture::CTexture(const CTexture& _rhs)
	: CComponent(_rhs)
{
	_uint iSize = _rhs.m_vecTexture.size();
	m_vecTexture.reserve(iSize);

	m_vecTexture = _rhs.m_vecTexture;

	for (_uint i = 0; i < iSize; i++)
		m_vecTexture[i]->AddRef();
}

CTexture::~CTexture()
{
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 _pGraphicDev, const _tchar* _pPath, TEXTUREID _eType, const _uint& _iCnt)
{
	CTexture* pTexture = new CTexture(_pGraphicDev);

	if (FAILED(pTexture->Ready_Texture(_pPath, _eType, _iCnt)))
	{
		Safe_Release(pTexture);
		MSG_BOX("Texture Create Failed");
		return nullptr;
	}

	return pTexture;
}

HRESULT CTexture::Ready_Texture(const _tchar* _pPath, TEXTUREID _eType, const int& _iCnt)
{
	m_vecTexture.reserve(_iCnt);

	IDirect3DBaseTexture9* pTexture = NULL;

	for (_int i = 0; i < _iCnt; i++)
	{
		TCHAR szFileName[128] = L"";

		wsprintf(szFileName, _pPath, i);

		switch (_eType)
		{
		case TEXTUREID::TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;
		case TEXTUREID::TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		default :
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

void CTexture::Set_Texture(const _uint& _iIndex)
{
	if (m_vecTexture.size() <= _iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[_iIndex]);
}

CComponent* CTexture::Clone()
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();

	CComponent::Free();
}