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
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <iostream>
#include <TLorentzVector.h>

using namespace std;

const bool DBG_GOODWP80 = true;

Bool_t analisi::GoodWP80(int i)
{
   // Checks if current entry is a good entry according to WP80 standard

   if (DBG_GOODWP80)
      cout << "particle " << i << " : ";

   // common
   if (missHits[i] > 0)
   {
      if (DBG_GOODWP80)
         cout << "missHits failed..." << endl;
      return kFALSE;
   }
   else if (abs(dist[i]) < 0.02 && abs(dcot[i]) < 0.02)
   {
      if (DBG_GOODWP80)
      {
         cout << (abs(dist[i]) < 0.2) << endl;
         cout << (abs(dcot[i]) < 0.2) << endl;
         cout << setprecision(2) << dist[i] << ' ' << dcot[i];
         cout << "dist or dcot failed..." << endl;
      }
      return kFALSE;
   }
   // Barrel
   else if (abs(eta[i]) <= 1.44)
   {
      if (DBG_GOODWP80)
         cout << "barrel, ";
      if (abs(combinedIso[i] / pt[i]) >= 0.07)
      {
         if (DBG_GOODWP80)
            cout << "combined ISO failed..." << endl;
         return kFALSE;
      }
      if (abs(trackerIso[i] / pt[i]) >= 0.09)
      {
         if (DBG_GOODWP80)
            cout << "tracker ISO failed..." << endl;
         return kFALSE;
      }
      if (abs(ecalJIso[i] / pt[i]) >= 0.07)
      {
         if (DBG_GOODWP80)
            cout << "ecal ISO..." << endl;
         return kFALSE;
      }
      if (abs(hcalIso[i] / pt[i]) >= 0.10)
      {
         if (DBG_GOODWP80)
            cout << "hcal ISO failed..." << endl;
         return kFALSE;
      }
      if (abs(deta[i]) >= 0.004)
      {
         if (DBG_GOODWP80)
            cout << "deta failed..." << endl;
         return kFALSE;
      }
      if (abs(dphi[i]) >= 0.06)
      {
         if (DBG_GOODWP80)
            cout << "dphi failed..." << endl;
         return kFALSE;
      }
      if (abs(hoe[i]) >= 0.04)
      {
         if (DBG_GOODWP80)
            cout << "hoe failed..." << endl;
         return kFALSE;
      }
      if (abs(see[i]) >= 0.01)
      {
         if (DBG_GOODWP80)
            cout << "see failed..." << endl;
         return kFALSE;
      }
      if (DBG_GOODWP80)
         cout << "GOOD! " << endl;
   }
   // Endcaps
   else if (abs(eta[i]) >= 1.56)
   {
      if (DBG_GOODWP80)
         cout << "endcaps, ";
      if (abs(combinedIso[i] / pt[i]) >= 0.06)
      {
         if (DBG_GOODWP80)
            cout << "combined ISO failed..." << endl;
         return kFALSE;
      }
      if (abs(trackerIso[i] / pt[i]) >= 0.04)
      {
         if (DBG_GOODWP80)
            cout << "tracker ISO failed..." << endl;
         return kFALSE;
      }
      if (abs(ecalJIso[i] / pt[i]) >= 0.05)
      {
         if (DBG_GOODWP80)
            cout << "ecalJISO failed..." << endl;
         return kFALSE;
      }
      if (abs(hcalIso[i] / pt[i]) >= 0.025)
      {
         if (DBG_GOODWP80)
            cout << "hcal ISO failed..." << endl;
         return kFALSE;
      }
      if (abs(deta[i]) >= 0.007)
      {
         if (DBG_GOODWP80)
            cout << "deta failed..." << endl;
         return kFALSE;
      }
      if (abs(dphi[i]) >= 0.03)
      {
         if (DBG_GOODWP80)
            cout << "dphi failed..." << endl;
         return kFALSE;
      }
      if (abs(hoe[i]) >= 0.025)
      {
         if (DBG_GOODWP80)
            cout << "hoe failed..." << endl;
         return kFALSE;
      }
      if (abs(see[i]) >= 0.03)
      {
         if (DBG_GOODWP80)
            cout << "see failed..." << endl;
         return kFALSE;
      }
      if (DBG_GOODWP80)
         cout << "GOOD! " << endl;
   }
   else
   {
      if (DBG_GOODWP80)
         cout << "outside range..." << endl;
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
   const double ELMASS = 5.11e-3; // GeV
   TLorentzVector particle[2], parent;

   // canvas & graphs
   auto canvas = new TCanvas;
   auto grBarrelPassed = new TH1D("WP80BRLPSS", "WP80 barrel passed", 60, 50, 120);
   auto grBarrelFailed = new TH1D("WP80BRLFLD", "WP80 barrel failed", 60, 50, 120);
   auto grEndcapsPassed = new TH1D("WP80ENDCPSS", "WP80 endcaps passed", 60, 50, 120);
   auto grEndcapsFailed = new TH1D("WP80ENDCFLD", "WP80 endcaps failed", 60, 50, 120);
   canvas->Divide(2, 2);

   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      if (DBG_GOODWP80)
         cout << "jentry: " << jentry << endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;

      // load particles
      for (int i : {0, 1})
      {
         particle[i].SetPtEtaPhiM(pt[i], eta[i], phi[i], ELMASS);
      }
      // parent=particle[0]+particle[1];

      for (int i : {0, 1})
      {
         if (GoodWP80(i))
         {
            if (particle[1].Eta() <= 1.44)
               grBarrelPassed->Fill(mee);
            if (particle[1].Eta() >= 1.566)
               grEndcapsPassed->Fill(mee);
         }
         else 
         {
            if (particle[1].Eta() <= 1.44)
               grBarrelFailed->Fill(mee);
            if (particle[1].Eta() >= 1.566)
               grEndcapsFailed->Fill(mee);
         }
      }
   }

   // draw stuff
   canvas->GetPad(1)->cd();
   grBarrelPassed->Draw();
   canvas->GetPad(2)->cd();
   grBarrelFailed->Draw();
   canvas->GetPad(3)->cd();
   grEndcapsPassed->Draw();
   canvas->GetPad(4)->cd();
   grEndcapsFailed->Draw();
}
