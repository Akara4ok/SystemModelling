//
// Created by vlad on 11/25/23.
//

#include "Dispose.h"

#include "iostream"
#include <utility>

Dispose::Dispose(std::string name) : Element(std::move(name), nullptr) {

}

void Dispose::start() {
    mProceed++;
}

void Dispose::finish() {

}

FreeStatus Dispose::isFree() {
    return Free;
}

void Dispose::log() const {
    Logger::log(mCurrentTime, mName, "", 0, mProceed, 0, 0);
}

void Dispose::summary() {
    if (mSummaryFunction) {
        mSummaryFunction(this);
        return;
    }
    std::cout << "------------\n";
    std::cout << "Name: " << mName << "\n";
    std::cout << "Processed items: " << mProceed << "\n";
    std::cout << "------------\n";
}
