#include "View.h"

View::View()
{
}

View::View(Side topSide, Side bottomSide, int id, double px, double py, double topZ, double bottomZ)
{
	_topSide = topSide;
	_bottomSide = bottomSide;
	_id = id;
	_topZ = topZ;
	_bottomZ = bottomZ;
	_position.x = px;
	_position.y = py;
}

View::~View()
{
	_tArr.clear();
}

void View::setTrackArray(std::vector<Track> tArr)
{
	for (int i = 0; i < static_cast<int>(tArr.size()); i++)
		_tArr.push_back(tArr.at(i));
}


void View::setTile(int x, int y)
{
	_tileX = x;
	_tileY = y;
}
