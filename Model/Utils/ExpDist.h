//
// Created by vlad on 9/9/23.
//

#ifndef EXPDIST_H
#define EXPDIST_H

#include <vector>
#include <utility>
#include <random>
#include "Distribution.h"

struct UniformRandomGen {
    UniformRandomGen(){
        gen = std::mt19937(rd());
        dis = std::uniform_real_distribution<>(0.0, 1.0);
    }

    double generateNext() {
        return dis(gen);
    }

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
};

class ExpDist : public Distribution{
public:
    ExpDist() = default;
    ExpDist(double lambda) : lambda(lambda){}
    double generateNext() override;
    UniformRandomGen mUniformGen;
private:
    double lambda{};
};


#endif //LAB1_EXPDIST_H
