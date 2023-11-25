//
// Created by vlad on 10/8/23.
//

#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "Model.h"

class ModelFactory {
public:
    static Model createTask1();
    static Model createTask2();
    static Model createTask3();
};


#endif //MODELFACTORY_H
