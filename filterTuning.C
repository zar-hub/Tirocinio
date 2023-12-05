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
    // auto canvas = new TCanvas("mycanvas", "mycanvas", 0, 0, 1600, 1000);
    // canvas->Divide(1, 1); // hack to create a single pad
    // TVirtualPad *rootPad = canvas->GetPad(1);

    FilterTree tree("", "g");
    switch (4)
    {
    case 0: // barrel probe passed
        tree.root->require("barrel", filters::isBarrel, "g");
        tree.root->left->branch("probe", filters::isProbeGood, "g");
        break;

    case 1: // endcpas probe passed
        tree.root->require("endcaps", filters::isEndcaps, "g");
        tree.root->left->branch("probe", filters::isProbeGood, "g");

    case 2: // barrel tag passed
        tree.root->require("tagBarrel", filters::isTagBarrel, "g");
        tree.root->left->branch("probe", filters::isTagGood, "g");
        break;

    case 3: // endcap tag passed
        tree.root->require("tagEndcap", filters::isTagEndcaps, "g");
        tree.root->left->branch("tag", filters::isTagGood, "g");
        break;

    case 4:
        tree.root->require("", filters::isTagGood, "g");
        tree.root->left->branch("barrel", "endcpas", filters::isBarrel, filters::isEndcaps, "g");
        tree.root->left->left->branch("probe", filters::isProbeGood, "g");
        tree.root->left->right->branch("probe", filters::isProbeGood, "g");
        break;

    default:
        break;
    }

    // tree.root->setPad(rootPad);

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