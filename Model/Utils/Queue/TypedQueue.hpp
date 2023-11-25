//
// Created by vlad on 10/29/23.
//

#ifndef TYPEDQUEUE_H
#define TYPEDQUEUE_H

#include "../Queue.h"
#include <vector>
#include <optional>

template <typename T>
class TypedQueue : public Queue{
public:
    bool push(std::optional<T> element);
    std::optional<T> pop();
    int getCurrentQueueSize() const { return mQueue.size(); }

private:
    std::vector<T> mQueue;
};

template <typename T>
bool TypedQueue<T>::push(std::optional<T> element)
{

    if(mQueue.size() >= mMaxQueueSize){
        return false;
    }
    mQueue.push_back(element);
    return true;
}

template <typename T>
std::optional<T> TypedQueue<T>::pop(){
    if(mQueue.empty()){
        return {};
    }
    auto el = mQueue[0];
    mQueue.erase(mQueue.begin());
    return el;
}


#endif //TYPEDQUEUE_H