//
// Created by vlad on 11/25/23.
//

#ifndef TYPEDPROCESS_HPP
#define TYPEDPROCESS_HPP

#include "Process.h"
#include "../Utils/Queue/TypedQueue.hpp"
#include "../Utils/ElementPickers/PredicateElementPicker.hpp"

class FirstPhaseMachine : public Process {
public:
    FirstPhaseMachine(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<TypedQueue<int>> queue);

    FirstPhaseMachine(std::string name, std::shared_ptr<ExpDist> gen,
                      std::shared_ptr<PredicateElementPicker<int>> elementPicker,
                      std::shared_ptr<TypedQueue<int>> queue);

    void start(int particle);

    void start() override;

    void finish() override;

    std::shared_ptr<Element> getNextElement(int attempt);

    std::shared_ptr<Queue> getQueue() const override { return mTypedQueue; }

    void setElementPicker(std::shared_ptr<PredicateElementPicker<int>> picker) {
        mPredElementPicker = picker;
        Element::setElementPicker(picker);
    };

protected:
    int mAttempt{};
    bool mProcessing{};
    double mNextTime{};
    double mAverageLoad{};

    std::shared_ptr<TypedQueue<int>> mTypedQueue;
    std::shared_ptr<PredicateElementPicker<int>> mPredElementPicker;
};

#endif //TYPEDPROCESS_HPP
