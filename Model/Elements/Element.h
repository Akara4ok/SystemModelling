//
// Created by vlad on 10/8/23.
//

#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <memory>
#include <utility>
#include <functional>
#include "../Utils/ExpDist.h"
#include "../Utils/Logger.h"
#include "../Utils/ElementPicker.h"

class Element {
public:
    Element(std::string name, std::shared_ptr<ExpDist> gen);
    Element(std::string name, std::shared_ptr<ExpDist> gen, std::shared_ptr<ElementPicker> elementPicker);

    virtual void start() = 0;
    virtual void finish() = 0;
    virtual bool isFinished();
    virtual void updateCurrentTime(double currentTime);
    virtual void summary();
    virtual void log() const = 0;

    virtual double getNextTime(){ return mNextTime; }
    double getCurrentTime(){ return mCurrentTime; }
    virtual std::string getName(){ return mName; }
    void setSummaryFunction(std::function<void(Element*)> summaryFunction) { mSummaryFunction = std::move(summaryFunction); }

    virtual int getProceed(){return mProceed;}

    void setElementPicker(std::shared_ptr<ElementPicker> picker) { mNextElementPicker = picker; };

protected:
    std::shared_ptr<Element> getNextElement();

    std::string mName{};
    double mCurrentTime{};
    double mNextTime{};
    std::shared_ptr<ExpDist> mGen{};
    int mProceed{};
    std::function<void(Element*)> mSummaryFunction;
    std::shared_ptr<ElementPicker> mNextElementPicker;
};


#endif //ELEMENT_H
