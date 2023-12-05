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
    rootPad->Divide(3, 1);
    auto branchPad = rootPad->GetPad(2);
    branchPad->Divide(1, 3);
    
    auto graph0 = new SmartGraph("graph0", rettaBigausPrototype, rootPad->GetPad(1));
    auto graph1 = new SmartGraph("graph1", rettaBigausPrototype, branchPad->GetPad(2));
    auto graph2 = new SmartGraph("graph2", rettaBigausPrototype, branchPad->GetPad(1));
    

    if (1)
    {

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

            if (!filters::GoodWP80(0)){
                continue;
            }
            if (filters::GoodWP80(1)){
                graph0->Fill(mee);
            }else{
                graph1->Fill(mee);
                graph2->Fill(mee);
            }
        }
        
        graph0->FitAndDraw();
        graph1->FitAndDraw();
        graph2->FitAndDraw();
    }

    graph0->printInfo();
    graph1->printInfo();
    graph2->printInfo();

    rootPad->Print();
    rootPad->GetPad(1)->Print();
    rootPad->GetPad(2)->GetPad(2)->Print();
    rootPad->GetPad(3)->Print();
}