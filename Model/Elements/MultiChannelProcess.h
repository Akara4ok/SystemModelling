//
// Created by vlad on 12/2/23.
//

#ifndef COURSEWORK_MULTICHANNELPROCESS_H
#define COURSEWORK_MULTICHANNELPROCESS_H

#include "Process.h"

class MultiChannelProcess;

struct SubProcess {
    bool mProcessing{};
    double mNextTime{};
    std::function<bool(const MultiChannelProcess*)> mBlockingPredicate;
    double mAverageLoad{};

    SubProcess() {
        mNextTime = std::numeric_limits<double>::max();
    }

    SubProcess(bool processing, double nextTime) : mProcessing(processing), mNextTime(nextTime) {
    }

    bool isBlocked(const MultiChannelProcess* process) const {
        if(!mBlockingPredicate){
            return false;
        }
        return mBlockingPredicate(process);
    }
};

class MultiChannelProcess : public Process {
public:
    MultiChannelProcess(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<Queue> queue, int processorNum = 1);

    MultiChannelProcess(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<ElementPicker> elementPicker,
                        std::shared_ptr<Queue> queue, int processorNum = 1);

    void start() override;

    void finish() override;

    void summary() override;

    void log() const override;

    double getNextTime() override;

    bool isFinished() override;

    double getAverageLoad() override;
    void updateAverageLoad(double currentTime) override;

    double getAverageLoad(int procNum);

    void setInitialValues(int currentQueueSize, std::vector<SubProcess> processors);

    void setBlocker(int num, std::function<bool(const MultiChannelProcess*)> blockingFunc);
private:
    std::vector<SubProcess> mProcessors;
};


#endif //COURSEWORK_MULTICHANNELPROCESS_H
