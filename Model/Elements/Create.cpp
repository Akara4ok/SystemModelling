//
// Created by vlad on 10/8/23.
//

#include "Create.h"
#include <iostream>
#include <utility>

Create::Create(std::string name, std::shared_ptr<Distribution> gen, std::shared_ptr<ElementPicker> elementPicker)
        : Element(std::move(name), gen, elementPicker) {
    mNextTime = mCurrentTime + mGen->generateNext();
}

void Create::start() {
}

void Create::finish() {
    mProceed++;
    Logger::log(mCurrentTime, mName, "Finished");
    mNextTime = mCurrentTime + mGen->generateNext();
    auto nextElement = getNextElement();
    if (nextElement) {
        nextElement->start();
    }
}

void Create::summary() {
    if(mSummaryFunction){
        mSummaryFunction(this);
        return;
    }
    std::cout << "Created: " << mProceed << "\n";
}

void Create::log() const {
    Logger::log(mCurrentTime, mName, "", std::nullopt, mProceed);
}
