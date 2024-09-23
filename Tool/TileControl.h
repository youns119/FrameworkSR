#pragma once
#include "FloorInfo.h"
#include "WallInfo.h"

class CTileControl
{

	// 테스트로 있는 클래스...이상하게 맞아요...
public:
	CTileControl();
	~CTileControl();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void Release();

//
//private:
//	CFloorInfo Add_Tile();
//
//private:
//	vector<CFloorInfo*> m_vecFloorList;
//	//vector<CWallInfo*> m_vecWallList;


};
