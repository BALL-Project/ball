// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/svrModel.h>

namespace BALL
{
	namespace QSAR
	{
		SVRModel::SVRModel(const QSARData& q, int k_type, double p1, double p2) : KernelModel(q, k_type, p1, p2) { }

		SVRModel::~SVRModel()
		{
		}
	}
}
