//
// Created by vlad on 10/29/23.
//

#include "ProbElementPicker.h"

ProbElementPicker::ProbElementPicker(std::vector<NextElement> elements) : mNextElements(elements){

}

std::shared_ptr<Element> ProbElementPicker::getNextElement() {
    double randNum = mUniformDist.generateNext();
    double sum = 0;
    int ind = 0;
    while (sum + mNextElements[ind].mProb < randNum) {
        sum += mNextElements[ind].mProb;
        ind++;
    }
    return mNextElements[ind].mElement;
}
