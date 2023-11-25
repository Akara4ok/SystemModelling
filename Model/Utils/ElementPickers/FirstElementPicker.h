//
// Created by vlad on 10/29/23.
//

#ifndef LAB3_FIRSTELEMENTPICKER_H
#define LAB3_FIRSTELEMENTPICKER_H

#include "../ElementPicker.h"
#include <vector>
#include <memory>


class FirstElementPicker : public ElementPicker {
public:
    FirstElementPicker(std::vector<std::shared_ptr<Element>> elements);
    std::shared_ptr<Element> getNextElement() override;
private:
    std::vector<std::shared_ptr<Element>> mElements;
};


#endif //LAB3_FIRSTELEMENTPICKER_H
