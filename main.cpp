#include <iostream>

#include "Model/ModelFactory.h"
#include "Model/Utils/Logger.h"

int main() {
    srand(time(NULL));
    Logger logger("Logs.csv");
    Model model1 = ModelFactory::createModel1();
    model1.simulate(100000);
    Logger::saveLogFile();

}
