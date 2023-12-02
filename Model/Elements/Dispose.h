//
// Created by vlad on 11/25/23.
//

#ifndef COURSEWORK_DISPOSE_H
#define COURSEWORK_DISPOSE_H

#include "Element.h"

class Dispose : public Element {
public:
    Dispose(std::string name);

    void start() override;

    void finish() override;

    void log() const override;

    void summary() override;
};


#endif //COURSEWORK_DISPOSE_H
