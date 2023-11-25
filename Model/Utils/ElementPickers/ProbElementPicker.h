//
// Created by vlad on 10/29/23.
//

#ifndef LAB3_PROBELEMENTPICKER_H
#define LAB3_PROBELEMENTPICKER_H

#include "../ElementPicker.h"
#include "../../../Distribution/UniformDist.h"
#include <vector>
#include <memory>

class ProbElementPicker : public ElementPicker {
public:
    struct NextElement{
        std::shared_ptr<Element> mElement;
        double mProb;

        NextElement(std::shared_ptr<Element> element, double prob) : mElement(element), mProb(prob){}
    };

    ProbElementPicker(std::vector<NextElement> elements);
    std::shared_ptr<Element> getNextElement() override;
private:
    std::vector<NextElement> mNextElements;
    UniformDist mUniformDist;
};


#endif //LAB3_PROBELEMENTPICKER_H
