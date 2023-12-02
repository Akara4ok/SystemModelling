//
// Created by vlad on 10/8/23.
//

#include "Process.h"
#include <iostream>
#include <utility>

Process::Process(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<Queue> queue) :
        Element(std::move(name), gen), mQueue(queue) {

}

Process::Process(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<ElementPicker> elementPicker,
                 std::shared_ptr<Queue> queue)
        : Element(std::move(name), gen, elementPicker), mQueue(queue) {
    mNextTime = std::numeric_limits<double>::max();
}

void Process::start() {
    if (!mProcessing) {
        Logger::log(mCurrentTime, mName, "Started");
        mProcessing = true;
        mNextTime = mCurrentTime + mGen->generateNext();
        return;
    }
    mQueue->push();
    Logger::log(mCurrentTime, mName, "Added to Queue");
}

void Process::finish() {
    mProceed++;
    Logger::log(mCurrentTime, mName, "Finish");

    if (std::abs(mNextTime - mCurrentTime) < 0.000001) {
        mProcessing = false;
        if (mQueue->pop()) {
            start();
        } else {
            mNextTime = std::numeric_limits<double>::max();
        }
    }

    auto nextElement = getNextElement();
    if (nextElement) {
        nextElement->start();
    }
}


void Process::updateCurrentTime(double currentTime) {
    updateAverageLoad(currentTime);
    Element::updateCurrentTime(currentTime);
}

void Process::summary() {
    if (mSummaryFunction) {
        mSummaryFunction(this);
        return;
    }
    std::cout << "------------\n";
    std::cout << "Name: " << mName << "\n";
    std::cout << "Queue size: " << getCurrentQueueSize() << "\n";
    std::cout << "Processed items: " << mProceed << "\n";
    std::cout << "Average load: " << getAverageLoad() << "\n";
    std::cout << "------------\n";
}

void Process::log() const {
    Logger::log(mCurrentTime, mName, "", mProcessing ? 1 : 0, mProceed, 0, getCurrentQueueSize());
}

int Process::getCurrentQueueSize() const {
    return mQueue->getCurrentQueueSize();
}

double Process::getNextTime() {
    return mNextTime;
}

bool Process::isFinished() {
    return std::abs(mNextTime - mCurrentTime) < 0.000001;
}

void Process::setInitialValues(int currentQueueSize, bool isProcessing) {
    mQueue->setCurrentQueue(currentQueueSize);
    mProcessing = isProcessing;
}

double Process::getAverageLoad() {
    return mAverageLoad / mCurrentTime;
}

void Process::updateAverageLoad(double currentTime) {
    if (mProcessing) {
        mAverageLoad += (currentTime - mCurrentTime);
    }
}
