#ifndef FilterNode_H
#define FilterNode_H

#include "global.h"
#include "SmartGraph.h"
#include <TString.h>
#include <fstream>

class FilterNode
{
    /*
        Rules for canvas creation:
        Pads can not be divided more than once, and it is
        not possible to know in advance how many pads will be needed.
        The solution is to create and allocate all the pads in the end.
        So a special function needs to be called to divide the canvases
        after the tree structure is in place.

        IMPORTANT: the right child can exist only if the left child exists.
    */

    // data
    TVirtualPad *pad = nullptr;
    Bool_t (*leftFilter)() = nullptr;
    Bool_t (*rightFilter)() = nullptr;

    // results
    Double_t efficiency;
    Double_t efficiencyError;

public:
    // children
    const string name;
    Bool_t shouldDraw = kFALSE;
    Bool_t verticalSplit = kFALSE;
    SmartGraph *graph = nullptr;
    FilterNode *left = nullptr;  // left child
    FilterNode *right = nullptr; // right child

    FilterNode(const string &name) : name(name) {}

    FilterNode *branch(const string &branchName, Bool_t (*filter)(), const char *option = "")
    {
        TString opt(option);
        left = new FilterNode(name + capitalize(branchName) + "Passed");
        right = new FilterNode(name + capitalize(branchName) + "Failed");
        leftFilter = filter;

        if (opt.Contains("G") || opt.Contains("g"))
        {
            left->graph = new SmartGraph(name + capitalize(branchName) + "Pass");
            right->graph = new SmartGraph(name + capitalize(branchName) + "Fail");
            left->graph->setStyle("good");
            right->graph->setStyle("bad");
        }
        if (opt.Contains("D") || opt.Contains("d"))
        {
            left->shouldDraw = kTRUE;
            right->shouldDraw = kTRUE;
        }
        left->verticalSplit = !verticalSplit;
        right->verticalSplit = !verticalSplit;

        return left;
    }

    FilterNode *branch(const string &branchName, Bool_t (*filter)(), FitPrototype &leftFit, FitPrototype &rightFit, const char *option = "")
    {
        TString opt(option);
        left = new FilterNode(name + capitalize(branchName) + "Passed");
        right = new FilterNode(name + capitalize(branchName) + "Failed");
        leftFilter = filter;

        left->graph = new SmartGraph(name + capitalize(branchName) + "Pass", leftFit);
        right->graph = new SmartGraph(name + capitalize(branchName) + "Fail", rightFit);
        left->graph->setStyle("good");
        right->graph->setStyle("bad");

        if (opt.Contains("D") || opt.Contains("d"))
        {
            left->shouldDraw = kTRUE;
            right->shouldDraw = kTRUE;
        }
        left->verticalSplit = !verticalSplit;
        right->verticalSplit = !verticalSplit;

        return left;
    }

    FilterNode *branch(const string &leftName, const string &rightName, Bool_t (*leftFilter)(), Bool_t (*rightFilter)(), const char *option = "")
    {
        TString opt(option);
        left = new FilterNode(name + capitalize(leftName));
        right = new FilterNode(name + capitalize(rightName));
        this->leftFilter = leftFilter;
        this->rightFilter = rightFilter;

        if (opt.Contains("G") || opt.Contains("g"))
        {
            left->graph = new SmartGraph(name + capitalize(leftName));
            right->graph = new SmartGraph(name + capitalize(rightName));
        }
        if (opt.Contains("D") || opt.Contains("d"))
        {
            left->shouldDraw = kTRUE;
            right->shouldDraw = kTRUE;
        }
        left->verticalSplit = !verticalSplit;
        right->verticalSplit = !verticalSplit;

        return left;
    }

    FilterNode *require(const string &branchName, Bool_t (*filter)(), const char *option = "")
    {
        // this is the same as branchLeft, but the intent is different.
        // branchLeft is used when a rightBranch will be created.
        // require is used when a rightBranch will not be created.
        TString opt(option);
        left = new FilterNode(name + capitalize(branchName));
        leftFilter = filter;

        if (opt.Contains("G") || opt.Contains("g"))
        {
            left->graph = new SmartGraph(name + capitalize(branchName));
        }
        if (opt.Contains("D") || opt.Contains("d"))
        {
            left->shouldDraw = kTRUE;
        }

        left->verticalSplit = !verticalSplit;
        return left;
    }

    FilterNode *branchLeft(const string &branchName, Bool_t (*filter)(), const char *option = "")
    {
        TString opt(option);
        left = new FilterNode(name + capitalize(branchName));
        leftFilter = filter;

        if (opt.Contains("G") || opt.Contains("g"))
        {
            left->graph = new SmartGraph(name + capitalize(branchName));
        }
        if (opt.Contains("D") || opt.Contains("d"))
        {
            left->shouldDraw = kTRUE;
        }

        left->verticalSplit = !verticalSplit;
        return left;
    }

    FilterNode *branchRight(const string &branchName, Bool_t (*filter)(), const char *option = "")
    {
        TString opt(option);
        right = new FilterNode(name + capitalize(branchName));
        rightFilter = filter;

        if (opt.Contains("G") || opt.Contains("g"))
        {
            right->graph = new SmartGraph(name + capitalize(branchName));
        }
        if (opt.Contains("D") || opt.Contains("d"))
        {
            right->shouldDraw = kTRUE;
        }

        right->verticalSplit = !verticalSplit;
        return right;
    }

    void Fill(Double_t x)
    {
        // Bottom condition
        // fill the graph if it exists
        if (graph != nullptr)
        {
            graph->Fill(x);
        }

        // Recursive condition
        // if only the left filter is given but there is a right child
        // fill the right child if the left filter fails,
        // otherwise fill the left child and the right child with their corresponding filters.
        if (left != nullptr && leftFilter != nullptr)
        {
            if (leftFilter())
            {
                left->Fill(x);
            }
            if (right != nullptr)
            {
                if (rightFilter != nullptr)
                {
                    if (rightFilter())
                    {
                        right->Fill(x);
                    }
                }
                else if (!leftFilter())
                {
                    right->Fill(x);
                }
            }
        }
    }

    void setPad(TVirtualPad *_pad)
    {
        // pads need to be splitted before assigning them to the graph
        pad = _pad;
        Int_t nPads = 0;

        // count how many pads are needed
        if (shouldDraw)
        {
            nPads++;
        }
        if (left != nullptr)
        {
            nPads++;
            if (right != nullptr)
            {
                nPads++;
            }
        }
        if (nPads > 1)
        {
            if (verticalSplit)
                pad->Divide(nPads, 1);
            else
                pad->Divide(1, nPads);
        }

        // assign the pads
        if (shouldDraw)
        {
            if (nPads > 1)
            {
                graph->setPad(pad->GetPad(1));
            }
            else
            {
                graph->setPad(pad);
            }
        }
        if (left != nullptr)
        {
            if (shouldDraw)
            {
                left->setPad(pad->GetPad(2));
            }
            else
            {
                left->setPad(pad->GetPad(1));
            }
            if (right != nullptr)
            {
                if (shouldDraw)
                {
                    right->setPad(pad->GetPad(3));
                }
                else
                {
                    right->setPad(pad->GetPad(2));
                }
            }
        }
    }

    void Draw()
    {
        if (shouldDraw)
        {
            graph->Draw();
        }
        if (left != nullptr)
        {
            left->Draw();
            if (right != nullptr)
            {
                right->Draw();
            }
        }
    }

    void Fit()
    {
        if (graph != nullptr)
        {
            graph->Fit();
        }
        if (left != nullptr)
        {
            left->Fit();
            if (right != nullptr)
            {
                right->Fit();
            }
        }
    }

    void FitAndDraw()
    {
        if (graph != nullptr)
        {
            graph->FitAndDraw();
        }
        if (left != nullptr)
        {
            left->FitAndDraw();
            if (right != nullptr)
            {
                right->FitAndDraw();

                if (childrenHaveFits())
                {
                    Double_t areaLeft = left->graph->getSignalIntegral();
                    Double_t areaRight = right->graph->getSignalIntegral();
                    efficiency = areaLeft / (areaLeft + areaRight);
                    efficiencyError = sqrt(efficiency * (1 - efficiency) / (areaLeft + areaRight));
                }
            }
        }
    }

    // state
    Bool_t hasGraph() { return graph != nullptr; }
    Bool_t hasPad() { return pad != nullptr; }
    Bool_t hasLeft() { return left != nullptr; }
    Bool_t hasRight() { return right != nullptr; }
    Bool_t hasLeftFilter() { return leftFilter != nullptr; }
    Bool_t hasRightFilter() { return rightFilter != nullptr; }
    Bool_t hasBothCildren() { return hasLeft() && hasRight(); }
    Bool_t childrenHaveGraphs() { return hasBothCildren() && left->hasGraph() && right->hasGraph(); }
    Bool_t childrenHaveFits() { return childrenHaveGraphs() && left->graph->hasFit() && right->graph->hasFit(); }

    const TVirtualPad *getPad() { return pad; }

    // print
    void Print(const string &prefix = "")
    {
        if (hasGraph())
        {
            std::cout << prefix << " graph: " << graph->getName() << endl;
            graph->printInfo(prefix + "    ");
        }
        // if (shouldDraw)
        // {
        //     std::cout << prefix << " pad: " << pad->GetName() << endl;
        // }
        if (childrenHaveFits())
        {
            std::cout << prefix << " efficiency: " << efficiency << "\u00b1" << efficiencyError << endl;
        }
    }
    void printToFile(ofstream &file, const string &prefix = "")
    {
        if (hasGraph())
        {
            file << prefix << " graph: " << graph->getName() << endl;
            file << prefix << "    " << "Entries: " << graph->getEntries() << endl;
        }
        if (childrenHaveFits())
        {
            file << prefix << " efficiency: " << efficiency << "\u00b1" << efficiencyError << endl;
        }
    }
};

class FilterTree
{
public:
    FilterNode *root;

    FilterTree() : root(nullptr) {}
    FilterTree(const string &name) : root(new FilterNode(name)) {}
    FilterTree(const string &name, const char *option) : root(new FilterNode(name))
    {
        TString opt(option);
        if (opt.Contains("G") || opt.Contains("g"))
        {
            root->graph = new SmartGraph(name);
        }
        if (opt.Contains("D") || opt.Contains("d"))
        {
            root->shouldDraw = kTRUE;
        }
    }

    void printTree(FilterNode *node , const std::string &prefix = "", bool isLeft = false)
    {

        if (node != nullptr)
        {
            std::cout << prefix;
            std::cout << (isLeft ? "├───" : "└───");

            std::cout << "<" << node->name << ">" << endl;

            // blank line
            if (node->left == nullptr && node->right == nullptr)
            {
                node->Print(prefix + (isLeft ? "│   " : "    "));
                std::cout << prefix + (isLeft ? "│   " : "    ") << endl;
            }
            else
            {
                node->Print(prefix + (isLeft ? "│   │" : "    │"));
                std::cout << prefix + (isLeft ? "│   |" : "    │") << endl;
            }

            // enter the next tree level - left and right branch
            if (node->left != nullptr && node->right != nullptr)
            {
                printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
                printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
            }
            if (node->left != nullptr && node->right == nullptr)
            {
                printTree(node->left, prefix + "    ", false);
            }
            if (node->left == nullptr && node->right != nullptr)
            {
                printTree(node->right, prefix + "    ", false);
            }
        }
    }

    void printToFile(FilterNode *node,  ofstream &file, const std::string &prefix = "", bool isLeft = false)
    {

        if (node != nullptr)
        {
            file << prefix;
            file << (isLeft ? "├───" : "└───");

            file << "<" << node->name << ">" << endl;

            // blank line
            if (node->left == nullptr && node->right == nullptr)
            {
                node->printToFile(file, prefix + (isLeft ? "│   " : "    "));
                file << prefix + (isLeft ? "│   " : "    ") << endl;
            }
            else
            {
                node->printToFile(file, prefix + (isLeft ? "│   │" : "    │"));
                file << prefix + (isLeft ? "│   |" : "    │") << endl;
            }

            // enter the next tree level - left and right branch
            if (node->left != nullptr && node->right != nullptr)
            {
                printToFile(node->left, file, prefix + (isLeft ? "│   " : "    "), true);
                printToFile(node->right, file, prefix + (isLeft ? "│   " : "    "), false);
            }
            if (node->left != nullptr && node->right == nullptr)
            {
                printToFile(node->left, file, prefix + "    ", false);
            }
            if (node->left == nullptr && node->right != nullptr)
            {
                printToFile(node->right, file, prefix + "    ", false);
            }
        }
    }
};

#endif // FilterNode_H