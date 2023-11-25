//
// Created by vlad on 10/29/23.
//

#ifndef LAB3_TYPEDQUEUE_H
#define LAB3_TYPEDQUEUE_H

#include "../Queue.h"
#include "../TypedParticle.h"
#include <vector>
#include <optional>

class TypedQueue : public Queue{
public:
    virtual bool push(TypedParticle element);
    virtual std::optional<TypedParticle> pop();
    virtual std::optional<TypedParticle> popType(int type);
    int getCurrentQueueSize() const { return mQueue.size(); }
    const std::vector<TypedParticle>& getQueue() const {return mQueue; }

protected:
    std::vector<TypedParticle> mQueue;
};


#endif //LAB3_TYPEDQUEUE_H
