#include "..\..\Header\Component.h"

CComponent::CComponent()
	: m_bClone(false)
	, m_pGraphicDev(nullptr)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_bClone(false)
	, m_pGraphicDev(_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent& _rhs)
	: m_bClone(true)
	, m_pGraphicDev(_rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CComponent::~CComponent()
{
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDev);
}