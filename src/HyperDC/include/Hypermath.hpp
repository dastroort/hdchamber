#pragma once
#include <Eigen/Dense>

namespace Hyper
{
    typedef Eigen::VectorXf PointND;
    inline size_t nrots(size_t dim) { return dim*(dim-1)/2; }
    inline float distance(const PointND& p, const PointND& q) { return (p-q).norm(); }
}

#endif