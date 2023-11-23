#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "global.h"

Bool_t analisi::GoodWP80(int i)
{
   debugMessageWP80("particle " + std::to_string(i), false);

   if (!debugMessageWP80("missHits failed...", missHits[i] <= 0))
      return kFALSE;
   if (!debugMessageWP80("dist or dcot failed...", abs(dist[i]) > 0.02 || abs(dcot[i]) > 0.02))
      return kFALSE;
   if (abs(eta[i]) <= 1.4442)
   {
      if (!debugMessageWP80("barrel", true))
         return kFALSE;
      if (!debugMessageWP80("combined ISO failed...", abs(combinedIso[i] / pt[i]) < 0.07))
         return kFALSE;
      if (!debugMessageWP80("tracker ISO failed...", abs(trackerIso[i] / pt[i]) < 0.09))
         return kFALSE;
      if (!debugMessageWP80("ecal ISO failed...", abs(ecalJIso[i] / pt[i]) < 0.07))
         return kFALSE;
      if (!debugMessageWP80("hcal ISO failed...", abs(hcalIso[i] / pt[i]) < 0.10))
         return kFALSE;
      if (!debugMessageWP80("deta failed...", abs(deta[i]) < 0.004))
         return kFALSE;
      if (!debugMessageWP80("dphi failed...", abs(dphi[i]) < 0.06))
         return kFALSE;
      if (!debugMessageWP80("hoe failed...", abs(hoe[i]) < 0.04))
         return kFALSE;
      if (!debugMessageWP80("see failed...", abs(see[i]) < 0.01))
         return kFALSE;
      if (!debugMessageWP80("GOOD!", true))
         return kFALSE;
   }
   else if (abs(eta[i]) >= 1.566 && (abs(eta[i]) <= 2.5))
   {
      if (!debugMessageWP80("endcaps", true))
         return kFALSE;
      if (!debugMessageWP80("combined ISO failed...", abs(combinedIso[i] / pt[i]) < 0.06))
         return kFALSE;
      if (!debugMessageWP80("tracker ISO failed...", abs(trackerIso[i] / pt[i]) < 0.04))
         return kFALSE;
      if (!debugMessageWP80("ecalJISO failed...", abs(ecalJIso[i] / pt[i]) < 0.05))
         return kFALSE;
      if (!debugMessageWP80("hcal ISO failed...", abs(hcalIso[i] / pt[i]) < 0.025))
         return kFALSE;
      if (!debugMessageWP80("deta failed...", abs(deta[i]) < 0.007))
         return kFALSE;
      if (!debugMessageWP80("dphi failed...", abs(dphi[i]) < 0.03))
         return kFALSE;
      if (!debugMessageWP80("hoe failed...", abs(hoe[i]) < 0.025))
         return kFALSE;
      if (!debugMessageWP80("see failed...", abs(see[i]) < 0.03))
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

Bool_t analisi::IsElectron(int i)
{
   debugMessage( DBG_CHARGE, "particle " + std::to_string(i) + " charge: " + std::to_string(charge[1]));
   if (-1*charge[1])
      return kTRUE;
   else
      return kFALSE;
}

#endif // CONDITIONS_H