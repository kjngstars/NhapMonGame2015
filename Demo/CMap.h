#pragma once

#include <regex>
#include <fstream>
#include <string>
#include <numeric>
#include <list>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <d3d9.h>
#include "GameConfig.h"
#include "CGraphics.h"

class CMap
{
public:
	CMap(std::string fileMap, std::string fileTileSet);
	CMap();
	void init(std::string fileMap, std::string fileTileSet);
	void draw();
	
private:
	TEXTURE _tileSet;
	std::vector<int> _tileList;
	std::set<int> _tileIndex;
	int _mapWidth;
	int _mapHeight;
	int _tileSize;
};


