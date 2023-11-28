#define analisi_cxx
#include "src/global.h"
#include "src/conditions.h"
#include "src/fitFunctions.h"
#include "src/SmartGraph.h"

#include <TCanvas.h>
#include <TH1F.h>
#include <TH2.h>
#include <TSystem.h>
#include <TF1.h>

void analisi::Loop()
{

    // create canvas and SmartGraphs
    auto canvas = new TCanvas("mycanvas", "mycanvas", 0, 0, 1600, 1000);
    auto grBarrelPassed = new SmartGraph("barrelPassed", true);

    // set fit functions
    grBarrelPassed->setFit(rettaBigaus, 6);
    grBarrelPassed->setFitPar(rettaBigausNames, rettaBigausParam);

    // style
    setGlobalStyle();

    // Set line colors
    grBarrelPassed->setStyle();

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
                grBarrelPassed->Fill(mee);
        }
    }

    // draw
    grBarrelPassed->Draw();
}