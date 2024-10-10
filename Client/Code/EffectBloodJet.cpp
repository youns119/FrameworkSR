#include "pch.h"
#include "../Header/EffectBloodJet.h"
#include "Export_Utility.h"

CEffectBloodJet::CEffectBloodJet(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
}

CEffectBloodJet::~CEffectBloodJet()
{
}

HRESULT CEffectBloodJet::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pEffectCom->Set_LifeTime(0.5f);
	//m_pEffectCom->Set_Repeatable(FALSE);
	m_pEffectCom->Set_Repeatable(FALSE);
	m_pEffectCom->Set_CallBack(OnOperate);
	m_pEffectCom->Set_CallBackParam(this);
	m_pEffectCom->Set_Visibility(FALSE);
	m_pEffectCom->Set_Billboard(TRUE);

	// 어짜피 오브젝트 풀의 operate 함수에 의해서 transform 이 변경됨
	//m_pTransformCom->Set_Pos(0.f, 0.f, 2.3f);
	//m_pTransformCom->Set_Scale(100.0f, 1.0f, 1.f);

	m_vDerivedPosition[(_uint)BLOOD::BULLKY] = { 0.05f, 0.50f, 0.f };
	m_vDerivedPosition[(_uint)BLOOD::BULLKY2] = { 0.40f, -0.60f, 0.f };
	m_vDerivedPosition[(_uint)BLOOD::FAN_SPREAD] = { 0.50f, .50f, 0.f };
	m_vDerivedPosition[(_uint)BLOOD::THIN_SPLATTER] = { .30f, 0.80f, 0.f };
	m_vDerivedPosition[(_uint)BLOOD::WHIP_LIKE] = { 0.30f, 0.00f, 0.f };

	//for (int i = 0; i < 5; ++i)
	//	m_vDerivedPosition[i] = _vec3(0.f, 0.f, 0.f);
	//m_vRender.fill({ BLOOD::WHIP_LIKE, _vec3(0.f, 0.f, 0.f) });


	Set_UVGrid();

	return S_OK;
}

_int CEffectBloodJet::Update_GameObject(const _float& _fTimeDelta)
{
	static _float fTimeLapsed = 0;
	if (!m_pEffectCom->Get_Visibility())
		return 0;

	//_vec3 vPos, vLook;
	//CComponent* pComponent(nullptr);
	//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform");
	//static_cast<CTransform*>(pComponent)->Get_Info(INFO::INFO_POS, &vPos);
	//static_cast<CTransform*>(pComponent)->Get_Info(INFO::INFO_LOOK, &vLook);
	//m_pTransformCom->Set_Pos(vPos + vLook * 5.f + _vec3(0.f, 0.5f, 0.f));

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectBloodJet::LateUpdate_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	_vec3 vPos;
	// render transform 설정하기
	for (_uint i = 0; i < m_vRenderTransform.size(); ++i)
	{
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		m_vRenderTransform[i]->Set_Pos(m_vRender[i].second);

		// 좌우 반전
		if (m_vRender[i].second.x < 0)
			m_vRenderTransform[i]->Set_Scale(-1.f, 1.f, 1.f);
		//m_vRenderTransform[i]->Set_Scale(-0.3f, 0.3f, 1.f);
		else
			m_vRenderTransform[i]->Set_Scale(1.f, 1.f, 1.f);
		//m_vRenderTransform[i]->Set_Scale(0.3f, 0.3f, 1.f);

	}

	// biilboard
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
	m_fViewZ -= 0.5f;

	Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectBloodJet::Render_GameObject()
{

	if (m_pEffectCom->Get_Visibility())
	{
		DWORD dwPreCOLOROP, dwPreCONSTANT, dwPreCOLOCARG1, dwPreCOLORARG2;
		m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwPreCOLOROP);
		m_pGraphicDev->GetTextureStageState(0, D3DTSS_CONSTANT, &dwPreCONSTANT);
		m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwPreCOLOCARG1);
		m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG2, &dwPreCOLORARG2);

		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_ARGB(255, 255, 0, 0));
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CONSTANT);

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		_float fPersentage = m_pEffectCom->Get_ElapsedPersentage();
		_matrix matWorldSrc, matWorldDst;
		for (_uint i = 0; i < m_vRender.size(); ++i)
		{
			BLOOD eBlood = m_vRender[i].first;
			_uint iIdx = (_uint)eBlood;

			matWorldSrc = *(m_vRenderTransform[i]->Get_WorldMatrix());
			matWorldDst = *(m_pTransformCom->Get_WorldMatrix());
			_matrix matWorld = matWorldSrc * matWorldDst;
			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			int iCurFrame = m_iTotalFrame[iIdx] * fPersentage;
			//m_pBufferCom->Set_UV(_vec2(1.f, 1.f), 0);
			m_pBufferCom->Set_UV(m_vGrid[iIdx], iCurFrame);
			m_pTextureCom->Set_Texture(iIdx);
			m_pBufferCom->Render_Buffer();
		}


		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, dwPreCOLOROP);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_CONSTANT, dwPreCONSTANT);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, dwPreCOLOCARG1);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, dwPreCOLORARG2);
	}
}

CEffectBloodJet* CEffectBloodJet::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffectBloodJet* pEffect = new CEffectBloodJet(_pGraphicDev);

	if (FAILED(pEffect->Ready_GameObject()))
	{
		Safe_Release(pEffect);
		MSG_BOX("Execution Effect Create Failed");
		return nullptr;
	}

	return pEffect;
}

void CEffectBloodJet::Free()
{
	Engine::CGameObject::Free();
}

HRESULT CEffectBloodJet::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_vRenderTransform[0] = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_TransformFirst", pComponent });

	pComponent = m_vRenderTransform[1] = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_TransformSecond", pComponent });

	pComponent = m_vRenderTransform[2] = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_TransformThird", pComponent });

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExecutionBlood"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pEffectCom = static_cast<CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });
}

void CEffectBloodJet::Set_UVGrid()
{
	m_vGrid[(_uint)BLOOD::BULLKY] = _vec2(3.f, 3.f);
	m_vGrid[(_uint)BLOOD::BULLKY2] = _vec2(3.f, 3.f);
	m_vGrid[(_uint)BLOOD::FAN_SPREAD] = _vec2(4.f, 3.f);
	m_vGrid[(_uint)BLOOD::THIN_SPLATTER] = _vec2(9.f, 2.f);
	m_vGrid[(_uint)BLOOD::WHIP_LIKE] = _vec2(4.f, 2.f);

	for (_uint i = 0; i < (_uint)BLOOD::BLOOD_END; ++i)
	{
		m_iTotalFrame[i] = m_vGrid[i].x * m_vGrid[i].y;
	}
}

void CEffectBloodJet::OnOperate(void* _pParam)
{
	// position 은 오브젝트 풀에서 줄거임
	CEffectBloodJet* pThis = (CEffectBloodJet*)_pParam;

	for (int i = 0; i < pThis->m_vRender.size(); ++i)
	{
		_uint iRandom = rand() % 1000 * 0.001 * 5;
		pThis->m_vRender[i].second = pThis->m_vDerivedPosition[iRandom];
		pThis->m_vRender[i].second.x += rand() % 1000 * 0.001 * 0.1f;
		pThis->m_vRender[i].second.y += rand() % 1000 * 0.001 * 0.1f;

		if (rand() % 2)
			pThis->m_vRender[i].second.x *= -1.f;

		pThis->m_vRender[i].first = (BLOOD)iRandom;
	}
}

