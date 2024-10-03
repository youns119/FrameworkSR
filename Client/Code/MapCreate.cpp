#include "pch.h"
#include "../Header/MapCreate.h"
#include "..\Header\MapCamera.h"
#include "..\Header\SkyBox.h"
#include "..\Header\Floor.h"
#include "..\Header\Wall.h"
#include "..\Header\WallTB.h"
#include "..\Header\GuideTerrain.h"


CMapCreate::CMapCreate(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
	, m_bCreateCheck(false)
	, m_fHeight(0.f)
	, m_vecRot(0.f, 0.f, 0.f)
	, m_iRidian(0)
	, m_bGuiHovered(false)
	, m_ImageName(nullptr)
{
}

CMapCreate::~CMapCreate()
{
}

CMapCreate* CMapCreate::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMapCreate* pMapCreate = new CMapCreate(_pGraphicDev);

	if (FAILED(pMapCreate->Ready_Scene()))
	{
		Safe_Release(pMapCreate);
		MSG_BOX("pMapCreate Create Failed");
		return nullptr;
	}

	return pMapCreate;
}

HRESULT CMapCreate::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_ToolEnvironment(L"Layer_ToolEnvironment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Terrain(L"Layer_GuideTerrain"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_PickingTile(L"Layer_PickingTile"), E_FAIL);

	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor0.png",L"Proto_Floor0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor1.png",L"Proto_Floor1", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor2.png",L"Proto_Floor2", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor3.png",L"Proto_Floor3", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor4.png",L"Proto_Floor4", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor5.png",L"Proto_Floor5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor6.png",L"Proto_Floor6", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor7.png",L"Proto_Floor7", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor8.png",L"Proto_Floor8", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/Floor9.png",L"Proto_Floor9", TEXTUREID::TEX_NORMAL, 1);

	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_GLASS01.png", L"Proto_WALL_GLASS01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_GLASS.png", L"Proto_WALL_GLASS", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_HOLE_01.png", L"Proto_WALL_HOLE_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_HOLE_02.png", L"Proto_WALL_HOLE_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_HOLE_03.png", L"Proto_WALL_HOLE_03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_HOLE_04.png", L"Proto_WALL_HOLE_04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_KATANAS.png", L"Proto_WALL_KATANAS", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_MOLTEN.png", L"Proto_WALL_MOLTEN", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_SEA_01.png", L"Proto_WALL_SEA_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_SEA_02.png", L"Proto_WALL_SEA_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_SIDE_DASH_01.png", L"Proto_WALL_SIDE_DASH_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_TRANSPARENT_01.png", L"Proto_WALL_TRANSPARENT_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_TRANSPARENT_02.png", L"Proto_WALL_TRANSPARENT_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_TRANSPARENT_03.png", L"Proto_WALL_TRANSPARENT_03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_TRANSPARENT_04.png", L"Proto_WALL_TRANSPARENT_04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA_EXTERIOR.png", L"Proto_WALLA_EXTERIOR", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA01.png", L"Proto_WALLA01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA02.png", L"Proto_WALLA02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA2_0.png", L"Proto_WALLA2_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA03_0.png", L"Proto_WALLA03_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA03_1.png", L"Proto_WALLA03_1", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA04.png", L"Proto_WALLA04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA5.png", L"Proto_WALLA5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA06.png", L"Proto_WALLA06", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA07.png", L"Proto_WALLA07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA08.png", L"Proto_WALLA08", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA09.png", L"Proto_WALLA09", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLA10.png", L"Proto_WALLA10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB_EXTERIOR.png", L"Proto_WALLB_EXTERIOR", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB01.png", L"Proto_WALLB01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB02.png", L"Proto_WALLB02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB03.png", L"Proto_WALLB03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB03_0.png", L"Proto_WALLB03_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB04.png", L"Proto_WALLB04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB5.png", L"Proto_WALLB5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB06.png", L"Proto_WALLB06", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB07.png", L"Proto_WALLB07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB08.png", L"Proto_WALLB08", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB09.png", L"Proto_WALLB09", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLB10.png", L"Proto_WALLB10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLBORDER01.png", L"Proto_WALLBORDER01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC01.png", L"Proto_WALLC01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC02.png", L"Proto_WALLC02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC03.png", L"Proto_WALLC03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC03_0.png", L"Proto_WALLC03_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC04.png", L"Proto_WALLC04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC04_0.png", L"Proto_WALLC04_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC5.png", L"Proto_WALLC5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC6.png", L"Proto_WALLC6", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC07.png", L"Proto_WALLC07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC08.png", L"Proto_WALLC08", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC09.png", L"Proto_WALLC09", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLC10.png", L"Proto_WALLC10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER_BOSS3_01.png", L"Proto_WALLCORNER_BOSS3_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER_BOSS7_01.png", L"Proto_WALLCORNER_BOSS7_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER01.png", L"Proto_WALLCORNER01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER02.png", L"Proto_WALLCORNER02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER03.png", L"Proto_WALLCORNER03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER04 .png", L"Proto_WALLCORNER04 ", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER05.png", L"Proto_WALLCORNER05", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER05_0.png", L"Proto_WALLCORNER05_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER6.png", L"Proto_WALLCORNER6", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER07.png", L"Proto_WALLCORNER07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER08.png", L"Proto_WALLCORNER08", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER09.png", L"Proto_WALLCORNER09", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALLCORNER10.png", L"Proto_WALLCORNER10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_ACID.png", L"Proto_WALL_ACID", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_BIO_01.png", L"Proto_WALL_BIO_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_BIO_02.png.png", L"Proto_WALL_BIO_02.png", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_Boss3_01.png", L"Proto_WALL_Boss3_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_Boss3_02.png", L"Proto_WALL_Boss3_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_Boss3_03.png", L"Proto_WALL_Boss3_03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_CONCRETE.png", L"Proto_WALL_CONCRETE", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_DECO_01.png", L"Proto_WALL_DECO_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_DECO_02.png", L"Proto_WALL_DECO_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_DECO_03.png", L"Proto_WALL_DECO_03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_DECO_04.png", L"Proto_WALL_DECO_04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_DECO_05.png", L"Proto_WALL_DECO_05", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_DECO_06.png", L"Proto_WALL_DECO_06", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_DECO_07.png", L"Proto_WALL_DECO_07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/WALL_ELECTRIC.png", L"Proto_WALL_ELECTRIC", TEXTUREID::TEX_NORMAL, 1);



	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;

}

_int CMapCreate::Update_Scene(const _float& _fTimeDelta)
{
	//이미지 마우스 따라가게
	if (m_bGuiHovered == false)
	{
		POINT	ptMouse{};
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_vec3	vMousePos;

		D3DVIEWPORT9		ViewPort;
		ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
		m_pGraphicDev->GetViewport(&ViewPort);

		vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
		vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
		vMousePos.z = 0.f;

		_matrix matProj;
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixInverse(&matProj, NULL, &matProj);
		D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	}

	if (Engine::Key_Hold(DIK_Z) && Engine::Mouse_Hold(MOUSEKEYSTATE::DIM_LB))
	{
		PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"Floor");
		// 바닥 타일 생성
		if (m_bCreateCheck == true)
		{
			Create_Layer_PickingFloor(Find_Layer(L"Layer_PickingTile"));

		}
	}
	if (Engine::Key_Hold(DIK_X) && Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
	{
		PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"Wall");
		if (m_bCreateCheck == true)
		{
			Create_Layer_PickingWall(Find_Layer(L"Layer_PickingTile"));
		}
	}
	if (Engine::Key_Hold(DIK_C) && Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))

	{
		PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"WallTB");
		if (m_bCreateCheck == true)
		{
			Create_Layer_PickingWallTB(Find_Layer(L"Layer_PickingTile"));
		}
	}
	if (Engine::Key_Hold(DIK_Z) && Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
	{
		//생성된 타일 삭제
		PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"Floor");
	}
	if (Engine::Key_Hold(DIK_X) && Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
	{
		//생성된 타일 삭제
		PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"Wall");
	}
	if (Engine::Key_Hold(DIK_C) && Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
	{
		//생성된 타일 삭제
		PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"WallTB");
	}
	if (Engine::Key_Press(DIK_O))
	{
		MapSave(Find_Layer(L"Layer_PickingTile"));
	}
	if (Engine::Key_Press(DIK_P))
	{
		MapLoad(Find_Layer(L"Layer_PickingTile"));
	}
	if (Engine::Key_Press(DIK_K))
	{
		m_fHeight += 1.f;
		CGuideTex* pGuideBufferCom = dynamic_cast<CGuideTex*>(Engine::Get_Component(Engine::COMPONENTID::ID_STATIC, L"Layer_GuideTerrain", L"GuideTerrain", L"Com_Buffer"));
		pGuideBufferCom->Set_ChangeY(m_fHeight);
		pGuideBufferCom->Ready_Buffer(VTXTILEX, VTXTILEZ, VTXITV);

	}
	if (Engine::Key_Press(DIK_L))
	{
		m_fHeight -= 1.f;
		CGuideTex* pGuideBufferCom = dynamic_cast<CGuideTex*>(Engine::Get_Component(Engine::COMPONENTID::ID_STATIC, L"Layer_GuideTerrain", L"GuideTerrain", L"Com_Buffer"));
		pGuideBufferCom->Set_ChangeY(m_fHeight);
		pGuideBufferCom->Ready_Buffer(VTXTILEX, VTXTILEZ, VTXITV);
	}
	if (Engine::Key_Press(DIK_N))
	{
		m_iRidian += 45;
		m_vecRot = { D3DXToRadian(m_iRidian),D3DXToRadian(0),D3DXToRadian(0) };

	}
	if (Engine::Key_Press(DIK_M))
	{
		m_iRidian -= 45;
		m_vecRot = { D3DXToRadian(m_iRidian),D3DXToRadian(0),D3DXToRadian(0) };

	}

	_int iExit = Engine::CScene::Update_Scene(_fTimeDelta);

	return iExit;
}

void CMapCreate::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();

}

void CMapCreate::Render_Scene()
{
	ShowGUI();
	ImGui::Render();
}

HRESULT CMapCreate::Ready_LightInfo()
{
	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Direction = { 1.f, -1.f, 1.f };

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CMapCreate::Ready_Layer_ToolEnvironment(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	_vec3 vEye(0.f, 10.f, -10.f);
	_vec3 vAt(0.f, 0.f, 1.f);
	_vec3 vUp(0.f, 1.f, 0.f);

	pGameObject = CMapCamera::Create
	(
		m_pGraphicDev,
		&vEye,
		&vAt,
		&vUp
	);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);
	//dynamic_cast<CDynamicCamera*>(pGameObject)->Toggle_Active();


	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);


	m_mapLayer.insert({ _pLayerTag , pLayer });


	return S_OK;
}

HRESULT CMapCreate::Ready_Layer_Terrain(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CGuideTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GuideTerrain", pGameObject), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CMapCreate::Ready_Layer_PickingTile(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ _pLayerTag, pLayer });

	return S_OK;
}

CLayer* CMapCreate::Find_Layer(const _tchar* _pLayerTag)
{
	// 레이어 태그 매개 변수로 해서 원하는 레이어 받아오도록 설정하기
	CLayer* pLayer = nullptr;

	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));
	pLayer = iter->second;

	return pLayer;
}

HRESULT CMapCreate::Create_Layer_PickingFloor(CLayer* _pLayer)
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CFloor::Create_Info(m_pGraphicDev, TilePiking_OnTerrain(1), m_ImageName);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"Floor", pGameObject);

	return S_OK;
}
	
HRESULT CMapCreate::Create_Layer_PickingWall(CLayer* _pLayer)
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CWall::Create_Info(m_pGraphicDev, TilePiking_OnTerrain(2), m_ImageName);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"Wall", pGameObject);

	return S_OK;
}

HRESULT CMapCreate::Create_Layer_PickingWallTB(CLayer* _pLayer)
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CWallTB::Create_Info(m_pGraphicDev, TilePiking_OnTerrain(3), m_ImageName);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"WallTB", pGameObject);

	return S_OK;
}

_vec3 CMapCreate::TilePiking_OnTerrain(int _iTile)
{
	CGuideTex* pGuideBufferCom = dynamic_cast<CGuideTex*>(Engine::Get_Component(Engine::COMPONENTID::ID_STATIC, L"Layer_GuideTerrain", L"GuideTerrain", L"Com_Buffer"));
	NULL_CHECK_RETURN(pGuideBufferCom, _vec3());
	//여기에 이넘값으로 스위치 문 넣어서 리턴할 수 있도록	
	switch (_iTile)
	{
	case 1:
		return FloorCreate_OnTerrain(g_hWnd, pGuideBufferCom);
		break;
	case 2:
		return WallCreate_OnTerrain1(g_hWnd, pGuideBufferCom);
		break;
	case 3:
		return WallCreate_OnTerrain2(g_hWnd, pGuideBufferCom);
		break;
	}
	
}

_vec3 CMapCreate::FloorCreate_OnTerrain(HWND _hWnd, CGuideTex* _pGuideBufferCom)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(_hWnd, &ptMouse);

	_vec3	vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayPos, vRayDir;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	const _vec3* pGuideTexPos = _pGuideBufferCom->Get_VtxPos();


	_ulong	dwVtxId[3]{};
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < VTXTILEZ - 1; i++)
	{
		for (_ulong j = 0; j < VTXTILEX - 1; j++)
		{
			_ulong	dwIndex = i * VTXTILEX + j;

			dwVtxId[0] = dwIndex + VTXTILEX;
			dwVtxId[1] = dwIndex + VTXTILEX + 1;
			dwVtxId[2] = dwIndex + 1;

			if (D3DXIntersectTri
			(
				// 터레인 -> 버텍스 인덱스를 그리는 순서와 동일하게 적용 231
				&pGuideTexPos[dwVtxId[1]],
				&pGuideTexPos[dwVtxId[2]],
				&pGuideTexPos[dwVtxId[0]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist
			))
			{
				return _vec3
				(
					(pGuideTexPos[dwVtxId[1]].x + fU * (pGuideTexPos[dwVtxId[2]].x - pGuideTexPos[dwVtxId[1]].x)) - 1,
					pGuideTexPos[dwVtxId[0]].y,
					(pGuideTexPos[dwVtxId[1]].z + fV * (pGuideTexPos[dwVtxId[0]].z - pGuideTexPos[dwVtxId[1]].z) - 1)
				);
			}
			// 터레인 -> 버텍스 인덱스를 그리는 순서와 동일하게 적용 210
			dwVtxId[0] = dwIndex + VTXTILEX;
			dwVtxId[1] = dwIndex + 1;
			dwVtxId[2] = dwIndex;

			if (D3DXIntersectTri
			(
				&pGuideTexPos[dwVtxId[2]],
				&pGuideTexPos[dwVtxId[0]],
				&pGuideTexPos[dwVtxId[1]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist
			))
			{
				// V1 + U(V2 - V1) + V(V3 - V1)

				return _vec3
				(
					pGuideTexPos[dwVtxId[2]].x + fU * (pGuideTexPos[dwVtxId[0]].x - pGuideTexPos[dwVtxId[2]].x),
					pGuideTexPos[dwVtxId[0]].y,
					pGuideTexPos[dwVtxId[2]].z + fV * (pGuideTexPos[dwVtxId[1]].z - pGuideTexPos[dwVtxId[2]].z)
				);
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);

}

_vec3 CMapCreate::WallCreate_OnTerrain1(HWND _hWnd, CGuideTex* _pGuideBufferCom)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(_hWnd, &ptMouse);

	_vec3	vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayPos, vRayDir;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	const _vec3* pGuideTexPos = _pGuideBufferCom->Get_VtxPos();


	_ulong	dwVtxId[3]{};
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < VTXTILEZ - 1; i++)
	{
		for (_ulong j = 0; j < VTXTILEX - 1; j++)
		{
			_ulong	dwIndex = i * VTXTILEX + j;

			dwVtxId[0] = dwIndex + VTXTILEX;
			dwVtxId[1] = dwIndex + VTXTILEX + 1;
			dwVtxId[2] = dwIndex + 1;

			if (D3DXIntersectTri
			(
				// 터레인 -> 버텍스 인덱스를 그리는 순서와 동일하게 적용 231
				&pGuideTexPos[dwVtxId[1]],
				&pGuideTexPos[dwVtxId[2]],
				&pGuideTexPos[dwVtxId[0]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist
			))
			{
				return _vec3
				(
					(pGuideTexPos[dwVtxId[1]].x + fU * (pGuideTexPos[dwVtxId[2]].x - pGuideTexPos[dwVtxId[1]].x)),
					pGuideTexPos[dwVtxId[0]].y,
					(pGuideTexPos[dwVtxId[1]].z + fV * (pGuideTexPos[dwVtxId[0]].z - pGuideTexPos[dwVtxId[1]].z) - 1)
				);
			}
			// 터레인 -> 버텍스 인덱스를 그리는 순서와 동일하게 적용 210
			dwVtxId[0] = dwIndex + VTXTILEX;
			dwVtxId[1] = dwIndex + 1;
			dwVtxId[2] = dwIndex;

			if (D3DXIntersectTri
			(
				&pGuideTexPos[dwVtxId[2]],
				&pGuideTexPos[dwVtxId[0]],
				&pGuideTexPos[dwVtxId[1]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist
			))
			{
				// V1 + U(V2 - V1) + V(V3 - V1)
				return _vec3
				(
					pGuideTexPos[dwVtxId[2]].x + fU * (pGuideTexPos[dwVtxId[0]].x - pGuideTexPos[dwVtxId[2]].x),
					pGuideTexPos[dwVtxId[0]].y,
					pGuideTexPos[dwVtxId[2]].z + fV * (pGuideTexPos[dwVtxId[1]].z - pGuideTexPos[dwVtxId[2]].z)
				);
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

_vec3 CMapCreate::WallCreate_OnTerrain2(HWND _hWnd, CGuideTex* _pGuideBufferCom)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(_hWnd, &ptMouse);

	_vec3	vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayPos, vRayDir;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	const _vec3* pGuideTexPos = _pGuideBufferCom->Get_VtxPos();


	_ulong	dwVtxId[3]{};
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < VTXTILEZ - 1; i++)
	{
		for (_ulong j = 0; j < VTXTILEX - 1; j++)
		{
			_ulong	dwIndex = i * VTXTILEX + j;

			dwVtxId[0] = dwIndex + VTXTILEX;
			dwVtxId[1] = dwIndex + VTXTILEX + 1;
			dwVtxId[2] = dwIndex + 1;

			if (D3DXIntersectTri
			(
				// 터레인 -> 버텍스 인덱스를 그리는 순서와 동일하게 적용 231
				&pGuideTexPos[dwVtxId[1]],
				&pGuideTexPos[dwVtxId[2]],
				&pGuideTexPos[dwVtxId[0]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist
			))
			{
				return _vec3
				(
					(pGuideTexPos[dwVtxId[1]].x + fU * (pGuideTexPos[dwVtxId[2]].x - pGuideTexPos[dwVtxId[1]].x))-1,
					pGuideTexPos[dwVtxId[0]].y,
					(pGuideTexPos[dwVtxId[1]].z + fV * (pGuideTexPos[dwVtxId[0]].z - pGuideTexPos[dwVtxId[1]].z))
				);
			}
			// 터레인 -> 버텍스 인덱스를 그리는 순서와 동일하게 적용 210
			dwVtxId[0] = dwIndex + VTXTILEX;
			dwVtxId[1] = dwIndex + 1;
			dwVtxId[2] = dwIndex;

			if (D3DXIntersectTri
			(
				&pGuideTexPos[dwVtxId[2]],
				&pGuideTexPos[dwVtxId[0]],
				&pGuideTexPos[dwVtxId[1]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist
			))
			{
				// V1 + U(V2 - V1) + V(V3 - V1)
				return _vec3
				(
					pGuideTexPos[dwVtxId[2]].x + fU * (pGuideTexPos[dwVtxId[0]].x - pGuideTexPos[dwVtxId[2]].x),
					pGuideTexPos[dwVtxId[0]].y,
					pGuideTexPos[dwVtxId[2]].z + fV * (pGuideTexPos[dwVtxId[1]].z - pGuideTexPos[dwVtxId[2]].z)
				);
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

HRESULT CMapCreate::PickingTile_PosDelete(CLayer* _pLayer, const _tchar* _TileTag)
{
	//이거 for문 써도 되는거 맞는거 같은디 나중에 바꿔보자 => 코드 지저분해서 바꾸고 싶음...애초에 타일로 바꿀까...
	multimap<const _tchar*, CGameObject*>::iterator it = _pLayer->Get_LayerObjects()->begin();
	while (it != _pLayer->Get_LayerObjects()->end())
	{
		if (_TileTag == L"Floor" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CFloor*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(1))
			{
				_pLayer->Get_LayerObjects()->erase(it++);
			}
			else
			{
				++it;
			}
		}
		else if (_TileTag == L"Wall" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CWall*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(2))
			{
				_pLayer->Get_LayerObjects()->erase(it++);
			}
			else
			{
				++it;
			}
		}
		else if (_TileTag == L"WallTB" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CWallTB*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(3))
			{
				_pLayer->Get_LayerObjects()->erase(it++);
			}
			else
			{
				++it;
			}
		}
		else
		{
			m_bCreateCheck = true;
			++it;
		}
	}
	return S_OK;
}

void CMapCreate::PickingTile_PosCheck(CLayer* _pLayer, const _tchar* _TileTag)
{
	multimap<const _tchar*, CGameObject*>::iterator it = _pLayer->Get_LayerObjects()->begin();
	while (it != _pLayer->Get_LayerObjects()->end())
	{
		if (_TileTag == L"Floor" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CFloor*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(1))
			{
				m_bCreateCheck = false;
				break;
			}
			else
			{
				m_bCreateCheck = true;
				++it;
			}
		}
		else if (_TileTag == L"Wall" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CWall*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(2))
			{
				m_bCreateCheck = false;
				break;
			}
			else
			{
				m_bCreateCheck = true;
				++it;
			}
		}
		else if (_TileTag == L"WallTB" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CWallTB*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(3))
			{
				m_bCreateCheck = false;
				break;
			}
			else
			{
				m_bCreateCheck = true;
				++it;
			}
		}
		else
		{
			m_bCreateCheck = true;
			++it;
		}

	}
	// 최초 생성 예외처리
	if (_pLayer->Get_LayerObjects()->size() == 0)
	{
		m_bCreateCheck = true;
	}
}

void CMapCreate::MapSave(CLayer* _pLayer)
{
	HANDLE		hFile = CreateFile(L"../Data/TutorialStage.txt",	// 파일 이름까지 포함된 경로
		GENERIC_WRITE,		// 파일 접근 모드(GENERIC_WRITE : 쓰기, GENERIC_READ : 읽기)
		NULL,				// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가 할 것인가)
		NULL,				// 보안 속성
		CREATE_ALWAYS,		// 생성 방식(CREATE_ALWAYS : 파일이 없다면 생성, 있으면 덮어쓰기, OPEN_EXISTING : 파일이 있는 경우에만 불러오기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(아무런 속성이 없는 파일로 생성)
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Save File", _T("Fail"), MB_OK);
		return;
	}

	DWORD	dwByte(0);
	DWORD dwStringSize(0);

	multimap<const _tchar*, CGameObject*>::iterator it;
	for (it = _pLayer->Get_LayerObjects()->begin(); it != _pLayer->Get_LayerObjects()->end(); it++)
	{
		dwStringSize = sizeof(wchar_t) * (wcslen((*it).first) + 1);
		WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
		WriteFile(hFile, (*it).first, dwStringSize, &dwByte, nullptr);

		if ((*it).first == L"Floor")
		{
			WriteFile(hFile, dynamic_cast<CFloor*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr);
			dwStringSize = sizeof(wchar_t) * (wcslen((dynamic_cast<CFloor*>((*it).second)->Get_FloorName())) + 1);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, dynamic_cast<CFloor*>((*it).second)->Get_FloorName(), dwStringSize, &dwByte, nullptr);

			//WriteFile(hFile, dynamic_cast<CFloor*>((*it).second)->Get_VecRot(), sizeof(_vec3), &dwByte, nullptr);//회전값 저장
		}
		if ((*it).first == L"Wall")
		{
			WriteFile(hFile, dynamic_cast<CWall*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr);
			dwStringSize = sizeof(wchar_t) * (wcslen((dynamic_cast<CWall*>((*it).second)->Get_FloorName())) + 1);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, dynamic_cast<CWall*>((*it).second)->Get_FloorName(), dwStringSize, &dwByte, nullptr);

		}
		if ((*it).first == L"WallTB")
		{
			WriteFile(hFile, dynamic_cast<CWallTB*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr);
			dwStringSize = sizeof(wchar_t) * (wcslen((dynamic_cast<CWallTB*>((*it).second)->Get_FloorName())) + 1);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, dynamic_cast<CWallTB*>((*it).second)->Get_FloorName(), dwStringSize, &dwByte, nullptr);

		}
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Save 완료", _T("성공"), MB_OK);

}

void CMapCreate::MapLoad(CLayer* _pLayer)
{
	multimap<const _tchar*, CGameObject*>::iterator it;
	it = _pLayer->Get_LayerObjects()->find(L"Floor");
	_pLayer->Get_LayerObjects()->erase(it, _pLayer->Get_LayerObjects()->end());

	it = _pLayer->Get_LayerObjects()->find(L"Wall");
	_pLayer->Get_LayerObjects()->erase(it, _pLayer->Get_LayerObjects()->end());

	it = _pLayer->Get_LayerObjects()->find(L"WallTB");
	_pLayer->Get_LayerObjects()->erase(it, _pLayer->Get_LayerObjects()->end());



	HANDLE		hFile = CreateFile(L"../Data/TutorialStage.txt",	// 파일 이름까지 포함된 경로
		GENERIC_READ,		// 파일 접근 모드(GENERIC_WRITE : 쓰기, GENERIC_READ : 읽기)
		NULL,				// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가 할 것인가)
		NULL,				// 보안 속성
		OPEN_EXISTING,		// 생성 방식(CREATE_ALWAYS : 파일이 없다면 생성, 있으면 덮어쓰기, OPEN_EXISTING : 파일이 있는 경우에만 불러오기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(아무런 속성이 없는 파일로 생성)
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load File", _T("Fail"), MB_OK);
		return;
	}

	DWORD	dwByte(0);
	DWORD dwStringSize(0);
	DWORD dwStringSize2(0);

	_vec3 pPos{};
	_vec3 pRot{};


	while (true)
	{
		ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr); // first 사이즈
		const _tchar* pTemp = NULL; // first 글자 받아올 변수
		ReadFile(hFile, &pTemp, dwStringSize, &dwByte, nullptr); // 글자 저장
		ReadFile(hFile, &pPos, sizeof(_vec3), &dwByte, nullptr); // 포지션 값 저장

		ReadFile(hFile, &dwStringSize2, sizeof(DWORD), &dwByte, nullptr); // 이미지 이름 사이즈
		TCHAR* pSrc = new TCHAR[dwStringSize2]; //  이미지 이름 글자 받아올 변수
		ReadFile(hFile, pSrc, dwStringSize2, &dwByte, nullptr); // 이미지 이름 저장

		if (0 == dwByte)
		{
			delete[] pTemp;
			pTemp = nullptr;

			delete[] pSrc;
			pSrc = nullptr;
			break;
		}

		if (wcscmp(pTemp, L"Floor") == 0)
		{
			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CFloor::Create_Info(m_pGraphicDev, pPos, pSrc);
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"Floor", pGameObject);
		}
		if (wcscmp(pTemp, L"Wall") == 0)
		{
			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CWall::Create_Info(m_pGraphicDev, pPos, pSrc);
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"Wall", pGameObject);
		}
		if (wcscmp(pTemp, L"WallTB") == 0)
		{
			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CWallTB::Create_Info(m_pGraphicDev, pPos, pSrc);
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"WallTB", pGameObject);
		}

		delete[] pTemp;
		pTemp = nullptr;

		delete[] pSrc;
		pSrc = nullptr;
	}

	CloseHandle(hFile);


	MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
}

void CMapCreate::ShowGUI()
{
	// 방향키가 영향을 주지 않게
	ImGui::GetIO().NavActive = false;
	// 마우스가 영향을 주도록
	ImGui::GetIO().WantCaptureMouse = true;

	ImGui::Begin(u8"맵 에디터", NULL, ImGuiWindowFlags_MenuBar);//창 이름


	SetMenu(); // 하나의 그룹이 되는 느낌?
	SetFloor();
	SetWall();

	//메뉴바
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			//메뉴
			ImGui::MenuItem("Save");
			ImGui::Separator();


			ImGui::MenuItem("Open");

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}


	//버튼
	ImGui::Text(u8"버튼");
	ImGui::SameLine(50.f, 0.0f);
	//기본 버튼 색													RGB 방식//HSV 방식도 있음
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 0.0f));
	//마우스가 올라갔을 때 버튼 색
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.0f, 1.0f, 0.0f));
	//클릭했을 때 버튼 색
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 1.0f));
	//버튼 생성
	ImGui::Button(u8"생성");
	ImGui::PopStyleColor(3);



	//마우스가 imgui 위에 있는지
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemHovered())
	{
		m_bGuiHovered = true;
	}
	else
	{
		m_bGuiHovered = false;
	}



	ImGui::End();

}

void CMapCreate::SetMenu()
{
	if (!ImGui::CollapsingHeader("setting")) // gui 숨겼다 보여주는 bool 값
		return;

	//슬라이더 오브제들 생성
	ImGui::Text(u8"좌표");
	ImGui::SameLine(59.f, 0.f);
	static _vec2 Position = _vec2(100.f, 100.f); // 픽킹한 마우스 포지션 값 가져오기?  만들지 고민
	ImGui::SliderFloat2("##1", Position, -800.f, 800.f); // 뒤에 내용이 출력되지 않게 된다?
	/*static int Pos[2] = { 0,0 };
	ImGui::SliderInt2(u8"위치", Pos, 0, 10);*/


	const char* items[] = { "Scene01", "Scene2" , "Scene3" };// 드롭다운에 들어갈 아이템 이름들 나열
	static int nCurrentItem = 1; // 몇번째 아이템이 선택되는지 설정
	//이름, 결과를 담을 주소값, 아이템들, 드롭다운에 표시될 개수?
	ImGui::Combo("##2", &nCurrentItem, items, IM_ARRAYSIZE(items)); // IM_ARRAYSIZE 문자열의 개수를 구하는 임구이 매크로


	static int nOffset[2] = { 2,3 };
	ImGui::Text(u8"int값");
	ImGui::SameLine(100.f, 0.0f);
	ImGui::InputInt2("##3", nOffset);

	static _vec2 vOffset = { 3.5f,7.57f };
	ImGui::Text(u8"Float값"); // 소수점 세자리 수 까지가 기본
	ImGui::SameLine(100.f, 0.0f);
	ImGui::InputFloat2("##3", vOffset,"%.2f");





}

void CMapCreate::SetFloor()
{
	// 인터넷 테스트 코드
	
	//bool ret = false;
	////  헤더. false 접음, true = 펼침
	//if (!ImGui::CollapsingHeader("Tile")) // gui 숨겼다 보여주는 bool 값
	//	return;
	////scene 선택 콤보 박스// 공부용 코드니깐 나중에 지울 것 
	//ImGui::Text(u8"Scene 종류 선택");
	//ImGui::SameLine(140.f, 0.f);
	//const char* items[] = { "Village", "Boss", "Dungeon" };
	//static int nCurrentScene = 0;
	//ret = ImGui::Combo("##Scene", &nCurrentScene, items, sizeof(items));
	//if (ret)
	//{
	//	printf("Scene = %s\n", items[nCurrentScene]);
	//}
	//// Map 크기 설정
	//static int nMapSizeXY[2] = { 100,100 };
	//{
	//	ImGui::Text(u8"Map크기(타일 개수)");
	//	ImGui::SameLine(140.f, 0.0f);
	//	ret = ImGui::InputInt2("##MapSize", nMapSizeXY);
	//	for (int i = 0; i < 2; i++)
	//	{
	//		if (nMapSizeXY[i] < 1)
	//		{
	//			nMapSizeXY[i] = 1;
	//		}
	//	}
	//	if (ret)
	//	{
	//		printf("Map X = %d칸, Y = %d칸\n", nMapSizeXY[0], nMapSizeXY[1]);
	//	}
	//}
	////맵 원점 조정(offset)
	//static _vec2 OriginPos = _vec2(100.f, 100.f);
	//{
	//	ImGui::Text(u8"원점 조정");
	//	ImGui::SameLine(140.f, 0.0f);
	//	//파일 데이터베이스에서 읽어오는 것을 여기서 처라?
	//	ret = ImGui::InputFloat2("##MapOffset", OriginPos, "%0.2", 0);
	//	if(ret)
	//		{
	//			printf("원점 X = %f, Y = %f\n",OriginPos.x,OriginPos.y);
	//		}
	//}
	////
	////버튼 색
	//ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 0.0f));
	////마우스가 올라갔을 때 버튼 색
	//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.0f, 1.0f, 0.0f));
	////클릭했을 때 버튼 색
	//ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 1.0f));
	////버튼 생성
	//ImGui::Button(u8"데이터 저장");
	//ImGui::PopStyleColor(3);
	//if (ret)
	//{
	//	//위에 설정한 변수 값들을 가져와서 Set 함수를 실행
	//}

	bool ret = false;
	if (!ImGui::CollapsingHeader("Floors"))
	{
		return;
	}

	//// Order(이미지 배치 순서) 선택
	//const char* items[] = { "0","1", "2", "3", "4", "5" };
	//static int nOrder = 0;
	//{
	//	ImGui::Text(u8"Display order 설정");
	//	ImGui::SameLine(140.f, 0.f);
	//	ret = ImGui::Combo("##Order", &nOrder, items, size(items));
	//	if (ret)
	//	{
	//		printf("display order = %d\n", nOrder);
	//	}
	//}


	// 텍스쳐 선택
	ImGui::Text(u8"바닥 선택");
	int iCount(0);

	map<const _tchar*, IDirect3DBaseTexture9*>::iterator it;
	for (it = m_mapImageFloor.begin(); it != m_mapImageFloor.end(); ++it)
	{
		char* pStr;
		int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, NULL, 0, NULL, NULL);
		pStr = new char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, pStr, iStrSize, 0, 0);

		ret = ImGui::ImageButton(pStr, (*it).second, ImVec2(50.f, 50.f));

		iCount++;
		if (iCount < 5)
		{
			ImGui::SameLine();
		}
		else
		{
			iCount = 0;
		}
		if (ret)
		{
			// 타일 이미지 변경
			// floor를 선택 했는지 wall 선택했는지 변수나 이넘값을 최초 세팅해주면 마우스 픽킹만으로 생성하도록 => 키보드 제거하기
			m_ImageName = (*it).first;
		}

		delete[] pStr;
		pStr = nullptr;
	}

	// 여기에 이미지 회전값 넣을 수 있도록 세팅 => 가능하다면 타일의 Pos? 그려지는 기준이 되는 정점의 좌표 값을 출력하도록 설정
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 0.0f));
	//마우스가 올라갔을 때 버튼 색
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.0f, 1.0f, 0.0f));
	//클릭했을 때 버튼 색
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.0f, 1.0f));
	//버튼 생성
	ImGui::Button(u8"이미지");
	ImGui::PopStyleColor(3);



}

void CMapCreate::SetWall()
{

	bool ret = false;
	if (!ImGui::CollapsingHeader("Walls"))
	{
		return;
	}

	ImGui::Text(u8"벽 선택");
	int iCount(0);

	map<const _tchar*, IDirect3DBaseTexture9*>::iterator it;
	for (it = m_mapImageWall.begin(); it != m_mapImageWall.end(); ++it)
	{
		char* pStr;
		int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, NULL, 0, NULL, NULL);
		pStr = new char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, pStr, iStrSize, 0, 0);

		ret = ImGui::ImageButton(pStr, (*it).second, ImVec2(50.f, 50.f));

		iCount++;
		if (iCount < 5)
		{
			ImGui::SameLine();
		}
		else
		{
			iCount = 0;
		}
		if (ret)
		{
			// 타일 이미지 변경
			// floor를 선택 했는지 wall 선택했는지 변수나 이넘값을 최초 세팅해주면 마우스 픽킹만으로 생성하도록 => 키보드 제거하기
			m_ImageName = (*it).first;
		}

		delete[] pStr;
		pStr = nullptr;
	}
}

HRESULT CMapCreate::Ready_Texture_FloorInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt)
{

	IDirect3DBaseTexture9* pTexture = NULL;

	for (_int i = 0; i < _iCnt; i++)
	{
		TCHAR szFileName[128] = L"";

		wsprintf(szFileName, _pPath, i);	

		switch (_eType)
		{
		case TEXTUREID::TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			m_mapImageFloor.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		case TEXTUREID::TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			m_mapImageFloor.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		default:
			break;
		}

	}

	return S_OK;
}
HRESULT CMapCreate::Ready_Texture_WallInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt)
{
	IDirect3DBaseTexture9* pTexture = NULL;

	for (_int i = 0; i < _iCnt; i++)
	{
		TCHAR szFileName[128] = L"";

		wsprintf(szFileName, _pPath, i);

		switch (_eType)
		{
		case TEXTUREID::TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			m_mapImageWall.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		case TEXTUREID::TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			m_mapImageWall.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		default:
			break;
		}

	}

	return S_OK;
}
void CMapCreate::Free()
{
	Engine::CScene::Free();
}
