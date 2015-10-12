#pragma once

#include <d3dx9.h>
#include <string>
#include "GameConfig.h"
#include "CGraphics.h"
#include "BoudingBox.h"

class CObject
{
protected:
	TEXTURE _texture;
	VECTOR2 _anchorPoint;
	VECTOR2 _scale;
	D3DCOLOR _color;
	double _rotate;
	VECTOR2 _position;
	float _width, _height;

public:
	CObject::CObject();
	CObject(std::string fileName);
	CObject(std::string fileName,D3DCOLOR color);
	~CObject();

	void setAnchorPoint(VECTOR2 center);
	void setPosition(float x,float y);
	void setFlipX();
	void setFlipY();
	void setScale(float r1, float r2);
	void setScaleX(float r);
	void setScaleY(float r);
	void setRotate(double d);

	BoudingBox getBox();
	
	virtual void draw();
};