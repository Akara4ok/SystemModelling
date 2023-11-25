//
// Created by vlad on 10/8/23.
//

#ifndef CREATE_H
#define CREATE_H

#include "Element.h"

class Create : public Element{
public:
    Create(std::string name, std::shared_ptr<TimeGenerator> gen);
    Create(std::string name, std::shared_ptr<TimeGenerator> gen, std::shared_ptr<ElementPicker> elementPicker);

    void start() override;
    void finish() override;
    void summary() override;
    void log() const override;

    void setInitialValues(double nextTime);

    FreeStatus isFree() override;
};


#endif //CREATE_H
