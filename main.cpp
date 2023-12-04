#include <iostream>

#include "Model/ModelFactory.h"
#include "Model/Utils/Logger.h"
#include "Model/Elements/MultiChannelProcess.h"

int main() {
    srand(time(NULL));
    Logger logger("Logs.csv");
    Model model = ModelFactory::createModel1();

    model.simulate(100000);

    Logger::saveLogFile();
}
