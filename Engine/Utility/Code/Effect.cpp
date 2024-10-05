#include "Effect.h"
#include "Transform.h"
#include "GameObject.h"

CEffect::CEffect()
	: m_fLifeTime(0.f)
	, m_fElapsed(0.f)
	, m_bIsVisible(false)
	, m_bBillboard(false)
	, m_bRepeatable(FALSE)
	, m_pCallBack(nullptr)
{
}

CEffect::CEffect(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
	, m_fLifeTime(0.f)
	, m_fElapsed(0.f)
	, m_bIsVisible(false)
	, m_bBillboard(false)
	, m_bRepeatable(FALSE)
	, m_pCallBack(nullptr)
{
}

CEffect::CEffect(const CEffect& _rhs)
	: CComponent(_rhs)
	, m_fLifeTime(_rhs.m_fLifeTime)
	, m_fElapsed(_rhs.m_fElapsed)
	, m_bIsVisible(_rhs.m_bIsVisible)
	, m_bBillboard(_rhs.m_bBillboard)
	, m_bRepeatable(FALSE)
	, m_pCallBack(_rhs.m_pCallBack)
{
}

CEffect::~CEffect()
{
}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffect* pEffect = new CEffect(_pGraphicDev);

	if (FAILED(pEffect->Ready_Effect()))
	{
		Safe_Release(pEffect);
		MSG_BOX("pEffect Create Failed");
		return nullptr;
	}

	return pEffect;
}

HRESULT CEffect::Ready_Effect()
{
	m_bRepeatable = FALSE;
	return S_OK;
}

_int CEffect::Update_Component(const _float& _fTimeDelta)
{
	if (!m_bIsVisible)
		return 0;


	m_fElapsed += _fTimeDelta;
	if (m_fLifeTime < m_fElapsed)
	{

		if (m_bRepeatable)
		{
			m_fElapsed = m_fElapsed - m_fLifeTime;
		}
		else
		{
			m_fElapsed = 0.f;
			m_bIsVisible = false;
		}
	}

	return 0;
}

void CEffect::LateUpdate_Component()
{
	if (!m_bIsVisible)
		return;

	if (m_bBillboard)
	{
		if (!GetOwner())
		{
			MSG_BOX("Owner is nullptr");
			return;
		}

		CTransform* pTransform = static_cast<CTransform*>(GetOwner()->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
		if (!pTransform)
		{
			MSG_BOX("pTransform is nullptr");
			return;
		}

		const _vec3* pScale, * pAngle;
		_vec3 vPos;
		pScale = pTransform->Get_Scale();
		pAngle = pTransform->Get_Angle();
		pTransform->Get_Info(INFO::INFO_POS, &vPos);

		_matrix matScale, matRotationZ, matScaleInverse;
		D3DXMatrixScaling(&matScale, pScale->x, pScale->y, pScale->z);
		D3DXMatrixRotationZ(&matRotationZ, pAngle->z);
		D3DXMatrixInverse(&matScaleInverse, 0, &matScale);


		_vec3 vCamPos;
		_matrix matBillboardY, matBillboardX;
		_matrix matView, matCamWorld;
		D3DXMatrixIdentity(&matBillboardY);
		D3DXMatrixIdentity(&matBillboardX);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		matBillboardY._11 = matView._11;
		matBillboardY._13 = matView._13;
		matBillboardY._31 = matView._31;
		matBillboardY._33 = matView._33;

		D3DXMatrixInverse(&matBillboardY, 0, &matBillboardY);

		matBillboardX._22 = matView._22;
		matBillboardX._23 = matView._23;
		matBillboardX._32 = matView._32;
		matBillboardX._33 = matView._33;

		D3DXMatrixInverse(&matBillboardX, 0, &matBillboardX);
		D3DXMatrixInverse(&matCamWorld, 0, &matView);

		memcpy(&vCamPos, matCamWorld.m[3], sizeof(_vec3));

		_matrix matLookAt;
		D3DXMatrixIdentity(&matLookAt);
		_vec3 vUp{ 0.f, 1.f, 0.f };
		D3DXMatrixLookAtLH(&matLookAt, &vPos, &vCamPos, &vUp);
		matLookAt.m[3][0] = 0.f;
		matLookAt.m[3][1] = 0.f;
		matLookAt.m[3][2] = 0.f;


		_matrix	matRot[(_uint)ROTATION::ROT_END];
		for (int i = 0; i < 3; ++i)
			D3DXMatrixIdentity(&matRot[i]);
		//D3DXMatrixRotationX(&matRot[(_uint)ROTATION::ROT_X], pAngle->x);
		//D3DXMatrixRotationY(&matRot[(_uint)ROTATION::ROT_Y], pAngle->y);
		D3DXMatrixRotationZ(&matRot[(_uint)ROTATION::ROT_Z], pAngle->z);
		_matrix matTotalRot, matTotalRotInverse;
		D3DXMatrixIdentity(&matTotalRot);
		matTotalRot = matRot[0] * matRot[1] * matRot[2];

		D3DXMatrixInverse(&matTotalRotInverse, 0, &matTotalRot);


		_matrix matWorld;
		pTransform->Get_WorldMatrix(&matWorld);
		//D3DXMatrixTranslation(&matWorld, vPos.x, vPos.y, vPos.z);

		matWorld = matScale * matTotalRot * matBillboardY * matTotalRotInverse * matScaleInverse * matWorld;

		pTransform->Set_WorldMatrix(&matWorld);
	}


}

CEffect* CEffect::Clone()
{
	return new CEffect(*this);
}

void CEffect::Free()
{
	CComponent::Free();
}

void CEffect::Operate_Effect()
{
	if (m_pCallBack)
		m_pCallBack(m_pParam);

	Set_Visibility(TRUE);
	// m_fElapsed = 0.f;
}

// 에이시 함수이름 왜이리 헷갈려 나도 헷갈려이젠
void CEffect::Stop_Effect()
{
	m_fElapsed = 0.f;
	m_bIsVisible = FALSE;
}

void CEffect::Reset()
{
	m_bIsVisible = FALSE;
}