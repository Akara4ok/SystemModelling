//
// Created by vlad on 11/25/23.
//

#include "Dispose.h"

#include "iostream"
#include <utility>

Dispose::Dispose(std::string name) : Element(std::move(name), nullptr) {
    mNextTime = std::numeric_limits<double>::max();
}

void Dispose::start() {
    if(mIsExperiment && mCurrentTime >= mStartObserveTime || !mIsExperiment){
        mProceed++;
    }
}

void Dispose::finish() {

}

void Dispose::log() const {
    Logger::log(mCurrentTime, mName, "", 0, mProceed);
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
