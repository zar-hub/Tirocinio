
double fitfunc(double* x, double* p) {

   double fitval;

   double norm = p[0];
   double mean = p[1];
   double sigma = p[2];

   fitval = norm * TMath::Gaus(x[0], mean, sigma);

   return fitval;

}

void fit() {

   const int mpt = 9999;
   float x1[mpt];

   int npt = 0;

   // read data file
   ifstream in;
   in.open("dat.txt");

   while ( kTRUE ) {

      in >> x1[npt];

      if ( ! in.good() ) break;

      cout << "g = " << x1[npt] << endl;

      npt++;

   }

   in.close();

   printf(" found %d points\n", npt);

   TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 500, 500);

   TH1F* h1 = new TH1F("h1", "h1", 100, -10.0, 25.0);

   for ( int i = 0; i < npt; i++ ) {

     h1->Fill(x1[i]);

   }

//   TF1* f1 = new TF1("f1", "gaus", -100.00, 100.0);
   TF1* f1 = new TF1("f1", fitfunc, -100.00, 100.0, 3);

   // sets initial values and parameter names
   f1->SetParameters(7.100, -5.100, 0.100);
   f1->SetParNames("A", "B", "C");

   gStyle->SetOptStat(kFALSE);
   gStyle->SetOptFit(kTRUE);

   c1->cd();
   h1->Fit("f1");
   f1->SetLineColor(kRed);
   f1->Draw("same");

}

