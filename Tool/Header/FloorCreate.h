#pragma once

#include "Tool_Include.h"

class CToolView;
class CFloorCreate
{
public:
	CFloorCreate();
	~CFloorCreate();

public:
	void		Set_MainView(CToolView* pMainView) { m_MainView = pMainView; }

public:
	HRESULT		Initialize();
	void		Update();
	void		Render();
	void		Release();

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_MainView;

};

// 직선의 방정식을 이용하여 타일 충돌을 해라

// y = ax + b 
// 
// a : 기울기(y 증가량 / x 증가량)
// b : y절편 (x값이 0일 때의 y 값)