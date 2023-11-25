//
// Created by vlad on 10/9/23.
//

#ifndef LAB2_TIMEGENERATOR_H
#define LAB2_TIMEGENERATOR_H

#include <functional>
#include <memory>
#include "../../Distribution/Distribution.h"

class TimeGenerator {
public:
    using DelayFunc = std::function<double(double)>;
    TimeGenerator(Distribution* gen, DelayFunc delayFunc);
    TimeGenerator() = default;;
    explicit TimeGenerator(Distribution* gen);
    explicit TimeGenerator(DelayFunc delayFunc);
    virtual double generateNext();
    virtual double generateNext(int type) {return 0;};
private:
    std::shared_ptr<Distribution> mGen;
    DelayFunc mDelayFunc;
};


#endif //LAB2_TIMEGENERATOR_H
