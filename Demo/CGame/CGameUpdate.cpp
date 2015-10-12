#include "../CGame.h"

void CGame::Update(float dt)
{
#pragma region inputDevice
	this->inputDevice.Update();
#pragma endregion

#pragma region mario
	this->mario.Update(dt, &this->inputDevice);
#pragma endregion
}