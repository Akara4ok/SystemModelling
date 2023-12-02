//
// Created by vlad on 10/29/23.
//


#include "Queue.h"

void Queue::push() {
    mQueueSize++;
}

bool Queue::pop() {
    if(mQueueSize <= 0){
        return false;
    }
    mQueueSize--;
    return true;
}