#include "..\..\Header\GameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_fViewZ(0.f)
	, m_bIsRender(true)
{
	m_mapComponent->clear();

	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject& _rhs)
	: m_pGraphicDev(_rhs.m_pGraphicDev)
	, m_fViewZ(_rhs.m_fViewZ)
	, m_bIsRender(_rhs.m_bIsRender)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

CComponent* CGameObject::Get_Component(COMPONENTID _eID, const _tchar* _pComponentTag)
{
	CComponent* pComponent = Find_Component(_eID, _pComponentTag);
	//NULL_CHECK_RETURN(pComponent, nullptr);
	if (pComponent == nullptr)
		return nullptr;

	return pComponent;
}

HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float& _fTimeDelta)
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->Update_Component(_fTimeDelta);

	return 0;
}

void CGameObject::LateUpdate_GameObject()
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

CComponent* CGameObject::Find_Component(COMPONENTID _eID, const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapComponent[(_uint)_eID].begin(), m_mapComponent[(_uint)_eID].end(), CTag_Finder(_pComponentTag));

	if (iter == m_mapComponent[(_uint)_eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Compute_ViewZ(const _vec3* _pPos)
{
	_matrix	matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3 vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	_vec3 vLength = vCamPos - *_pPos;
	m_fViewZ = D3DXVec3Length(&vLength);
}

void CGameObject::Free()
{
	for (_uint i = 0; i < (_uint)COMPONENTID::ID_END; i++)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}