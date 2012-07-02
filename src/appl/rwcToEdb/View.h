#include "Side.h"
#include "Track.h"
#include <vector>

#ifndef VIEW_H
#define VIEW_H

class View
{
private:
	Side _topSide;
	Side _bottomSide;
	Vector2 _position;
	int _tileX;
	int _tileY;
	int _id;
	double _topZ;
	double _bottomZ;
	std::vector<Track> _tArr;
	

public:
	View();
	View(Side topSide, Side bottomSide, int id, double px, double py, double topZ, double bottomZ);
	~View();
	
	Side getTopSide() const {return _topSide;}
	Side getBottomSide() const {return _bottomSide;}
	int getTileX() const  {return _tileX;}
	int getTileY() const {return _tileY;}
	int getId()const {return _id;}
	Vector2 getPosition() const {return _position;}
	double getTopZ() const  {return _topZ;}
	double getBottomZ() const  {return _bottomZ;}
	std::vector<Track> getTrackArray() const  {return _tArr;}
	Side* getTopSide() {return &_topSide;}
	Side* getBottomSide() {return &_bottomSide;}

	void setTopSide(Side side) {_topSide = side;}
	void setBottomSide(Side side) {_bottomSide = side;}
	void setId(int id) {_id = id;}
	void setPosition(Vector2 position) {_position = position;}
	void setTopZ(double topZ) {_topZ = topZ;}
	void setBottomZ(double bottomZ) {_bottomZ = bottomZ;}
	void setTrackArray(std::vector<Track> tArr);
	void setTile(int x, int y);
	void setTileX(int x) {_tileX = x;}
	void setTileY(int y) {_tileX = y;}
	

};

#endif
