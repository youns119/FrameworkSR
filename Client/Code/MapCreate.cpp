#include "pch.h"
#include "../Header/MapCreate.h"
#include "..\Header\MapCamera.h"
#include "..\Header\SkyBox.h"
#include "..\Header\Floor.h"
#include "..\Header\Wall.h"
#include "..\Header\Door.h"
#include "..\Header\ItemTile.h"
#include "..\Header\MonsterTile.h"
#include "..\Header\GuideTerrain.h"
#include "../Header/TileContorl.h"


CMapCreate::CMapCreate(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
	, m_bCreateCheck(false)
	, m_fHeight(0.f)
	, m_vecRot(0.f, 0.f, 0.f)
	, m_vecPosition(0.f, 0.f, 0.f)
	, m_vecScale(0.f, 0.f, 0.f)
	, m_iNumber(0)
	, m_iTriggerNumber(0)
{
}

CMapCreate::~CMapCreate()
{
	Free();
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

	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/0.png",L"Proto_Floor0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/1.png",L"Proto_Floor1", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/2.png",L"Proto_Floor2", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/3.png",L"Proto_Floor3", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/4.png",L"Proto_Floor4", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/5.png",L"Proto_Floor5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/6.png",L"Proto_Floor6", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/7.png",L"Proto_Floor7", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/8.png",L"Proto_Floor8", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/9.png",L"Proto_Floor9", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/10.png",L"Proto_Floor10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/11.png",L"Proto_Floor11", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/12.png",L"Proto_Floor12", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/13.png",L"Proto_Floor13", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/14.png",L"Proto_Floor14", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/15.png",L"Proto_Floor15", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/16.png",L"Proto_Floor16", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/17.png",L"Proto_Floor17", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/18.png",L"Proto_Floor18", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/19.png",L"Proto_Floor19", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/20.png",L"Proto_Floor20", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/21.png",L"Proto_Floor21", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/22.png",L"Proto_Floor22", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/23.png",L"Proto_Floor23", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/24.png",L"Proto_Floor24", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/25.png",L"Proto_Floor25", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/26.png",L"Proto_Floor26", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/27.png",L"Proto_Floor27", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/28.png", L"Proto_Floor28", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/29.png", L"Proto_Floor29", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/30.png", L"Proto_Floor30", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_FloorInsert(L"../Bin/Resource/Texture/MMJ_Floor/31.png", L"Proto_Floor31", TEXTUREID::TEX_NORMAL, 1);

	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/0.png", L"Proto_WALL_GLASS01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/1.png", L"Proto_WALL_GLASS", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/2.png", L"Proto_WALL_HOLE_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/3.png", L"Proto_WALL_HOLE_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/4.png", L"Proto_WALL_HOLE_03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/5.png", L"Proto_WALL_HOLE_04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/6.png", L"Proto_WALL_KATANAS", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/7.png", L"Proto_WALL_MOLTEN", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/8.png", L"Proto_WALL_SEA_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/9.png", L"Proto_WALL_SEA_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/10.png", L"Proto_WALL_SIDE_DASH_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/11.png", L"Proto_WALL_TRANSPARENT_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/12.png", L"Proto_WALL_TRANSPARENT_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/13.png", L"Proto_WALL_TRANSPARENT_03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/14.png", L"Proto_WALL_TRANSPARENT_04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/15.png", L"Proto_WALLA_EXTERIOR", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/16.png", L"Proto_WALLA01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/17.png", L"Proto_WALLA02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/18.png", L"Proto_WALLA2_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/19.png", L"Proto_WALLA03_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/20.png", L"Proto_WALLA03_1", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/21.png", L"Proto_WALLA04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/22.png", L"Proto_WALLA5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/23.png", L"Proto_WALLA06", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/24.png", L"Proto_WALLA07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/25.png", L"Proto_WALLA08", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/26.png", L"Proto_WALLA09", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/27.png", L"Proto_WALLA10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/28.png", L"Proto_WALLB_EXTERIOR", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/29.png", L"Proto_WALLB01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/30.png", L"Proto_WALLB02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/31.png", L"Proto_WALLB03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/32.png", L"Proto_WALLB03_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/33.png", L"Proto_WALLB04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/34.png", L"Proto_WALLB5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/35.png", L"Proto_WALLB06", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/36.png", L"Proto_WALLB07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/37.png", L"Proto_WALLB08", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/38.png", L"Proto_WALLB09", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/39.png", L"Proto_WALLB10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/40.png", L"Proto_WALLBORDER01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/41.png", L"Proto_WALLC01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/42.png", L"Proto_WALLC02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/43.png", L"Proto_WALLC03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/44.png", L"Proto_WALLC03_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/45.png", L"Proto_WALLC04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/46.png", L"Proto_WALLC04_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/47.png", L"Proto_WALLC5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/48.png", L"Proto_WALLC6", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/49.png", L"Proto_WALLC07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/50.png", L"Proto_WALLC08", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/51.png", L"Proto_WALLC09", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/52.png", L"Proto_WALLC10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/53.png", L"Proto_WALLCORNER_BOSS3_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/54.png", L"Proto_WALLCORNER_BOSS7_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/55.png", L"Proto_WALLCORNER01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/56.png", L"Proto_WALLCORNER02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/57.png", L"Proto_WALLCORNER03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/58.png", L"Proto_WALLCORNER04 ", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/59.png", L"Proto_WALLCORNER05", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/60.png", L"Proto_WALLCORNER05_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/61.png", L"Proto_WALLCORNER6", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/62.png", L"Proto_WALLCORNER07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/63.png", L"Proto_WALLCORNER08", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/64.png", L"Proto_WALLCORNER09", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/65.png", L"Proto_WALLCORNER10", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/66.png", L"Proto_WALL_ACID", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/67.png", L"Proto_WALL_BIO_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/68.png", L"Proto_WALL_BIO_02.png", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/69.png", L"Proto_WALL_Boss3_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/70.png", L"Proto_WALL_Boss3_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/71.png", L"Proto_WALL_Boss3_03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/72.png", L"Proto_WALL_CONCRETE", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/73.png", L"Proto_WALL_DECO_01", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/74.png", L"Proto_WALL_DECO_02", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/75.png", L"Proto_WALL_DECO_03", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/76.png", L"Proto_WALL_DECO_04", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/77.png", L"Proto_WALL_DECO_05", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/78.png", L"Proto_WALL_DECO_06", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/79.png", L"Proto_WALL_DECO_07", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/80.png", L"Proto_WALL_ELECTRIC", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/81.png", L"Proto_WALL_Wreck", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/82.png", L"Proto_WALL_82", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/83.png", L"Proto_WALL_83", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/84.png", L"Proto_WALL_84", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/85.png", L"Proto_WALL_85", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_WallInsert(L"../Bin/Resource/Texture/MMJ_Wall/86.png", L"Proto_WALL_86", TEXTUREID::TEX_NORMAL, 1);

	Ready_Texture_MonsterInsert(L"../Bin/Resource/Texture/MMJ_Monster/MapTool/0.png", L"Proto_Monster1", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_MonsterInsert(L"../Bin/Resource/Texture/MMJ_Monster/MapTool/1.png", L"Proto_Monster2", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_MonsterInsert(L"../Bin/Resource/Texture/MMJ_Monster/MapTool/2.png", L"Proto_Monster3", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_MonsterInsert(L"../Bin/Resource/Texture/MMJ_Monster/MapTool/3.png", L"Proto_Monster4", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_MonsterInsert(L"../Bin/Resource/Texture/MMJ_Monster/MapTool/4.png", L"Proto_Monster5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_MonsterInsert(L"../Bin/Resource/Texture/MMJ_Monster/MapTool/5.png", L"Proto_Monster6", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_MonsterInsert(L"../Bin/Resource/Texture/MMJ_Monster/MapTool/6.png", L"Proto_Monster7", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_MonsterInsert(L"../Bin/Resource/Texture/MMJ_Monster/MapTool/7.png", L"Proto_Monster8", TEXTUREID::TEX_NORMAL, 1);

	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/0.png", L"Proto_DOOR_0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/1.png", L"Proto_DOOR_1", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/2.png", L"Proto_DOOR_2", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/3.png", L"Proto_DOOR_3", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/4.png", L"Proto_DOOR_4", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/5.png", L"Proto_DOOR_5", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/6.png", L"Proto_DOOR_6", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/20.png", L"Proto_DOOR_25", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/21.png", L"Proto_DOOR_26", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_DoorInsert(L"../Bin/Resource/Texture/MMJ_Door/22.png", L"Proto_DOOR_27", TEXTUREID::TEX_NORMAL, 1);

	Ready_Texture_ItemInsert(L"../Bin/Resource/Texture/MMJ_Item/DrinkMachine/Idle/0.png", L"Proto_Item0", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_ItemInsert(L"../Bin/Resource/Texture/MMJ_Item/Drop_Item/Axe.png", L"Proto_Item1", TEXTUREID::TEX_NORMAL, 1);
	Ready_Texture_ItemInsert(L"../Bin/Resource/Texture/MMJ_Item/Drop_Item/Knife.png", L"Proto_Item3", TEXTUREID::TEX_NORMAL, 1);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;

}

_int CMapCreate::Update_Scene(const _float& _fTimeDelta)
{
	if (Engine::Key_Press(DIK_F1))
		Engine::Toggle_Collider();


	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
	{
		m_vecPosition = TilePiking_OnTerrain(1);
	}
	if (Engine::Key_Hold(DIK_Z))
	{		// 바닥 생성
		if (Engine::Mouse_Hold(MOUSEKEYSTATE::DIM_LB))
		{
			PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"Floor");
			if (m_bCreateCheck == true)
			{
				Create_Layer_PickingFloor(Find_Layer(L"Layer_PickingTile"));
			}
		}
		//생성된 바닥 삭제
		if (Engine::Mouse_Hold(MOUSEKEYSTATE::DIM_RB))
		{
			PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"Floor");
		}
	}

	if (Engine::Key_Hold(DIK_X))
	{
		// 문 생성
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
		{
			PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"Door");
			if (m_bCreateCheck == true)
			{
  				Create_Layer_PickingDoor(Find_Layer(L"Layer_PickingTile"), Engine::TILE_DIRECTION::TILE_FORWARD);
			}
		}
		//생성된 문 삭제
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
		{
			PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"Door");
		}

	}
	if (Engine::Key_Hold(DIK_T) || Engine::Key_Hold(DIK_3))
	{	//벽 생성
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
		{
			PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"WallTB");
			if (m_bCreateCheck == true)
			{
				Create_Layer_PickingWallTB(Find_Layer(L"Layer_PickingTile"), Engine::TILE_DIRECTION::TILE_FORWARD);
			}
		}
		//벽 삭제
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
		{
			PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"WallTB");
		}
	}


	if (Engine::Key_Hold(DIK_H) || Engine::Key_Hold(DIK_2))
	{	//벽 생성
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
		{
			PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"Wall");
			if (m_bCreateCheck == true)
			{
				Create_Layer_PickingWall(Find_Layer(L"Layer_PickingTile"), Engine::TILE_DIRECTION::TILE_RIGHT);
			}
		}
		//벽 삭제
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
		{
			PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"Wall");
		}
	}

	if (Engine::Key_Hold(DIK_F) || Engine::Key_Hold(DIK_1))
	{
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
		{
			//벽 생성
			PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"Wall");
			if (m_bCreateCheck == true)
			{
				Create_Layer_PickingWall(Find_Layer(L"Layer_PickingTile"), Engine::TILE_DIRECTION::TILE_LEFT);
			}
		}
		//벽 삭제
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
		{
			PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"Wall");
		}
	}

	if (Engine::Key_Hold(DIK_G) || Engine::Key_Hold(DIK_4))
	{
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
		{
			//벽 생성
			PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"WallTB");
			if (m_bCreateCheck == true)
			{
				Create_Layer_PickingWallTB(Find_Layer(L"Layer_PickingTile"), Engine::TILE_DIRECTION::TILE_BACK);
			}
		}
		//벽 삭제
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
		{
			PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"WallTB");
		}
	}

	if (Engine::Key_Hold(DIK_V))
	{		// 몬스터 타일 생성
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
		{
			PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"MonsterTile");
			if (m_bCreateCheck == true)
			{
				Create_Layer_PickingMonster(Find_Layer(L"Layer_PickingTile"));
			}
		}
		//생성된 몬스터 삭제
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
		{
			PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"MonsterTile");
		}
	}

	if (Engine::Key_Hold(DIK_C))
	{		// 아이템 타일 생성
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
		{
			PickingTile_PosCheck(Find_Layer(L"Layer_PickingTile"), L"ItemTile");
			if (m_bCreateCheck == true)
			{
				Create_Layer_PickingItem(Find_Layer(L"Layer_PickingTile"));
			}
		}
		//생성된 아이템 삭제
		if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
		{
			PickingTile_PosDelete(Find_Layer(L"Layer_PickingTile"), L"ItemTile");
		}
	}

	if (Engine::Key_Press(DIK_O))
	{
		MapSave2(Find_Layer(L"Layer_PickingTile"));
	}
	if (Engine::Key_Press(DIK_P))
	{
		MapLoad2(Find_Layer(L"Layer_PickingTile"));
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


	pGameObject = CSkyBox::Create(m_pGraphicDev,3);
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
	CLayer* pLayer = nullptr;

	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));
	pLayer = iter->second;

	return pLayer;
}

HRESULT CMapCreate::Create_Layer_PickingFloor(CLayer* _pLayer) 
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CFloor::Create_Info(m_pGraphicDev, m_vecPosition = TilePiking_OnTerrain(1),m_vecRot,m_vecScale, m_iNumber, m_iTriggerNumber);//10.06
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"Floor", pGameObject);

	return S_OK;
}
	
HRESULT CMapCreate::Create_Layer_PickingWall(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection)
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CWall::Create_InfoNumberDirectionTrigger(m_pGraphicDev, m_vecPosition = TilePiking_OnTerrain(2), m_iNumber, _eTileDirection, m_iTriggerNumber);//10.06
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"Wall", pGameObject);

	return S_OK;
}


HRESULT CMapCreate::Create_Layer_PickingWallTB(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection)
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CWallTB::Create_InfoNumberDirectionTrigger(m_pGraphicDev, m_vecPosition = TilePiking_OnTerrain(3), m_iNumber, _eTileDirection, m_iTriggerNumber);//10.06
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"WallTB", pGameObject);

	return S_OK;
}

HRESULT CMapCreate::Create_Layer_PickingMonster(CLayer* _pLayer)
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CMonsterTile::Create_InfoNumberTrigger(m_pGraphicDev, m_vecPosition = TilePiking_OnTerrain(4), m_iNumber, m_iTriggerNumber);//10.06
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"MonsterTile", pGameObject);

	return S_OK;
}

HRESULT CMapCreate::Create_Layer_PickingDoor(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection)
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CDoor::Create_InfoSave(m_pGraphicDev, m_vecPosition = TilePiking_OnTerrain(3), m_vecRot, m_vecScale, m_iNumber, _eTileDirection, m_iTriggerNumber);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"Door", pGameObject);

	return S_OK;
}

HRESULT CMapCreate::Create_Layer_PickingItem(CLayer* _pLayer)
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CItemTile::Create_InfoNumberTrigger(m_pGraphicDev, m_vecPosition = TilePiking_OnTerrain(4), m_iNumber , m_iTriggerNumber);//10.06
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	_pLayer->Add_GameObject(L"ItemTile", pGameObject);

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
	case 4:
		return MonsterCreate_OnTerrain(g_hWnd, pGuideBufferCom);
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
					(pGuideTexPos[dwVtxId[1]].z + fV * (pGuideTexPos[dwVtxId[0]].z - pGuideTexPos[dwVtxId[1]].z) - 1 )
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

_vec3 CMapCreate::MonsterCreate_OnTerrain(HWND _hWnd, CGuideTex* _pGuideBufferCom)
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

HRESULT CMapCreate::PickingTile_PosDelete(CLayer* _pLayer, const _tchar* _TileTag)
{
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
		else if (_TileTag == L"MonsterTile" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CMonsterTile*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(4))
			{
				_pLayer->Get_LayerObjects()->erase(it++);
			}
			else
			{
				++it;
			}
		}
		else if (_TileTag == L"ItemTile" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CItemTile*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(4))
			{
				_pLayer->Get_LayerObjects()->erase(it++);
			}
			else
			{
				++it;
			}
		}
		else if (_TileTag == L"Door" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CDoor*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(3))
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
			if (dynamic_cast<CFloor*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(1) &&
				dynamic_cast<CFloor*>((*it).second)->Get_VecRot() == m_vecRot)
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
			if (dynamic_cast<CWallTB*>((*it).second) == nullptr)
			{
				break;
			}
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
		else if (_TileTag == L"MonsterTile" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CMonsterTile*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(4))
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
		else if (_TileTag == L"ItemTile" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CItemTile*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(4))
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
		else if (_TileTag == L"Door" && _TileTag == (*it).first)
		{
			if (dynamic_cast<CDoor*>((*it).second) == nullptr)
			{
				break;
			}
			if (dynamic_cast<CDoor*>((*it).second)->Get_VecPos() == TilePiking_OnTerrain(3))
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

void CMapCreate::MapSave2(CLayer* _pLayer)
{
	//1_TutorialStage
	//2_SlidingStage
	//3_FloorStage
	//4_BuildingStage
	//5_RobotBossStage
	 HANDLE		hFile = CreateFile(L"../Data/3_FloorStage.txt",	// 파일 이름까지 포함된 경로
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
			
	_int iNumber_Type(0); // 설치하는 타일의 종류
	_int iNumber(0); // 설치하는 타일의 이미지 번호
	_int iTrigger(0); // 공간 그룹 => 트리거 작동을 위한 값
	DWORD	dwByte(0);

	multimap<const _tchar*, CGameObject*>::iterator it;
	for (it = _pLayer->Get_LayerObjects()->begin(); it != _pLayer->Get_LayerObjects()->end(); it++)
	{
		iNumber_Type = 0;
		iNumber = 0;
		iTrigger = 0;

		if (nullptr != dynamic_cast<CFloor*>((*it).second)) // 바닥
		{
			iNumber_Type = 0;
			WriteFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr); //타일 종류 번호 저장 0 = 바닥

			iNumber = dynamic_cast<CFloor*>((*it).second)->Get_Number(); // 이미지 번호
			iTrigger = dynamic_cast<CFloor*>((*it).second)->Get_Trigger();//트리거 번호

			WriteFile(hFile, dynamic_cast<CFloor*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr); //바닥 위치 저장
			WriteFile(hFile, dynamic_cast<CFloor*>((*it).second)->Get_VecRot(), sizeof(_vec3), &dwByte, nullptr); //바닥 회전 방향 저장
			WriteFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr); // 이미지 번호 저장
			WriteFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr); // 트리거 번호 저장

			WriteFile(hFile, dynamic_cast<CFloor*>((*it).second)->Get_VecScale(), sizeof(_vec3), &dwByte, nullptr); //크기 저장

		}
		if (nullptr != dynamic_cast<CWall*>((*it).second)) // 좌우 벽
		{
			iNumber_Type = 1;
			WriteFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr); //타일 종류 번호 저장 1 = 좌우 벽

			iNumber = dynamic_cast<CWall*>((*it).second)->Get_Number();  // 이미지 번호 
			iTrigger = dynamic_cast<CWall*>((*it).second)->Get_Trigger();// 트리거 번호

			WriteFile(hFile, dynamic_cast<CWall*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr); //위치 저장
			WriteFile(hFile, dynamic_cast<CWall*>((*it).second)->Get_TileDirection(), sizeof(_vec3), &dwByte, nullptr); //바라보는 방향
			WriteFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr); // 이미지 번호 저장
			WriteFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr); // 트리거 번호 저장
		}
		if (nullptr != dynamic_cast<CWallTB*>((*it).second))// 앞뒤 벽
		{
			iNumber_Type = 2;
			WriteFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr); //타일 종류 번호 저장 2 = 앞뒤 벽

			iNumber = dynamic_cast<CWallTB*>((*it).second)->Get_Number();  // 이미지 번호 
			iTrigger = dynamic_cast<CWallTB*>((*it).second)->Get_Trigger();// 트리거 번호

			WriteFile(hFile, dynamic_cast<CWallTB*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr);  //위치 저장
			WriteFile(hFile, dynamic_cast<CWallTB*>((*it).second)->Get_TileDirection(), sizeof(_vec3), &dwByte, nullptr); //바라보는 방향 저장
			WriteFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr); // 이미지 번호 저장
			WriteFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr); // 트리거 번호 저장
		}
		if (nullptr != dynamic_cast<CMonsterTile*>((*it).second))
		{
			_vec3 vecTemp = { 0.f, 0.f, 0.f }; // 회전 or 방향벡터 값을 사용하지 경우 저장 양식을 위해 빈 값 적용

			iNumber_Type = 3;
			WriteFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr); //타일 종류 번호 저장 3 = 몬스터
			
			iNumber = dynamic_cast<CMonsterTile*>((*it).second)->Get_Number();  // 이미지 번호 
			iTrigger = dynamic_cast<CMonsterTile*>((*it).second)->Get_Trigger();// 트리거 번호

			WriteFile(hFile, dynamic_cast<CMonsterTile*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr);
			WriteFile(hFile, vecTemp, sizeof(_vec3), &dwByte, nullptr); //깡통(필요없지만 저장양식을 위해)
			WriteFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr);  // 이미지 번호 저장
			WriteFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr);  // 트리거 번호 저장

		}
		if (nullptr != dynamic_cast<CDoor*>((*it).second))
		{
			iNumber_Type = 4;
			WriteFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr);//타일 종류 번호 저장 4 = 문

			iNumber = dynamic_cast<CDoor*>((*it).second)->Get_Number();  // 이미지 번호 
			iTrigger = dynamic_cast<CDoor*>((*it).second)->Get_Trigger();// 트리거 번호

			WriteFile(hFile, dynamic_cast<CDoor*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr); //위치 저장
			WriteFile(hFile, dynamic_cast<CDoor*>((*it).second)->Get_TileDirection(), sizeof(_vec3), &dwByte, nullptr); //바라보는 방향 저장
			WriteFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr); // 이미지 번호 저장
			WriteFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr); // 트리거 번호 저장

			WriteFile(hFile, dynamic_cast<CDoor*>((*it).second)->Get_VecRot(), sizeof(_vec3), &dwByte, nullptr); //회전 저장
			WriteFile(hFile, dynamic_cast<CDoor*>((*it).second)->Get_VecScale(), sizeof(_vec3), &dwByte, nullptr); //크기 저장

		}
		if (nullptr != dynamic_cast<CItemTile*>((*it).second))
		{
			_vec3 vecTemp = { 0.f, 0.f, 0.f };

			iNumber_Type = 5;
			WriteFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr); //타일 종류 번호 저장 5 = 아이템

			iNumber = dynamic_cast<CItemTile*>((*it).second)->Get_Number();  // 이미지 번호
			iTrigger = dynamic_cast<CItemTile*>((*it).second)->Get_Trigger();// 트리거 번호

			WriteFile(hFile, dynamic_cast<CItemTile*>((*it).second)->Get_VecPos(), sizeof(_vec3), &dwByte, nullptr); //위치 저장
			WriteFile(hFile, vecTemp, sizeof(_vec3), &dwByte, nullptr); //깡통(필요없지만 저장양식을 위해)
			WriteFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr); // 이미지 번호 저장
			WriteFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr); // 트리거 번호 저장
		}
	}
	
	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Save 완료", _T("성공"), MB_OK);
}

void CMapCreate::MapLoad2(CLayer* _pLayer)
{

	// 현재 츨력되고 있는 레이어의 오브젝트 모두 제거
	multimap<const _tchar*, CGameObject*>::iterator it = _pLayer->Get_LayerObjects()->begin();
	_pLayer->Get_LayerObjects()->erase(it, _pLayer->Get_LayerObjects()->end());


	//1_TutorialStage
	//2_SlidingStage
	//3_FloorStage
	//4_BuildingStage
	//5_RobotBossStage

	HANDLE		hFile = CreateFile(L"../Data/3_FloorStage.txt",	// 파일 이름까지 포함된 경로
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
	_int iNumber_Type(0); // 바닥인지(0) 벽인지(1) 벽TB인지(2) 몬스터인지(3)  문(4) 아이템(5)
	_int iNumber(0); //이미지 번호
	_int iTrigger(0); //트리거 번호
	_vec3 pPos{};
	_vec3 pRot{};
	_vec3 pRotation{};
	_vec3 pScale{};


	while (true)
	{
		ReadFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr); // 바닥인지(0) 벽인지(1) 벽TB인지(2) 몬스터인지(3)  문(4) 아이템(5)
		ReadFile(hFile, &pPos, sizeof(_vec3), &dwByte, nullptr); // 포지션 값 저장
		ReadFile(hFile, &pRot, sizeof(_vec3), &dwByte, nullptr); // 회전 또는 바라보는 방향 값 저장
		ReadFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr); // 이게 그럼 몇번째 녀석인지
		ReadFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr); // 이게 그럼 몇번째 trigger인지//10.06

		if (0 == dwByte)
		{
			break;
		}

		if (iNumber_Type == 0)
		{
			Engine::CGameObject* pGameObject = nullptr;

			ReadFile(hFile, &pScale, sizeof(_vec3), &dwByte, nullptr); // 크기

			//pGameObject = CFloor::Create_InfoNumberTrigger2(m_pGraphicDev, pPos, pRot, iNumber, iTrigger);//10.06
			pGameObject = CFloor::Create_Info(m_pGraphicDev, pPos, pRot,pScale, iNumber, iTrigger);//10.06


			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"Floor", pGameObject);
		}
		if (iNumber_Type == 1)
		{
			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CWall::Create_InfoNumberDirectionTrigger2(m_pGraphicDev, pPos, pRot, iNumber, iTrigger);//10.06
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"Wall", pGameObject);
		}
		if (iNumber_Type == 2)
		{
			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CWallTB::Create_InfoNumberDirectionTrigger2(m_pGraphicDev, pPos, pRot, iNumber, iTrigger);//10.06
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"WallTB", pGameObject);
		}
		if (iNumber_Type == 3)
		{
			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CMonsterTile::Create_InfoNumberTrigger(m_pGraphicDev, pPos, iNumber, iTrigger);//10.06
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"MonsterTile", pGameObject);
		}
		if (iNumber_Type == 4)
		{
			Engine::CGameObject* pGameObject = nullptr;
			
			ReadFile(hFile, &pRotation, sizeof(_vec3), &dwByte, nullptr); // 회전
			ReadFile(hFile, &pScale, sizeof(_vec3), &dwByte, nullptr); // 크기

			//pGameObject = CDoor::Create_InfoNumberDirectionTrigger2(m_pGraphicDev, pPos, pRot, iNumber, iTrigger);//10.06
			pGameObject = CDoor::Create_Infoload(m_pGraphicDev, pPos, pRotation, pScale, iNumber, pRot, iTrigger);//10.06
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"Door", pGameObject);
		}
		if (iNumber_Type == 5)
		{
			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CItemTile::Create_InfoNumberTrigger(m_pGraphicDev, pPos, iNumber, iTrigger);//10.07
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"ItemTile", pGameObject);
		}
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


	SetMenu(); 
	SetFloor();
	SetWall();
	SetMonster();
	SetDoor();
	SetItem();

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


	ImGui::End();

}

void CMapCreate::SetMenu()
{
	!ImGui::CollapsingHeader("setting"); // gui 숨겼다 보여주는 bool 값
		//return;

	static _vec3 vecPosition = {};
	vecPosition.x =  m_vecPosition.x;
	vecPosition.y =  m_vecPosition.y;
	vecPosition.z =  m_vecPosition.z;
	ImGui::Text(u8"설치된 좌표 값");
	ImGui::SameLine(100.f, 0.f);
	ImGui::InputFloat3("##10", vecPosition, "%2.f");
	
	static _vec3 vecScale = {};
	m_vecScale.x = vecScale.x;
	m_vecScale.y = vecScale.y;
	m_vecScale.z = vecScale.z;
	ImGui::Text(u8"스케일 값");
	ImGui::SameLine(110.f, 0.f);
	ImGui::InputFloat3("##11", vecScale);

	static _vec3 vecRidian = {};
	m_vecRot.x = D3DXToRadian(vecRidian.x);
	m_vecRot.y = D3DXToRadian(vecRidian.y);
	m_vecRot.z = D3DXToRadian(vecRidian.z);
	ImGui::Text(u8"현재 회전 값");
	ImGui::SameLine(120.f, 0.0f);
	ImGui::InputFloat3("##12", vecRidian);

	static int iTriggerOffset = 0;
	ImGui::Text(u8"TriggerNumber값");
	ImGui::SameLine(130.f, 0.0f);
	ImGui::InputInt("#13", &iTriggerOffset);
	m_iTriggerNumber = iTriggerOffset;

	ImGui::NewLine();


}

void CMapCreate::SetFloor()
{

	bool ret = false;
	if (!ImGui::CollapsingHeader("Floors"))
	{
		return;
	}

	// 텍스쳐 선택
	ImGui::Text(u8"바닥 선택");
	int iCount(0);
	_int iTemp(0);
	map<const _tchar*, IDirect3DBaseTexture9*>::iterator it;
	for (it = m_mapImageFloor.begin(); it != m_mapImageFloor.end(); ++it)
	{
		char* pStr;
		int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, NULL, 0, NULL, NULL);
		pStr = new char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, pStr, iStrSize, 0, 0);

		ret = ImGui::ImageButton(pStr, (*it).second, ImVec2(50.f, 50.f));
		iTemp++;
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
			m_iNumber = iTemp;
		}

		delete[] pStr;
		pStr = nullptr;
	}
	ImGui::NewLine();

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
	_int iTemp(0);
	map<const _tchar*, IDirect3DBaseTexture9*>::iterator it;
	for (it = m_mapImageWall.begin(); it != m_mapImageWall.end(); ++it)
	{
		char* pStr;
		int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, NULL, 0, NULL, NULL);
		pStr = new char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, pStr, iStrSize, 0, 0);

		ret = ImGui::ImageButton(pStr, (*it).second, ImVec2(50.f, 50.f));
		iTemp++;
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
			m_iNumber = iTemp;
		}

		delete[] pStr;
		pStr = nullptr;
	}
	ImGui::NewLine();
}

void CMapCreate::SetMonster()
{
	bool ret = false;
	if (!ImGui::CollapsingHeader("Monsters"))
	{
		return;
	}

	ImGui::Text(u8"Monster 선택");
	int iCount(0);
	_int iTemp(0);
	map<const _tchar*, IDirect3DBaseTexture9*>::iterator it;
	for (it = m_mapImageMonster.begin(); it != m_mapImageMonster.end(); ++it)
	{
		char* pStr;
		int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, NULL, 0, NULL, NULL);
		pStr = new char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, pStr, iStrSize, 0, 0);

		ret = ImGui::ImageButton(pStr, (*it).second, ImVec2(50.f, 50.f));
		iTemp++;
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
			m_iNumber = iTemp;
		}

		delete[] pStr;
		pStr = nullptr;
	}
	ImGui::NewLine();
}

void CMapCreate::SetDoor()
{
	bool ret = false;
	if (!ImGui::CollapsingHeader("Doors"))
	{
		return;
	}

	ImGui::Text(u8"문 선택");
	int iCount(0);
	_int iTemp(0);
	map<const _tchar*, IDirect3DBaseTexture9*>::iterator it;
	for (it = m_mapImageDoor.begin(); it != m_mapImageDoor.end(); ++it)
	{
		char* pStr;
		int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, NULL, 0, NULL, NULL);
		pStr = new char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, pStr, iStrSize, 0, 0);

		ret = ImGui::ImageButton(pStr, (*it).second, ImVec2(50.f, 50.f));
		iTemp++;
		if (iTemp == 8)
		{
			iTemp = 21;
		}
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
			m_iNumber = iTemp;
		}

		delete[] pStr;
		pStr = nullptr;
	}
	ImGui::NewLine();
}

void CMapCreate::SetItem()
{
	bool ret = false;
	if (!ImGui::CollapsingHeader("Items"))
	{
		return;
	}

	ImGui::Text(u8"아이템 선택");
	int iCount(0);
	_int iTemp(0);
	map<const _tchar*, IDirect3DBaseTexture9*>::iterator it;
	for (it = m_mapImageItem.begin(); it != m_mapImageItem.end(); ++it)
	{
		char* pStr;
		int iStrSize = WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, NULL, 0, NULL, NULL);
		pStr = new char[iStrSize];
		WideCharToMultiByte(CP_ACP, 0, (*it).first, -1, pStr, iStrSize, 0, 0);

		ret = ImGui::ImageButton(pStr, (*it).second, ImVec2(50.f, 50.f));
		iTemp++;
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
			m_iNumber = iTemp;
		}

		delete[] pStr;
		pStr = nullptr;
	}
	ImGui::NewLine();
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
HRESULT CMapCreate::Ready_Texture_MonsterInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt)
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
			m_mapImageMonster.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		case TEXTUREID::TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			m_mapImageMonster.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		default:
			break;
		}

	}

	return S_OK;
}
HRESULT CMapCreate::Ready_Texture_DoorInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt)
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
			//m_vecImageDoor.push_back(_pComponentTag);
			m_mapImageDoor.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		case TEXTUREID::TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			//m_vecImageDoor.push_back(_pComponentTag);
			m_mapImageDoor.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		default:
			break;
		}

	}

	return S_OK;
}

HRESULT CMapCreate::Ready_Texture_ItemInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt)
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
			m_mapImageItem.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
			break;
		case TEXTUREID::TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			m_mapImageItem.insert(pair<const _tchar*, IDirect3DBaseTexture9*>(_pComponentTag, pTexture));
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
