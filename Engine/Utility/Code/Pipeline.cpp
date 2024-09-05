#include "..\..\Header\Pipeline.h"

CPipeline::CPipeline()
{
}

CPipeline::~CPipeline()
{
}

void CPipeline::MakeTransformMatrix
(
	_matrix* _pOut, 
	const _vec3* _pRight, 
	const _vec3* _pUp, 
	const _vec3* _pLook, 
	const _vec3* _pPos
)
{
	memcpy(&_pOut->m[0][0], _pRight, sizeof(_vec3));
	memcpy(&_pOut->m[1][0], _pUp, sizeof(_vec3));
	memcpy(&_pOut->m[2][0], _pLook, sizeof(_vec3));
	memcpy(&_pOut->m[3][0], _pPos, sizeof(_vec3));
}

void CPipeline::MakeViewSpaceMatrix
(
	_matrix* _pOut, 
	const _vec3* _pEye, 
	const _vec3* _pAt, 
	const _vec3* _pUp
)
{
	D3DXMatrixIdentity(_pOut);

	_vec3 vRight = { 1.f, 0.f, 0.f };
	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vLook = { 0.f, 0.f, 1.f };
	_vec3 vPos = { 0.f, 0.f, 0.f };

	vLook = *_pAt - *_pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXVec3Cross(&vRight, _pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	vPos = *_pEye;

	MakeTransformMatrix(_pOut, &vRight, &vUp, &vLook, &vPos);

	D3DXMatrixInverse(_pOut, NULL, _pOut);
}

void CPipeline::MakeProjMatrix
(
	_matrix* _pOut, 
	const _float& _fFov, 
	const _float& _fAspect, 
	const _float& _fNear, 
	const _float& _fFar
)
{
	D3DXMatrixIdentity(_pOut);

	_pOut->_11 = (1.f / tan(_fFov * 0.5f)) / _fAspect;
	_pOut->_22 = 1.f / tan(_fFov * 0.5f);

	_pOut->_33 = _fFar / (_fFar - _fNear);
	_pOut->_34 = 1.f;

	_pOut->_43 = (-_fNear * _fFar) / (_fFar - _fNear);
	_pOut->_44 = 0.f;
}