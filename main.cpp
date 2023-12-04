#include <iostream>

#include "Model/ModelFactory.h"
#include "Model/Utils/Logger.h"
#include "Model/Elements/MultiChannelProcess.h"

void simulate1();

int main() {
    srand(time(NULL));
    Logger logger("Logs.csv");
    simulate1();
    Logger::saveLogFile();
}

void simulate1(){
    Model model = ModelFactory::createModel1();

    std::ofstream additionalLogs("Additional Logs.csv");
    additionalLogs << "Time, Avg load\n";
    double interval = 25;
    double sumInterval = 0;

    model.addCallback([interval, &sumInterval, &additionalLogs](Model* model){
        double currentTime = model->getCurrentTime();
        MultiChannelProcess* process = dynamic_cast<MultiChannelProcess*>(model->getElementByName("Machine Second Phase").get());
        while (sumInterval < currentTime){
            additionalLogs << sumInterval << "," << process->getAverageLoad(1) << "\n";
            sumInterval += interval;
        }
    });

    model.simulate(100000);
}
