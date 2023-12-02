//
// Created by vlad on 10/29/23.
//

#ifndef QUEUE_H
#define QUEUE_H

#include "vector"
#include <limits>

class Queue {
public:
    virtual void push();
    virtual bool pop();
    virtual int getCurrentQueueSize() const { return mQueueSize; }
    void setCurrentQueue(int queueSize) { mQueueSize = queueSize; }

protected:
    int mQueueSize{};
};


#endif //QUEUE_H
