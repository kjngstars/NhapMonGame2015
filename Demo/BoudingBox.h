#pragma once

struct BoudingBox
{
	float _x, _y;
	float _w, _h;
	float _vx, _vy;

	BoudingBox(float x, float y, float width, float height, float vx, float vy) : _x(x),_y(y),_w(width),_h(height),_vx(vx),_vy(vy)
	{
	}
	BoudingBox(float x, float y, float width, float height) : _x(x),_y(y),_w(width),_h(height),_vx(0),_vy(0)
	{
	}
};