//
// Created by vlad on 9/9/23.
//

#include "ExpDist.h"

double ExpDist::generateNext() {
    return (-1.0 / lambda) * log(mUniformGen.generateNext());
}