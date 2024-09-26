#include "Effect.h"

CEffect::CEffect()
	: m_fLifeTime(0.f)
	, m_fElapsed(0.f)
	, m_bIsVisible(false)
	, m_bRepeatable(FALSE)
	, m_pCallBack(nullptr)
{
}

CEffect::CEffect(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
	, m_fLifeTime(0.f)
	, m_fElapsed(0.f)
	, m_bIsVisible(false)
	, m_bRepeatable(FALSE)
	, m_pCallBack(nullptr)
{
}

CEffect::CEffect(const CEffect& _rhs)
	: CComponent(_rhs)
	, m_fLifeTime(_rhs.m_fLifeTime)
	, m_fElapsed(_rhs.m_fElapsed)
	, m_bIsVisible(_rhs.m_bIsVisible)
	, m_bRepeatable(FALSE)
	, m_pCallBack(_rhs.m_pCallBack)
{
}

CEffect::~CEffect()
{
}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffect* pEffect = new CEffect(_pGraphicDev);

	if (FAILED(pEffect->Ready_Effect()))
	{
		Safe_Release(pEffect);
		MSG_BOX("pEffect Create Failed");
		return nullptr;
	}

	return pEffect;
}

HRESULT CEffect::Ready_Effect()
{
	m_bRepeatable = FALSE;
	return S_OK;
}

_int CEffect::Update_Component(const _float& _fTimeDelta)
{
	if (m_bIsVisible)
	{
		m_fElapsed += _fTimeDelta;
		if (m_fLifeTime < m_fElapsed)
		{

			if (m_bRepeatable)
			{
				m_fElapsed = m_fElapsed - m_fLifeTime;
			}
			else
			{
				m_fElapsed = 0.f;
				m_bIsVisible = false;
			}
		}
	}
	return 0;
}

void CEffect::LateUpdate_Component()
{
}

CEffect* CEffect::Clone()
{
	return new CEffect(*this);
}

void CEffect::Free()
{
	CComponent::Free();
}

void CEffect::Operate_Effect()
{
	if (m_pCallBack)
		m_pCallBack(m_pParam);

	Set_Visibility(TRUE);
}

// 에이시 함수이름 왜이리 헷갈려 나도 헷갈려이젠
void CEffect::Stop_Effect()
{
	m_fElapsed = 0.f;
	m_bIsVisible = FALSE;
}

void CEffect::Reset()
{
	m_bIsVisible = FALSE;
}