//
// Created by vlad on 11/25/23.
//

#ifndef TYPEDPROCESS_HPP
#define TYPEDPROCESS_HPP

#include "Process.h"
#include "../Utils/Queue/TypedQueue.hpp"
#include "../Utils/ElementPickers/PredicateElementPicker.hpp"

struct SubMachineProcess : public SubProcess {
    int mAttempt;

    SubMachineProcess() {
        mNextTime = std::numeric_limits<double>::max();
    }

    SubMachineProcess(bool processing, int attempt, double nextTime) : SubProcess(processing, nextTime),
                                                                      mAttempt(attempt) {
    }
};

class FirstPhaseMachine : public Process {
public:
    FirstPhaseMachine(std::string name, std::shared_ptr<TimeGenerator> gen, std::shared_ptr<TypedQueue<int>> queue,
                 int processorNum = 1);

    FirstPhaseMachine(std::string name, std::shared_ptr<TimeGenerator> gen, std::shared_ptr<PredicateElementPicker<int>> elementPicker,
                 std::shared_ptr<TypedQueue<int>> queue, int processorNum = 1);

    void start(int particle);

    void start() override;

    void finish() override;

    std::shared_ptr<Element> getNextElement(int attempt);

    std::shared_ptr<Queue> getQueue() const override { return mTypedQueue; }

    std::vector<std::shared_ptr<SubProcess>> getProcessors() const override {
        std::vector<std::shared_ptr<SubProcess>> res;
        for (int i = 0; i < mTypedProcessors.size(); ++i) {
            res.push_back(mTypedProcessors[i]);
        }
        return res;
    }

protected:
    std::vector<std::shared_ptr<SubMachineProcess>> mTypedProcessors;
    std::shared_ptr<TypedQueue<int>> mTypedQueue;
    std::shared_ptr<PredicateElementPicker<int>> mPredElementPicker;
};

#endif //TYPEDPROCESS_HPP
