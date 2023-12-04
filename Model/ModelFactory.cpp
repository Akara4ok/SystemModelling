//
// Created by vlad on 10/8/23.
//

#include "ModelFactory.h"

#include <memory>
#include "Elements/Process.h"
#include "Elements/Dispose.h"
#include "Elements/FirstPhaseMachine.h"
#include "Elements/Create.h"
#include "Elements/MultiChannelProcess.h"
#include "Utils/ExpDist.h"
#include "Utils/ElementPickers/ProbabilityElementPicker.h"
#include "Utils/ElementPickers/LessQueuePicker.h"
#include <map>

#include <iostream>

Model ModelFactory::createModel1() {
    std::shared_ptr<Dispose> finished(new Dispose("Finished"));
    std::shared_ptr<Dispose> defected(new Dispose("Defected"));

    std::shared_ptr<MultiChannelProcess> process3(new MultiChannelProcess("Machine Second Phase",
                                                  std::make_shared<ExpDist>(1.0 / 100),
                                                  std::shared_ptr<ProbabilityElementPicker>(
                                                          new ProbabilityElementPicker({{finished, 1}})),
                                                  std::make_shared<Queue>(), 2));
    process3->setBlocker(1, [](const MultiChannelProcess* el){
        return el->getCurrentQueueSize() <= 3;
    });

    std::shared_ptr<FirstPhaseMachine> process2(
            new FirstPhaseMachine("Second Machine First Phase",
                                  std::make_shared<ExpDist>(1.0 / 60),
                                  std::make_shared<TypedQueue<int>>()));
    process2->setElementPicker(std::shared_ptr<PredicateElementPicker<int>>(
            new PredicateElementPicker<int>({{process3, 0.92},
                                             {process2, 0.08},
                                             {defected}}, [](int attempt) { return attempt == 2; })));

    std::shared_ptr<Element> process1(
            new FirstPhaseMachine("First Machine First Phase",
                                  std::make_shared<ExpDist>(1.0 / 40),
                                  std::shared_ptr<ProbabilityElementPicker>(
                                          new ProbabilityElementPicker({{process3, 0.96},
                                                                           {process2, 0.04}})),
                                  std::make_shared<TypedQueue<int>>()));

    std::shared_ptr<Element> create(
            new Create("Create",
                       std::make_shared<ExpDist>(1.0 / 50),
                       std::shared_ptr<LessQueuePicker>(new LessQueuePicker({process1, process2}))
            ));

    std::vector<std::shared_ptr<Element>> elements;
    elements.push_back(create);
    elements.push_back(process1);
    elements.push_back(process2);
    elements.push_back(process3);
    elements.push_back(finished);
    elements.push_back(defected);

    auto model = Model(std::move(elements));
    auto emptySummary = [](Element* el){};
    create->setSummaryFunction(emptySummary);
    process1->setSummaryFunction(emptySummary);
    process2->setSummaryFunction(emptySummary);
    defected->setSummaryFunction(emptySummary);
    finished->setSummaryFunction(emptySummary);

    model.getElementByName("Machine Second Phase")->setSummaryFunction([](Element* element){
        auto* casted_el = dynamic_cast<MultiChannelProcess*>(element);
        std::cout << "------------\n";
        std::cout << "Name: " << casted_el->getName() << " Second Channel " << "\n";
        std::cout << "Average load: " << casted_el->getAverageLoad(1) << "\n";
        std::cout << "------------\n";
    });
    model.setSummaryFunction([](Model* model) {
        int defected = model->getElementByName("Defected")->getProceed();
        int proceed11 = model->getElementByName("First Machine First Phase")->getProceed();
        int proceed12 = model->getElementByName("Second Machine First Phase")->getProceed();

        std::cout << "------------\n";
        std::cout << "defected percent: " << (double) defected / (proceed11 + proceed12) << "\n";
        std::cout << "------------\n";
    });

    return model;
}