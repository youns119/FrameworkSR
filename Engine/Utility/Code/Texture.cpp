#include "..\..\Header\Texture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
{
}

CTexture::CTexture(const CTexture& _rhs)
	: CComponent(_rhs)
{
	_uint iSize = (_uint)_rhs.m_vecTexture.size();
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

void CTexture::Change_TextureColor(_float _fHue, _float _fSaturation, _float _fValue)
{
	D3DCOLOR tColor = Get_HSVRGB(_fHue, _fSaturation, _fValue);

	D3DLOCKED_RECT tLockRect;
	IDirect3DBaseTexture9* pTexture;

	m_pGraphicDev->GetTexture(0, &pTexture);
	IDirect3DTexture9* pLockTexture = static_cast<IDirect3DTexture9*>(pTexture);

	pLockTexture->LockRect(0, &tLockRect, NULL, 0);

	DWORD* pTexels = (DWORD*)tLockRect.pBits;
	D3DSURFACE_DESC tDesc;

	pLockTexture->GetLevelDesc(0, &tDesc);

	for (UINT y = 0; y < tDesc.Height; y++)
		for (UINT x = 0; x < tDesc.Width; x++)
		{
			D3DCOLOR currentColor = pTexels[x + y * (tLockRect.Pitch / 4)];
			BYTE alpha = (BYTE)(currentColor >> 24);

			if (alpha != 0)
				pTexels[x + y * (tLockRect.Pitch / 4)] = tColor;
		}

	pLockTexture->UnlockRect(0);
}

void CTexture::Change_TextureColor(D3DCOLOR _tColor)
{
	D3DLOCKED_RECT tLockRect;
	IDirect3DBaseTexture9* pTexture;

	m_pGraphicDev->GetTexture(0, &pTexture);
	IDirect3DTexture9* pLockTexture = static_cast<IDirect3DTexture9*>(pTexture);

	pLockTexture->LockRect(0, &tLockRect, NULL, 0);

	DWORD* pTexels = (DWORD*)tLockRect.pBits;
	D3DSURFACE_DESC tDesc;

	pLockTexture->GetLevelDesc(0, &tDesc);

	for (UINT y = 0; y < tDesc.Height; y++)
		for (UINT x = 0; x < tDesc.Width; x++)
		{
			D3DCOLOR currentColor = pTexels[x + y * (tLockRect.Pitch / 4)];
			BYTE alpha = (BYTE)(currentColor >> 24);

			if (alpha != 0)
				pTexels[x + y * (tLockRect.Pitch / 4)] = _tColor;
		}

	pLockTexture->UnlockRect(0);
}

void CTexture::Change_Alpha(_float _fAlpha)
{
	DWORD textureFactor;

	textureFactor = D3DCOLOR_ARGB((int)(_fAlpha * 255), 255, 255, 255);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, textureFactor);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

D3DCOLOR CTexture::Get_HSVRGB(_float _fHue, _float _fSaturation, _float _fValue)
{
	_float fRed(0.f), fGreen(0.f), fBlue(0.f);

	_int iDigit = int(_fHue / 60.0f) % 6;
	_float fDec = (_fHue / 60.0f) - iDigit;
	_float fRatio1 = _fValue * (1 - _fSaturation);
	_float fRatio2 = _fValue * (1 - fDec * _fSaturation);
	_float fRatio3 = _fValue * (1 - (1 - fDec) * _fSaturation);

	switch (iDigit)
	{
	case 0:
		fRed = _fValue; fGreen = fRatio3; fBlue = fRatio1;
		break;
	case 1:
		fRed = fRatio2; fGreen = _fValue; fBlue = fRatio1;
		break;
	case 2:
		fRed = fRatio1; fGreen = _fValue; fBlue = fRatio3;
		break;
	case 3:
		fRed = fRatio1; fGreen = fRatio2; fBlue = _fValue;
		break;
	case 4:
		fRed = fRatio3; fGreen = fRatio1; fBlue = _fValue;
		break;
	case 5:
		fRed = _fValue; fGreen = fRatio1; fBlue = fRatio2;
		break;
	default:
		break;
	}

	return D3DCOLOR_XRGB(int(fRed * 255), int(fGreen * 255), int(fBlue * 255));
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