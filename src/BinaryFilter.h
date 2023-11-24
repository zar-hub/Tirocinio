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
    Bool_t (*filter)() = nullptr;
    Bool_t isLeaf = false;

public:
    SmartGraph *histo;             // this is the histo of the PARENT!
    BinaryFilter *left = nullptr;  // left child
    BinaryFilter *right = nullptr; // right child

    BinaryFilter(const string &name, FitPrototype &fitPrototype, TVirtualPad *nodePad) : name(name)
    {
        // < Filter NOT given />
        // the object acts like a leaf
        cout << "BinaryFilter leaf : " << name << " created" << endl;
        isLeaf = true;
        histo = new SmartGraph(name.c_str(), fitPrototype, nodePad);
    }

    BinaryFilter(const string &name, Bool_t (*filter)(), FitPrototype &passedPrototype, FitPrototype &failedPrototype, TVirtualPad *nodePad) : name(name), filter(filter)
    {
        // < Filter IS given />
        // the object acts like a node
        cout << "BinaryFilter: " << name << " created" << endl;
        nodePad->Divide(2, 1);
        left = new BinaryFilter(name + "Passed", passedPrototype, nodePad->GetPad(1));
        right = new BinaryFilter(name + "Failed", failedPrototype, nodePad->GetPad(2));
    }

    Double_t FitAndDraw()
    {
        // the leaf just renders the histo
        if (isLeaf)
        {
            histo->FitAndDraw();
            return histo->getSignalIntegral();
        }

        left->FitAndDraw();
        right->FitAndDraw();
        areaPassed = left->histo->getSignalIntegral();
        areaFailed = right->histo->getSignalIntegral();
        efficiency = areaPassed / (areaPassed + areaFailed);
        efficencyError = sqrt(efficiency * (1 - efficiency) / (areaPassed + areaFailed));
        return areaPassed + areaFailed;
    }

    void printInfo()
    {
        icout << "BinaryFilter: " << name << endl;
        icout.identUp();
        icout << "Passed: " << endl;
        icout.identUp();
        left->histo->printInfo();
        icout.identDown();
        icout << "Failed: " << endl;
        icout.identUp();
        right->histo->printInfo();
        icout.identDown();
        // efficiency considerations
        icout << "Efficiency: " << efficiency << "\u00b1" << efficencyError << endl;
        icout.identDown();
    }
};

#endif // BINARYFILTER_H