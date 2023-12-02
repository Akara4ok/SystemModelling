//
// Created by vlad on 12/2/23.
//

#ifndef COURSEWORK_LESSQUEUEPICKER_H
#define COURSEWORK_LESSQUEUEPICKER_H

#include "../ElementPicker.h"
#include <vector>
#include <memory>

class LessQueuePicker : public ElementPicker{
public:
    LessQueuePicker(std::vector<std::shared_ptr<Element>> elements);
    std::shared_ptr<Element> getNextElement() override;

private:
    std::vector<std::shared_ptr<Element>> mElements;
};


#endif //COURSEWORK_LESSQUEUEPICKER_H
