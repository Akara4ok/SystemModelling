//
// Created by vlad on 11/25/23.
//

#include "FirstPhaseMachine.h"

FirstPhaseMachine::FirstPhaseMachine(std::string name, std::shared_ptr<TimeGenerator> gen,
                                        std::shared_ptr<TypedQueue<int>> queue, int processorNum) : Process(std::move(name), gen,
                                                                                                          queue, processorNum),
                                                                                                  mTypedQueue(queue) {
    for (int i = 0; i < processorNum; ++i) {
        mTypedProcessors.push_back(std::make_shared<SubMachineProcess>());
    }
}

FirstPhaseMachine::FirstPhaseMachine(std::string name, std::shared_ptr<TimeGenerator> gen, std::shared_ptr<PredicateElementPicker<int>> elementPicker,
                                     std::shared_ptr<TypedQueue<int>> queue, int processorNum) : Process(std::move(name), gen, elementPicker, queue, processorNum),
                                                            mTypedQueue(queue), mPredElementPicker(elementPicker) {
    for (int i = 0; i < processorNum; ++i) {
        mTypedProcessors.push_back(std::make_shared<SubMachineProcess>());
    }
}

void FirstPhaseMachine::start(int attempt) {
    for (auto& mProcessor: mTypedProcessors) {
        if (!mProcessor->mProcessing) {
            Logger::log(mCurrentTime, mName, "Started", std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                        attempt);
            mProcessor->mProcessing = true;
            mProcessor->mNextTime = mCurrentTime + mGen->generateNext();
            mProcessor->mAttempt = attempt;
            return;
        }
    }
    if (mTypedQueue->push(attempt)) {
        Logger::log(mCurrentTime, mName, "Added to Queue", std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                    attempt);
        return;
    }
    Logger::log(mCurrentTime, mName, "Fail", std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                attempt);
    mFailed++;
}

void FirstPhaseMachine::finish() {
    mProceed++;
    int attempt{};
    for (auto& mProcessor: mTypedProcessors) {
        if (std::abs(mProcessor->mNextTime - mCurrentTime) < 0.000001) {
            attempt = mProcessor->mAttempt;
            Logger::log(mCurrentTime, mName, "Finish", std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                        attempt);
            mProcessor->mProcessing = false;
            auto nextAttempt = mTypedQueue->popElement();
            if (nextAttempt.has_value()) {
                start(nextAttempt.value());
            } else {
                mProcessor->mNextTime = std::numeric_limits<double>::max();
            }
        }
    }

    auto nextElement = getNextElement(attempt);
    if (nextElement) {
        if(auto castedNextElement = dynamic_cast<FirstPhaseMachine*>(nextElement.get())){
            attempt++;
            castedNextElement->start(attempt);
        } else {
            nextElement->start();
        }
    }
}

void FirstPhaseMachine::start() {
    start(0);
}

std::shared_ptr<Element> FirstPhaseMachine::getNextElement(int attempt) {
    if(!mPredElementPicker){
        return {};
    }
    return mPredElementPicker->getNextElement(attempt);
}
