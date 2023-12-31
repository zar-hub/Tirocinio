#ifndef FILTERS_H
#define FILTERS_H

#include "global.h"

// < Doc />
// This namespace contains all the filters used in the analysis.
// Before using any of the functions, you must call the loadData function
// with the object you want to analyze as argument.

namespace filters
{
   Float_t *mee;
   Float_t (*pt)[2];
   Float_t (*eta)[2];
   Float_t (*phi)[2];
   Float_t (*deta)[2];
   Float_t (*dphi)[2];
   Float_t (*hoe)[2];
   Float_t (*see)[2];
   Float_t (*trackerIso)[2];
   Float_t (*hcalIso)[2];
   Float_t (*ecalJIso)[2];
   Float_t (*ecalGTIso)[2];
   Float_t (*combinedIso)[2];
   Int_t (*charge)[2];
   Int_t (*missHits)[2];
   Float_t (*dist)[2];
   Float_t (*dcot)[2];

   // < namespace was initialized />
   Bool_t filtersInitialized = kFALSE;

   void loadData(analisi *obj)
   {
      filtersInitialized = kTRUE;

      mee = &(obj->mee);
      pt = &(obj->pt);
      eta = &(obj->eta);
      phi = &(obj->phi);
      deta = &(obj->deta);
      dphi = &(obj->dphi);
      hoe = &(obj->hoe);
      see = &(obj->see);
      trackerIso = &(obj->trackerIso);
      hcalIso = &(obj->hcalIso);
      ecalJIso = &(obj->ecalJIso);
      ecalGTIso = &(obj->ecalGTIso);
      combinedIso = &(obj->combinedIso);
      charge = &(obj->charge);
      missHits = &(obj->missHits);
      dist = &(obj->dist);
      dcot = &(obj->dcot);
   }

   Bool_t GoodWP80(int i)
   {
      debugMessageWP80("particle " + std::to_string(i), false);

      if (!debugMessageWP80("missHits failed...", (*missHits)[i] <= 0))
         return kFALSE;
      if (!debugMessageWP80("dist or dcot failed...", abs((*dist)[i]) > 0.02 || abs((*dcot)[i]) > 0.02))
         return kFALSE;
      if (abs((*eta)[i]) <= 1.4442)
      {
         if (!debugMessageWP80("barrel", true))
            return kFALSE;
         if (!debugMessageWP80("combined ISO failed...", abs((*combinedIso)[i] / (*pt)[i]) < 0.07))
            return kFALSE;
         if (!debugMessageWP80("tracker ISO failed...", abs((*trackerIso)[i] / (*pt)[i]) < 0.09))
            return kFALSE;
         if (!debugMessageWP80("ecal ISO failed...", abs((*ecalJIso)[i] / (*pt)[i]) < 0.07))
            return kFALSE;
         if (!debugMessageWP80("hcal ISO failed...", abs((*hcalIso)[i] / (*pt)[i]) < 0.10))
            return kFALSE;
         if (!debugMessageWP80("deta failed...", abs((*deta)[i]) < 0.004))
            return kFALSE;
         if (!debugMessageWP80("dphi failed...", abs((*dphi)[i]) < 0.06))
            return kFALSE;
         if (!debugMessageWP80("hoe failed...", abs((*hoe)[i]) < 0.04))
            return kFALSE;
         if (!debugMessageWP80("see failed...", abs((*see)[i]) < 0.01))
            return kFALSE;
         if (!debugMessageWP80("GOOD!", true))
            return kFALSE;
      }
      else if (abs((*eta)[i]) >= 1.566 && (abs((*eta)[i]) <= 2.5))
      {
         if (!debugMessageWP80("endcaps", true))
            return kFALSE;
         if (!debugMessageWP80("combined ISO failed...", abs((*combinedIso)[i] / (*pt)[i]) < 0.06))
            return kFALSE;
         if (!debugMessageWP80("tracker ISO failed...", abs((*trackerIso)[i] / (*pt)[i]) < 0.04))
            return kFALSE;
         if (!debugMessageWP80("ecalJISO failed...", abs((*ecalJIso)[i] / (*pt)[i]) < 0.05))
            return kFALSE;
         if (!debugMessageWP80("hcal ISO failed...", abs((*hcalIso)[i] / (*pt)[i]) < 0.025))
            return kFALSE;
         if (!debugMessageWP80("deta failed...", abs((*deta)[i]) < 0.007))
            return kFALSE;
         if (!debugMessageWP80("dphi failed...", abs((*dphi)[i]) < 0.03))
            return kFALSE;
         if (!debugMessageWP80("hoe failed...", abs((*hoe)[i]) < 0.025))
            return kFALSE;
         if (!debugMessageWP80("see failed...", abs((*see)[i]) < 0.03))
            return kFALSE;
         if (!debugMessageWP80("GOOD!", true))
            return kFALSE;
      }
      else
      {
         if (!debugMessageWP80("outside range...", false))
            return kFALSE;
      }
      return kTRUE;
   }

   // < Particle 1 cuts />
   Bool_t isBarrel()
   {
      if (abs((*eta)[1]) <= 1.4442)
         return kTRUE;
      return kFALSE;
   }

   Bool_t isEndcaps()
   {
      if (abs((*eta)[1]) >= 1.566 && (abs((*eta)[1]) <= 2.5))
         return kTRUE;
      return kFALSE;
   }
   Bool_t isProbeGood()
   {
      if (!GoodWP80(1))
         return kFALSE;
      return kTRUE;
   }

   Bool_t isTagGood()
   {
      if (!GoodWP80(0))
         return kFALSE;
      return kTRUE;
   }

   Bool_t isPositive()
   {
      if ((*charge)[1] > 0)
         return kTRUE;
      return kFALSE;
   }

   Bool_t isPtBin0()
   {
      if ((*pt)[1] < 25)
         return kTRUE;
      return kFALSE;
   }
   
   Bool_t isPtBin3()
   {
      if ((*pt)[1] > 41)
         return kTRUE;
      return kFALSE;
   }
}

#endif // FILTERS_H