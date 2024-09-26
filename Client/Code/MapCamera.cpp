#include "pch.h"
#include "../Header/MapCamera.h"
#include "Export_System.h"
#include "Export_Utility.h"
CMapCamera::CMapCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev), m_bFix(false), m_bCheck(false)
{
}

CMapCamera::~CMapCamera()
{
}

HRESULT CMapCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& _fFov, const _float& _fAspect, const _float& _fNear, const _float& _fFar)
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

_int CMapCamera::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CCamera::Update_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);

	return iExit;
}

void CMapCamera::LateUpdate_GameObject()
{
	if (false == m_bFix)
	{
		Mouse_Fix();
		Mouse_Move();
	}



	CCamera::LateUpdate_GameObject();
}

void CMapCamera::Key_Input(const _float& _fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_float fTimeDelta = 0.05f;

	if (Engine::Key_Hold(DIK_W))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 5.f;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Key_Hold(DIK_S))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 5.f;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}


	if (Engine::Key_Hold(DIK_D))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 5.f;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Key_Hold(DIK_A))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 5.f;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::Key_Hold(DIK_TAB))
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

void CMapCamera::Mouse_Move()
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_long	dwMouseMove(0);

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
	{
		_vec3	vUp{ 0.f, 1.f, 0.f };

		_vec3	vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
}

void CMapCamera::Mouse_Fix()
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

CMapCamera* CMapCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev
	, const _vec3* pEye
	, const _vec3* pAt
	, const _vec3* pUp
	, const _float& _fFov
	, const _float& _fAspect
	, const _float& _fNear
	, const _float& _fFar)
{
	CMapCamera* pCamera = new CMapCamera(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, _fFov, _fAspect, _fNear, _fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Map Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CMapCamera::Free()
{
	CCamera::Free();
}
