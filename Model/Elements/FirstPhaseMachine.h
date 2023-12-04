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
    FirstPhaseMachine(std::string name, std::shared_ptr<Distribution> gen, std::shared_ptr<TypedQueue<int>> queue);

    FirstPhaseMachine(std::string name, std::shared_ptr<Distribution> gen,
                      std::shared_ptr<ElementPicker> elementPicker,
                      std::shared_ptr<TypedQueue<int>> queue);

    void start(int particle);

    void start() override;

    void finish() override;

    std::shared_ptr<Element> getNextElement(int attempt);

private:
    int mAttempt{};
    std::shared_ptr<TypedQueue<int>> mTypedQueue;
};

#endif //TYPEDPROCESS_HPP
