//
// Created by vlad on 10/8/23.
//

#ifndef PROCESS_H
#define PROCESS_H

#include "Element.h"
#include <numeric>
#include "../Utils/Queue.h"

class Process;

class Process : public Element {
public:
    Process(std::string name, std::shared_ptr<Distribution> gen, std::shared_ptr<Queue> queue);

    Process(std::string name, std::shared_ptr<Distribution> gen, std::shared_ptr<ElementPicker> elementPicker,
            std::shared_ptr<Queue> queue);

    void start() override;

    void finish() override;

    void updateCurrentTime(double currentTime) override;

    void summary() override;

    void log() const override;

    double getNextTime() override;

    bool isFinished() override;

    int getCurrentQueueSize() const;
    virtual double getAverageLoad();
    virtual void updateAverageLoad(double currentTime);

    void setInitialValues(int currentQueueSize, bool isProcessing);

protected:
    std::shared_ptr<Queue> mQueue;
    bool mProcessing{};
    double mNextTime{};
    double mAverageLoad{};
};


#endif //PROCESS_H
