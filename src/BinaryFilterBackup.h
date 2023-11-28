#ifndef BINARYFILTER_H
#define BINARYFILTER_H

#include "global.h"
#include "SmartGraph.h"

class BinaryFilter
{
    // data
    string name;
    Double_t areaPassed;
    Double_t areaFailed;
    Double_t efficiency;
    Double_t efficencyError;

    // state variables
    Bool_t isLeaf = false;
    Bool_t isBranch = false;
    Bool_t isSeed = true;

    // children
    Bool_t (*filter)() = nullptr;
    TVirtualPad *branchPad = nullptr;
    BinaryFilter *left = nullptr;  // left child
    BinaryFilter *right = nullptr; // right child

public:
    SmartGraph *histo = nullptr; // this is the histo of the PARENT!

    // < SEED: can become a leaf or a Branch />
    BinaryFilter(const string &name, TVirtualPad *branchPad) : name(name), branchPad(branchPad)
    {
        isSeed = true;
        isLeaf = false;
        isBranch = false;
        cout << "BinaryFilter seed: " << name << " created" << endl;
    }

    // < LEAF: filter NOT given />
    // the object acts like a wrapper for the smartgraph
    BinaryFilter(const string &name, FitPrototype &fitPrototype, TVirtualPad *branchPad) : name(name)
    {
        isSeed = false;
        isLeaf = true;
        isBranch = false;
        cout << "BinaryFilter leaf : " << name << " created" << endl;
        histo = new SmartGraph(name.c_str(), fitPrototype, branchPad);
    }

    // < BRANCH: one filter IS given />
    BinaryFilter(const string &name, Bool_t (*filter)(), FitPrototype &passedPrototype, FitPrototype &failedPrototype, TVirtualPad *branchPad) : name(name), filter(filter)
    {
        isSeed = false;
        isLeaf = false;
        isBranch = true;
        cout << "BinaryFilter Branch: " << name << " created" << endl;
        branchPad->Divide(2, 1);
        left = new BinaryFilter(name + "Passed", passedPrototype, branchPad->GetPad(1));
        right = new BinaryFilter(name + "Failed", failedPrototype, branchPad->GetPad(2));
        left->histo->setStyle("good");
        right->histo->setStyle("bad");
    }
    BinaryFilter(const string &name, Bool_t (*filter)(), TVirtualPad *branchPad ) : name(name), filter(filter)
    {
        isSeed = false;
        isLeaf = false;
        isBranch = true;
        cout << "BinaryFilter Branch: " << name << " created" << endl;
        branchPad->Divide(2, 1);
        left = new BinaryFilter(name + "Passed", branchPad->GetPad(1));
        right = new BinaryFilter(name + "Failed", branchPad->GetPad(2));
    }

    // < Set seeds />
    void branch(const string &name, Bool_t (*filter)(), FitPrototype &fitPrototype, FitPrototype &fitFailedPrototype)
    {
        if (!isSeed)
        {
            throw std::runtime_error("This is not a seed");
        }
        isSeed = false;
        isLeaf = false;
        isBranch = true;
        cout << "BinaryFilter Branch: " << name << " created" << endl;
        branchPad->Divide(2, 1);
        left = new BinaryFilter(name + "Passed", fitPrototype, branchPad->GetPad(1));
        right = new BinaryFilter(name + "Failed", fitFailedPrototype, branchPad->GetPad(2));
        left->histo->setStyle("good");
        right->histo->setStyle("bad");
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

    BinaryFilter *getLeft()
    {
        if (left == nullptr)
        {
            throw std::runtime_error("Left child is nullptr");
        }
        return left;
    }

    BinaryFilter *getRight()
    {
        if (right == nullptr)
        {
            throw std::runtime_error("Right child is nullptr");
        }
        return right;
    }

    void printInfo()
    {
        if (isLeaf)
        {
            icout << "BinaryFilter leaf: " << name << endl;
            icout.identUp();
            histo->printInfo();
            icout.identDown();
            return;
        }
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

    void Fill(Double_t x)
    {
        // data reaches the correct leaf
        if (isLeaf)
        {
            histo->Fill(x);
            return;
        }
        if (filter())
        {
            left->Fill(x);
        }
        else
        {
            right->Fill(x);
        }
    }

    void Draw(const char *option = "")
    {
        if (isLeaf)
        {
            histo->Draw(option);
            return;
        }
        left->Draw(option);
        right->Draw(option);
    }
};

#endif // BINARYFILTER_H