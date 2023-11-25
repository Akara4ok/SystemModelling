//
// Created by vlad on 10/9/23.
//

#include "TimeGenerator.h"

#include <utility>

TimeGenerator::TimeGenerator(Distribution* gen, TimeGenerator::DelayFunc delayFunc) : mDelayFunc(std::move(delayFunc)){
    mGen.reset(gen);
}

TimeGenerator::TimeGenerator(Distribution* gen) {
    mGen.reset(gen);
}

TimeGenerator::TimeGenerator(TimeGenerator::DelayFunc delayFunc) : mDelayFunc(std::move(delayFunc)) {

}

double TimeGenerator::generateNext() {
    if(!mGen){
        return mDelayFunc(0.0);
    }
    if(!mDelayFunc){
        return mGen->generateNext();
    }
    return mDelayFunc(mGen->generateNext());
}

