//
// Created by vlad on 10/29/23.
//

#include "FirstElementPicker.h"
#include <algorithm>
#include "../../Elements/Process.h"

FirstElementPicker::FirstElementPicker(std::vector<std::shared_ptr<Element>> elements) : mElements(elements) {

}

std::shared_ptr<Element> FirstElementPicker::getNextElement() {
    return *std::min_element(mElements.begin(), mElements.end(),
                            [](std::shared_ptr<Element> el1, std::shared_ptr<Element> el2){
                                auto process1 = dynamic_cast<Process*>(el1.get());
                                auto process2 = dynamic_cast<Process*>(el2.get());
                                if(!process1 || !process2){
                                    return false;
                                }

                                return process1->getCurrentQueue() < process2->getCurrentQueue();
    });
}
