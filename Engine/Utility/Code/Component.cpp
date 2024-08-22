#include "..\..\Header\Component.h"

CComponent::CComponent()
	: m_bClone(false)
	, m_pGraphicDev(nullptr)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_bClone(false)
	, m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent& rhs)
	: m_bClone(true)
	, m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CComponent::~CComponent()
{
	Free();
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDev);
}