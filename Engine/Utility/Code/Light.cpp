#include "..\..\Header\Light.h"

CLight::CLight(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_iIndex(0)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
	m_pGraphicDev->AddRef();
}

CLight::~CLight()
{
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 _pGraphicDev, const D3DLIGHT9* _pLightInfo, const _uint& _iIndex)
{
	CLight* pLight = new CLight(_pGraphicDev);

	if (FAILED(pLight->Ready_Light(_pLightInfo, _iIndex)))
	{
		Safe_Release(pLight);
		MSG_BOX("Light Create Failed");
		return nullptr;
	}

	return pLight;
}

HRESULT CLight::Ready_Light(const D3DLIGHT9* _pLightInfo, const _uint& _iIndex)
{
	memcpy(&m_tLightInfo, _pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = _iIndex;

	m_pGraphicDev->SetLight(_iIndex, _pLightInfo);

	m_pGraphicDev->LightEnable(_iIndex, TRUE);

	return S_OK;
}

void CLight::Free()
{
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);

	Safe_Release(m_pGraphicDev);
}