#pragma once

#include "Scene.h"
#include "BackGround.h"

#include "Player.h"

#include "WhiteSuit.h"
#include "ShotGun.h"  
#include "BlackMan.h"    //Jonghan Change
#include "FlyingDrone.h" //Jonghan Change
#include "SpiderDrone.h" //Jonghan Change
#include "DogDrone.h"	 //Jonghan Change
#include "Ammo.h"

#include "Terrain.h"
#include "SkyBox.h"
#include "Floor.h"
#include "Wall.h"

// ¿¬¿í
#include "UIPlus.h"

class CStage 
	: public Engine::CScene
{
private :
	explicit CStage(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CStage();

public :
	static CStage* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	virtual HRESULT	Ready_Scene();
	virtual _int Update_Scene(const _float& _fTimeDelta);
	virtual void LateUpdate_Scene();
	virtual void Render_Scene();

private :
	HRESULT Ready_LightInfo();
	HRESULT Ready_Layer_Environment(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_GameLogic(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* _pLayerTag);

private :
	void Set_Collision();

private :
	virtual void Free();
};