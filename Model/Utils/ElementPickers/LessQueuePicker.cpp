//
// Created by vlad on 12/2/23.
//

#include "LessQueuePicker.h"
#include "../../Elements/Process.h"
#include <utility>

LessQueuePicker::LessQueuePicker(std::vector<std::shared_ptr<Element>> elements) : mElements(std::move(elements)) {

}

std::shared_ptr<Element> LessQueuePicker::getNextElement() {
    for (const auto& element : mElements) {
        if(element->getNextTime() == std::numeric_limits<double>::max()){
            return element;
        }
    }

    std::shared_ptr<Element> nextElement = *std::min_element(mElements.begin(), mElements.end(),
                                                             [](const std::shared_ptr<Element>& p1, const std::shared_ptr<Element>& p2){
                                                                 auto casted_p1 = dynamic_cast<Process*>(p1.get());
                                                                 auto casted_p2 = dynamic_cast<Process*>(p2.get());
                                                                 if(!casted_p1){
                                                                     return false;
                                                                 }
                                                                 if(!casted_p2){
                                                                     return true;
                                                                 }
                                                                 return casted_p1->getCurrentQueueSize() < casted_p2->getCurrentQueueSize();
                                                             });
    return nextElement;
}