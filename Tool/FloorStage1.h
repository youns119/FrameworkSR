#pragma once
#include "FloorInfo.h"

class CFloorStage1 : public CFloorInfo
{

private:
	explicit CFloorStage1(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CFloorStage1();

public:
	static CFloorStage1* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT Add_Component();
	HRESULT Setup_Material();
	void Setup_Position(_vec3 _vecPos);

private:
	virtual void Free();

};

