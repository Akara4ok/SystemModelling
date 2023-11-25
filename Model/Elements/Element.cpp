//
// Created by vlad on 10/8/23.
//

#include "Element.h"

#include <utility>

Element::Element(std::string name, std::shared_ptr<TimeGenerator> gen)
        : mName(std::move(name)), mGen(gen) {
}

Element::Element(std::string name, std::shared_ptr<TimeGenerator> gen, std::shared_ptr<ElementPicker> elementPicker)
        : mName(std::move(name)), mGen(gen), mNextElementPicker(elementPicker) {
}

bool Element::isFinished() {
    return std::abs(mNextTime - mCurrentTime) < 0.00001;
}

void Element::updateCurrentTime(double currentTime) {
    mCurrentTime = currentTime;
}

void Element::setNextTime(double nextTime) {
    mNextTime = nextTime;
}

std::shared_ptr<Element> Element::getNextElement() {
    if(!mNextElementPicker){
        return nullptr;
    }
    return mNextElementPicker->getNextElement();
}

void Element::summary() {
    if(mSummaryFunction){
        mSummaryFunction(this);
        return;
    }
}
