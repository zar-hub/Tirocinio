
void plot() {

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

   gStyle->SetOptStat(kTRUE);

   c1->cd();
   h1->Draw();

}

