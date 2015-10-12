#include "CMario.h"
#include <math.h>

#define LEFT_DIRECTION -1
#define RIGHT_DIRECTION 1

#define II_DELAYTIME 12.5f
#define ID_DELAYTIME 12.5f
#define N_DELAYTIME 120.0f
#define F_DELAYTIME 70.0f

#define II_VELOCITYX 4.0f
#define ID_VELOCITYX 4.0f
#define N_VELOCITYX 16.0f
#define F_VELOCITYX 32.0f

#define II_VELOCITYY 14.0f
#define S_VELOCITYY -49.0f
#define N_VELOCITYY -56.0f

#define IB_VELOCITYY -7.0f

#define MASS 56.0f

#define S_MARIO_W 36
#define S_MARIO_H 48

#define B_MARIO_W 40
#define B_MARIO_H 64

static const int Frame_S_JumpType2[4] = { 10,0,11,0 };

static const int Frame_B_Running0[3] = { 4,3,0 };
static const int Frame_B_Running1[3] = { 7,6,5 };
static const int Frame_B_JumpType2[4] = { 12,0,13,0 };

static const int Frame_Grow[2] = { 0,15 };

int GetSignNum(float x)
{
	if (x < 0)
		return -1;
	else
		return 1;
}

void CMario::Update(float elapsedTime, CDXInput* inputDevice)
{
	switch (this->state)
	{
	case MarioState::Normal:
		this->UpdateState(inputDevice);
		this->UpdateVelocity(elapsedTime, inputDevice);
		this->UpdateDelayTime();
		this->UpdateFrame(elapsedTime, inputDevice);
		this->UpdateScale();
		this->UpdateSourceRect();

		this->_position += this->velocity * (elapsedTime / N_DELAYTIME);
		this->UpdateGhostTime(elapsedTime);
		break;

	case MarioState::ToBig:
		this->Grow(elapsedTime);

		break;

	case MarioState::ToFire:
		this->Fire(elapsedTime);

		break;

	case MarioState::ToSmall:
		this->Shrink(elapsedTime);

		break;
	}

	if (this->_position.y > 600.0f)
	{
		this->_position.y = 600.0f;
		this->velocity.y = 0.0f;
		this->StopJump();
	}

	if (inputDevice->KeyPress(DIK_F))
	{
		if (this->type == MarioType::Mario)
			this->GrowUp();
		else
			this->ShrinkDown();
	}

	if (inputDevice->KeyPress(DIK_C))
	{
		this->ToFMario();
	}
}

void CMario::UpdateState(CDXInput* inputDevice)
{
	this->CheckShotting(inputDevice);

	switch (this->moveType)
	{
	case MarioMoveType::Standing:
#pragma region ToRunning

		if (inputDevice->KeyDown(DIK_LEFT))
		{
			this->direction = LEFT_DIRECTION;
			this->elapsedTime = this->delayTime;
			this->moveType = MarioMoveType::Running;
		}
		else if (inputDevice->KeyDown(DIK_RIGHT))
		{
			this->direction = RIGHT_DIRECTION;
			this->elapsedTime = this->delayTime;
			this->moveType = MarioMoveType::Running;
		}
#pragma endregion

#pragma region ToJumping
		if (inputDevice->KeyPress(DIK_S))
		{
			this->elapsedTime = this->delayTime;
			this->moveType = MarioMoveType::Jumping;

			this->velocity.y = N_VELOCITYY;
			this->jumpType = MarioJumpType::JumpType0;
		}
		else if (inputDevice->KeyPress(DIK_D))
		{
			this->elapsedTime = this->delayTime;
			this->moveType = MarioMoveType::Jumping;

			this->velocity.y = S_VELOCITYY;
			this->jumpType = MarioJumpType::JumpType2;
		}
#pragma endregion

		break;

	case MarioMoveType::Running:
#pragma region ToJumping
		if (inputDevice->KeyPress(DIK_S))
		{
			this->elapsedTime = this->delayTime;
			this->moveType = MarioMoveType::Jumping;

			if (std::abs(this->velocity.x) == F_VELOCITYX)
			{
				this->velocity.y = N_VELOCITYY + IB_VELOCITYY;
				this->jumpType = MarioJumpType::JumpType1;
			}
			else
			{
				this->velocity.y = N_VELOCITYY;
				this->jumpType = MarioJumpType::JumpType0;
			}
		}
		else if (inputDevice->KeyPress(DIK_D))
		{
			this->elapsedTime = this->delayTime;
			this->moveType = MarioMoveType::Jumping;

			if (std::abs(this->velocity.x) == F_VELOCITYX)
				this->velocity.y = S_VELOCITYY + IB_VELOCITYY;
			else
				this->velocity.y = S_VELOCITYY;

			this->jumpType = MarioJumpType::JumpType2;
		}
#pragma endregion

#pragma region ToStanding
		if (this->velocity.x == 0 &&
			this->velocity.y == 0)
			this->moveType = MarioMoveType::Standing;
#pragma endregion

		break;

	case MarioMoveType::Jumping:

		break;
	}
}

void CMario::UpdateVelocity(float elapsedTime, CDXInput* inputDevice)
{
	switch (this->moveType)
	{
#pragma region Standing
	case MarioMoveType::Standing:
		if (this->velocity.x != 0.0f)
			this->velocity.x = 0.0f;

		if (this->velocity.y != 0.0f)
			this->velocity.y = 0.0f;

		if (this->isFDVelocityX)
			this->isFDVelocityX = false;

		break;
#pragma endregion

#pragma region Running
	case MarioMoveType::Running:
		if (inputDevice->KeyDown(DIK_LEFT) ||
			inputDevice->KeyDown(DIK_RIGHT) &&
			!this->isFDVelocityX)
		{
		Running_Increase_VelocityX:
			float temp = std::abs(this->velocity.x);

			if (inputDevice->KeyDown(DIK_LEFT))
				this->direction = LEFT_DIRECTION;
			else if (inputDevice->KeyDown(DIK_RIGHT))
				this->direction = RIGHT_DIRECTION;

			if ((this->direction * this->velocity.x < 0.0f) ||
				inputDevice->KeyDown(DIK_DOWN))
			{
				this->isFDVelocityX = true;
				goto Running_Decrease_VelocityX;
			}

			if (!inputDevice->KeyDown(DIK_A))
			{
				if (temp < N_VELOCITYX)
				{
					this->velocity.x += this->direction *
						II_VELOCITYX * (elapsedTime / N_DELAYTIME);

					if (std::abs(this->velocity.x) > N_VELOCITYX)
						this->velocity.x = this->direction * N_VELOCITYX;
				}
				else if (temp > N_VELOCITYX)
					goto Running_Decrease_VelocityX;
			}
			else
			{
				if (temp < F_VELOCITYX)
					this->velocity.x += this->direction *
					II_VELOCITYX * (elapsedTime / N_DELAYTIME);

				if (std::abs(this->velocity.x)>F_VELOCITYX)
					this->velocity.x = this->direction * F_VELOCITYX;
			}
		}
		else
		{
		Running_Decrease_VelocityX:
			float temp = std::abs(this->velocity.x);

			if (temp > 0.0f)
			{
				if (this->isFDVelocityX)
					temp -= ID_VELOCITYX * 1.5f * (elapsedTime / N_DELAYTIME);
				else
					temp -= ID_VELOCITYX * (elapsedTime / N_DELAYTIME);

				if (temp < 0.0f)
				{
					this->isFDVelocityX = false;
					this->velocity.x = 0.0f;
				}
				else
					this->velocity.x = GetSignNum(this->velocity.x) * temp;
			}
		}

		if (this->velocity.y < MASS)
		{
			this->velocity.y += II_VELOCITYY * (elapsedTime / N_DELAYTIME);

			if (this->velocity.y > MASS)
				this->velocity.y = MASS;
		}

		break;
#pragma endregion

#pragma region Jumping
	case MarioMoveType::Jumping:
		if (inputDevice->KeyDown(DIK_LEFT) ||
			inputDevice->KeyDown(DIK_RIGHT))
		{
		Jumping_Increase_VelocityX:
			float temp = std::abs(this->velocity.x);

			if (inputDevice->KeyDown(DIK_LEFT))
				this->direction = LEFT_DIRECTION;
			else if (inputDevice->KeyDown(DIK_RIGHT))
				this->direction = RIGHT_DIRECTION;

			if (this->direction * this->velocity.x < 0.0f)
				goto Jumping_Decrease_VelocityX;

			if (!inputDevice->KeyDown(DIK_A))
			{
				if (temp < N_VELOCITYX)
				{
					this->velocity.x += this->direction *
						II_VELOCITYX * (elapsedTime / N_DELAYTIME);

					if (std::abs(this->velocity.x) > N_VELOCITYX)
						this->velocity.x = this->direction * N_VELOCITYX;
				}
				else if (temp > N_VELOCITYX)
					goto Jumping_Decrease_VelocityX;
			}
			else
			{
				if (temp < F_VELOCITYX)
					this->velocity.x += this->direction *
					II_VELOCITYX * (elapsedTime / N_DELAYTIME);

				if (std::abs(this->velocity.x)>F_VELOCITYX)
					this->velocity.x = this->direction * F_VELOCITYX;
			}
		}
		else
		{
		Jumping_Decrease_VelocityX:
			float temp = std::abs(this->velocity.x);

			if (temp > 0.0f)
			{
				if (this->direction * this->velocity.x < 0.0f)
					temp -= ID_VELOCITYX * 1.5f * (elapsedTime / N_DELAYTIME);
				else
					temp -= ID_VELOCITYX * (elapsedTime / N_DELAYTIME);

				if (temp < 0.0f)
				{
					this->isFDVelocityX = false;
					this->velocity.x = 0.0f;
				}
				else
					this->velocity.x = GetSignNum(this->velocity.x) * temp;
			}
		}

		if (this->velocity.y < MASS)
		{
			if (this->velocity.y < 0.0f)
			{
				if ((!inputDevice->KeyDown(DIK_S) &&
					this->jumpType == MarioJumpType::JumpType0) ||
					(!inputDevice->KeyDown(DIK_D) &&
						this->jumpType == MarioJumpType::JumpType2))
					this->velocity.y += II_VELOCITYY * (elapsedTime / N_DELAYTIME);
			}

			this->velocity.y += II_VELOCITYY * (elapsedTime / N_DELAYTIME);

			if (this->velocity.y > MASS)
				this->velocity.y = MASS;
		}

		break;
#pragma endregion
	}
}

void CMario::UpdateDelayTime()
{
	if (std::abs(this->velocity.x) > N_VELOCITYX)
	{
		if (this->delayTime > F_DELAYTIME)
		{
			this->delayTime -= ID_DELAYTIME*
				(elapsedTime / N_DELAYTIME);

			if (this->delayTime < F_DELAYTIME)
				this->delayTime = F_DELAYTIME;
		}
	}
	else
	{
		if (this->delayTime < N_DELAYTIME)
		{
			this->delayTime += II_DELAYTIME*
				(elapsedTime / N_DELAYTIME);

			if (this->delayTime > N_DELAYTIME)
				this->delayTime = N_DELAYTIME;
		}
	}
}

void CMario::UpdateGhostTime(float elapsedTime)
{
	if (this->ghostTime > 0.0f)
	{
		int(this->ghostTime / N_DELAYTIME) % 2 == 0 ? this->color.a = 0 : this->color.a = 255;

		this->ghostTime -= elapsedTime;

		if (this->ghostTime < 0.0f)
		{
			this->ghostTime = 0.0f;

			if (this->color.a != 255) this->color.a = 255;
		}
	}
}

void CMario::UpdateFrame(float elapsedTime, CDXInput* inputDevice)
{
	if (this->type == MarioType::Mario)
	{
		switch (this->moveType)
		{
#pragma region Standing
		case MarioMoveType::Standing:
			if (inputDevice->KeyDown(DIK_DOWN))
				this->frame = 1;
			else if (inputDevice->KeyDown(DIK_UP))
				this->frame = 2;
			else
				this->frame = 0;
			this->elapsedTime = 0.0f;

			break;
#pragma endregion

#pragma region Running
		case MarioMoveType::Running:
			this->elapsedTime += elapsedTime;

			if (this->elapsedTime >= this->delayTime)
			{
				if (inputDevice->KeyDown(DIK_DOWN))
					this->frame = 1;
				else if (this->isFDVelocityX)
					this->frame = 6;
				else if (std::abs(this->velocity.x) == F_VELOCITYX)
					this->frame == 4 ? this->frame = 5 : this->frame = 4;
				else
					this->frame == 0 ? this->frame = 3 : this->frame = 0;
				this->elapsedTime = 0.0f;
			}

			break;
#pragma endregion

#pragma region Jumping
		case MarioMoveType::Jumping:
			switch (this->jumpType)
			{
			case MarioJumpType::JumpType0:
				if (this->frame != 1)
					this->velocity.y < 0 ? this->frame = 7 : this->frame = 8;

				break;

			case MarioJumpType::JumpType1:
				this->frame = 9;

				break;

			case MarioJumpType::JumpType2:
				static int i = 0;

				this->elapsedTime += elapsedTime;

				if (this->elapsedTime >= this->delayTime / 2.0f)
				{
					switch (i)
					{
					case 1:
						this->_scale.x = -std::abs(this->_scale.x);
						break;

					case 3:
						this->_scale.x = std::abs(this->_scale.x);
						break;
					}

					this->frame = Frame_S_JumpType2[i];

					i++;
					if (i > 3) i = 0;

					this->elapsedTime = 0.0f;
				}
			}

			break;
#pragma endregion
		}
	}
	else
	{
		switch (this->moveType)
		{
#pragma region Standing
		case MarioMoveType::Standing:

			if (this->isShotting && !inputDevice->KeyDown(DIK_DOWN))
			{
				this->frame = 14;

				this->elapsedTime += elapsedTime;

				if (this->elapsedTime > this->delayTime)
				{
					this->isShotting = false;

					this->elapsedTime = 0.0f;
				}
			}
			else
			{
				if (inputDevice->KeyDown(DIK_DOWN))
					this->frame = 1;
				else if (inputDevice->KeyDown(DIK_UP))
					this->frame = 2;
				else
					this->frame = 0;

				this->isShotting = false;
				this->elapsedTime = 0.0f;
			}

			break;
#pragma endregion

#pragma region Running
		case MarioMoveType::Running:
			this->elapsedTime += elapsedTime;

			if (this->isShotting && !inputDevice->KeyDown(DIK_DOWN))
			{
				this->frame = 14;

				if (this->elapsedTime >= this->delayTime)
				{
					this->isShotting = false;

					this->elapsedTime = 0.0f;
				}
			}
			else if (this->elapsedTime >= this->delayTime)
			{
				static int i = 0;

				if (inputDevice->KeyDown(DIK_DOWN))
					this->frame = 1;
				else if (this->isFDVelocityX)
					this->frame = 8;
				else if (std::abs(this->velocity.x) == F_VELOCITYX)
					this->frame = Frame_B_Running1[i];
				else
					this->frame = Frame_B_Running0[i];

				i++;
				if (i > 2) i = 0;

				this->elapsedTime = 0.0f;
			}

			break;
#pragma endregion

#pragma region Jumping
		case MarioMoveType::Jumping:

			switch (this->jumpType)
			{
			case MarioJumpType::JumpType0:
				if (this->frame != 1)
				{
					if (this->isShotting)
					{
						this->frame = 11;

						this->elapsedTime += elapsedTime;

						if (this->elapsedTime >= this->delayTime * 4.0f)
						{
							this->isShotting = false;

							this->elapsedTime = 0.0f;
						}
					}
					else
						this->velocity.y < 0 ? this->frame = 9 : this->frame = 10;
				}

				break;

			case MarioJumpType::JumpType1:
				this->frame = 11;

				break;

			case MarioJumpType::JumpType2:
				this->elapsedTime += elapsedTime;

				if (this->elapsedTime >= this->delayTime / 3.0f)
				{
					static int i = 0;

					switch (i)
					{
					case 1:
						this->_scale.x = -std::abs(this->_scale.x);
						break;

					case 3:
						this->_scale.x = std::abs(this->_scale.x);
						break;
					}

					this->frame = Frame_B_JumpType2[i];

					i++;
					if (i > 3) i = 0;

					this->elapsedTime = 0.0f;
				}
			}

			break;
#pragma endregion
		}
	}
}

void CMario::UpdateScale()
{
	if (!(this->moveType == MarioMoveType::Jumping &&
		this->jumpType == MarioJumpType::JumpType2))
		this->_scale.x = this->direction*std::abs(this->_scale.x);
}

void CMario::UpdateSourceRect()
{
	if (this->type == MarioType::Mario)
	{
		this->sourceRect.left = this->frame * S_MARIO_W;
		this->sourceRect.right = this->sourceRect.left + S_MARIO_W;
		this->sourceRect.top = this->type;
		this->sourceRect.bottom = this->sourceRect.top + S_MARIO_H;
	}
	else
	{
		this->sourceRect.left = this->frame * B_MARIO_W;
		this->sourceRect.right = this->sourceRect.left + B_MARIO_W;
		this->sourceRect.top = this->type;
		this->sourceRect.bottom = this->sourceRect.top + B_MARIO_H;
	}
}

void CMario::Grow(float elapsedTime)
{
	this->elapsedTime += elapsedTime;

	if (this->elapsedTime >= N_DELAYTIME)
	{
		static int i = 0;

		int k = 0;

		i % 2 == 0 ? k = 0 : k = 15;

		if (i < 7)
		{
			if (k == 0)
			{
				this->sourceRect.left = k*S_MARIO_W;
				this->sourceRect.right = this->sourceRect.left + S_MARIO_W;
				this->sourceRect.top = MarioType::Mario;
				this->sourceRect.bottom = this->sourceRect.top + S_MARIO_H;
			}
			else
			{
				this->sourceRect.left = k*B_MARIO_W;
				this->sourceRect.right = this->sourceRect.left + B_MARIO_W;
				this->sourceRect.top = MarioType::BMario;
				this->sourceRect.bottom = this->sourceRect.top + B_MARIO_H;
			}
		}
		else
		{
			this->sourceRect.left = k*B_MARIO_W;
			this->sourceRect.right = this->sourceRect.left + B_MARIO_W;
			this->sourceRect.top = MarioType::BMario;
			this->sourceRect.bottom = this->sourceRect.top + B_MARIO_H;
		}

		i++;
		if (i > 12)
		{
			i = 0;
			this->type = MarioType::BMario;
			this->state = MarioState::Normal;
		}

		this->elapsedTime = 0.0f;
	}
}

void CMario::Shrink(float elapsedTime)
{
	this->elapsedTime += elapsedTime;

	if (this->elapsedTime >= N_DELAYTIME)
	{
		static int i = 0;

		int k = 0;

		i % 2 == 0 ? k = 0 : k = 15;

		if (i > 3)
		{
			if (k == 0)
			{
				this->sourceRect.left = k*S_MARIO_W;
				this->sourceRect.right = this->sourceRect.left + S_MARIO_W;
				this->sourceRect.top = MarioType::Mario;
				this->sourceRect.bottom = this->sourceRect.top + S_MARIO_H;
			}
			else
			{
				this->sourceRect.left = k*B_MARIO_W;
				this->sourceRect.right = this->sourceRect.left + B_MARIO_W;
				this->sourceRect.top = MarioType::BMario;
				this->sourceRect.bottom = this->sourceRect.top + B_MARIO_H;
			}
		}
		else
		{
			this->sourceRect.left = k*B_MARIO_W;
			this->sourceRect.right = this->sourceRect.left + B_MARIO_W;
			this->sourceRect.top = MarioType::BMario;
			this->sourceRect.bottom = this->sourceRect.top + B_MARIO_H;
		}

		i++;
		if (i > 12)
		{
			i = 0;
			this->ghostTime = N_DELAYTIME * 5.0f;
			this->type = MarioType::Mario;
			this->state = MarioState::Normal;
		}

		this->elapsedTime = 0.0f;
	}
}

void CMario::Fire(float elapsedTime)
{
	this->elapsedTime += elapsedTime;

	if (this->elapsedTime >= N_DELAYTIME)
	{
		static int i = 0;

		this->type != MarioType::FMario ?
			this->type = MarioType::FMario : this->type = MarioType::BMario;

		this->sourceRect.left = this->frame * B_MARIO_W;
		this->sourceRect.right = this->sourceRect.left + B_MARIO_W;
		this->sourceRect.top = this->type;
		this->sourceRect.bottom = this->sourceRect.top + B_MARIO_H;

		i++;
		if (i > 5)
		{
			i = 0;
			this->type = MarioType::FMario;
			this->state = MarioState::Normal;
		}

		this->elapsedTime = 0.0f;
	}
}