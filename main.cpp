#include <iostream>

#include "Model/ModelFactory.h"
#include "Model/Utils/Logger.h"
#include "Model/Elements/MultiChannelProcess.h"

void findMixedState1();
void findMixedState2();
void tacticalExperiment();
void analysisOfVariance();

int main() {
    srand(time(NULL));
//    Logger logger("Logs.csv");
    analysisOfVariance();
    Logger::saveLogFile();
}

void findMixedState1() {
    std::ofstream additionalLogs("Additional Logs.csv");
    std::vector<std::vector<double>> values(5);
    double interval = 25;

    for (int i = 0; i < 5; ++i) {
        Model model = ModelFactory::createModel1();

        double sumInterval = 0;

        model.addCallback([interval, &sumInterval, &additionalLogs, &values, i](Model* model) {
            double currentTime = model->getCurrentTime();
            MultiChannelProcess* process = dynamic_cast<MultiChannelProcess*>(model->getElementByName(
                    "Machine Second Phase").get());
            while (sumInterval < currentTime) {
                values[i].push_back(process->getAverageLoad(1));
                sumInterval += interval;
            }
        });

        model.simulate(100000);
    }

    double sumInterval = 0;
    additionalLogs << "Time; Avg load1;Avg load2;Avg load3;Avg load4;Avg load5;\n";
    for (int i = 0; i < values[0].size(); ++i) {
        additionalLogs << sumInterval << ";" << values[0][i] << ";" << values[1][i] << ";" << values[2][i]
                       << ";" << values[3][i] << ";" << values[4][i] << "\n";
        sumInterval += interval;
    }
}

void findMixedState2() {
    std::ofstream additionalLogs("Additional Logs.csv");
    std::vector<std::vector<double>> values(5);
    double interval = 25;

    for (int i = 0; i < 5; ++i) {
        Model model = ModelFactory::createModel1();

        double sumInterval = 0;

        model.addCallback([interval, &sumInterval, &additionalLogs, &values, i](Model* model) {
            int defected = model->getElementByName("Defected")->getProceed();
            int proceed11 = model->getElementByName("First Machine First Phase")->getProceed();
            int proceed12 = model->getElementByName("Second Machine First Phase")->getProceed();

            double currentTime = model->getCurrentTime();
            while (sumInterval < currentTime) {
                if(proceed11 + proceed12 != 0){
                    values[i].push_back((double) defected / (proceed11 + proceed12));
                } else {
                    values[i].push_back(0);
                }

                sumInterval += interval;
            }
        });

        model.simulate(100000);
    }

    double sumInterval = 0;
    additionalLogs << "Time; Probability1;Probability2;Probability3;Probability4;Probability5;\n";
    for (int i = 0; i < values[0].size(); ++i) {
        additionalLogs <<  sumInterval << ";" << values[0][i] << ";" << values[1][i] << ";" << values[2][i]
                       << ";" << values[3][i] << ";" << values[4][i] << "\n";
        sumInterval += interval;
    }
}

void tacticalExperiment() {
    int num = 80;
    double avgLoad{};
    double avgProb{};

    for (int i = 0; i < num; ++i) {
        Model model = ModelFactory::createModel2();

        model.getElementByName("Machine Second Phase")->setSummaryFunction([&avgLoad](Element* element){
            auto* casted_el = dynamic_cast<MultiChannelProcess*>(element);
            avgLoad += casted_el->getAverageLoad(1);
        });
        model.setSummaryFunction([&avgProb](Model* model){
            int defected = model->getElementByName("Defected")->getProceed();
            int proceed11 = model->getElementByName("First Machine First Phase")->getProceed();
            int proceed12 = model->getElementByName("Second Machine First Phase")->getProceed();
            avgProb += (double)defected / (proceed11 + proceed12);
        });
        model.setExperimentData(60000);
        model.simulate(200000);
    }

    std::cout << "Avg load: " << avgLoad / num << "\n";
    std::cout << "Avg prob: " << avgProb / num << "\n";
}

void analysisOfVariance() {
    int num = 80;
    int level = 3;

    std::vector<std::vector<double>> avgLoads(num, std::vector<double>(level));

    //collect avg
    for (int j = 0; j < level; ++j) {
        for (int i = 0; i < num; ++i) {
            Model model = ModelFactory::createModel2();
            auto* secondPhase = dynamic_cast<MultiChannelProcess*>(model.getElementByName("Machine Second Phase").get());
            if(j == 0){
                secondPhase->setBlocker(1, [](const MultiChannelProcess* el){
                    return el->getCurrentQueueSize() <= 3;
                });
            } else if(j == 1){
                secondPhase->setBlocker(1, [](const MultiChannelProcess* el){
                    return el->getCurrentQueueSize() <= 2;
                });
            } else if(j == 2){
                secondPhase->setBlocker(1, [](const MultiChannelProcess* el){
                    return el->getCurrentQueueSize() <= 1;
                });
            }
            secondPhase->setSummaryFunction([&avgLoads, i, j](Element* element){
                auto* casted_el = dynamic_cast<MultiChannelProcess*>(element);
                avgLoads[i][j] = casted_el->getAverageLoad(1);
            });
            model.setSummaryFunction([](Model* model){});
            model.setExperimentData(60000);
            model.simulate(200000);
        }
    }

    //calculate averages
    std::vector<double> avgByLevel(level);
    double totalAvg{};

    for (int j = 0; j < level; ++j) {
        double levelSum{};
        for (int i = 0; i < num; ++i) {
            levelSum += avgLoads[i][j];
        }
        avgByLevel[j] = levelSum / num;
        totalAvg += levelSum;
    }
    totalAvg /= level * num;

    //calculate F
    double sigma1{};
    double sigma2{};

    for (int j = 0; j < level; ++j) {
        sigma1 += num * pow(avgByLevel[j] - totalAvg, 2);
    }
    sigma1 *= level * num - level;

    for (int j = 0; j < level; ++j) {
        for (int i = 0; i < num; ++i) {
            sigma2 += num * pow(avgLoads[i][j] - avgByLevel[j], 2);
        }
    }
    sigma2 *= level - 1;

    double f = sigma1 / sigma2;
    std::cout << "F: " << f << "\n";
}