//
// Created by vlad on 10/8/23.
//

#ifndef PROCESS_H
#define PROCESS_H

#include "Element.h"
#include <numeric>
#include "../Utils/Queue.h"

class Process;

struct SubProcess {
    bool mProcessing{};
    double mNextTime{};
    bool isBlocked{};
    std::function<bool(Process*)> mBlockingPredicate;
    double mAverageLoad{};

    SubProcess() {
        mNextTime = std::numeric_limits<double>::max();
    }

    SubProcess(bool processing, double nextTime) : mProcessing(processing), mNextTime(nextTime) {
    }
};

class Process : public Element {
public:
    Process(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<Queue> queue, int processorNum = 1);

    Process(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<ElementPicker> elementPicker,
            std::shared_ptr<Queue> queue, int processorNum = 1);

    void start() override;

    void finish() override;

    void updateCurrentTime(double currentTime) override;

    void summary() override;

    void log() const override;

    double getNextTime() override;

    bool isFinished() override;

    int getCurrentQueueSize();
    virtual std::shared_ptr<Queue> getQueue() const {return mQueue;}
    virtual double getAverageLoad(int procNum = 0);
    virtual void updateAverageLoad(double currentTime);

    void setInitialValues(int currentQueueSize, std::vector<std::shared_ptr<SubProcess>> processors);

    void setBlocker(int num, std::function<bool(Process*)> blockingFunc);

private:
    std::vector<std::shared_ptr<SubProcess>> mProcessors;
    std::shared_ptr<Queue> mQueue;
};


#endif //PROCESS_H
