//
// Created by vlad on 12/2/23.
//

#include "MultiChannelProcess.h"

#include <utility>
#include <iostream>

MultiChannelProcess::MultiChannelProcess(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<Queue> queue,
                                         int processorNum) : Process(std::move(name), gen, queue) {
    for (int i = 0; i < processorNum; ++i) {
        mProcessors.push_back({});
    }
}

MultiChannelProcess::MultiChannelProcess(std::string name, std::shared_ptr<ExpDist> gen,
                                         std::shared_ptr<ElementPicker> elementPicker, std::shared_ptr<Queue> queue,
                                         int processorNum) : Process(std::move(name), gen, elementPicker, queue){
    for (int i = 0; i < processorNum; ++i) {
        mProcessors.push_back({});
    }
}

void MultiChannelProcess::start() {
    for (auto& mProcessor: mProcessors) {
        if (!mProcessor.mProcessing && !mProcessor.isBlocked) {
            Logger::log(mCurrentTime, mName, "Started");
            mProcessor.mProcessing = true;
            mProcessor.mNextTime = mCurrentTime + mGen->generateNext();
            return;
        }
    }
    mQueue->push();
    Logger::log(mCurrentTime, mName, "Added to Queue");
}

void MultiChannelProcess::finish() {
    mProceed++;
    Logger::log(mCurrentTime, mName, "Finish");

    for (auto& mProcessor: mProcessors) {
        if(mProcessor.mBlockingPredicate && mProcessor.mBlockingPredicate(this)){
            if(!mProcessor.isBlocked){
                Logger::log(mCurrentTime, mName, "Block subprocess");
            }
            mProcessor.isBlocked = true;
        } else {
            if(mProcessor.isBlocked){
                Logger::log(mCurrentTime, mName, "Unblock subprocess");
            }
            mProcessor.isBlocked = false;
        }
    }

    for (auto& mProcessor: mProcessors) {
        if (std::abs(mProcessor.mNextTime - mCurrentTime) < 0.000001) {
            mProcessor.mProcessing = false;
            if (mQueue->pop() && !mProcessor.isBlocked) {
                start();
            } else {
                mProcessor.mNextTime = std::numeric_limits<double>::max();
            }
        }
    }

    auto nextElement = getNextElement();
    if (nextElement) {
        nextElement->start();
    }
}

void MultiChannelProcess::summary() {
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

void MultiChannelProcess::log() const {
    int busyProcessors = 0;
    for (const auto& mProcessor: mProcessors) {
        if (mProcessor.mProcessing) {
            busyProcessors++;
        }
    }
    Logger::log(mCurrentTime, mName, "", busyProcessors, mProceed, 0, getCurrentQueueSize());
}

double MultiChannelProcess::getNextTime() {
    auto nextElement = *std::min_element(mProcessors.begin(), mProcessors.end(),
                                         [](const SubProcess& p1, const SubProcess& p2) {
                                             return p1.mNextTime < p2.mNextTime;
                                         });
    return nextElement.mNextTime;
}

bool MultiChannelProcess::isFinished() {
    for (auto& mProcessor: mProcessors) {
        if (std::abs(mProcessor.mNextTime - mCurrentTime) < 0.000001) {
            return true;
        }
    }
    return false;
}

void MultiChannelProcess::setInitialValues(int currentQueueSize, std::vector<SubProcess> processors) {
    mQueue->setCurrentQueue(currentQueueSize);
    mProcessors = std::move(processors);
}

void MultiChannelProcess::setBlocker(int num, std::function<bool(Process*)> blockingFunc) {
    mProcessors[num].isBlocked = blockingFunc(this);
    mProcessors[num].mBlockingPredicate = std::move(blockingFunc);
}

double MultiChannelProcess::getAverageLoad(int procNum) {
    if(procNum >= mProcessors.size()){
        return {};
    }
    return mProcessors[procNum].mAverageLoad / mCurrentTime;
}

void MultiChannelProcess::updateAverageLoad(double currentTime) {
    for (auto& processor : mProcessors){
        if(processor.mProcessing){
            processor.mAverageLoad += (currentTime - mCurrentTime);
        }
    }
}

double MultiChannelProcess::getAverageLoad() {
    return getAverageLoad(0);
}
