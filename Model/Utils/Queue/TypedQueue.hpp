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
    bool push(T element);
    std::optional<T> popElement();
    int getCurrentQueueSize() const { return mQueue.size(); }

private:
    std::vector<T> mQueue;
};

template <typename T>
bool TypedQueue<T>::push(T element)
{
    if(mQueue.size() >= mMaxQueueSize){
        return false;
    }
    mQueue.push_back(element);
    return true;
}

template <typename T>
std::optional<T> TypedQueue<T>::popElement(){
    if(mQueue.empty()){
        return {};
    }
    auto el = mQueue[0];
    mQueue.erase(mQueue.begin());
    return el;
}


#endif //TYPEDQUEUE_H