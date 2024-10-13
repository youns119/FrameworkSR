#include "pch.h"
#include "../Header/Door.h"
#include "../Header/Player.h"
#include "../Header/UIScreen.h"
#include "../Header/UIVictory.h"
#include "../Header/UIFloor.h"
#include "Export_System.h"
#include "Export_Utility.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CTileContorl(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pColliderCom(nullptr)
    , m_vecWallDirection({ 0.f, 0.f, 0.f })
    , m_fMovingSpeed(0.f)
    , m_bIsOpen(false)
    , m_bEleOpen(false)
{
    m_iNumber = 0;
    m_iNumber_Type = 4;
}

CDoor::~CDoor()
{
}

CDoor* CDoor::Create_InfoSave(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, _vec3 _vecScale, const _int& _iNumber, Engine::TILE_DIRECTION _eTileDirection, const _int& _iTrigger)
{
    CDoor* pDoor = new CDoor(_pGraphicDev);


    if (FAILED(pDoor->Ready_GameObject()))
    {
        Safe_Release(pDoor);
        MSG_BOX("Door Create Failed");
        return nullptr;
    }

    if (_iNumber == 5 || _iNumber == 6 || _iNumber == 7)
    {
        pDoor->m_bFirstDoor = true;
    }
    if (_iNumber == 21 || _iNumber == 22 || _iNumber == 23)
    {
        pDoor->m_bLastDoor = true;
    }

    pDoor->Setup_Position(_vecPos);
    pDoor->m_vecPos = _vecPos;

    pDoor->Setup_Angle(_vecRot);
    pDoor->m_vecRot = _vecRot;

    pDoor->Setup_Scale(_vecScale);
    pDoor->m_vecScale = _vecScale;

    pDoor->Set_Number(_iNumber);
    pDoor->m_iNumber = _iNumber;

    pDoor->Set_TileDirection(_eTileDirection);
    pDoor->Set_Trigger(_iTrigger);

    return pDoor;
}

CDoor* CDoor::Create_Infoload(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, _vec3 _vecScale, const _int& _iNumber, _vec3 _vecTileDirection, const _int& _iTrigger)
{
    CDoor* pDoor = new CDoor(_pGraphicDev);


    if (FAILED(pDoor->Ready_GameObject()))
    {
        Safe_Release(pDoor);
        MSG_BOX("Door Create Failed");
        return nullptr;
    }
    if (_iNumber == 5 || _iNumber == 6 || _iNumber == 7)
    {
        pDoor->m_bFirstDoor = true;
    }
    if (_iNumber == 21 || _iNumber == 22 || _iNumber == 23)
    {
        pDoor->m_bLastDoor = true;
    }

    pDoor->Setup_Position(_vecPos);
    pDoor->m_vecPos = _vecPos;

    pDoor->Setup_Angle(_vecRot);
    pDoor->m_vecRot = _vecRot;

    pDoor->Setup_Scale(_vecScale);
    pDoor->m_vecScale = _vecScale;

    pDoor->Set_Number(_iNumber);
    pDoor->m_iNumber = _iNumber;

    pDoor->Set_TileDirection(_vecTileDirection);
    pDoor->Set_Trigger(_iTrigger);

    return pDoor;
}


void CDoor::Set_TileDirection(const _vec3& _vecDir)
{
    m_vecWallDirection = _vecDir;

    m_pColliderCom->SetShow(true);
}

HRESULT CDoor::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetTransform(m_pTransformCom);
    m_pColliderCom->SetRadius(1.5f);
    m_pColliderCom->SetShow(true);
    m_pColliderCom->SetActive(true);

    return S_OK;
}

_int CDoor::Update_GameObject(const _float& _fTimeDelta)
{
    if (!m_bIsRender)
        return 0;

    if (m_bIsOpen)
    {
        if (m_fMovingSpeed >= 21 || m_fMovingSpeed <= -21)
        {
            m_bIsOpen = false;
        }
        else
        {
            if (m_iNumber == 1 || m_iNumber == 3 || m_iNumber == 5 || m_iNumber == 21)
            {
                m_fMovingSpeed -= 3.f;
                m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(m_fMovingSpeed));

            }
            if (m_iNumber == 2 || m_iNumber == 4 || m_iNumber == 6 || m_iNumber == 22)
            {
                m_fMovingSpeed += 3.f;
                m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(m_fMovingSpeed));

            }
        }

    }

    if (m_bEleOpen)
    {
        m_iNumber += 1;

        if (m_iNumber == 20)
        {
            m_bEleOpen = false;
        }
    }
    //if (m_bIsOpen)
    //{
    //    if (m_fMovingSpeed > 0.6f)
    //    {
    //        m_bIsOpen = false;
    //    }
    //    else
    //    {
    //        _vec3 vPos, vLook, vUp, vDir;
    //        m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
    //        m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(-5.5f));
    //        m_fMovingSpeed += _fTimeDelta;
    //    }


    //    //vPos.x -= _fTimeDelta * 2.f;
    //    //m_pTransformCom->Set_Pos(vPos);
    //}

    Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

    Engine::Add_Collider(m_pColliderCom);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CDoor::LateUpdate_GameObject()
{
    if (!m_bIsRender)
        return;

    Engine::CGameObject::LateUpdate_GameObject();
}

void CDoor::Render_GameObject()
{
    if (!m_bIsRender)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    FAILED_CHECK_RETURN(Setup_Material(), );

    if (m_iNumber <= 20 && m_iNumber > 7)
    {
        m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pTextureCom->Set_Texture(m_iNumber - 1);
        //m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    }
    else
    {
        m_pTextureCom->Set_Texture(m_iNumber - 1);

    }
    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDoor::Set_TileDirection(Engine::TILE_DIRECTION _eTileDirection)
{
    switch (_eTileDirection)
    {
    case Engine::TILE_DIRECTION::TILE_FORWARD:
        m_vecWallDirection = { 0.f, 0.f, 1.f };
        break;
    case Engine::TILE_DIRECTION::TILE_RIGHT:
        m_vecWallDirection = { 1.f, 0.f, 0.f };
        break;
    case Engine::TILE_DIRECTION::TILE_LEFT:
        m_vecWallDirection = { -1.f, 0.f, 0.f };
        break;
    case Engine::TILE_DIRECTION::TILE_BACK:
        m_vecWallDirection = { 0.f, 0.f, -1.f };
        break;
    }
}

void CDoor::Set_IsRender(const _bool& _bool)
{
    m_bIsRender = _bool;
    m_pColliderCom->SetActive(_bool);
    m_pColliderCom->SetShow(_bool);
}

HRESULT CDoor::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CWallTBTex*>(Engine::Clone_Proto(L"Proto_WallTBTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Door"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
    pComponent->SetOwner(*this);

    return S_OK;
}

HRESULT CDoor::Setup_Material()
{
    D3DMATERIAL9 tMtrl;
    ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

    tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };
    tMtrl.Specular = { 1.f, 1.f, 1.f, 1.f };
    tMtrl.Ambient = { 1.f, 1.f, 1.f, 1.f };

    tMtrl.Emissive = { 0.f, 0.f, 0.f, 0.f };
    tMtrl.Power = 0.f;

    m_pGraphicDev->SetMaterial(&tMtrl);

    return S_OK;
}

void CDoor::Setup_Position(_vec3 _vecPos)
{
    m_pTransformCom->Set_Pos(_vecPos.x, _vecPos.y, _vecPos.z);
}

void CDoor::Setup_Angle(_vec3 _vecRot)
{
    m_pTransformCom->Set_Angle(_vecRot.x, _vecRot.y, _vecRot.z);
}

void CDoor::Setup_Scale(_vec3 _vecScale)
{
    m_pTransformCom->Set_Scale(_vecScale.x, _vecScale.y, _vecScale.z);
}

void CDoor::Moving_Open()
{
    if (!m_bIsOpen)
    {
        m_bIsOpen = true;
        Engine::Play_Sound(L"Door_Open.wav", CHANNELID::SOUND_EFFECT, 0.85f);
    }
    if (m_iNumber == 7 || m_iNumber == 23)
    {
        m_iNumber = 7;
        m_bEleOpen = true;
    }
}

void CDoor::OnCollisionEnter(CCollider& _pOther)
{
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther.GetOwner());
    if (pPlayer != nullptr)
    {
        Engine::Collision_With_Trigger(m_iTriggerNumber);
        Engine::Calculate_Trigger();
        Moving_Open();

        if (m_bLastDoor)
        {
            if (pPlayer->Get_BossStage())
            {
                //범승이가 보스를 소환하는 함수추가요망
                //Engine::Get_CurrScene()->Get_GameObject(L"Layer_Monster", L"Boss_Humanoid"); 
                //Engine::Get_CurrScene()->Get_GameObject(L"Layer_Monster", L"Boss_Robot"); 
            }
            else if (Engine::Get_ListUI(UITYPE::UI_SCREEN)->empty())
            {
                Engine::Activate_UI(UITYPE::UI_VICTORY);
                Engine::Activate_UI(UITYPE::UI_SCREEN);
                static_cast<CUIVictory*>(Engine::Get_ListUI(UITYPE::UI_VICTORY)->front())->Set_FloorTime(Engine::Get_Elapsed());
                static_cast<CUIScreen*>(Engine::Get_ListUI(UITYPE::UI_SCREEN)->front())->
                    Set_Floor(static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Get_CurrFloor());
                m_bLastDoor = false;
                pPlayer->Set_Clear();
                Engine::Stop_All();
                Engine::Play_Sound(L"Stage_Clear.wav", CHANNELID::SOUND_EFFECT, 0.7f);
                Engine::Play_BGM(L"Shop_BGM.wav", 0.8f);
            }
        }
        else if (m_bFirstDoor)
        {
            if (Engine::Get_ListUI(UITYPE::UI_SCREEN)->empty())
            {

            }
            else static_cast<CUIScreen*>(Engine::Get_ListUI(UITYPE::UI_SCREEN)->front())->Set_Return(true);

            if (Engine::Get_ListUI(UITYPE::UI_FLOOR)->empty())
            {
                if (!static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Get_BossStage())
                {
                    Engine::Activate_UI(UITYPE::UI_FLOOR);
                    static_cast<CUIFloor*>(Engine::Get_ListUI(UITYPE::UI_FLOOR)->front())->Set_FloorType(0);
                }
                else if (static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Get_CurrFloor() == 4)
                {
                    Engine::Activate_UI(UITYPE::UI_FLOOR);
                    static_cast<CUIFloor*>(Engine::Get_ListUI(UITYPE::UI_FLOOR)->front())->Set_FloorType(1);
                }
            }

            if (static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Get_CurrFloor() == 2)
                Engine::Activate_UI(UITYPE::UI_SKILL);
        }
    }
}

void CDoor::OnCollisionExit(CCollider& _pOther)
{
}

void CDoor::Free()
{
    CTileContorl::Free();
}
