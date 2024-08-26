#include "FrameManager.h"

IMPLEMENT_SINGLETON(CFrameManager)

CFrameManager::CFrameManager()
{
	m_mapFrame.clear();
}

CFrameManager::~CFrameManager()
{
	Free();
}

HRESULT CFrameManager::Ready_Frame(const _tchar* _pFrameTag, const _float& _fCallLimit)
{
	CFrame* pFrame = Find_Frame(_pFrameTag);

	if (pFrame != nullptr)
		return E_FAIL;

	pFrame = CFrame::Create(_fCallLimit);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_mapFrame.insert({ _pFrameTag, pFrame });

	return S_OK;
}

_bool CFrameManager::IsPermit_Call(const _tchar* _pFrameTag, const _float& _fTimeDelta)
{
	CFrame* pFrame = Find_Frame(_pFrameTag);
	NULL_CHECK_RETURN(_pFrameTag, false);

	return pFrame->IsPermit_Call(_fTimeDelta);
}

CFrame* CFrameManager::Find_Frame(const _tchar* _pFrameTag)
{
	auto iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CTag_Finder(_pFrameTag));

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void CFrameManager::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}