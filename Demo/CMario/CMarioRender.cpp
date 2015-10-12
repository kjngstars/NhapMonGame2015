#include "CMario.h"

void CMario::Render(ID3DXSprite* pSprite)
{
	MATRIX matTransform;

	this->_anchorPoint = D3DXVECTOR2(this->GetWSrcRect() / 2.0f,
		this->GetHSrcRect() / 2.0f);

	D3DXMatrixTransformation2D(
		&matTransform,
		&this->_anchorPoint, 0, &this->_scale,
		&this->_anchorPoint, (float)this->_rotate,
		&D3DXVECTOR2(this->_position.x - this->_anchorPoint.x,
			this->_position.y - this->GetHSrcRect()));
	pSprite->SetTransform(&matTransform);

	switch (this->state)
	{
	case MarioState::Normal:
		pSprite->Draw(
			this->_texture, &this->sourceRect,
			0, 0,
			this->color);

		break;

	case MarioState::ToBig:
		pSprite->Draw(
			this->_texture, &this->sourceRect,
			0, 0,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		break;

	case MarioState::ToFire:
		pSprite->Draw(
			this->_texture, &this->sourceRect,
			0, 0,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		break;

	case MarioState::ToSmall:
		pSprite->Draw(
			this->_texture, &this->sourceRect,
			0, 0,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		break;
	}
}