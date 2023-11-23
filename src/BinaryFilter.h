#ifndef BINARYFILTER_H
#define BINARYFILTER_H

#include "global.h"
#include "SmartGraph.h"

class BinaryFilter
{
    string name;
    Double_t areaPassed;
    Double_t areaFailed;
    Double_t efficiency;
    Double_t efficencyError;

public:
    SmartGraph *passed;
    SmartGraph *failed;

    BinaryFilter(const string &name, FitPrototype &passedPrototype, FitPrototype &failedPrototype, TVirtualPad *padPassed, TVirtualPad *padFailed) : name(name)
    {
        passed = new SmartGraph((name + "Passed").c_str(), passedPrototype, padPassed);
        failed = new SmartGraph((name + "Failed").c_str(), failedPrototype, padFailed);
        passed->setStyle(true);
        failed->setStyle(false);
    }

    void FitAndDraw()
    {
        // the two operations must be done both
        passed->FitAndDraw();
        failed->FitAndDraw();
        areaPassed = passed->getSignalIntegral();
        areaFailed = failed->getSignalIntegral();
        efficiency = areaPassed / (areaPassed + areaFailed);
        efficencyError = sqrt(efficiency * (1 - efficiency) / (areaPassed + areaFailed));
    }

    void printInfo()
    {
        icout << "BinaryFilter: " << name << endl;
        icout.identUp();
        icout << "Passed: " << endl;
        icout.identUp();
        passed->printInfo();
        icout.identDown();
        icout << "Failed: " << endl;
        icout.identUp();
        failed->printInfo();
        icout.identDown();
        // efficiency considerations
        icout << "Efficiency: " << efficiency << "\u00b1" << efficencyError << endl;
        icout.identDown();
    }
};

#endif // BINARYFILTER_H