//
// Created by vlad on 9/9/23.
//

#include "UniformDist.h"
#include "time.h"

namespace {
    int calcZ0() {

        int n = 1'000;
        std::vector<bool> isPrime(n, true);
        std::vector<int> primes;

        for (int p = 2; p * p <= n; p++) {
            if (isPrime[p]) {
                for (int i = p * p; i <= n; i += p)
                    isPrime[i] = false;
            }
        }

        for (int p = 2; p <= n; p++) {
            if (isPrime[p]) {
                primes.push_back(p);
            }
        }
        return primes[rand() % primes.size()];
    }
}

void UniformDist::generate(int num) {
    values.clear();
    values.resize(num);
    std::vector<uint64_t> z(num);
    z[0] = calcZ0();
    values[0] = double(z[0]) / c;
    for (int i = 1; i < num; ++i) {
        z[i] = a * z[i - 1] % c;
        values[i] = double(z[i]) / c;
    }
}

double UniformDist::findCumulative(double x) const {
    if(x > 1){
        return 1;
    }
    if(x < 0){
        return 0;
    }
    return x;
}

double UniformDist::generateNext() {
    std::vector<uint64_t> z(2);
    z[0] = calcZ0();
    z[1] = a * z[0] % c;
    double res = double(z[1]) / c;
    z[0] = z[1];
    return res;
}
