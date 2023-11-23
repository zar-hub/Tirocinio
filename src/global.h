// collection of definitions, libraries and parameters
// common to all files
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include <TROOT.h>
#include <iostream>
#include "../analisi.h"

using namespace std;

// < SETTINGS />
// positioning and size of graphical elements
#define grBins 60
#define grMinX 60
#define grMaxX 120
#define fitMinX 60
#define fitMaxX 120

// < FIT PARAMETERS />
vector<string> rettaBigausNames = {"line y-intercept", "line angular coeff.", "normal mean", "normal amplitude", "sigma left", "sigma right"};
vector<Double_t> rettaBigausParam = {0, -0.02, 90, 1000, 1, 1};

// < DEBUGGING />
const bool DBG_GOODWP80 = false;
const bool DBG_CHARGE = true;

/**
 * @brief Prints a debug message if the condition is false,
 *        it is a wrapper, does not change the flow of the program.
 *
 * @param message The message to print.
 * @param condition The condition to check.
 * @return The value of the condition.
 */
bool debugMessageWP80(const std::string &message, bool condition = false)
{
   if (DBG_GOODWP80 && !condition)
   {
      cout << message << endl;
   }

   return condition;
}

bool debugMessage(const bool DBG_FLAG, const std::string &message, bool condition = false)
{
   if (DBG_FLAG && !condition)
   {
      cout << message << endl;
   }

   return condition;
}

// useful functions
string capitalize(string s)
{
   s[0] = toupper(s[0]);
   return s;
}

void setGlobalStyle(){
   gROOT->SetStyle("Plain");
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(101);
   gStyle->SetStatX(.89);
   gStyle->SetStatY(.89);
   gStyle->SetStatW(.17);
   gStyle->SetStatH(.17);
   gStyle->SetStatBorderSize(0);
}

#endif // __GLOBAL_H__