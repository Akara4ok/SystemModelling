//
// Created by vlad on 10/29/23.
//

#include "ProbabilityElementPicker.h"

ProbabilityElementPicker::ProbabilityElementPicker(std::vector<NextElement> elements) : mNextElements(elements){

}

std::shared_ptr<Element> ProbabilityElementPicker::getNextElement() {
    double randNum = mUniformDist.generateNext();
    double sum = 0;
    int ind = 0;
    while (sum + mNextElements[ind].mProb < randNum) {
        sum += mNextElements[ind].mProb;
        ind++;
    }
    return mNextElements[ind].mElement;
}
