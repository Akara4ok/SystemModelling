//
// Created by vlad on 12/5/23.
//

#include "HardcodedDistribution.h"

HardcodedDistribution::HardcodedDistribution(std::vector<double> values) : mValues(std::move(values)){

}

double HardcodedDistribution::generateNext() {
    double res = mValues[mCurrentIdx];
    mCurrentIdx++;
    return res;
}
