#include "CMap.h"



CMap::CMap() :_tileSize(64)
{	
	
}

void CMap::init(std::string fileMap, std::string fileTileSet)
{
	//load tileset
	CREATE_TEXTURE(fileTileSet.c_str(), _tileSet);

	std::ifstream reader(fileMap);
	reader.ignore(256, '\n');
	std::string firstLine{}, content{}, line{};

	//read first line
	std::getline(reader, firstLine);

	//read the rest
	while (std::getline(reader, line))
	{
		content += line;
	}

	//parse content
	std::smatch m;
	std::regex reg1("<.+ w=\"([0-9]+)\" h=\"([0-9]+)\">");
	std::regex reg2("<[^>]*\"([0-9]+)\" />");

	//parse first line
	bool found = std::regex_search(firstLine.cbegin(), firstLine.cend(), m, reg1);
	assert(found);
	_mapWidth = std::stoi(m.str(1));
	_mapHeight = std::stoi(m.str(2));
	//rest
	auto pos = content.cbegin();

	for (; std::regex_search(pos, content.cend(), m, reg2); pos = m.suffix().first)
	{
		_tileList.push_back(std::stoi(m[1].str()));
	}

	//
	for (int i = 0; i < _tileList.size(); i++)
	{
		_tileIndex.insert(_tileList.at(i));
	}
}

void CMap::draw()
{
	RECT r;
	int index{};
	int count{};

	for (size_t i = 0; i < _tileList.size(); ++i)
	{	
		int x = (i%_mapWidth) * _tileSize;
		int y = (i / _mapWidth)*_tileSize;
		auto pos = std::find(_tileIndex.cbegin(), _tileIndex.cend(), _tileList.at(i));
		index = (std::distance(_tileIndex.cbegin(), pos));

		r.left = index*_tileSize;
		r.top = 0;
		r.right = _tileSize + r.left;
		r.bottom = _tileSize + r.top;

		DRAW(_tileSet, &r, nullptr, &VECTOR3(x, y,0), D3DCOLOR_XRGB(255, 255, 255));
	}	
}
