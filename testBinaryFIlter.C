#define analisi_cxx
#include "src/global.h"
#include "src/filters.h"
#include "src/fitFunctions.h"
#include "src/SmartGraph.h"
#include "src/BinaryFilter.h"

#include <TCanvas.h>
#include <TH1F.h>
#include <TH2.h>
#include <TSystem.h>
#include <TF1.h>

void analisi::Loop()
{
    // load data in the filters namespace
    filters::loadData(this);

    // create canvas and SmartGraphs
    auto canvas = new TCanvas("mycanvas", "mycanvas", 0, 0, 1600, 1000);
    canvas->Divide(1, 1); // hack to create a single pad

    TVirtualPad *rootPad = canvas->GetPad(1);
    FilterTree tree("", "g");
    tree.root->require("tagGood", filters::isTagGood, "g");
    tree.root->branch("positive", filters::isPositive, "g");

    tree.root->left->branchLeft("barrel", filters::isBarrel, "g");
    tree.root->left->left->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");
    ;

    tree.root->left->branchRight("endcaps", filters::isEndcaps, "g");
    tree.root->left->right->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");

    tree.root->right->branchLeft("barrel", filters::isBarrel, "g");
    tree.root->right->left->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");

    tree.root->right->branchRight("endcaps", filters::isEndcaps, "g");
    tree.root->right->right->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");

    tree.root->setPad(rootPad);

    if (1)
    {
        setGlobalStyle();
        // health check
        if (fChain == 0)
            return;

        // variable definitions
        Long64_t nentries = fChain->GetEntriesFast();
        Long64_t nbytes = 0, nb = 0;

        for (Long64_t jentry = 0; jentry < nentries; jentry++)
        {
            Long64_t ientry = LoadTree(jentry);
            if (ientry < 0)
                break;
            nb = fChain->GetEntry(jentry);
            nbytes += nb;

            tree.root->Fill(mee);
        }
    }

    tree.root->FitAndDraw();
    tree.printTree(tree.root);
}