#ifndef FRAGMENT_H
#define FRAGMENT_H

#include <vector>
#include "View.h"
#include "Struct.h"

class Fragment
{
private:
	int _fragmentCoding;
	unsigned int _index;
	unsigned int _startView;
	int _nViews;
	std::vector<View> _vArr;
	HeaderInfo _id;

	
	
public:
	Fragment();
	~Fragment();
	inline void setFragmentCoding(int fragmentCoding) {_fragmentCoding = fragmentCoding;}
	inline void setIndex(unsigned int index)          {_index = index;}
	inline void setStartView(unsigned int startView)  {_startView = startView;}
	inline void setViews(int nViews)                  {_nViews = nViews;}
	void setViewsArray(std::vector<View> vArr);
	void setView(View view) {_vArr.push_back(view);}
	void setId (HeaderInfo id);

	int getFragmentCoding() {return _fragmentCoding;}
	unsigned int getIndex() {return _index;}
	unsigned int getStartView()  {return _startView;}
	int getViews()                 {return _nViews; }
	View* getView(int index) {return &(_vArr.at(index));}
	HeaderInfo getId()               {return _id;}
	
	
};

#endif

