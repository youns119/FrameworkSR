#include "Transform.h"

CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform& _rhs)
	: CComponent(_rhs)
	, m_vScale(_rhs.m_vScale)
	, m_vAngle(_rhs.m_vAngle)
{
	for (size_t i = 0; i < (_uint)INFO::INFO_END; i++)
		m_vInfo[i] = _rhs.m_vInfo[i];

	m_matWorld = _rhs.m_matWorld;
}

CTransform::~CTransform()
{
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CTransform* pTransform = new CTransform(_pGraphicDev);

	if (FAILED(pTransform->Ready_Transform()))
	{
		Safe_Release(pTransform);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pTransform;
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < (_uint)INFO::INFO_END; i++)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update_Component(const _float& _fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < (_uint)INFO::INFO_POS; i++)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	for (_int i = 0; i < (_uint)INFO::INFO_POS; i++)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	_matrix	matRot[(_uint)ROTATION::ROT_END];

	D3DXMatrixRotationX(&matRot[(_uint)ROTATION::ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[(_uint)ROTATION::ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[(_uint)ROTATION::ROT_Z], m_vAngle.z);

	for (_int i = 0; i < (_uint)INFO::INFO_POS; i++)
		for (_int j = 0; j < (_uint)ROTATION::ROT_END; j++)
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);

	for (_int i = 0; i < (_uint)INFO::INFO_END; i++)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

void CTransform::LateUpdate_Component()
{
}

void CTransform::Chase_Target(const _vec3* _pTargetPos, const _float& _fSpeed)
{
	_vec3 vDir = *_pTargetPos - m_vInfo[(_uint)INFO::INFO_POS];

	m_vInfo[(_uint)INFO::INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * _fSpeed;

	_matrix	matRot = *Compute_LookAtTarget(_pTargetPos);

	_matrix	matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[(_uint)INFO::INFO_POS].x, m_vInfo[(_uint)INFO::INFO_POS].y, m_vInfo[(_uint)INFO::INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix* CTransform::Compute_LookAtTarget(const _vec3* _pTargetPos)
{
	_vec3 vDir = *_pTargetPos - m_vInfo[(_uint)INFO::INFO_POS];

	_vec3 vAxis, vUp;
	_matrix	matRot;

	return D3DXMatrixRotationAxis
	(
		&matRot,
		D3DXVec3Cross(&vAxis, &m_vInfo[(_uint)INFO::INFO_UP], &vDir),
		acos(D3DXVec3Dot(D3DXVec3Normalize(&vUp, &m_vInfo[(_uint)INFO::INFO_UP]), D3DXVec3Normalize(&vDir, &vDir)))
	);
}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}