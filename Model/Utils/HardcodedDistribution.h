//
// Created by vlad on 12/5/23.
//

#ifndef COURSEWORK_HARDCODEDDISTRIBUTION_H
#define COURSEWORK_HARDCODEDDISTRIBUTION_H

#include "Distribution.h"

class HardcodedDistribution : public Distribution {
public:
    HardcodedDistribution(std::vector<double> values);
    double generateNext() override;
private:
    std::vector<double> mValues;
    int mCurrentIdx{};
};


#endif //COURSEWORK_HARDCODEDDISTRIBUTION_H
