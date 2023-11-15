/*
DOMANDE:
se i due elettroni passano i test ma sono uno di barrel e uno di endcaps dove devo mettere
il bosone? facciamo solo per la seconda particella
se i due elettroni sono uno di barrel e uno di endcaps dove devo mettere i failed?
*/

#define analisi_cxx
#include "analisi.h"
#include <TH1F.h>
#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <iostream>
#include <TLorentzVector.h>
#include "fitFunctions.h"
#include <TROOT.h>

using namespace std;

// DBG
const bool DBG_GOODWP80 = false;

// Tweaking
#define grMinX 60
#define grMaxX 120
#define fitMinX 60
#define fitMaxX 120

/**
 * @brief Prints a debug message if the condition is false,
 *        it is a wrapper, does not change the flow of the program.
 *
 * @param message The message to print.
 * @param condition The condition to check.
 * @return The value of the condition.
 */
bool debugMessage(const std::string &message, bool condition)
{
   if (DBG_GOODWP80 && !condition)
   {
      cout << message << endl;
   }
   return condition;
}

Bool_t analisi::GoodWP80(int i)
{
   debugMessage("particle " + std::to_string(i), false);

   if (!debugMessage("missHits failed...", missHits[i] <= 0))
      return kFALSE;
   if (!debugMessage("dist or dcot failed...", abs(dist[i]) > 0.02 || abs(dcot[i]) > 0.02))
      return kFALSE;
   if (abs(eta[i]) <= 1.4442)
   {
      if (!debugMessage("barrel", true))
         return kFALSE;
      if (!debugMessage("combined ISO failed...", abs(combinedIso[i] / pt[i]) < 0.07))
         return kFALSE;
      if (!debugMessage("tracker ISO failed...", abs(trackerIso[i] / pt[i]) < 0.09))
         return kFALSE;
      if (!debugMessage("ecal ISO failed...", abs(ecalJIso[i] / pt[i]) < 0.07))
         return kFALSE;
      if (!debugMessage("hcal ISO failed...", abs(hcalIso[i] / pt[i]) < 0.10))
         return kFALSE;
      if (!debugMessage("deta failed...", abs(deta[i]) < 0.004))
         return kFALSE;
      if (!debugMessage("dphi failed...", abs(dphi[i]) < 0.06))
         return kFALSE;
      if (!debugMessage("hoe failed...", abs(hoe[i]) < 0.04))
         return kFALSE;
      if (!debugMessage("see failed...", abs(see[i]) < 0.01))
         return kFALSE;
      if (!debugMessage("GOOD!", true))
         return kFALSE;
   }
   else if (abs(eta[i]) >= 1.566 && (abs(eta[i]) <= 2.5))
   {
      if (!debugMessage("endcaps", true))
         return kFALSE;
      if (!debugMessage("combined ISO failed...", abs(combinedIso[i] / pt[i]) < 0.06))
         return kFALSE;
      if (!debugMessage("tracker ISO failed...", abs(trackerIso[i] / pt[i]) < 0.04))
         return kFALSE;
      if (!debugMessage("ecalJISO failed...", abs(ecalJIso[i] / pt[i]) < 0.05))
         return kFALSE;
      if (!debugMessage("hcal ISO failed...", abs(hcalIso[i] / pt[i]) < 0.025))
         return kFALSE;
      if (!debugMessage("deta failed...", abs(deta[i]) < 0.007))
         return kFALSE;
      if (!debugMessage("dphi failed...", abs(dphi[i]) < 0.03))
         return kFALSE;
      if (!debugMessage("hoe failed...", abs(hoe[i]) < 0.025))
         return kFALSE;
      if (!debugMessage("see failed...", abs(see[i]) < 0.03))
         return kFALSE;
      if (!debugMessage("GOOD!", true))
         return kFALSE;
   }
   else
   {
      if (!debugMessage("outside range...", false))
         return kFALSE;
   }
   return kTRUE;
}

void analisi::Loop()
{
   // health check
   if (fChain == 0)
      return;

   // variable definitions
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   // some info
   Long64_t discarded = 0;

   // fit functions
   auto fitBarrelPassed = new TF1("fitBarrelPassed", rettaBigaus, fitMinX, fitMaxX, 6);
   fitBarrelPassed->SetParNames("x0", "m", "mu", "A", "L", "R");
   fitBarrelPassed->SetParameters(0, -0.02, 90, 1000, 1, 1);

   auto fitEndcapsPassed = new TF1("fitEndcapsPassed", rettaBigaus, fitMinX, fitMaxX, 6);
   fitEndcapsPassed->SetParNames("x0", "m", "mu", "A", "L", "R");
   fitEndcapsPassed->SetParameters(0, -0.02, 90, 1000, 1, 1);

   auto fitBarrelFailed = new TF1("fitBarrelFailed", expBigausV0, fitMinX, fitMaxX, 7);
   fitBarrelFailed->SetParNames("x0 exp", "A exp", "k exp", "mu bigaus", "amplitude", "sigma left", "sigma right");
   fitBarrelFailed->SetParameters(36, 305, 0.02, 90, 590, 5, 4);

   auto fitEndcapsFailed = new TF1("fitEndcapsFailed", expBigausV0, fitMinX, fitMaxX, 7);
   fitEndcapsFailed->SetParNames("x0 exp", "A exp", "k exp", "mu bigaus", "amplitude", "sigma left", "sigma right");
   fitEndcapsFailed->SetParameters(36, 305, 0.02, 90, 590, 5, 4);

   // canvas & graphs
   auto canvas = new TCanvas("mycanvas", "mycanvas", 0, 0, 1600, 1000);
   auto grBarrelPassed = new TH1D("WP80BRLPSS", "WP80 barrel passed", 60, grMinX, grMaxX);
   auto grBarrelFailed = new TH1D("WP80BRLFLD", "WP80 barrel failed", 60, grMinX, grMaxX);
   auto grEndcapsPassed = new TH1D("WP80ENDCPSS", "WP80 endcaps passed", 60, grMinX, grMaxX);
   auto grEndcapsFailed = new TH1D("WP80ENDCFLD", "WP80 endcaps failed", 60, grMinX, grMaxX);
   canvas->Divide(2, 2);

   // style
   gStyle->SetOptStat(000000011);
   gStyle->SetOptFit(0001);

   grBarrelPassed->GetXaxis()->SetTitle("mee");
   grBarrelPassed->GetYaxis()->SetTitle("events");

   grBarrelFailed->GetXaxis()->SetTitle("mee");
   grBarrelFailed->GetYaxis()->SetTitle("events");

   grEndcapsPassed->GetXaxis()->SetTitle("mee");
   grEndcapsPassed->GetYaxis()->SetTitle("events");

   grEndcapsFailed->GetXaxis()->SetTitle("mee");
   grEndcapsFailed->GetYaxis()->SetTitle("events");

   // Set line colors
   grBarrelPassed->SetLineColor(kGreen);
   grBarrelFailed->SetLineColor(kRed);
   grEndcapsPassed->SetLineColor(kGreen);
   grEndcapsFailed->SetLineColor(kRed);
   grBarrelPassed->SetFillColor(407);
   grEndcapsPassed->SetFillColor(407);
   grBarrelFailed->SetFillColor(40);
   grEndcapsFailed->SetFillColor(40);

   fitBarrelPassed->SetLineColor(kGreen);
   fitEndcapsPassed->SetLineColor(kGreen);
   fitBarrelFailed->SetLineColor(kRed);
   fitEndcapsFailed->SetLineColor(kRed);

   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      if (DBG_GOODWP80)
         cout << "jentry: " << jentry << endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;

      // check the first, continue if failed
      if (!GoodWP80(0))
      {
         discarded++;
         continue;
      }
      // do statistic with the second
      if (GoodWP80(1))
      {
         if (eta[1] <= 1.4442)
            grBarrelPassed->Fill(mee);
         else if ((abs(eta[1]) >= 1.566) && (abs(eta[1]) <= 2.5))
            grEndcapsPassed->Fill(mee);
         else
            discarded++;
      }
      else
      {
         if (eta[1] <= 1.4442)
            grBarrelFailed->Fill(mee);
         else if ((abs(eta[1]) >= 1.566) && (abs(eta[1]) <= 2.5))
            grEndcapsFailed->Fill(mee);
         else
            discarded++;
      }
   }

   // draw stuff
   canvas->cd(1);
   grBarrelPassed->Draw();
   grBarrelPassed->Fit(fitBarrelPassed);

   canvas->cd(2);
   grBarrelFailed->Draw();
   grBarrelFailed->Fit(fitBarrelFailed);

   canvas->cd(3);
   grEndcapsPassed->Draw();
   grEndcapsPassed->Fit(fitEndcapsPassed);

   canvas->cd(4);
   grEndcapsFailed->Draw();
   grEndcapsFailed->Fit(fitEndcapsFailed);

   // Some info on the data
   cout << "------------- LOOP INFO -------------" << endl;
   cout << "total entries: " << nentries << endl;
   cout << "Barrel passed: " << grBarrelPassed->GetEntries() << " vs " << fitBarrelPassed->Integral(fitMinX, fitMaxX) << endl;
   cout << "Barrel failed: " << grBarrelFailed->GetEntries() << " vs " << fitBarrelFailed->Integral(fitMinX, fitMaxX) << endl;
   cout << "Endcaps passed: " << grEndcapsPassed->GetEntries() << " vs " << fitEndcapsPassed->Integral(fitMinX, fitMaxX) << endl;
   cout << "Endcaps failed: " << grEndcapsFailed->GetEntries() << " vs " << fitEndcapsFailed->Integral(fitMinX, fitMaxX) << endl;
   cout << "Discarded: " << discarded << endl;

   // Efficiency considerations
   // Set the amplitude of the peak to 0 to get only the noise
   fitBarrelPassed->SetParameter("A", 0);
   fitBarrelFailed->SetParameter("amplitude", 0);
   fitEndcapsPassed->SetParameter("A", 0);
   fitEndcapsFailed->SetParameter("amplitude", 0);
   Double_t noiseBarrelPassed = fitBarrelPassed->Integral(fitMinX, fitMaxX);
   Double_t noiseBarrelFailed = fitBarrelFailed->Integral(fitMinX, fitMaxX);
   Double_t noiseEndcapsPassed = fitEndcapsPassed->Integral(fitMinX, fitMaxX);
   Double_t noiseEndcapsFailed = fitEndcapsFailed->Integral(fitMinX, fitMaxX);
   Double_t areaBarrelPassed = grBarrelPassed->Integral() - noiseBarrelPassed;
   Double_t areaBarrelFailed = grBarrelFailed->Integral() - noiseBarrelFailed;
   Double_t areaEndcapsPassed = grEndcapsPassed->Integral() - noiseEndcapsPassed;
   Double_t areaEndcapsFailed = grEndcapsFailed->Integral() - noiseEndcapsFailed;
   Double_t areaBarrel = areaBarrelPassed + areaBarrelFailed;
   Double_t areaEndcaps = areaEndcapsPassed + areaEndcapsFailed;

   cout << "------------- EFFICIENCY -------------" << endl;
   cout << "Area for BarrelPassed: " << areaBarrelPassed << " removed noise: " << noiseBarrelPassed << endl;
   cout << "Area for BarrelFailed: " << areaBarrelFailed << " removed noise: " << noiseBarrelFailed << endl;
   cout << "Area for EndcapsPassed: " << areaEndcapsPassed << " removed noise: " << noiseEndcapsPassed << endl;
   cout << "Area for EndcapsFailed: " << areaEndcapsFailed << " removed noise: " << noiseEndcapsFailed << endl;

   // Calculate efficiencies
   Double_t efficiencyBarrel = areaBarrelPassed / (areaBarrelPassed + areaBarrelFailed);
   Double_t efficiencyEndcaps = areaEndcapsPassed / (areaEndcapsPassed + areaEndcapsFailed);
   Double_t sigmaAreaBarrelPassed = sqrt(areaBarrel * efficiencyBarrel * (1 - efficiencyBarrel));
   Double_t sigmaAreaEndcapsPassed = sqrt(areaEndcaps * efficiencyEndcaps * (1 - efficiencyEndcaps));
   Double_t sigmaEfficiencyBarrel = sigmaAreaBarrelPassed / areaBarrel;
   Double_t sigmaEfficiencyEndcaps = sigmaAreaEndcapsPassed / areaEndcaps;

   // Print efficiencies
   cout << "Efficiency for Barrel: " << efficiencyBarrel << "\u00b1" << sigmaEfficiencyBarrel << endl;
   cout << "Efficiency for Endcaps: " << efficiencyEndcaps << "\u00b1" << sigmaEfficiencyEndcaps <<endl;

   // save canvas
   canvas->SaveAs("canvas.png");
}