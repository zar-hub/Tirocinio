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

void analisi::Loop()
{
    // style
    setGlobalStyle();

    int test = 0;

    // create canvas and SmartGraphs
    auto canvas0 = new TCanvas("mycanvas0", "mycanvas0", 0, 0, 1600, 1000);
    auto canvas1 = new TCanvas("mycanvas1", "mycanvas1", 0, 0, 1600, 1000);
    canvas0->Divide(2, 2);
    canvas1->Divide(2, 2);
    auto grEleBarrel = new BinaryFilter("barrelElectron", rettaBigausPrototype, expBigausPrototype, canvas0->GetPad(1), canvas0->GetPad(2));
    auto grEleEndcaps = new BinaryFilter("endcapsElectron", rettaBigausPrototype, expBigausPrototype, canvas0->GetPad(3), canvas0->GetPad(4));
    auto grPosBarrel = new BinaryFilter("barrelPositron", rettaBigausPrototype, expBigausPrototype, canvas1->GetPad(1), canvas1->GetPad(2));
    auto grPosEndcaps = new BinaryFilter("endcapsPositron", rettaBigausPrototype, expBigausPrototype, canvas1->GetPad(3), canvas1->GetPad(4));

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
            if (charge[1] < 0)
            {
                if (eta[1] <= 1.4442)
                    grEleBarrel->passed->Fill(mee);
                else if ((abs(eta[1]) >= 1.566) && (abs(eta[1]) <= 2.5))
                    grEleEndcaps->passed->Fill(mee);
            }
            else if (charge[1] > 0)
            {
                if (eta[1] <= 1.4442)
                    grPosBarrel->passed->Fill(mee);
                else if ((abs(eta[1]) >= 1.566) && (abs(eta[1]) <= 2.5))
                    grPosEndcaps->passed->Fill(mee);
            }
        }
        else
        {
            if (charge[1] < 0)
            {
                if (eta[1] <= 1.4442)
                    grEleBarrel->failed->Fill(mee);
                else if ((abs(eta[1]) >= 1.566) && (abs(eta[1]) <= 2.5))
                    grEleEndcaps->failed->Fill(mee);
            }
            else if (charge[1] > 0)
            {
                if (eta[1] <= 1.4442)
                    grPosBarrel->failed->Fill(mee);
                else if ((abs(eta[1]) >= 1.566) && (abs(eta[1]) <= 2.5))
                    grPosEndcaps->failed->Fill(mee);
            }
        }
    }

    // Draw the graphs
    grPosBarrel->FitAndDraw();
    grPosEndcaps->FitAndDraw();
    grEleBarrel->FitAndDraw();
    grEleEndcaps->FitAndDraw();

    printHeader("Fit results");
    grPosBarrel->printInfo();
    grPosEndcaps->printInfo();
    grEleBarrel->printInfo();
    grEleEndcaps->printInfo();
}