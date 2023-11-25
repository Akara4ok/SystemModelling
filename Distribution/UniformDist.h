//
// Created by vlad on 9/9/23.
//

#ifndef LAB1_UNIFORMDIST_H
#define LAB1_UNIFORMDIST_H

#include "Distribution.h"

class UniformDist : public Distribution {
public:
    UniformDist(uint64_t a = std::pow<uint64_t>(5, 13), uint64_t c = std::pow<uint64_t>(2, 35)) : a(a), c(c){}

    void generate(int num) override;

    double generateNext() override;

    double findCumulative(double x) const override;
private:
    uint64_t a{};
    uint64_t c{};
};

#endif //LAB1_UNIFORMDIST_H
