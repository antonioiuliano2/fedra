#include "Fragment.h"

Fragment::Fragment()
{
	
}

Fragment::~Fragment()
{

}

void Fragment::setViewsArray(std::vector<View> vArr)
{
	for (int i = 0; i < static_cast<int>(vArr.size()); i++)
	{
		_vArr.push_back(vArr.at(i));
	}
}


void Fragment::setId(HeaderInfo id)
{
	_id.part0 = id.part0;
	_id.part1 = id.part1;
	_id.part2 = id.part2;
	_id.part3 = id.part3;
}
