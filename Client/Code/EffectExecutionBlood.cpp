#include "pch.h"
#include "../Header/EffectExecutionBlood.h"
#include "Export_Utility.h"

CEffectExecutionBlood::CEffectExecutionBlood(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
}

CEffectExecutionBlood::~CEffectExecutionBlood()
{
}

HRESULT CEffectExecutionBlood::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fViewZ = 0.1f;

	// first
	m_pEffectComFirst->Set_LifeTime(2.5f);
	m_pEffectComFirst->Set_Repeatable(FALSE);
	m_pEffectComFirst->Set_Visibility(FALSE);
	m_pEffectComFirst->Set_CallBack(OnOperateFirst);
	m_pEffectComFirst->Set_CallBackParam(this);
	m_pTransformComFirst->Set_Pos(0.f, 0.f, m_fViewZ);
	Set_ParticleParam();

	// second
	m_pEffectComSecond->Set_LifeTime(1.0f);
	m_pEffectComSecond->Set_Repeatable(FALSE);
	m_pEffectComSecond->Set_CallBack(OnOperateSecond);
	m_pEffectComSecond->Set_CallBackParam(this);
	Set_UVGrid();

	// 형님 여기입니다 ! 이 transform second 를 조종하시면 됩니다. 
	// 아니면 다른곳에서 조종하셔도 상관은ㄴ업습ㄴ디ㅏ.
	m_pTransformComSecond->Set_Pos(0.f, 0.f, m_fViewZ);

	m_pTransformComArray[(_uint)BLOOD::BULLKY]->Set_Pos(50.f, 80.f, m_fViewZ);
	m_pTransformComArray[(_uint)BLOOD::BULLKY2]->Set_Pos(100.f, -80.f, m_fViewZ);
	m_pTransformComArray[(_uint)BLOOD::FAN_SPREAD]->Set_Pos(150.f, 80.f, m_fViewZ);


	m_fEmissionInterval = 0.05f;
	return S_OK;
}

_int CEffectExecutionBlood::Update_GameObject(const _float& _fTimeDelta)
{
	static _float fTimeLapsed = 0;
	if (!m_pEffectComFirst->Get_Visibility() && !m_pEffectComSecond->Get_Visibility())
		return 0;

	Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

	fTimeLapsed += _fTimeDelta;
	if (fTimeLapsed >= m_fEmissionInterval)
	{
		fTimeLapsed = 0.f;

		if (m_iCurEmmisionIdx < m_EmissionBuffer.size())
		{
			++m_iCurEmmisionIdx;
		}
	}


	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectExecutionBlood::LateUpdate_GameObject()
{
	Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectExecutionBlood::Render_GameObject()
{
	if (m_pEffectComFirst->Get_Visibility())
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformComFirst->Get_WorldMatrix());

		m_pTextureComFirst->Set_Texture();
		m_pParticleSystem->Render_Parcitle();
	}

	if (m_pEffectComSecond->Get_Visibility())
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

		if (true)
		{

			for (_int i = (_int)m_iCurEmmisionIdx - 1; i >= 0; --i)
			{
				BLOOD eBlood = m_EmissionBuffer[i].first;
				_uint iIdx = (_uint)eBlood;
				_vec3 vPos = m_EmissionBuffer[i].second;

				_matrix matWorld = *(m_pTransformComArray[iIdx]->Get_WorldMatrix());
				matWorld.m[3][0] += vPos.x;
				matWorld.m[3][1] += vPos.y;
				matWorld.m[3][2] += vPos.z;
				matWorld = matWorld * *(m_pTransformComSecond->Get_WorldMatrix());

				m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
				m_pTextureComSecond->Set_Texture(iIdx);

				// animate UV;

				_float fPersentage = m_pEffectComSecond->Get_ElapsedPersentage();
				_float fStartPer = i * m_fEmissionInterval / m_pEffectComSecond->Get_LifeTime();
				_float fEndPer = (i * m_fEmissionInterval + 0.5f) / m_pEffectComSecond->Get_LifeTime();

				if (fStartPer > fEndPer)
					continue;

				if (fPersentage > fEndPer)
					continue;

				fPersentage = (fPersentage - fStartPer) / (fEndPer - fStartPer);

				int iCurFrame = m_iTotalFrame[iIdx] * fPersentage;
				m_pBufferCom->Set_UV(m_vGrid[iIdx], iCurFrame);

				m_pBufferCom->Render_Buffer();
			}

		}
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, dwPreCOLOROP);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_CONSTANT, dwPreCONSTANT);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, dwPreCOLOCARG1);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, dwPreCOLORARG2);
	}
}

CEffectExecutionBlood* CEffectExecutionBlood::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffectExecutionBlood* pEffect = new CEffectExecutionBlood(_pGraphicDev);

	if (FAILED(pEffect->Ready_GameObject()))
	{
		Safe_Release(pEffect);
		MSG_BOX("Execution Effect Create Failed");
		return nullptr;
	}

	return pEffect;
}

void CEffectExecutionBlood::Free()
{
	Engine::CGameObject::Free();
}

HRESULT CEffectExecutionBlood::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	// first
	pComponent = m_pTextureComFirst = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BloodParticle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_TextureFirst", pComponent });

	pComponent = m_pTransformComFirst = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_TransformFirst", pComponent });

	pComponent = m_pParticleSystem = static_cast<CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem", pComponent });

	pComponent = m_pEffectComFirst = static_cast<CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_EffectFirst", pComponent });


	// second
	pComponent = m_pTransformComSecond = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_TransformSecond", pComponent });

	pComponent = m_pTextureComSecond = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExecutionBlood"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_TextureSecond", pComponent });

	pComponent = m_pEffectComSecond = static_cast<CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_EffectSecond", pComponent });

	void* tempArray[(_uint)BLOOD::BLOOD_END];
	void* tempArray2[(_uint)BLOOD::BLOOD_END];
	for (_uint i = 0; i < (_uint)BLOOD::BLOOD_END; ++i)
	{
		pComponent = m_pTransformComArray[i] = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(*this);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ (_tchar*)&tempArray[i], pComponent });
		m_pTransformComArray[i]->Set_Scale(200.f, 200.f, 1.f);
		//m_pTransformComArray[i]->Set_Pos(100.f, 100.f, 1.f);

		//pComponent = m_pBufferCom[i] = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
		//NULL_CHECK_RETURN(pComponent, E_FAIL);
		//pComponent->SetOwner(*this);
		////m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });
		//m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ (_tchar*)&tempArray2, pComponent });
	}

	pComponent = m_pBufferCom = static_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });
}

void CEffectExecutionBlood::Set_ParticleParam()
{
	CParticleSystem::PARAM tParticleParam;
	ZeroMemory(&tParticleParam, sizeof(CParticleSystem::PARAM));
	//tParticleParam.tStartBoundary.vMin = { -WINCX * 0.5f, -WINCY * 0.45f, 0.f };
	//tParticleParam.tStartBoundary.vMax = { WINCX * 0.5f, -WINCY * 0.1f, 0.f };

	tParticleParam.tInit.tHexahedron.vInitVelocity = { 1500.f, 750.f, 0.f };
	tParticleParam.vVelocityNoise = { 500.f, 400.f, 0.f };
	tParticleParam.vColor = _vec4(1.0f, 0.f, 0.f, 1.f);
	tParticleParam.vColorFade = _vec4(1.0f, 0.0f, 0.0f, 0.0f); // 255 대신 500 넣어서 투명해지기 전에 더 빨리 초록색이 되도록
	tParticleParam.iTotalCnt = 40;

	tParticleParam.fSize = 200.f;
	tParticleParam.fLifeTime = 0.5f;

	tParticleParam.fEmitRate = 20.f;
	tParticleParam.iEmitCnt = 10;

	tParticleParam.fGravity = 5000.f;

	m_pParticleSystem->Set_Parameter(tParticleParam);
	//m_pParticleSystem->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::POINT_SCALE_DISABLE, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::GRAVITY, TRUE);

	m_pParticleSystem->SetUp_Particle();
}

void CEffectExecutionBlood::Set_UVGrid()
{
	m_vGrid[(_uint)BLOOD::BULLKY] = _vec2(3.f, 3.f);
	m_vGrid[(_uint)BLOOD::BULLKY2] = _vec2(3.f, 3.f);
	m_vGrid[(_uint)BLOOD::FAN_SPREAD] = _vec2(4.f, 3.f);
	//m_vGrid[(_uint)BLOOD::THIN_SPLATTER] = _vec2(9.f, 2.f);
	//m_vGrid[(_uint)BLOOD::WHIP_LIKE] = _vec2(4.f, 2.f);

	for (_uint i = 0; i < (_uint)BLOOD::BLOOD_END; ++i)
	{
		m_iTotalFrame[i] = m_vGrid[i].x * m_vGrid[i].y;
	}
}

void CEffectExecutionBlood::OnOperateFirst(void* _pParam)
{
	CEffectExecutionBlood* pThis = (CEffectExecutionBlood*)_pParam;

	if (pThis->m_pEffectComFirst->Get_Visibility())
		return;

	pThis->m_pParticleSystem->Reset();
	pThis->m_pEffectComFirst->Stop_Effect();
}

void CEffectExecutionBlood::OnOperateSecond(void* _pParam)
{
	CEffectExecutionBlood* pThis = (CEffectExecutionBlood*)_pParam;

	if (pThis->m_pEffectComSecond->Get_Visibility())
		return;
	//for (_uint i = 0; i < (_uint)BLOOD::BLOOD_END; ++i)
	//{
	//	_vec3 vRandPos;
	//	vRandPos.x = rand() % 1000 * 0.001 * 100.f;
	//	vRandPos.y = rand() % 1000 * 0.001 * 100.f;
	//	vRandPos.z = pThis->m_fViewZ;
	//	pThis->m_pTransformComArray[i]->Set_Pos(vRandPos);
	//}

	std::pair<BLOOD, _vec3> tToRender;
	for (_uint i = 0; i < pThis->m_EmissionBuffer.size(); ++i)
	{
		_uint iIdx = (rand() % (_uint)BLOOD::BLOOD_END);
		tToRender.first = (BLOOD)iIdx;
		pThis->m_pTransformComArray[iIdx]->Get_Info(INFO::INFO_POS, &tToRender.second);
		tToRender.second.x = rand() % 1000 * 0.001 * 75.f;
		tToRender.second.y = rand() % 1000 * 0.001 * 75.f;

		pThis->m_EmissionBuffer[i] = tToRender;
	}

	pThis->m_iCurEmmisionIdx = 0;

	pThis->m_pEffectComSecond->Stop_Effect();
}
