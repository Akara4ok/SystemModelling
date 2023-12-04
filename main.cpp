#include <iostream>

#include "Model/ModelFactory.h"
#include "Model/Utils/Logger.h"
#include "Model/Elements/MultiChannelProcess.h"

int main() {
    srand(time(NULL));
    Logger logger("Logs.csv");
//    int num = 100;
//    double avgLoad{};
//    double avgProb{};

//    for (int i = 0; i < num; ++i) {
        Model model = ModelFactory::createModel1();

//        model.getElementByName("Machine Second Phase")->setSummaryFunction([&avgLoad](Element* element){
//            auto* casted_el = dynamic_cast<MultiChannelProcess*>(element);
//            avgLoad += casted_el->getAverageLoad(1);
//        });
//        model.setSummaryFunction([&avgProb](Model* model){
//            int defected = model->getElementByName("Defected")->getProceed();
//            int proceed11 = model->getElementByName("First Machine First Phase")->getProceed();
//            int proceed12 = model->getElementByName("Second Machine First Phase")->getProceed();
//            avgProb += (double)defected / (proceed11 + proceed12);
//        });
        model.simulate(100000);
//    }

//    std::cout << "Avg load: " << avgLoad / num << "\n";
//    std::cout << "Avg prob: " << avgProb / num << "\n";

    Logger::saveLogFile();
}
