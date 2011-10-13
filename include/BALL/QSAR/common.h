#ifndef BALL_QSAR_COMMON_H
#define BALL_QSAR_COMMON_H

#include <vector>
#include <Eigen/Core>
#include <Eigen/StdVector>

namespace BALL
{
	typedef std::vector<Eigen::MatrixXd, Eigen::aligned_allocator<Eigen::MatrixXd> > MatrixVector;
}

#endif //BALL_QSAR_COMMON_H
