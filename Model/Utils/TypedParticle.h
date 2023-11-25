//
// Created by vlad on 10/29/23.
//

#ifndef LAB3_TYPEDPARTICLE_H
#define LAB3_TYPEDPARTICLE_H

struct TypedParticle{
    int mInitialType{};
    int mCurrentType{};
    double mCreatedTime{};
    TypedParticle() = default;
    TypedParticle(int initialType, int currentType, double time) : mInitialType(initialType), mCurrentType(currentType), mCreatedTime(time) {}
};

#endif //LAB3_TYPEDPARTICLE_H
