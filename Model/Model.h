//
// Created by vlad on 10/8/23.
//

#ifndef MODEL_H
#define MODEL_H

#include "Elements/Element.h"

class Model {
public:
    Model(std::vector<std::shared_ptr<Element>> elements);

    void simulate(double totalTime);

    void summary();

    void setSummaryFunction(std::function<void(const std::vector<std::shared_ptr<Element>>&, double)> summaryFunction) {
        mSummaryFunction = std::move(summaryFunction);
    }

private:
    double mCurrentTime{};
    std::vector<std::shared_ptr<Element>> mElements;
    std::function<void(const std::vector<std::shared_ptr<Element>>&, double)> mSummaryFunction;
};


#endif //LAB2_MODEL_H
