#include "pch.h"

#include "../Header/TileContorl.h"

CTileContorl::CTileContorl(LPDIRECT3DDEVICE9 _pGraphicDev)
    : Engine::CGameObject(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pTransformCom(nullptr)
    , m_pTextureCom(nullptr)
{
}

CTileContorl::~CTileContorl()
{
}

CTileContorl* CTileContorl::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CTileContorl* pTileContorl = new CTileContorl(_pGraphicDev);

    if (FAILED(pTileContorl->Ready_GameObject()))
    {
        Safe_Release(pTileContorl);
        MSG_BOX("pTileContorl Create Failed");
        return nullptr;
    }

    return pTileContorl;
}

HRESULT CTileContorl::Ready_GameObject()
{
    return E_NOTIMPL;
}

_int CTileContorl::Update_GameObject(const _float& _fTimeDelta)
{
    return _int();
}

void CTileContorl::LateUpdate_GameObject()
{
}

void CTileContorl::Render_GameObject()
{
}

HRESULT CTileContorl::Add_Component()
{
    return E_NOTIMPL;
}

HRESULT CTileContorl::Setup_Material()
{
    return E_NOTIMPL;
}

void CTileContorl::Free()
{
}
