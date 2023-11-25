//
// Created by vlad on 10/29/23.
//

#ifndef ELEMENTPICKER_H
#define ELEMENTPICKER_H

#include <memory>

class Element;

class ElementPicker {
public:
    virtual std::shared_ptr<Element> getNextElement() = 0;
};


#endif //LAB3_ELEMENTPICKER_H
