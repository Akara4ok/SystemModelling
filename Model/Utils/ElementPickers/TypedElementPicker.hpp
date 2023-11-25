
// Created by vlad on 11/25/23.
//

#ifndef COURSEWORK_TYPEDELEMENTPICKER_H
#define COURSEWORK_TYPEDELEMENTPICKER_H

#include "../ElementPicker.h"
#include "../../../Distribution/UniformDist.h"
#include <vector>
#include <memory>

template<typename T>
class TypedElementPicker {
public:
    virtual std::shared_ptr<Element> getNextElement(T particle) = 0;
};

#endif //COURSEWORK_TYPEDELEMENTPICKER_H
