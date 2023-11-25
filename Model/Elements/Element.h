//
// Created by vlad on 10/8/23.
//

#ifndef LAB2_ELEMENT_H
#define LAB2_ELEMENT_H

#include <string>
#include <memory>
#include <utility>
#include "../../Distribution/Distribution.h"
#include "../Utils/TimeGenerator.h"
#include "../Utils/Logger.h"
#include "../Utils/ElementPicker.h"

enum FreeStatus {
    Full,
    Busy,
    Free
};

class Element {
public:
    Element(std::string name, std::shared_ptr<TimeGenerator> gen);
    Element(std::string name, std::shared_ptr<TimeGenerator> gen, std::shared_ptr<ElementPicker> elementPicker);

    virtual void start() = 0;
    virtual void finish() = 0;
    virtual bool isFinished();
    virtual FreeStatus isFree() = 0;
    virtual void updateCurrentTime(double currentTime);
    virtual void summary();
    virtual void log() const = 0;

    virtual double getNextTime(){ return mNextTime; }
    double getCurrentTime(){ return mCurrentTime; }
    virtual std::string getName(){ return mName; }
    void setNextTime(double nextTime);
    void setSummaryFunction(std::function<void(Element*)> summaryFunction) { mSummaryFunction = std::move(summaryFunction); }

    virtual int getProceed(){return mProceed;}

    void setElementPicker(std::shared_ptr<ElementPicker> picker) { mNextElementPicker = picker; };

protected:
    std::shared_ptr<Element> getNextElement();

    std::string mName{};
    double mCurrentTime{};
    double mNextTime{};
    std::shared_ptr<TimeGenerator> mGen{};
    int mProceed{};
    std::function<void(Element*)> mSummaryFunction;
    std::shared_ptr<ElementPicker> mNextElementPicker;
};


#endif //LAB2_ELEMENT_H
