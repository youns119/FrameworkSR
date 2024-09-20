#include "pch.h"
#include "ToolCamera.h"
#include "Export_System.h"
#include "Engine_Define.h"
CToolCamera::CToolCamera(LPDIRECT3DDEVICE9 pGraphicDev)
    : CCamera(pGraphicDev), m_bFix(false), m_bCheck(false)
{
}

CToolCamera::~CToolCamera()
{
}

HRESULT CToolCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& _fFov, const _float& _fAspect, const _float& _fNear, const _float& _fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = _fFov;
	m_fAspect = _fAspect;
	m_fNear = _fNear;
	m_fFar = _fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

_int CToolCamera::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CCamera::Update_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);

	return iExit;
}

void CToolCamera::LateUpdate_GameObject()
{
	if (false == m_bFix)
	{
		Mouse_Fix();
		Mouse_Move();
	}



	CCamera::LateUpdate_GameObject();
}

void CToolCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 5.f;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 5.f;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}


	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 5.f;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 5.f;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}

	else
		m_bCheck = false;

	if (false == m_bFix)
		return;

}

void CToolCamera::Mouse_Move()
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_long	dwMouseMove(0);

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::MOUSEMOVESTATE::DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::MOUSEMOVESTATE::DIMS_X))
	{
		_vec3	vUp{ 0.f, 1.f, 0.f };

		_vec3	vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
}

void CToolCamera::Mouse_Fix()
{

}

CToolCamera* CToolCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& _fFov, const _float& _fAspect, const _float& _fNear, const _float& _fFar)
{
	CToolCamera* pCamera = new CToolCamera(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, _fFov, _fAspect, _fNear, _fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Dynamic Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CToolCamera::Free()
{
}
