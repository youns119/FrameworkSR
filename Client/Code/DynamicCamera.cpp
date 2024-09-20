#include "pch.h"
#include "..\Header\DynamicCamera.h"
#include "Export_System.h"
#include "Export_Utility.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CCamera(_pGraphicDev)
	, m_bFix(false)
	, m_bCheck(false)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

CDynamicCamera* CDynamicCamera::Create
(
	LPDIRECT3DDEVICE9 _pGraphicDev,
	const _vec3* _pEye,
	const _vec3* _pAt,
	const _vec3* _pUp,
	const _float& _fFov,
	const _float& _fAspect,
	const _float& _fNear,
	const _float& _fFar
)
{
	CDynamicCamera* pCamera = new CDynamicCamera(_pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(_pEye, _pAt, _pUp, _fFov, _fAspect, _fNear, _fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("Dynamic Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

HRESULT CDynamicCamera::Ready_GameObject
(
	const _vec3* _pEye,
	const _vec3* _pAt,
	const _vec3* _pUp,
	const _float& _fFov,
	const _float& _fAspect,
	const _float& _fNear,
	const _float& _fFar
)
{
	m_vEye = *_pEye;
	m_vAt = *_pAt;
	m_vUp = *_pUp;

	m_fFov = _fFov;
	m_fAspect = _fAspect;
	m_fNear = _fNear;
	m_fFar = _fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float& _fTimeDelta)
{
	_int iExit = CCamera::Update_GameObject(_fTimeDelta);

	m_vAt = { Test().x - 1.5f,Test().y,Test().z };
	m_vEye = { Test().x - 1.5f,Test().y + 1.f,Test().z - 2.f };
	Key_Input(_fTimeDelta);


	return iExit;
}

void CDynamicCamera::LateUpdate_GameObject()
{
	if (m_bFix == false)
	{
		//Mouse_Fix();
		Mouse_Move();
	}

	CCamera::LateUpdate_GameObject();
}

void CDynamicCamera::Key_Input(const _float& _fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	/*if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * _fTimeDelta * 15.f;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * _fTimeDelta * 15.f;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}


	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * _fTimeDelta * 15.f;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * _fTimeDelta * 15.f;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}*/

	if (Engine::Key_Press(DIK_TAB))
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

	if (m_bFix == false)
		return;
}

void CDynamicCamera::Mouse_Move()
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_long dwMouseMove(0);

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y))
	{
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 vLook = m_vAt - m_vEye;

		_matrix	matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
	{
		_vec3 vUp{ 0.f, 1.f, 0.f };
		_vec3 vLook = m_vAt - m_vEye;

		_matrix	matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
}

void CDynamicCamera::Mouse_Fix()
{
	POINT ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

_vec3 CDynamicCamera::Test()
{
	Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Left_Transform"));
	_vec3 vPlayerPos;
	pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);

	return vPlayerPos;
}

void CDynamicCamera::Free()
{
	CCamera::Free();
}