#include "../CGame.h"


CGame::CGame()
{
}

CGame::~CGame()
{
	SAFE_RELEASE(this->pDefaultFont);
}

void CGame::GameInit()
{
	//map
	_map.init("./content/map/map1-1.txt", "./content/map/tileset1.png");

#pragma region inputDevice
	this->inputDevice.Initialize(
		CGraphics::GetInstancePtr()->GetHInst(),
		CGraphics::GetInstancePtr()->GetWND());
#pragma endregion

#pragma region pDefaultFont
	SAFE_RELEASE(this->pDefaultFont);
	HR(D3DXCreateFont(
		CGraphics::GetInstancePtr()->GetDevice(),
		20, 0,
		FW_NORMAL, 0, false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"Times New Roman",
		&this->pDefaultFont));
#pragma endregion

#pragma region mario
	this->mario.Initialize(CGraphics::GetInstancePtr()->GetDevice());
#pragma endregion
}

bool CGame::IsLostDevice()
{
	HRESULT hr = CGraphics::GetInstancePtr()->GetDevice()->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST)
		return true;
	else if (hr == D3DERR_DRIVERINTERNALERROR)
	{
		MessageBox(0, "[CGame::IsLostDevice] Internal Driver Error", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		this->OnLostDevice();

		HR(CGraphics::GetInstancePtr()->GetDevice()->Reset(
			CGraphics::GetInstancePtr()->GetD3DPP()));

		this->OnResetDevice();
		return false;
	}
	else
		return false;
}

void CGame::ToggleFullScreen()
{
	if (CGraphics::GetInstancePtr()->GetD3DPP()->Windowed)
	{
		CGraphics::GetInstancePtr()->GetD3DPP()->Windowed = false;

		SetWindowLongPtr(
			CGraphics::GetInstancePtr()->GetWND()
			, GWL_STYLE, WS_POPUP);

		SetWindowPos(
			CGraphics::GetInstancePtr()->GetWND(), HWND_TOP,
			0, 0,
			GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		CGraphics::GetInstancePtr()->GetD3DPP()->Windowed = true;

		SetWindowLongPtr(CGraphics::GetInstancePtr()->GetWND(), GWL_STYLE,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

		SetWindowPos(
			CGraphics::GetInstancePtr()->GetWND(), HWND_TOP,
			100, 100,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SWP_NOZORDER | SWP_SHOWWINDOW);

	
		this->OnLostDevice();

		HR(CGraphics::GetInstancePtr()->GetDevice()->Reset(
			CGraphics::GetInstancePtr()->GetD3DPP()));
		
		this->OnResetDevice();
	}
}

void CGame::OnLostDevice()
{
	HR(CGraphics::GetInstancePtr()->GetSprite()->OnLostDevice());
	HR(this->pDefaultFont->OnLostDevice());
}

void CGame::OnResetDevice()
{
	HR(CGraphics::GetInstancePtr()->GetSprite()->OnResetDevice());
	HR(this->pDefaultFont->OnResetDevice());
}