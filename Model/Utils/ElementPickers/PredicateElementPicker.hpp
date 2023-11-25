//
// Created by vlad on 11/25/23.
//

#ifndef PREDICATEELEMENTPICKER_H
#define PREDICATEELEMENTPICKER_H

#include "../ElementPickers/ProbabilityElementPicker.h"
#include "TypedElementPicker.hpp"
#include <functional>
#include <optional>

template<typename T>
class PredicateElementPicker : public ProbabilityElementPicker, public TypedElementPicker<T> {
public:
    PredicateElementPicker(std::vector<NextElement> elements, std::optional<std::function<bool(T element)>> pred = {});

    std::shared_ptr<Element> getNextElement(T particle);

private:
    std::function<bool(T element)> mPredicate;
};

template<typename T>
PredicateElementPicker<T>::PredicateElementPicker(std::vector<NextElement> elements, std::optional<std::function<bool(T element)>> pred)
        : ProbabilityElementPicker(elements){
            if(pred.has_value()){
                mPredicate = pred.value();
            }
}

template<typename T>
std::shared_ptr<Element> PredicateElementPicker<T>::getNextElement(T particle) {
    if(mPredicate && mPredicate(particle)){
        return mNextElements.back().mElement;
    }
    return ProbabilityElementPicker::getNextElement();
}

#endif //PREDICATEELEMENTPICKER_H
