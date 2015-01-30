// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef RMSDBINNER_H
#define RMSDBINNER_H

#include "../sources/starAligner.h"

#include <BALL/KERNEL/atomContainer.h>

#include <util.h>
#include <algorithm>

using namespace BALL;
using namespace std;

class RMSDBinner
{
public:
	RMSDBinner();
	
	~RMSDBinner();
	
private:
	
	static bool comparator(pair<int, AtomContainer*>& a, pair<int, AtomContainer*>& b)
	{
		return b.first < a.first;
	}
	
	
	/**
	 *
	 *  P R I V A T E    F I E L D S
	 * 
	 */
	
};

#endif // RMSDBINNER_H
