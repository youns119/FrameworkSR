#include "pch.h"
#include "..\Header\UIUpgrade.h"

CUIUpgrade::CUIUpgrade(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
{
}

CUIUpgrade::~CUIUpgrade()
{
}

CUIUpgrade* CUIUpgrade::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	return nullptr;
}

HRESULT CUIUpgrade::Ready_UI()
{
	return S_OK;
}

_int CUIUpgrade::Update_UI(const _float& _fTimeDelta)
{
	return 0;
}

void CUIUpgrade::LateUpdate_UI()
{
}

void CUIUpgrade::Render_UI()
{
}

HRESULT CUIUpgrade::Add_Component()
{
	return E_NOTIMPL;
}

void CUIUpgrade::Free()
{
}
