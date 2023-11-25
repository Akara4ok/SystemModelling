//
// Created by vlad on 10/8/23.
//

#include "ModelFactory.h"

#include <memory>
#include "Elements/Process.h"
#include "Elements/Create.h"
#include "Elements/TypedCreate.h"
#include "Elements/TypedProcess.h"
#include "Elements/PriorTypedProcess.h"
#include "Elements/ChangingTypedProcess.h"
#include "Utils/TimeGenerator.h"
#include "Utils/TypedTimeGenerator.h"
#include "../Distribution/UniformDist.h"
#include "../Distribution/ExpDist.h"
#include "../Distribution/ErlangDist.h"
#include "../Distribution/NormalDist.h"
#include "Utils/ElementPickers/PriorityElementPicker.h"
#include "Utils/ElementPickers/ProbabilityElementPicker.h"
#include "Utils/ElementPickers/FirstElementPicker.h"
#include "Utils/ElementPickers/TypedPicker.h"
#include <map>

#include "Utils/Queue/DecreaseObserverQueue.h"
#include <iostream>

Model ModelFactory::createTask1() {
    std::shared_ptr<Element> process3(
            new Process("Process3",
                        std::make_shared<TimeGenerator>(new ExpDist(20)),
                        std::make_shared<Queue>(5)));

    std::shared_ptr<Element> process2(
            new Process("Process2",
                        std::make_shared<TimeGenerator>(new ExpDist(10)),
                        std::shared_ptr<ElementPicker>(new PriorityElementPicker({process3})),
                        std::make_shared<Queue>(5)));

    std::shared_ptr<Element> process1(
            new Process("Process1",
                        std::make_shared<TimeGenerator>(new ExpDist(3)),
                        std::shared_ptr<ElementPicker>(new ProbabilityElementPicker({{process2, 0.25},
                                                                                     {process3, 0.75}})),
                        std::make_shared<Queue>(5)));

    std::shared_ptr<Element> create(
            new Create("Create",
                       std::make_shared<TimeGenerator>(new ExpDist(5)),
                       std::shared_ptr<ElementPicker>(new ProbabilityElementPicker({{process1, 0.5},
                                                                                    {process2, 0.5}}))
            ));

    std::vector<std::shared_ptr<Element>> elements;
    elements.push_back(create);
    elements.push_back(process1);
    elements.push_back(process2);
    elements.push_back(process3);

    return Model(std::move(elements));
}

Model ModelFactory::createTask2() {
    std::shared_ptr<DecreaseObserverQueue> queue1(new DecreaseObserverQueue(3));
    std::shared_ptr<DecreaseObserverQueue> queue2(new DecreaseObserverQueue(3));
    queue1->setSubscribers({queue2});
    queue2->setSubscribers({queue1});

    std::function<void(Element*)> summaryCashier = [](Element* cashier) {
        Process* castedCashier = dynamic_cast<Process*>(cashier);
        std::cout << "------------\n";
        std::cout << "Name: " << castedCashier->getName() << "\n";
        std::cout << "Failures: " << castedCashier->getFailures() << "\n";
        std::cout << "Processed: " << castedCashier->getProceed() << "\n";
        std::cout << "Average load: " << castedCashier->getAverageLoad() << "\n";
        std::cout << "Average queue size: " << castedCashier->getAverageQueue() << "\n";
        std::cout << "Swaps: "
                  << dynamic_cast<DecreaseObserverQueue*>(castedCashier->getQueue().get())->getSwaps() << "\n";
        std::cout << "------------\n";
    };

    std::function<void(const std::vector<std::shared_ptr<Element>>&, double)> summaryModel = [](
            const std::vector<std::shared_ptr<Element>>& elements, double time) {
        int created = 0;
        int proceed = 0;
        int failed = 0;
        int processingElements = 0;
        int elementsInQueue = 0;
        double summedElementsInProcess = 0;
        double onlyWorkingTime = 0;
        for (const auto& element: elements) {
            if (auto createdElement = dynamic_cast<Create*>(element.get())) {
                created += createdElement->getProceed();
            }
            if (auto processElement = dynamic_cast<Process*>(element.get())) {
                proceed += processElement->getProceed();
                failed += processElement->getFailures();
                onlyWorkingTime += time * processElement->getAverageLoad();
                elementsInQueue += processElement->getCurrentQueue();
                summedElementsInProcess += processElement->getSummedElements();
                processingElements++;
            }
        }
        onlyWorkingTime /= processingElements;
        double avgClients = (double) summedElementsInProcess / time;
        std::cout << "Average clients: " << avgClients << "\n";
        std::cout << "Average leave interval: " << (double) time / proceed << "\n";
        std::cout << "Client average time in bank: " << avgClients * (time / proceed) << "\n";
        std::cout << "Failed probability: " << (double) failed / created << "\n";
    };

    std::shared_ptr<Element> cashier2(
            new Process("Cashier2",
                        std::make_shared<TimeGenerator>(new ExpDist(1 / 0.3)),
                        queue2));

    dynamic_cast<Process*>(cashier2.get())->setInitialValues(
            2,
            {std::make_shared<SubProcess>(SubProcess(true, NormalDist(0.3, 1).generateNext()))}
    );
    cashier2->setSummaryFunction(summaryCashier);

    std::shared_ptr<Element> cashier1(
            new Process("Cashier1",
                        std::make_shared<TimeGenerator>(new ExpDist(1 / 0.3)),
                        queue1));

    dynamic_cast<Process*>(cashier1.get())->setInitialValues(
            2,
            {std::make_shared<SubProcess>(SubProcess(true, NormalDist(0.3, 1).generateNext()))}
    );
    cashier1->setSummaryFunction(summaryCashier);

    std::shared_ptr<Element> create(
            new Create("Create",
                       std::make_shared<TimeGenerator>(new ExpDist(1 / 0.5)),
                       std::shared_ptr<ElementPicker>(new FirstElementPicker({cashier1, cashier2}))
            ));

    dynamic_cast<Create*>(create.get())->setInitialValues(0.1);

    std::vector<std::shared_ptr<Element>> elements;
    elements.push_back(create);
    elements.push_back(cashier1);
    elements.push_back(cashier2);

    auto model = Model(std::move(elements));
    model.setSummaryFunction(summaryModel);
    return model;
}

Model ModelFactory::createTask3() {
    auto emptySummary = [](Element* el){};
    std::shared_ptr<Element> toReception(
            new ChangingTypedProcess("To Reception",
                             std::make_shared<TimeGenerator>(new UniformDist(), [](double n){
                                 return 3 * n + 2;
                             }),
                             std::make_shared<TypedQueue>(),
                                     1, 0,
                            100
            )
    );
    toReception->setSummaryFunction(emptySummary);

    std::shared_ptr<Element> analysis(
            new TypedProcess("Analysis",
                             std::make_shared<TimeGenerator>(new ErlangDist(4, 2)),
                             std::make_shared<TypedPicker>(std::map<int, std::shared_ptr<Element>>({
                                       std::make_pair(1, toReception),
                                       std::make_pair(2, nullptr)
                               })),
                             std::make_shared<TypedQueue>(),
                             2
            )
    );

    std::function<void(Element*)> summaryAnalysis = [](Element* analysis) {
        TypedProcess* castedAnalysis = dynamic_cast<TypedProcess*>(analysis);
        std::cout << "------------\n";
        std::cout << "Name: " << castedAnalysis->getName() << "\n";
        std::cout << "Processed: " << castedAnalysis->getProceed() << "\n";
        std::cout << "Processed 1: " << castedAnalysis->getTypedProceed()[1] << "\n";
        std::cout << "Processed 2: " << castedAnalysis->getTypedProceed()[2] << "\n";
        std::cout << "Avg time 2: " << castedAnalysis->getTypedTime()[2] / castedAnalysis->getTypedProceed()[2] << "\n";
        std::cout << "------------\n";
    };

    analysis->setSummaryFunction(summaryAnalysis);

    std::shared_ptr<Element> registration(
            new TypedProcess("Registration",
                             std::make_shared<TimeGenerator>(new ErlangDist(4.5, 3)),
                             std::make_shared<PriorityElementPicker>(std::vector<std::shared_ptr<Element>>({analysis})),
                             std::make_shared<TypedQueue>()
            )
    );

    registration->setSummaryFunction(emptySummary);

    std::shared_ptr<Element> toRegistration(
            new TypedProcess("To Registration",
                             std::make_shared<TimeGenerator>(new UniformDist(), [](double n){
                                 return 5 * n + 3;
                             }),
                             std::make_shared<PriorityElementPicker>(std::vector<std::shared_ptr<Element>>({registration})),
                             std::make_shared<TypedQueue>(),
                             1000
            )
    );

    toRegistration->setSummaryFunction([](Element* toRegistration){
        TypedProcess* castedToRegistration = dynamic_cast<TypedProcess*>(toRegistration);
        std::cout << "------------\n";
        std::cout << "Name: " << castedToRegistration->getName() << "\n";
        std::cout << "Interval: " << castedToRegistration->getCurrentTime() / castedToRegistration->getProceed() << "\n";
        std::cout << "------------\n";
    });

    std::function<void(Element*)> summaryPalata = [](Element* palata) {
        TypedProcess* castedPalata = dynamic_cast<TypedProcess*>(palata);
        std::cout << "------------\n";
        std::cout << "Name: " << castedPalata->getName() << "\n";
        std::cout << "Processed: " << castedPalata->getProceed() << "\n";
        std::cout << "Processed 0: " << castedPalata->getTypedProceed()[0] << "\n";
        std::cout << "Processed 1: " << castedPalata->getTypedProceed()[1] << "\n";
        std::cout << "Avg time 0: " << castedPalata->getTypedTime()[0] / castedPalata->getTypedProceed()[0] << "\n";
        std::cout << "Avg time 1: " << castedPalata->getTypedTime()[1] / castedPalata->getTypedProceed()[1] << "\n";
        std::cout << "------------\n";
    };

    std::shared_ptr<Element> toPalata(
            new TypedProcess("To Palata",
                             std::make_shared<TimeGenerator>(new UniformDist(), [](double n){
                                 return 5 * n + 3;
                             }),
                             std::make_shared<TypedQueue>(),
                             3
            )
            );

    toPalata->setSummaryFunction(summaryPalata);

    std::shared_ptr<Element> reception(
            new PriorTypedProcess("Reception",
                              std::make_shared<TypedTimeGenerator>(std::map<int, TimeGenerator>({
                                            std::make_pair(0, TimeGenerator(new ExpDist(1.0 / 15))),
                                            std::make_pair(1, TimeGenerator(new ExpDist(1.0 / 40))),
                                            std::make_pair(2, TimeGenerator(new ExpDist(1.0 / 30)))
                                    })),
                             std::make_shared<TypedPicker>(std::map<int, std::shared_ptr<Element>>({
                                   std::make_pair(0, toPalata),
                                   std::make_pair(1, toRegistration),
                                   std::make_pair(2, toRegistration)
                             })),
                             std::make_shared<TypedQueue>(),
                             0,
                             2
            ));

    toReception->setElementPicker(std::make_shared<PriorityElementPicker>(std::vector<std::shared_ptr<Element>>({reception})));
    reception->setSummaryFunction(emptySummary);

    auto createFreq = std::map<int, double>({
                                                    std::make_pair(0, 0.5),
                                                    std::make_pair(1, 0.1),
                                                    std::make_pair(2, 0.4)
                                            });

    std::shared_ptr<Element> create(
            new TypedCreate("Create",
                            std::make_shared<TimeGenerator>(new ExpDist(1.0 / 15)),
                            std::shared_ptr<ElementPicker>(new PriorityElementPicker({reception})),
                            createFreq
            ));
    create->setSummaryFunction(emptySummary);

    std::vector<std::shared_ptr<Element>> elements;
    elements.push_back(toReception);
    elements.push_back(analysis);
    elements.push_back(registration);
    elements.push_back(toRegistration);
    elements.push_back(toPalata);
    elements.push_back(reception);
    elements.push_back(create);

    auto model = Model(std::move(elements));

    return model;
}
