//
// Created by vlad on 9/9/23.
//

#include "Distribution.h"
#include "iostream"
#include "limits"

    std::vector<double> Distribution::TheoreticalChiSquare = {
            3.841, 7.378, 9.348, 11.143, 12.833, 14.449, 16.013, 17.535, 19.023, 20.483, 21.920, 23.337, 24.736, 26.119,
            27.488, 28.845, 30.191, 31.526, 32.852, 34.170, 35.479, 36.781, 38.076, 39.364, 40.646, 41.923, 43.195,
            44.461, 45.722, 46.979, 48.232, 49.480, 50.725, 51.966, 53.203, 54.437, 55.668, 56.896, 58.120, 59.342,
            60.561, 61.777, 62.990, 64.201, 65.410, 66.617, 67.821, 69.023, 70.222, 71.420, 72.616, 73.810, 75.002,
            76.192, 77.380, 78.567, 79.752, 80.936, 82.117, 83.298, 84.476, 85.654, 86.830, 88.004, 89.177, 90.349,
            91.519, 92.689, 93.856, 95.023, 96.189, 97.353, 98.516, 99.678, 100.839, 101.999, 103.158, 104.316, 105.473,
            106.629, 107.783, 108.937, 110.090, 111.242, 112.393, 113.544, 114.693, 115.841, 116.989, 118.136, 119.282,
            120.427, 121.571, 122.715, 123.858, 125.000, 126.141, 127.282, 128.422, 129.561
    };

double Distribution::findMean() const {
    double sum = 0;
    for (double value: values) {
        sum += value;
    }
    return sum / values.size();
}

double Distribution::findVar() const {
    double squaredSum = 0;
    for (double value: values) {
        squaredSum += value * value;
    }
    return squaredSum / values.size() - mean * mean;
}

void Distribution::print() const {
    for (double value: values) {
        std::cout << value << "\n";
    }
}

void Distribution::findGist(int intervalNum, std::vector<int>& gistVec, std::vector<double>& rangesVec) const {
    gistVec.resize(intervalNum, 0);
    rangesVec.resize(intervalNum, 0);
    auto [min, max] = findMinMax();
    double step = (max - min) / intervalNum;
    for (const auto& value: values) {
        int interval = 0;
        while (min + (interval + 1) * step < value - 0.000001) {
            interval++;
        }
        gistVec[interval]++;
    }

    rangesVec[0] = min;
    for (int i = 1; i < intervalNum; ++i) {
        rangesVec[i] = rangesVec[i - 1] + step;
    }
}

std::pair<double, double> Distribution::findMinMax() const {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();
    for (const auto& value: values) {
        min = std::min(value, min);
        max = std::max(value, max);
    }

    return {min, max};
}

void Distribution::drawGist(int multiplication) const {
    std::vector<std::vector<char>> result;
    int maxGist = -1;
    for (int count: gist) {
        maxGist = std::max(maxGist, count / multiplication + 1);
    }
    for (int count: gist) {
        result.emplace_back(maxGist, ' ');
        int total = count == 0 ? 0 : count / multiplication + 1;
        for (int j = 0; j < total; ++j) {
            result.back()[j] = '*';
        }
    }
    for (int j = maxGist - 1; j >= 0; --j) {
        for (int i = 0; i < gist.size(); ++i) {
            std::cout << result[i][j];
        }
        std::cout << '\n';
    }
}

bool Distribution::ChiSquaredTest(int basicIntervalNum, const Distribution* dist) {
    std::vector<int> initGistVec;
    std::vector<double> initRangesVec;
    std::vector<int> gistVec;
    std::vector<double> rangesVec;
    dist->findGist(basicIntervalNum, initGistVec, initRangesVec);
    auto [min, max] = dist->findMinMax();
    int sumGist = 0;
    double lastRange = min;
    int totalSum = 0;
    for (int i = 0; i < basicIntervalNum; ++i) {
        sumGist += initGistVec[i];
        totalSum += initGistVec[i];
        if (sumGist > 5) {
            gistVec.push_back(sumGist);
            rangesVec.push_back(lastRange);
            sumGist = 0;
            if (i < basicIntervalNum - 1) {
                lastRange = initRangesVec[i + 1];
            } else {
                lastRange = max;
            }
        }
    }
    rangesVec.push_back(lastRange);
    if (sumGist != 0) {
        gistVec.back() += sumGist;
        rangesVec.back() = max;
    }

    double ChiSquared = 0;
    for (int i = 0; i < gistVec.size(); ++i) {
        double cumulative1 = dist->findCumulative(rangesVec[i]);
        double cumulative2 = dist->findCumulative(rangesVec[i + 1]);
        double pt = cumulative2 - cumulative1;
        double t = pt * totalSum;
        double k = gistVec[i];
        double l = pow(gistVec[i] - totalSum * pt, 2) / (totalSum * pt);
        ChiSquared += pow(gistVec[i] - totalSum * pt, 2) / (totalSum * pt);
    }
    int k = gistVec.size() - 2;
    return ChiSquared < TheoreticalChiSquare[k];
}

void Distribution::printGist() const {
    for (int count: gist) {
        std::cout << count << " ";
    }
    std::cout << "\n";
}
