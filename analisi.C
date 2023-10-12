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

double getE(double * v){
   double res = 0;
   for (int i = 0; i < 4; i++)
      res += v[i] * v[i];
   return sqrt(res);
}

void analisi::Loop()
{
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
   if (fChain == 0)
      return;

   double em = 0.511e-3; // mass of the electron [Gev/C^2]

   auto he_1 = new TH1F("h1", "h1", 100, 0, 120);
   auto he_2 = new TH1F("h2", "h2", 100, 0, 120);
   
   he_1->SetLineColor(kRed);
   he_2->SetLineColor(kGreen);
   TH1F* he[3] = {he_1, he_2};

   double theta, E;
   double p[4], both[4];

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

      // Compute the energy
      for(int i = 0; i < 4; i++){
         both[i] = 0;
         p[i] = 0;
      }

      // Compute the momentum
      for (int i = 0; i < 2; i++)
      {
         theta = 2 * atan(exp(-eta[i]));
         p[0] = em;
         p[1] = pt[i] * sin(phi[i]);
         p[2] = pt[i] * cos(phi[i]);
         p[3] = pt[i] * tan(theta);

         for (int i = 0; i < 4; i++)
            both[i] = both[i] + p[i];

         // Get the energy
         E = getE(p);
         he[i]->Fill(E);
      }
   }
   he_1->Draw();
   he_2->Draw("SAME");
   cout << "hi";
}
