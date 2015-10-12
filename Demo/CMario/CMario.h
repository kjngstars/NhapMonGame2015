#pragma once

#include "../CObject.h"
#include "../DX/CDXInput.h"

class CMario :public CObject
{
private:
	enum MarioType
	{
		Mario = 0,
		BMario = 48,
		FMario = 112,
	};
	MarioType type = MarioType::Mario;

	enum MarioState
	{
		Normal,
		ToBig,
		ToFire,
		ToSmall
	};
	MarioState state = MarioState::Normal;

	enum MarioMoveType {
		Standing,
		Running,
		Jumping
	};
	MarioMoveType moveType = MarioMoveType::Standing;

	enum MarioJumpType {
		JumpType0,
		JumpType1,
		JumpType2
	};
	MarioJumpType jumpType = MarioJumpType::JumpType0;

	float delayTime = 120.0f;
	float elapsedTime = 0.0f;
	float ghostTime = 0.0f;

	int direction = 1;
	D3DXVECTOR2 velocity = { 0.0f,0.0f };
	bool isFDVelocityX = false;

	bool isShotting = false;

	int frame = 0;

	RECT sourceRect = RECT{ 0, 0, 0, 0 };
	D3DXCOLOR color = { 255,255,255,255 };

	void UpdateState(CDXInput* inputDevice);
	void UpdateVelocity(float elapsedTime, CDXInput* inputDevice);
	void UpdateDelayTime();
	void UpdateGhostTime(float elapsedTime);
	void UpdateFrame(float elapsedTime, CDXInput* inputDevice);
	void UpdateScale();
	void UpdateSourceRect();

	float GetWSrcRect() { return float(this->sourceRect.right - this->sourceRect.left); }
	float GetHSrcRect() { return float(this->sourceRect.bottom - this->sourceRect.top); }

	void CheckShotting(CDXInput* inputDevice);

public:
	CMario() {}

	void Initialize(IDirect3DDevice9* pD3DDevice);
	void Update(float elapsedTime, CDXInput* inputDevice);
	void Render(ID3DXSprite* pSprite);

	void StopJump();

	void Grow(float elapsedTime);
	void GrowUp();

	void Fire(float elapsedTime);
	void ToFMario();

	void Shrink(float elapsedTime);
	void ShrinkDown();

	int GetFrame() { return this->frame; }
	float GetDelayTime() { return this->delayTime; };
	int GetDirection() { return this->direction; }
	D3DXVECTOR2 GetVelocity() { return this->velocity; };
	int GetState() { return this->moveType; }
};