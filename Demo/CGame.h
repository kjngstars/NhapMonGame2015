#pragma once

#include "CGraphics.h"
#include "GameConfig.h"
#include "CObject.h"
#include "DX\CDXInput.h"
#include "CMario\CMario.h"
#include "CMap.h"

class CGame
{
private:
	CDXInput inputDevice;
	ID3DXFont* pDefaultFont = 0;

	CMap _map;

	CMario mario;
	
public:
	bool isPaused = false;

	CGame();
	~CGame();

	void GameInit();

	bool IsLostDevice();

	void ToggleFullScreen();
	void OnLostDevice();
	void OnResetDevice();

	void Update(float dt);
	void Render();
};
