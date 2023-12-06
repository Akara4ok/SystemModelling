#include <iostream>

#include "Model/ModelFactory.h"
#include "Model/Utils/Logger.h"
#include "Model/Elements/MultiChannelProcess.h"

void simulate1();
void simulate2();
void simulate3();

int main() {
    srand(time(NULL));
//    Logger logger("Logs.csv");
    simulate3();
    Logger::saveLogFile();
}

void simulate1() {
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

void simulate2() {
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

void simulate3() {
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
        model.setExperimentData(70000);
        model.simulate(200000);
    }

    std::cout << "Avg load: " << avgLoad / num << "\n";
    std::cout << "Avg prob: " << avgProb / num << "\n";
}