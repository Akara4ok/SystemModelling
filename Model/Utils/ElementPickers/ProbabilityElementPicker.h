//
// Created by vlad on 10/29/23.
//

#ifndef PROBABILITYELEMENTPICKER_H
#define PROBABILITYELEMENTPICKER_H

#include "../ElementPicker.h"
#include "../../../Distribution/UniformDist.h"
#include <vector>
#include <memory>

class ProbabilityElementPicker : public ElementPicker {
public:
    struct NextElement{
        std::shared_ptr<Element> mElement;
        double mProb;

        NextElement(std::shared_ptr<Element> element, double prob) : mElement(element), mProb(prob){}
    };

    ProbabilityElementPicker(std::vector<NextElement> elements);
    std::shared_ptr<Element> getNextElement() override;

protected:
    std::vector<NextElement> mNextElements;
    UniformDist mUniformDist;
};


#endif //PROBABILITYELEMENTPICKER_H
