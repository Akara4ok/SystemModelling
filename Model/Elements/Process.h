//
// Created by vlad on 10/8/23.
//

#ifndef LAB2_PROCESS_H
#define LAB2_PROCESS_H

#include "Element.h"
#include <numeric>
#include "../Utils/Queue.h"

struct SubProcess {
    bool mProcessing{};
    double mNextTime{};

    SubProcess() {
        mNextTime = std::numeric_limits<double>::max();
    }

    SubProcess(bool processing, double nextTime) : mProcessing(processing), mNextTime(nextTime) {
    }
};

class Process : public Element {
public:
    Process(std::string name, std::shared_ptr<TimeGenerator> gen, std::shared_ptr<Queue> queue, int processorNum = 1);

    Process(std::string name, std::shared_ptr<TimeGenerator> gen, std::shared_ptr<ElementPicker> elementPicker,
            std::shared_ptr<Queue> queue, int processorNum = 1);

    void start() override;

    void finish() override;

    void updateCurrentTime(double currentTime) override;

    void summary() override;

    void log() const override;

    double getNextTime() override;

    bool isFinished() override;

    FreeStatus isFree() override;

    int getCurrentQueue();
    virtual std::shared_ptr<Queue> getQueue() const {return mQueue;}
    double getAverageQueue();
    double getAverageLoad();
    double getSummedElements() const {return mSummedElements;}

    int getFailures() const;

    void setInitialValues(int currentQueueSize, std::vector<std::shared_ptr<SubProcess>> processors);
    virtual std::vector<std::shared_ptr<SubProcess>> getProcessors() const {return mProcessors;}

private:
    double mAverageLoad{};
    double mSummedElements{};
    std::vector<std::shared_ptr<SubProcess>> mProcessors;
    std::shared_ptr<Queue> mQueue;
protected:
    int mFailed{};
};


#endif //LAB2_PROCESS_H
