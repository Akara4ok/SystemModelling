//
// Created by vlad on 9/9/23.
//

#include "ExpDist.h"

void ExpDist::generate(int num) {
    values.clear();
    values.resize(num);

    for (int i = 0; i < num; ++i) {
        values[i] = (-1.0 / lambda) * log(gen.next());
    }
}

double ExpDist::generateNext() {
    return (-1.0 / lambda) * log(gen.next());
}


double ExpDist::findCumulative(double x) const {
    return 1 - std::exp(-lambda * x);
}