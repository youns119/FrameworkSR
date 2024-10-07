#include "..\..\Header\Collider.h"
#include "Export_Utility.h"

UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
	: m_pTransform(nullptr)
	, m_vOffsetPos(0.f, 0.f, 0.f)
	, m_vFinalPos(0.f, 0.f, 0.f)
	, m_vLookDir(0.f, 0.f, 0.f)
	, m_bActive(false)
	, m_bShow(false)
	, m_fRadius(0.f)
	, m_iCollided(0)
	, m_iID(g_iNextID++)
	, m_pSphere(nullptr)
{
}

CCollider::CCollider(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
	, m_pTransform(nullptr)
	, m_vOffsetPos(0.f, 0.f, 0.f)
	, m_vFinalPos(0.f, 0.f, 0.f)
	, m_vLookDir(0.f, 0.f, 0.f)
	, m_bActive(false)
	, m_bShow(false)
	, m_fRadius(0.f)
	, m_iCollided(0)
	, m_iID(g_iNextID++)
	, m_pSphere(nullptr)
{
}

CCollider::CCollider(const CCollider& _rhs)
	: CComponent(_rhs)
	, m_pTransform(_rhs.m_pTransform)
	, m_vOffsetPos(_rhs.m_vOffsetPos)
	, m_vFinalPos(_rhs.m_vFinalPos)
	, m_vLookDir(_rhs.m_vLookDir)
	, m_bActive(_rhs.m_bActive)
	, m_bShow(_rhs.m_bShow)
	, m_fRadius(_rhs.m_fRadius)
	, m_iCollided(_rhs.m_iCollided)
	, m_iID(g_iNextID++)
	, m_pSphere(_rhs.m_pSphere)
{
}

CCollider::~CCollider()
{
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CCollider* pCollider = new CCollider(_pGraphicDev);

	if (FAILED(pCollider->Ready_Collider()))
	{
		Safe_Release(pCollider);
		return nullptr;
	}

	return pCollider;
}

HRESULT CCollider::Ready_Collider()
{
	return S_OK;
}

void CCollider::LateUpdate_Component()
{
	//if (!m_bActive)
	//	return;

	_vec3 vObjectPos;
	m_pTransform->Get_Info(INFO::INFO_POS, &vObjectPos);

	m_vFinalPos = vObjectPos + m_vOffsetPos;

	m_tAABB.vMin =
	{
		m_vFinalPos.x - m_fRadius + m_vLookDir.x,
		m_vFinalPos.y - m_fRadius + m_vLookDir.y,
		m_vFinalPos.z - m_fRadius + m_vLookDir.z
	};
	m_tAABB.vMax =
	{
		m_vFinalPos.x + m_fRadius - m_vLookDir.x,
		m_vFinalPos.y + m_fRadius - m_vLookDir.y,
		m_vFinalPos.z + m_fRadius - m_vLookDir.z
	};
}

void CCollider::Render_Collider()
{
	if (m_bShow)
	{
		D3DMATERIAL9 tMaterial;
		ZeroMemory(&tMaterial, sizeof(D3DMATERIAL9));

		if (m_iCollided)
			tMaterial.Emissive = { 1.f, 0.f, 0.f, 1.f };
		else
			tMaterial.Emissive = { 0.f, 1.f, 0.f, 1.f };

		m_pGraphicDev->SetMaterial(&tMaterial);

		_matrix matWorld;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixTranslation(&matWorld, m_vFinalPos.x, m_vFinalPos.y, m_vFinalPos.z);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		D3DXCreateSphere(m_pGraphicDev, m_fRadius, 20, 20, &m_pSphere, NULL);

		m_pSphere->DrawSubset(0);
		m_pSphere->Release();
	}
}

void CCollider::OnCollision(CCollider& _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider& _pOther)
{
	m_iCollided++;
	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionExit(CCollider& _pOther)
{
	m_iCollided--;
	m_pOwner->OnCollisionExit(_pOther);
}

CComponent* CCollider::Clone()
{
	return new CCollider(*this);
}

void CCollider::Free()
{
	CComponent::Free();
}