//
// Created by vlad on 10/8/23.
//

#include "Model.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include "Elements/Create.h"
#include "Elements/Process.h"

Model::Model(std::vector<std::shared_ptr<Element>> elements) : mElements(std::move(elements)) {
}

void Model::simulate(double totalTime) {
    while (mCurrentTime < totalTime)
    {
        std::shared_ptr<Element> nextElement = *std::min_element(mElements.begin(), mElements.end(),
                                                                [](const std::shared_ptr<Element>& p1, const std::shared_ptr<Element>& p2){
            return p1->getNextTime() < p2->getNextTime();
        });

        mCurrentTime = nextElement->getNextTime();
        if(mCurrentTime >= totalTime){
            break;
        }

        for (const auto& element : mElements) {
            element->updateCurrentTime(mCurrentTime);
        }

        for (const auto& element : mElements) {
            if (element->isFinished())
            {
                element->finish();
            }
        }

        for (const auto& element : mElements) {
            element->log();
        }
    }

    summary();
}

void Model::summary() {
    for (const auto& element : mElements) {
        element->summary();
    }
    if(mSummaryFunction){
        mSummaryFunction(mElements, mCurrentTime);
    }
}
