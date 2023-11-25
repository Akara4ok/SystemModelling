//
// Created by vlad on 10/29/23.
//


#include "Queue.h"

Queue::Queue(int queueSize) : mMaxQueueSize(queueSize) {

}

bool Queue::increase() {
    if(mQueueSize >= mMaxQueueSize){
        return false;
    }
    mQueueSize++;
    return true;
}

bool Queue::decrease() {
    if(mQueueSize <= 0){
        return false;
    }
    mQueueSize--;
    return true;
}

void Queue::updateAverageQueue(double timeDiff) {
    mSummedQueue += timeDiff * getCurrentQueueSize();
}
