//
// Created by vlad on 10/8/23.
//

#ifndef CREATE_H
#define CREATE_H

#include "Element.h"

class Create : public Element{
public:
    Create(std::string name, std::shared_ptr<Distribution> gen, std::shared_ptr<ElementPicker> elementPicker);

    void start() override;
    void finish() override;
    void summary() override;
    void log() const override;
};


#endif //CREATE_H
