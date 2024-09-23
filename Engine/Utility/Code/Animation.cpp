#include "..\..\Header\Animation.h"
#include "Export_Utility.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_pTexture(nullptr)
	, m_fCurrFrame(0.f)
	, m_fAnimSpeed(0.f)
	, m_bAnimFinish(false)
{
	m_pGraphicDev->AddRef();
}

CAnimation::~CAnimation()
{
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 _pGraphicDev, CTexture* _pTexture, _float _fAnimSpeed)
{
	CAnimation* pAnimation = new CAnimation(_pGraphicDev);

	if (FAILED(pAnimation->Ready_Animation(_pTexture, _fAnimSpeed)))
	{
		Safe_Release(pAnimation);
		return nullptr;
	}

	return pAnimation;
}

HRESULT CAnimation::Ready_Animation(CTexture* _pTexture, _float _fAnimSpeed)
{
	m_pTexture = _pTexture;
	m_fAnimSpeed = _fAnimSpeed;

	return S_OK;
}

void CAnimation::Update_Animation(const _float& _fTimeDelta)
{
	if (m_bAnimFinish)
		return;

	m_fCurrFrame += m_fAnimSpeed * _fTimeDelta;

	if ((float)m_pTexture->Get_TextureCount() <= m_fCurrFrame)
	{
		m_fCurrFrame -= 1;
		m_bAnimFinish = true;
		return;
	}
}

void CAnimation::Free()
{
	Safe_Release(m_pGraphicDev);
}