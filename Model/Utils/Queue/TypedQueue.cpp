//
// Created by vlad on 10/29/23.
//

#include "TypedQueue.h"

bool TypedQueue::push(TypedParticle element) {
    if(mQueue.size() >= mMaxQueueSize){
        return false;
    }
    mQueue.push_back(element);
    return true;
}

std::optional<TypedParticle> TypedQueue::pop() {
    if(mQueue.empty()){
        return {};
    }
    auto el = mQueue[0];
    mQueue.erase(mQueue.begin());
    return el;
}

std::optional<TypedParticle> TypedQueue::popType(int type) {
    if(mQueue.empty()){
        return {};
    }

    int ind = 0;
    for (int i = 0; i < mQueue.size(); ++i) {
        if(mQueue[i].mCurrentType == type){
            ind = i;
            break;
        }
    }

    auto el = mQueue[ind];
    mQueue.erase(mQueue.begin() + ind);
    return el;
}
