#define analisi_cxx
#include "src/global.h"
#include "src/conditions.h"
#include "src/fitFunctions.h"
#include "src/SmartGraph.h"
#include "src/BinaryFilter.h"

#include <TCanvas.h>
#include <TH1F.h>
#include <TH2.h>
#include <TSystem.h>
#include <TF1.h>

Bool_t testCondition(){
    return true;
}

void analisi::Loop()
{

    // create canvas and SmartGraphs
    auto canvas = new TCanvas("mycanvas", "mycanvas", 0, 0, 1600, 1000);
    canvas->Divide(1, 1); // hack to create a single pad
    TVirtualPad *rootPad = canvas->GetPad(1);
    // leaf element
    auto grBarrel = new BinaryFilter("Barrel", expBigausPrototype, rootPad);
    
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

        // fill the graphs
        if (!GoodWP80(0))
            continue;

        // do statistic with the second
        if (GoodWP80(1))
        {
            if (eta[1] <= 1.4442)
                grBarrel->histo->Fill(mee);
            //else if ((abs(eta[1]) >= 1.566) && (abs(eta[1]) <= 2.5))
            // grEndcaps->passed->Fill(mee);
        }
        else
        {
            if (eta[1] <= 1.4442)
                grBarrel->histo->Fill(mee);
           // else if ((abs(eta[1]) >= 1.566) && (abs(eta[1]) <= 2.5))
            // grEndcaps->failed->Fill(mee);
        }
    }

    // Draw the graphs
    grBarrel->FitAndDraw();
    // grEndcaps->FitAndDraw();

    printHeader("Fit results");
    
    // test draw
    // grBarrel->left->histo->Draw();

    //grBarrel->printInfo();
    // grEndcaps->printInfo();
}