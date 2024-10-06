#pragma once
#include "../Header/WallTB.h"
BEGIN(Engine)

class CWallTBTex;
class CTransform;
class CTexture;
class CCollider;

END

class CDoor :
    public CTileContorl
{
private:
    explicit CDoor(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CDoor();

public:
    static CDoor* Create_InfoNumberDirectionTrigger(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber, Engine::TILE_DIRECTION _eTileDirection, const _int& _iTrigger);
    static CDoor* Create_InfoNumberDirectionTrigger2(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, const _int& _iNumber, const _int& _iTrigger);

    virtual _vec3 Get_VecPos() { return m_vecPos; }
    virtual _int Get_Number() { return m_iNumber; }
    virtual _int Get_Number_Type() { return m_iNumber_Type; }
    _int Get_Trigger() { return m_iTriggerNumber; }//10.06
    _vec3 Get_TileDirection() { return m_vecWallDirection; }
    void Set_TileDirection(const _vec3& _vecDir) { m_vecWallDirection = _vecDir; }
    void Set_Number(const _int& _iNumber) { m_iNumber = _iNumber; }
    void Set_Trigger(const _int& _iTrigger) { m_iTriggerNumber = _iTrigger; }//10.06

public:
    virtual HRESULT	Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
    void Setup_ImageName(const _tchar* _pName);
    void Set_TileDirection(Engine::TILE_DIRECTION _eTileDirection);

private:
    virtual HRESULT Add_Component();
    virtual HRESULT Setup_Material();
    void Setup_Position(_vec3 _vecPos);
    void Setup_Angle(_vec3 _vecRot);
    const _tchar* m_pName;

private:
    Engine::CWallTBTex* m_pBufferCom;
    Engine::CCollider* m_pColliderCom;
    _vec3 m_vecWallDirection;

public:
    void OnCollisionEnter(CCollider& _pOther);

private:
    virtual void Free();
};

