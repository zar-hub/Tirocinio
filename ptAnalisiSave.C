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
#include <fstream>

void analisi::Loop()
{
    // load data into the filters
    filters::loadData(this);

    // create canvas and SmartGraphs
    auto canvas = new TCanvas("mycanvas", "mycanvas", 0, 0, 1600, 1000);
    FilterTree unfiltered("");

    unfiltered.root->require("tagPassed", filters::isTagGood, "g");
    auto ptBin0 = unfiltered.root->left->require("negative", filters::isPositive, "g");
    ptBin0->branch("barrel", "endcaps", filters::isBarrel, filters::isEndcaps, "g");
    ptBin0->left->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");
    ptBin0->right->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");

    // generate pads for ptBin0
    ptBin0->setPad(canvas);


    // style
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

        unfiltered.root->Fill(mee);
    }

    ptBin0->FitAndDraw();
    printHeader("Fit results");
    unfiltered.printTree(ptBin0);

    // export results
    ofstream file;
    file.open("ptBin3.md");
    file << "```" << endl;
    file << "ptBin3 results" << endl;
    unfiltered.printToFile(ptBin0, file);
    file << "```" << endl;
    file.close();
    
    // save canvas
    canvas->SaveAs("ptBin3.png");
}