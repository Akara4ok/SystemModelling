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

    std::shared_ptr<Element> getElementByName(const std::string& name);

    void setSummaryFunction(std::function<void(Model* model)> summaryFunction) {
        mSummaryFunction = std::move(summaryFunction);
    }

private:
    double mCurrentTime{};
    std::vector<std::shared_ptr<Element>> mElements;
    std::function<void(Model* model)> mSummaryFunction;
};


#endif //LAB2_MODEL_H
