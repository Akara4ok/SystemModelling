//
// Created by vlad on 12/5/23.
//

#ifndef COURSEWORK_DISTRIBUTION_H
#define COURSEWORK_DISTRIBUTION_H

#include <vector>
#include <utility>
#include <random>

class Distribution {
public:
    Distribution() = default;
    virtual double generateNext() = 0;
};

#endif //COURSEWORK_DISTRIBUTION_H
