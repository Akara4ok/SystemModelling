//
// Created by vlad on 10/29/23.
//

#ifndef QUEUE_H
#define QUEUE_H

#include "vector"
#include <limits>

class Queue {
public:
    Queue(int queueSize = std::numeric_limits<int>::max());

    void setCurrentQueueSize(int queueSize) { mQueueSize = queueSize; }
    virtual bool push();
    virtual bool pop();
    virtual int getCurrentQueueSize() const { return mQueueSize; }
    double getAverageQueue(double currentTime) const { return mSummedQueue / currentTime; }
    void updateAverageQueue(double timeDiff);
    bool isFull() const { return getCurrentQueueSize() < mMaxQueueSize; }

protected:
    int mQueueSize{};
    int mMaxQueueSize{};
    double mSummedQueue{};
};


#endif //LAB3_QUEUE_H
