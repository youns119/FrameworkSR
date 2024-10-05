#include "..\..\Header\ComponentCamera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Pipeline.h"

CComponentCamera::CComponentCamera(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
	, m_vEye({ 0.f, 0.f, 0.f })
	, m_vAt({ 0.f, 0.f, 0.f })
	, m_vUp({ 0.f, 0.f, 0.f })
	, m_fFov(0.f)
	, m_fAspect(0.f)
	, m_fNear(0.f)
	, m_fFar(0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CComponentCamera::~CComponentCamera()
{
}


HRESULT CComponentCamera::Ready_ComponentCamera()
{
	const _float& _fFov = D3DXToRadian(60.f);
	const _float& _fAspect = (float)WINCX / WINCY;
	const _float& _fNear = 0.1f;
	const _float& _fFar = 1000.f;

	m_fFov = _fFov;
	m_fAspect = _fAspect;
	m_fNear = _fNear;
	m_fFar = _fFar;

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CComponentCamera::Update_Component(const _float& _fTimeDelta)
{
	return 0;
}

void CComponentCamera::LateUpdate_Component()
{
	auto target_transform = static_cast<CTransform*>(m_pOwner->Get_Component(Engine::COMPONENTID::ID_DYNAMIC, L"Com_Body_Transform"));

	_matrix matWorld = *target_transform->Get_WorldMatrix();
	matWorld._42 += 0.5f;

	D3DXMatrixInverse(&m_matView, 0, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

CComponentCamera* CComponentCamera::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CComponentCamera* pComponentCamera = new CComponentCamera(_pGraphicDev);

	if (FAILED(pComponentCamera->Ready_ComponentCamera()))
	{
		Safe_Release(pComponentCamera);
		return nullptr;
	}

	return pComponentCamera;
}

CComponentCamera* CComponentCamera::Clone()
{
	return new CComponentCamera(*this);
}

void CComponentCamera::Free()
{
	CComponent::Free();
}