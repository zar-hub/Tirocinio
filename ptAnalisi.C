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
    auto canvas0 = new TCanvas("mycanvas0", "PT_BIN_0", 0, 0, 960, 540);
    auto canvas1 = new TCanvas("mycanvas1", "PT_BIN_1", 960, 0, 960, 540);
    auto canvas2 = new TCanvas("mycanvas2", "PT_BIN_2", 0, 540, 960, 540);
    auto canvas3 = new TCanvas("mycanvas3", "PT_BIN_3", 960, 540, 960, 540);

    // create the first filter tree for ptBin0
    FilterTree pt0Tree("");
    pt0Tree.root->require("", filters::isTagGood, "g");
    auto ptBin0 = pt0Tree.root->left->require("", filters::isPtBin0, "g");
    ptBin0->branch("barrel", "endcaps", filters::isBarrel, filters::isEndcaps, "g");
    ptBin0->left->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");
    ptBin0->right->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");

    ptBin0->left->right->graph->setParameters(57, 122, 0.04, 90, 15, 4, 4);

    // create the first filter tree for ptBin1
    FilterTree pt1Tree("");
    pt1Tree.root->require("", filters::isTagGood, "g");
    auto ptBin1 = pt1Tree.root->left->require("", filters::isPtBin1, "g");
    ptBin1->branch("barrel", "endcaps", filters::isBarrel, filters::isEndcaps, "g");
    ptBin1->left->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");
    ptBin1->right->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");

    ptBin1->left->left->graph->setParameters(120, -0.18, 93, 180, -3, -3);
    ptBin1->right->right->graph->setParameters(20, 11, 0.01, 90, 14, 8, 4);

    // create the first filter tree for ptBin2
    FilterTree pt2Tree("");
    pt2Tree.root->require("", filters::isTagGood, "g");
    auto ptBin2 = pt2Tree.root->left->require("", filters::isPtBin2, "g");
    ptBin2->branch("barrel", "endcaps", filters::isBarrel, filters::isEndcaps, "g");
    ptBin2->left->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");
    ptBin2->right->branch("probe", filters::isProbeGood, rettaBigausPrototype, expBigausPrototype, "gd");

    // create the first filter tree for ptBin3
    FilterTree pt3Tree("");
    pt3Tree.root->require("", filters::isTagGood, "g");
    auto ptBin3 = pt3Tree.root->left->require("", filters::isPtBin3, "g");
    ptBin3->branch("barrel", "endcaps", filters::isBarrel, filters::isEndcaps, "g");
    ptBin3->left->branch("probe", filters::isProbeGood, bigausPrototype, expBigausPrototype, "gd");
    ptBin3->right->branch("probe", filters::isProbeGood, bigausPrototype, expBigausPrototype, "gd");
    
    // set canvases
    ptBin0->setPad(canvas0);
    ptBin1->setPad(canvas1);
    ptBin2->setPad(canvas2);
    ptBin3->setPad(canvas3);

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

        // fill the trees
        pt0Tree.root->Fill(mee);
        pt1Tree.root->Fill(mee);
        pt2Tree.root->Fill(mee);
        pt3Tree.root->Fill(mee);
    }

    printHeader("Fit 0");
    ptBin0->FitAndDraw();
    printHeader("Fit 1");
    ptBin1->FitAndDraw();
    printHeader("Fit 2");
    ptBin2->FitAndDraw();
    printHeader("Fit 3");
    ptBin3->FitAndDraw();

    // printHeader("Fit results");
    // pt0Tree.printTree(ptBin0);
    // pt1Tree.printTree(ptBin1);
    // pt2Tree.printTree(ptBin2);
    // pt3Tree.printTree(ptBin3);

    // export results
    ofstream file;

    for (int i = 0; i < 4; i++)
    {
        file.open("ptBin" + to_string(i) + ".md");
        file << "```" << endl;
        file << "ptBin" + to_string(i) + "results" << endl;
        pt0Tree.printToFile(ptBin0, file);
        file << "```" << endl;
        file.close();
    }

    // save canvas
    canvas0->SaveAs("ptBin0.png");
    canvas1->SaveAs("ptBin1.png");
    canvas2->SaveAs("ptBin2.png");
    canvas3->SaveAs("ptBin3.png");
}