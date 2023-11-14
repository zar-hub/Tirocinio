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

using namespace std;

const bool DBG_GOODWP80 = true;

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
   if (!debugMessage("particle " + std::to_string(i), true))
      return kFALSE;
   if (!debugMessage("missHits failed...", missHits[i] <= 0))
      return kFALSE;
   if (!debugMessage("dist or dcot failed...", abs(dist[i]) >= 0.02 || abs(dcot[i]) >= 0.02))
      return kFALSE;
   if (abs(eta[i]) <= 1.44)
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
   else if (abs(eta[i]) >= 1.56)
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
   auto fitBarrelPassed = new TF1("fitBarrelPassed", peakBigausV1, 60, 120, 7);
   fitBarrelPassed->SetParNames("mu peak", "A peak", "sigma peak", "mu bigaus", "amplitude", "sigma left", "sigma right");
   fitBarrelPassed->SetParameters(90, 110, 2, 90, 590, 5, 4);
   fitBarrelPassed->SetParLimits(1, 0, 600); // Set limits for "A peak"
   fitBarrelPassed->SetParLimits(2, 0, 4);   // Set limits for "sigma peak"
   //fitBarrelPassed->FixParameter(0, 90);
   //fitBarrelPassed->FixParameter(1, 0);
   //fitBarrelPassed->FixParameter(2, 1);
   //fitBarrelPassed->FixParameter(3, 91);
   //fitBarrelPassed->FixParameter(4, 918);
   //fitBarrelPassed->FixParameter(5, 4.5);
   //fitBarrelPassed->FixParameter(6, 2.7);

   auto fitEndcapsPassed = new TF1("fitEndcapsPassed", peakBigausV1, 60, 120, 7);
   fitEndcapsPassed->SetParNames("mu peak", "A peak", "sigma peak", "mu bigaus", "amplitude", "sigma left", "sigma right");
   fitEndcapsPassed->SetParameters(90, 326, 3, 91, 918, 4.5, 2.7);
   fitEndcapsPassed->SetParLimits(1, 0, 600); // Set limits for "A peak"
   fitEndcapsPassed->SetParLimits(2, 0, 6);   // Set limits for "sigma peak"`
  
   auto fitBarrelFailed = new TF1("fitBarrelFailed", expBigausV0, 60, 120, 7);
   fitBarrelFailed->SetParNames("x0 exp", "A exp", "k exp", "mu bigaus", "amplitude", "sigma left", "sigma right");
   fitBarrelFailed->SetParameters(36, 305, 0.02, 90, 590, 5, 4);

   auto fitEndcapsFailed = new TF1("fitEndcapsFailed", expBigausV0, 60, 120, 7);
   fitEndcapsFailed->SetParNames("x0 exp", "A exp", "k exp", "mu bigaus", "amplitude", "sigma left", "sigma right");
   fitEndcapsFailed->SetParameters(36, 305, 0.02, 90, 590, 5, 4);

   // canvas & graphs
   auto canvas = new TCanvas("mycanvas", "mycanvas", 0,0, 1920,1080);
   auto grBarrelPassed = new TH1D("WP80BRLPSS", "WP80 barrel passed", 60, 50, 120);
   auto grBarrelFailed = new TH1D("WP80BRLFLD", "WP80 barrel failed", 60, 50, 120);
   auto grEndcapsPassed = new TH1D("WP80ENDCPSS", "WP80 endcaps passed", 60, 50, 120);
   auto grEndcapsFailed = new TH1D("WP80ENDCFLD", "WP80 endcaps failed", 60, 50, 120);
   canvas->Divide(2, 2);

   // style
   gStyle->SetOptStat(000000011);
   gStyle->SetOptFit(0001);

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
         if (eta[1] <= 1.44)
            grBarrelPassed->Fill(mee);
         if (eta[1] >= 1.566)
            grEndcapsPassed->Fill(mee);
      }
      else
      {
         if (eta[1] <= 1.44)
            grBarrelFailed->Fill(mee);
         if (eta[1] >= 1.566)
            grEndcapsFailed->Fill(mee);
      }
   }
   cout << discarded << " discarded out of " << nentries << endl;
   // draw stuff
   canvas->cd(1);
   grBarrelPassed->Draw();
   grBarrelPassed->Fit(fitBarrelPassed, "WQRN");
   grBarrelPassed->Fit(fitBarrelPassed, "RM+");

   canvas->cd(2);
   grBarrelFailed->Draw();
   grBarrelFailed->Fit(fitBarrelFailed);

   canvas->cd(3);
   grEndcapsPassed->Draw();
   grEndcapsPassed->Fit(fitEndcapsPassed);

   canvas->cd(4);
   grEndcapsFailed->Draw();
   grEndcapsFailed->Fit(fitEndcapsFailed);
}
