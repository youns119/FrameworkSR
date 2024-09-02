#include "pch.h"
#include "..\Header\SkyBox.h"
#include "Export_Utility.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CGameObject(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pTextureCom(nullptr)
{
}

CSkyBox::~CSkyBox()
{
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CSkyBox* pSkyBox = new CSkyBox(_pGraphicDev);

	if (FAILED(pSkyBox->Ready_GameObject()))
	{
		Safe_Release(pSkyBox);
		MSG_BOX("pSkyBox Create Failed");
		return nullptr;
	}

	return pSkyBox;
}

HRESULT CSkyBox::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(40.f, 40.f, 40.f);

	return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& _fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_PRIORITY, this);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CSkyBox::LateUpdate_GameObject()
{
	Engine::CGameObject::LateUpdate_GameObject();

	_matrix	matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 + 3.f, matCamWorld._43);
}

void CSkyBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(3);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSkyBox::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SkyBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CSkyBox::Free()
{
	Engine::CGameObject::Free();
}