#include "../CGame.h"

void CGame::Update(float dt, CDXInput* inputDevice)
{
#pragma region luigi
	this->luigi.Update(dt, inputDevice);
#pragma endregion
}