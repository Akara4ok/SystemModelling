//
// Created by vlad on 9/9/23.
//

#ifndef LAB1_EXPDIST_H
#define LAB1_EXPDIST_H

#include "Distribution.h"

class ExpDist : public Distribution {
public:
    ExpDist(double lambda) : lambda(lambda){}

    void generate(int num) override;

    double generateNext() override;

    double findCumulative(double x) const override;
private:
    double lambda{};
};


#endif //LAB1_EXPDIST_H
