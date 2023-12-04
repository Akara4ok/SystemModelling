//
// Created by vlad on 11/25/23.
//

#include "FirstPhaseMachine.h"

FirstPhaseMachine::FirstPhaseMachine(std::string name, std::shared_ptr<Distribution> gen,
                                     std::shared_ptr<TypedQueue<int>> queue) : Process(std::move(name), gen,
                                                                                       queue),
                                                                               mTypedQueue(queue) {
    mNextTime = std::numeric_limits<double>::max();
}


FirstPhaseMachine::FirstPhaseMachine(std::string name, std::shared_ptr<Distribution> gen, std::shared_ptr<ElementPicker> elementPicker,
                                     std::shared_ptr<TypedQueue<int>> queue) : Process(std::move(name), gen, elementPicker, queue),
                                                            mTypedQueue(queue) {
    mNextTime = std::numeric_limits<double>::max();
}

void FirstPhaseMachine::start(int attempt) {
    if (!mProcessing) {
        Logger::log(mCurrentTime, mName, "Started", std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                    attempt);
        mProcessing = true;
        mNextTime = mCurrentTime + mGen->generateNext();
        mAttempt = attempt;
        return;
    }

    mTypedQueue->push(attempt);
    Logger::log(mCurrentTime, mName, "Added to Queue", std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                attempt);
}

void FirstPhaseMachine::finish() {
    mProceed++;
    int attempt = mAttempt;
    if (std::abs(mNextTime - mCurrentTime) < 0.000001) {
        Logger::log(mCurrentTime, mName, "Finish", std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                    attempt);
        mProcessing = false;
        auto nextAttempt = mTypedQueue->popElement();
        if (nextAttempt.has_value()) {
            start(nextAttempt.value());
        } else {
            mNextTime = std::numeric_limits<double>::max();
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
    auto predElementPicker = dynamic_cast<PredicateElementPicker<int>*>(mNextElementPicker.get());
    if(!predElementPicker){
        return mNextElementPicker->getNextElement();
    }
    return predElementPicker->getNextElement(attempt);
}
