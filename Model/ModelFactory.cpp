//
// Created by vlad on 10/8/23.
//

#include "ModelFactory.h"

#include <memory>
#include "Elements/Process.h"
#include "Elements/Dispose.h"
#include "Elements/FirstPhaseMachine.h"
#include "Elements/Create.h"
#include "Elements/FirstPhaseMachine.h"
#include "Utils/TimeGenerator.h"
#include "../Distribution/UniformDist.h"
#include "../Distribution/ExpDist.h"
#include "Utils/ElementPickers/ProbabilityElementPicker.h"
#include <map>

#include <iostream>

Model ModelFactory::createModel1() {
    std::shared_ptr<Dispose> Finished(new Dispose("Finished"));
    std::shared_ptr<Dispose> Defected(new Dispose("Defected"));

    std::shared_ptr<Process> process3(new Process("Machine Second Phase",
                                                  std::make_shared<TimeGenerator>(new ExpDist(1.0 / 100)),
                                                  std::shared_ptr<ProbabilityElementPicker>(
                                                          new PredicateElementPicker<int>({{Finished, 1}})),
                                                  std::make_shared<Queue>(), 2));
    process3->setBlocker(1, [](Process* el){
        std::cout << el->getCurrentQueue() << "\n";
        return el->getCurrentQueue() < 2;
    });

    std::shared_ptr<FirstPhaseMachine> process2(
            new FirstPhaseMachine("Second Machine First Phase",
                                  std::make_shared<TimeGenerator>(new ExpDist(1.0 / 60)),
                                  std::make_shared<TypedQueue<int>>()));
    process2->setElementPicker(std::shared_ptr<PredicateElementPicker<int>>(
            new PredicateElementPicker<int>({{process3, 0.92},
                                             {process2, 0.08},
                                             {Defected}}, [](int attempt) { return attempt == 2; })));

    std::shared_ptr<Element> process1(
            new FirstPhaseMachine("First Machine First Phase",
                                  std::make_shared<TimeGenerator>(new ExpDist(1.0 / 40)),
                                  std::shared_ptr<PredicateElementPicker<int>>(
                                          new PredicateElementPicker<int>({{process3, 0.92},
                                                                           {process2, 0.08}})),
                                  std::make_shared<TypedQueue<int>>()));

    std::shared_ptr<Element> create(
            new Create("Create",
                       std::make_shared<TimeGenerator>(new ExpDist(1.0 / 50)),
                       std::shared_ptr<ElementPicker>(new ProbabilityElementPicker({{process1, 0.5},
                                                                                    {process2, 0.5}}))
            ));

    std::vector<std::shared_ptr<Element>> elements;
    elements.push_back(create);
    elements.push_back(process1);
    elements.push_back(process2);
    elements.push_back(process3);
    elements.push_back(Finished);
    elements.push_back(Defected);

    return Model(std::move(elements));
}