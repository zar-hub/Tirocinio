//   In a ROOT session, you can do:
   //      root> .L analisi.C
   //      root> analisi t
   //      root> t.GetEntry(12); // Fill t data members with entry number 12
   //      root> t.Show();       // Show values of entry 12
   //      root> t.Show(16);     // Read and show values of entry 16
   //      root> t.Loop();       // Loop on all entries
   //
   

   //     This is the loop skeleton where:
   //    jentry is the global entry number in the chain
   //    ientry is the entry number in the current Tree
   //  Note that the argument to GetEntry must be:
   //    jentry for TChain::GetEntry
   //    ientry for TTree::GetEntry and TBranch::GetEntry
   //
   //       To read only selected branches, Insert statements like:
   // METHOD1:
   //    fChain->SetBranchStatus("*",0);  // disable all branches
   //    fChain->SetBranchStatus("branchname",1);  // activate branchname
   // METHOD2: replace line
   //    fChain->GetEntry(jentry);       //read all branches
   // by  b_branchname->GetEntry(ientry); //read only this branch


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

#define ETRN_MASS 0.511e-3 // mass of the electron [Gev/C^2]

// high level trigger parameters
struct HLTP_Isolation
{
   Float_t combinedISO;
   Float_t trackerRelISO;
   Float_t ECALRelISO;
   Float_t HCALRelISO;
};

struct HLTP_ETRN_ID
{
   Float_t dEta;
   Float_t dPhi;
   Float_t HoE;
   Float_t sig;
};

struct HLTP_ConversionRejection{
   Float_t missHits;
   Float_t dist;
   Float_t dCot;
};

struct HLTP{
   HLTP_ConversionRejection convRej;
   HLTP_Isolation isolation;
   HLTP_ETRN_ID etrnID;
};

struct WPXX{
   HLTP barrel;
   HLTP endcaps;
};

const WPXX WP80 = {
    // BARREL
    {
        // conversion rejection
        // missHits, dist, deltaCot
        {0, .02, .02},

        // isolation
        // COMB, TRK, ECAL, HCAL
        {.07, .09, .07, .1},

        // electron ID
        // dEta, dPhi, HoE, sig
        {.004, .06, .04, .01}},
    // ENDCAPS
    {
        // conversion rejection
        // missHits, dist, deltaCot
        {0, .02, .02},

        // isolation
        // COMB, TRK, ECAL, HCAL
        {.06, .04, .05, .025},

        // electron ID
        // dEta, dPhi, HoE, sig
        {.007, .03, .025, .03}}};

void analisi::Loop()
{   
   if (fChain == 0)
      return;

   // graphs pipeline
   auto canvas = new TCanvas("myCanvas", "Z boson experiment", 800, 600);
   canvas->Divide(2,2);
   auto hMass = new TH1F("hMass", "hMass", 100, 60, 120);
   hMass->SetLineColor(kRed);

   // particle variables
   TLorentzVector etrn[2];
   TLorentzVector parent;

   // main loop
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
         break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // load particles for the specific event
      for (int i : {0, 1}){
         etrn[i].SetPtEtaPhiM(pt[i],eta[i],phi[i],ETRN_MASS);
      }
         
      // compute the sum 
      parent = etrn[0] + etrn[1];
      hMass->Fill(parent.M());
   }
   hMass->Draw();
}
