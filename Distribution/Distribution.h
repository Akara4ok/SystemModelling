//
// Created by vlad on 9/9/23.
//

#ifndef LAB1_DISTRIBUTION_H
#define LAB1_DISTRIBUTION_H

#include <utility>
#include <vector>
#include <utility>
#include <random>

struct UniformRandomGen {
    UniformRandomGen(){
        gen = std::mt19937(rd());
        dis = std::uniform_real_distribution<>(0.0, 1.0);
    }

    double next() {
        return dis(gen);
    }

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
};

struct DistCharacteristic {
    DistCharacteristic(double mean, double variance) : mean(mean), variance(variance){};
    double mean{};
    double variance{};
};

class Distribution {
public:
    virtual ~Distribution(){};

    virtual void generate(int num) = 0;
    virtual double generateNext() = 0;

    DistCharacteristic findCharacteristic(int intervalNum) {
        mean = findMean();
        variance = findVar();
        findGist(intervalNum, gist, ranges);
        return {mean, variance};
    };

    static bool ChiSquaredTest(int basicIntervalNum, const Distribution* dist);

    void drawGist(int multiplication) const;
    void printGist() const;

    void print() const;

    static double ChiSquaredTheoretical(int m);
protected:

    [[nodiscard]] double findMean() const;

    [[nodiscard]] double findVar() const;

    void findGist(int intervalNum, std::vector<int>& gistVec, std::vector<double>& rangesVec) const;

    [[nodiscard]] virtual double findCumulative(double x) const = 0;
    std::vector<double> values;
    double mean{};
    double variance{};
    std::vector<int> gist;

    std::vector<double> ranges;

    UniformRandomGen gen;
private:
    static std::vector<double> TheoreticalChiSquare;
    [[nodiscard]] std::pair<double, double> findMinMax() const;
};


#endif //LAB1_DISTRIBUTION_H
