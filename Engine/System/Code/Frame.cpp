#include "..\..\Header\Frame.h"

CFrame::CFrame() 
	: m_fCallLimit(0.f)
	, m_fAccTimeDelta(0.f)
{
}

CFrame::~CFrame()
{
}

CFrame* CFrame::Create(const _float& _fCallLimit)
{
	CFrame* pFrame = new CFrame;

	if (FAILED(pFrame->Ready_Frame(_fCallLimit)))
	{
		Safe_Release(pFrame);

		return nullptr;
	}

	return pFrame;
}

HRESULT CFrame::Ready_Frame(const _float& _fCallLimit)
{
	m_fCallLimit = 1.f / _fCallLimit;

	return S_OK;
}

_bool CFrame::IsPermit_Call(const _float& _fTimeDelta)
{
	m_fAccTimeDelta += _fTimeDelta;

	if (m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fAccTimeDelta = 0.f;

		return true;
	}

	return false;
}

void CFrame::Free()
{
}