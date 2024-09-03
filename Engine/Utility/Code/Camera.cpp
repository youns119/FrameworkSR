#include "..\..\Header\Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
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

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_GameObject()
{
	//D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	D3DXMatrixLookAtLH_(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	//D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	D3DXMatrixPerspectiveFovLH_(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CCamera::Update_GameObject(const _float& _fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void CCamera::LateUpdate_GameObject()
{
}

void CCamera::Free()
{
	CGameObject::Free();
}

void CCamera::D3DXMatrixLookAtLH_
(
	D3DXMATRIX* _pOut, 
	const D3DXVECTOR3* _pEye, 
	const D3DXVECTOR3* _pAt, 
	const D3DXVECTOR3* _pUp
)
{
	_matrix matCamTrans, matCamRot;

	D3DXMatrixIdentity(&matCamTrans);
	D3DXMatrixIdentity(&matCamRot);

	matCamTrans._41 = _pEye->x;
	matCamTrans._42 = _pEye->y;
	matCamTrans._43 = _pEye->z;

	D3DXMatrixInverse(&matCamTrans, 0, &matCamTrans);

	_vec3 vLook = (*_pAt - *_pEye);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight(0.f, 1.f, 0.f);
	D3DXVec3Cross(&vRight, &vRight, &vLook);
	_vec3 vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	memcpy(&matCamRot.m[(_uint)INFO::INFO_RIGHT][0], &vRight, sizeof(_vec3));
	memcpy(&matCamRot.m[(_uint)INFO::INFO_UP][0], &vUp, sizeof(_vec3));
	memcpy(&matCamRot.m[(_uint)INFO::INFO_LOOK][0], &vLook, sizeof(_vec3));

	D3DXMatrixInverse(&matCamRot, 0, &matCamRot);

	*_pOut = matCamTrans * matCamRot;
}

void CCamera::D3DXMatrixPerspectiveFovLH_
(
	D3DXMATRIX* _pOut, 
	float _fFov, 
	float _fAspect, 
	float _fZNear, 
	float _fZFar
)
{
	_matrix matPerspective;
	float fYndc(0.f), fXndc(0.f), fZndc(0.f);

	D3DXMatrixIdentity(&matPerspective);

	fYndc = 1 / tanf(_fFov * 0.5f);
	fXndc = fYndc / _fAspect;
	fZndc = _fZFar / (_fZFar - _fZNear);

	matPerspective._11 = fXndc;
	matPerspective._22 = fYndc;
	matPerspective._33 = fZndc;
	matPerspective._34 = 1.f;
	matPerspective._43 = -_fZNear * fZndc;
	matPerspective._44 = 0;

	*_pOut = matPerspective;
}