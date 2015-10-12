#include "CMario.h"

void CMario::Initialize(IDirect3DDevice9* pD3DDevice)
{
	D3DXIMAGE_INFO info;
	HR(D3DXGetImageInfoFromFile("./Content/Mario/Mario.png", &info));
	D3DXCreateTextureFromFileEx(
		pD3DDevice, "./Content/Mario/Mario.png",
		info.Width, info.Height, D3DX_DEFAULT, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT,
		0, &info, 0, &this->_texture);

	this->_rotate = 0.0f;
	this->_scale = { 1.0f, 1.0f };
	this->_position = { 0.0f,600.0f };

	this->type = MarioType::Mario;
	this->direction = 1;
	this->sourceRect = { 0,0,36,48 };
}

void CMario::StopJump()
{
	this->moveType = MarioMoveType::Running;
	this->isFDVelocityX = false;
}

void CMario::GrowUp()
{
	if (this->type != MarioType::Mario)
		this->state = MarioState::Normal;
	else
		this->state = MarioState::ToBig;
}

void CMario::ToFMario()
{
	if (this->type == MarioType::FMario)
		this->state = MarioState::Normal;
	else
		this->state = MarioState::ToFire;
}

void CMario::ShrinkDown()
{
	if (this->type == MarioType::Mario)
		this->state = MarioState::Normal;
	else
		this->state = MarioState::ToSmall;
}

void CMario::CheckShotting(CDXInput* inputDevice)
{
	if (this->type == MarioType::FMario)
	{
		if (inputDevice->KeyPress(DIK_A))
			this->isShotting = true;
	}
	else
		this->isShotting = false;
}