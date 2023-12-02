//
// Created by vlad on 10/8/23.
//

#include "Process.h"
#include <iostream>
#include <utility>

Process::Process(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<Queue> queue, int processorsNum)
        : Element(std::move(name), gen), mQueue(queue) {
    mNextTime = std::numeric_limits<double>::max();
    for (int i = 0; i < processorsNum; ++i) {
        mProcessors.push_back(std::make_shared<SubProcess>());
    }
}

Process::Process(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<ElementPicker> elementPicker,
                 std::shared_ptr<Queue> queue, int processorsNum)
        : Element(std::move(name), gen, elementPicker), mQueue(queue) {
    mNextTime = std::numeric_limits<double>::max();
    for (int i = 0; i < processorsNum; ++i) {
        mProcessors.push_back(std::make_shared<SubProcess>());
    }
}

void Process::start() {
    for (auto& mProcessor: mProcessors) {
        if (!mProcessor->mProcessing && !mProcessor->isBlocked) {
            Logger::log(mCurrentTime, mName, "Started");
            mProcessor->mProcessing = true;
            mProcessor->mNextTime = mCurrentTime + mGen->generateNext();
            return;
        }
    }
    mQueue->push();
    Logger::log(mCurrentTime, mName, "Added to Queue");
}

void Process::finish() {
    mProceed++;
    Logger::log(mCurrentTime, mName, "Finish");

    for (auto& mProcessor: mProcessors) {
        if(mProcessor->mBlockingPredicate && mProcessor->mBlockingPredicate(this)){
            if(!mProcessor->isBlocked){
                Logger::log(mCurrentTime, mName, "Block subprocess");
            }
            mProcessor->isBlocked = true;
        } else {
            if(mProcessor->isBlocked){
                Logger::log(mCurrentTime, mName, "Unblock subprocess");
            }
            mProcessor->isBlocked = false;
        }
    }

    for (auto& mProcessor: mProcessors) {
        if (std::abs(mProcessor->mNextTime - mCurrentTime) < 0.000001) {
            mProcessor->mProcessing = false;
            if (mQueue->pop() && !mProcessor->isBlocked) {
                start();
            } else {
                mProcessor->mNextTime = std::numeric_limits<double>::max();
            }
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
    std::cout << "Queue size: " << getQueue()->getCurrentQueueSize() << "\n";
    std::cout << "Processed items: " << mProceed << "\n";
    std::cout << "Average load: " << getAverageLoad() << "\n";
    std::cout << "------------\n";
}

void Process::log() const {
    int busyProcessors = 0;
    for (auto mProcessor: mProcessors) {
        if (mProcessor->mProcessing) {
            busyProcessors++;
        }
    }
    Logger::log(mCurrentTime, mName, "", busyProcessors, mProceed, 0, getQueue()->getCurrentQueueSize());
}

int Process::getCurrentQueueSize() {
    return getQueue()->getCurrentQueueSize();
}

double Process::getNextTime() {
    auto nextElement = *std::min_element(mProcessors.begin(), mProcessors.end(),
                                               [](const std::shared_ptr<SubProcess>& p1, const std::shared_ptr<SubProcess>& p2) {
                                                   return p1->mNextTime < p2->mNextTime;
                                               });
    return nextElement->mNextTime;
}

bool Process::isFinished() {
    for (auto& mProcessor: mProcessors) {
        if (std::abs(mProcessor->mNextTime - mCurrentTime) < 0.000001) {
            return true;
        }
    }
    return false;
}

void Process::setInitialValues(int currentQueueSize, std::vector<std::shared_ptr<SubProcess>> processors) {
    mQueue->setCurrentQueue(currentQueueSize);
    mProcessors = std::move(processors);
}

void Process::setBlocker(int num, std::function<bool(Process*)> blockingFunc) {
    mProcessors[num]->isBlocked = blockingFunc(this);
    mProcessors[num]->mBlockingPredicate = std::move(blockingFunc);
}

double Process::getAverageLoad(int procNum) {
    if(procNum >= mProcessors.size()){
        return {};
    }
    return mProcessors[procNum]->mAverageLoad / mCurrentTime;
}

void Process::updateAverageLoad(double currentTime) {
    for (const auto& processor : mProcessors){
        if(processor->mProcessing){
            processor->mAverageLoad += (currentTime - mCurrentTime);
        }
    }
}
