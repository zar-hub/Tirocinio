#include <TROOT.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>

int FinalGraph()
{
    // make a tgraph with errors and print it to a canvas
    TCanvas *c1 = new TCanvas("c1", "Confronto pt", 200, 10, 700, 500);
    c1->SetGrid();

    TCanvas *c2 = new TCanvas("c2", "Confronto picco fondo standard", 200, 10, 700, 500);
    c2->SetGrid();

    TCanvas *c3 = new TCanvas("c3", "Confronto carica", 200, 10, 700, 500);
    c3->SetGrid();

    TCanvas *c4 = new TCanvas("c4", "Confronto Bin", 200, 10, 700, 500);
    c4->SetGrid();

    // Picco Barrel, fondo barrel, picco endcap, fondo endcaps
    Double_t x0[4] = {1, 2, 3, 4};
    Double_t dati0[4] = {0.813, 0.812, 0.724, 0.71};
    Double_t dati0_sig[4] = {0.004, 0.011, 0.007, 0.01};
    TGraphErrors *gr0 = new TGraphErrors(4, x0, dati0, 0, dati0_sig);
    gr0->SetTitle("set0"); // pass: gaussianaretta, fail: gaussianaexp
    gr0->SetMarkerColor(4);
    gr0->SetMarkerStyle(21);

    // fondo barrel, fondo endcaps
    Double_t x1[2] = {
        1.1,
        3.1,
    };
    Double_t dati1[2] = {0.837, 0.738};
    Double_t dati1_sig[2] = {0.004, 0.008};
    TGraphErrors *gr1 = new TGraphErrors(2, x1, dati1, 0, dati1_sig);
    gr1->SetTitle("set1"); // pass: gaussianaretta, fail: gaussianaretta
    gr1->SetMarkerColor(6);
    gr1->SetMarkerStyle(21);

    // carica picco barrel, carica fondo barrel, carica picco endcaps, carica fondo endcaps
    Double_t x2[4] = {1.2, 2.2, 3.2, 4.2};
    Double_t dati2[4] = {0.822, 0.815, 0.70, 0.678};
    Double_t dati2_sig[4] = {0.005, 0.007, 0.01, 0.01};
    TGraphErrors *gr2 = new TGraphErrors(4, x2, dati2, 0, dati2_sig);
    gr2->SetTitle("set2"); // pass: gaussianaretta, fail: gaussianaexp
    gr2->SetMarkerColor(kGreen);
    gr2->SetMarkerStyle(21);

    // carica picco barrel, carica fondo barrel, carica picco endcaps, carica fondo endcaps
    Double_t x2b[4] = {1.3, 2.3, 3.3, 4.3};
    Double_t dati2b[4] = {0.8, 0.805, 0.8, 0.68};
    Double_t dati2b_sig[4] = {0., 0.006, 0., 0.01};
    TGraphErrors *gr2b = new TGraphErrors(4, x2b, dati2b, 0, dati2b_sig);
    gr2b->SetTitle("set2b"); // pass: gaussianaretta, fail: gaussianaexp
    gr2b->SetMarkerColor(kRed);
    gr2b->SetMarkerStyle(21);

    // pt0 picco barrel,  pt0 fondo barrel, pt0 picco endcaps, pt0 fondo endcaps
    Double_t x3[4] = {1.3, 3.3, 5.3, 7.3};
    Double_t dati3[4] = {0.71, 0.556, 0.64, 0.59};
    Double_t dati3_sig[4] = {0.02, 0.016, 0.02, 0.02};
    TGraphErrors *gr3 = new TGraphErrors(4, x3, dati3, 0, dati3_sig);
    gr3->SetTitle("set3"); // pass: gaussianaretta, fail: gaussianaexp
    gr3->SetMarkerColor(2);
    gr3->SetMarkerStyle(65);

    // pt1 picco barrel, pt1 fondo barrel, pt1 picco endcaps, pt1 fondo endcaps
    Double_t x4[4] = {1.4, 3.4, 5.4, 7.4};
    Double_t dati4[4] = {0.78, 0.75, 0.73, 0.69};
    Double_t dati4_sig[4] = {0.01, 0.01, 0.02, 0.02};
    TGraphErrors *gr4 = new TGraphErrors(4, x4, dati4, 0, dati4_sig);
    gr4->SetTitle("set4"); // pass: gaussianaretta, fail: gaussianaexp
    gr4->SetMarkerColor(9);
    gr4->SetMarkerStyle(65);

    // pt2 picco barrel, pt2 fondo barrel, pt2 picco endcaps, pt2 fondo endcaps
    Double_t x5[4] = {1.5, 3.5, 5.5, 7.5};
    Double_t dati5[4] = {0.857, 0.844, 0.82, 0.78};
    Double_t dati5_sig[4] = {0.006, 0.006, 0.01, 0.01};
    TGraphErrors *gr5 = new TGraphErrors(4, x5, dati5, 0, dati5_sig);
    gr5->SetTitle("set5"); // pass: gaussianaretta, fail: gaussianaexp
    gr5->SetMarkerColor(kRed + 1);
    gr5->SetMarkerStyle(65);

    // pt3 picco barrel,  pt3 fondo barrel,pt3 picco endcaps, pt3 fondo endcaps
    Double_t x6[4] = {1.6, 3.6, 5.6, 7.6};
    Double_t dati6[4] = {0.75, 0.737, 0.68, 0.66};
    Double_t dati6_sig[4] = {0.01, 0.009, 0.02, 0.02};
    TGraphErrors *gr6 = new TGraphErrors(4, x6, dati6, 0, dati6_sig);
    gr6->SetTitle("set6"); // pass: gaussianaretta, fail: gaussianaexp
    gr6->SetMarkerColor(kOrange + 1);
    gr6->SetMarkerStyle(65);

    // 100 bin picco barrel, 100 bin fondo barrel, 100 bin picco endcaps, 100 bin fondo endcaps
    Double_t x7[4] = {1.2, 2.2, 3.2, 4.2};
    Double_t dati7[4] = {0.810, 0.809, 0.726, 0.713};
    Double_t dati7_sig[4] = {0.004, 0.004, 0.008, 0.008};
    TGraphErrors *gr7 = new TGraphErrors(4, x7, dati7, 0, dati7_sig);
    gr7->SetTitle("set7"); // pass: gaussianaretta, fail: gaussianaexp
    gr7->SetMarkerColor(kOrange + 1);
    gr7->SetMarkerStyle(21);

    // 40 bin picco barrel, 40 bin fondo barrel, 40 bin picco endcaps, 40 bin fondo endcaps
    Double_t x8[4] = {1.3, 2.3, 3.3, 4.3};
    Double_t dati8[4] = {0.813, 0.817, 0.728, 0.731};
    Double_t dati8_sig[4] = {0.004, 0.004, 0.008, 0.008};
    TGraphErrors *gr8 = new TGraphErrors(4, x8, dati8, 0, dati8_sig);
    gr8->SetTitle("set8"); // pass: gaussianaretta, fail: gaussianaexp
    gr8->SetMarkerColor(kOrange + 4);
    gr8->SetMarkerStyle(21);

    // 200 bin picco barrel, 200 bin fondo barrel, 200 bin picco endcaps, 200 bin fondo endcaps
    Double_t x9[4] = {1.4, 2.4, 3.4, 4.4};
    Double_t dati9[4] = {0.805, 0.788, 0.726, 0.694};
    Double_t dati9_sig[4] = {0.004, 0.004, 0.008, 0.008};
    TGraphErrors *gr9 = new TGraphErrors(4, x9, dati9, 0, dati9_sig);
    gr9->SetTitle("set9"); // pass: gaussianaretta, fail: gaussianaexp
    gr9->SetMarkerColor(kOrange + 8);
    gr9->SetMarkerStyle(21);





    // STANDARD
    c2->cd();
    gr0->Draw("AP");

    // CARICA
    c3->cd();
    TMultiGraph *mg2 = new TMultiGraph();
    mg2->Add(gr0);
    mg2->Add(gr2);
    mg2->Add(gr2b);
    mg2->Draw("AP");

    // BIN
    c4->cd();
    TMultiGraph *mg3 = new TMultiGraph();
    mg3->Add(gr0);
    mg3->Add(gr7);
    mg3->Add(gr8);
    mg3->Add(gr9);
    mg3->Draw("AP");

    // PT
    c1->cd();
    // create a multigraph and draw it
    TMultiGraph *mg = new TMultiGraph();
    mg->Add(gr3);
    mg->Add(gr4);
    mg->Add(gr5);
    mg->Add(gr6);
    mg->Draw("AP");

    // make a legend
    TLegend *legend = new TLegend(0.13, 0.20, 0.28, 0.40);
    legend->AddEntry(gr3, "pt <= 25", "p");
    legend->AddEntry(gr4, "25 < pt <= 33", "p");
    legend->AddEntry(gr5, "33 < pt <= 41", "p");
    legend->AddEntry(gr6, "41 < pt", "p");
    legend->Draw();

    return 0;
}