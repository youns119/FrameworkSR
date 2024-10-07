#pragma once
#include "Scene.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "Engine_Enum.h"

class CMapCreate : public CScene
{
private:
	explicit CMapCreate(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMapCreate();

public:
	static CMapCreate* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_Scene();
	virtual _int Update_Scene(const _float& _fTimeDelta);
	virtual void LateUpdate_Scene();
	virtual void Render_Scene();

private:
	//맵툴 기본 레이어들
	HRESULT Ready_LightInfo();
	HRESULT Ready_Layer_ToolEnvironment(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* _pLayerTag);

	//타일 저장할 레이어
	HRESULT Ready_Layer_PickingTile(const _tchar* _pLayerTag);
	//타일 저장한 레이어 찾아서 가져오기
	CLayer* Find_Layer(const _tchar* _pLayerTag);

	//바닥 오브젝트 생성
	HRESULT Create_Layer_PickingFloor(CLayer* _pLayer);
	//벽 오브젝트 생성
	HRESULT Create_Layer_PickingWall(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection);
	//벽 오브젝트 생성
	HRESULT Create_Layer_PickingWallTB(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection);
	//Monster 오브젝트 생성
	HRESULT Create_Layer_PickingMonster(CLayer* _pLayer);

	//Door 오브젝트 생성
	HRESULT Create_Layer_PickingDoor(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection);

	// 가이드 지형 버퍼 가져오기
	_vec3 TilePiking_OnTerrain(int _iTile);
	// 가이드 지형의 버퍼와 마우스 좌표를 통해 [바닥] 타일 생성
	_vec3 FloorCreate_OnTerrain(HWND _hWnd, CGuideTex* _pGuideBufferCom);


	// 가이드 지형의 버퍼와 마우스 좌표를 통해 [벽] 타일 생성
	//left,right
	_vec3 WallCreate_OnTerrain1(HWND _hWnd, CGuideTex* _pGuideBufferCom);
	//top,bottom
	_vec3 WallCreate_OnTerrain2(HWND _hWnd, CGuideTex* _pGuideBufferCom);

	// 가이드 지형의 버퍼와 마우스 좌표를 통해 [Monster] 타일 생성
	_vec3 MonsterCreate_OnTerrain(HWND _hWnd, CGuideTex* _pGuideBufferCom);

	// 바닥 버퍼와 마우스 좌표를 통해 삭제
	HRESULT PickingTile_PosDelete(CLayer* _pLayer, const _tchar* _TileTag);
	// 이미 타일이 있으면 생성 X
	void PickingTile_PosCheck(CLayer* _pLayer, const _tchar* _TileTag);


	void MapSave2(CLayer* _pLayer);
	void MapLoad2(CLayer* _pLayer);


	//imgui 함수
	void ShowGUI();
	void SetMenu();
	void SetFloor();//imgui바닥 
	void SetWall();//imgui벽
	void SetMonster();//imgui몬스터
	void SetDoor();//imgui Door

	HRESULT Ready_Texture_FloorInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui 출력할 바닥 세팅
	HRESULT Ready_Texture_WallInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui 출력할 벽 세팅
	HRESULT Ready_Texture_MonsterInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui 출력할 Monster 세팅
	HRESULT Ready_Texture_DoorInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui 출력할 Door 세팅




private:
	virtual void Free();


private:
	bool m_bCreateCheck;//타일 생성 여부 체크
	_vec3 m_vecRot;//바닥 회전 적용
	int m_iRidian;//바닥 회전 값
	float m_fHeight;// 가이드 지형 높이
	int m_iByte;

	_int m_iTriggerNumber; // 이거 지금 몇번째 트리거에서 발동시킬것이냐 에 관한 int 값//10.06
	bool m_bGuiHovered;

	const _tchar* m_ImageName;
	_int m_iNumber;


	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageFloor;
	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageWall;
	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageMonster;
	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageDoor;

};
