// collection of definitions, libraries and parameters
// common to all files
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include <TROOT.h>
#include <iostream>
#include "../analisi.h"
#include "fitFunctions.h"

using namespace std;

// < SETTINGS />
// positioning and size of graphical elements
#define grBins 60
#define grMinX 60
#define grMaxX 120
#define fitMinX 60
#define fitMaxX 120

// < DEBUGGING />
const bool DBG_GOODWP80 = false;
const bool DBG_CHARGE = false;

// < PRINTING />
void printHeader(const string &message)
{
   cout << endl
        << "==============================" << endl
        << message << endl
        << "==============================" << endl
        << endl;
}

class identCout
{
   int ident_level = 0;
   string ident_string = "├──";
   string ident(int n)
   {
      string s = "";
      for (int i = 0; i < n; i++)
      {
            s += ident_string;
      }
      return s;
   }

public:
   ostream &operator<<(const string &message)
   {
      cout << ident(ident_level) << message;
      return cout;
   }
   void identUp()
   {
      ident_level++;
   }
   void identDown()
   {
      ident_level--;
   }
};

// < FUNCTIONS />
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

void setGlobalStyle()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptTitle(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(101);
   gStyle->SetStatX(.89);
   gStyle->SetStatY(.89);
   gStyle->SetStatW(.17);
   gStyle->SetStatH(.17);
   gStyle->SetStatBorderSize(0);
}

// < FIT PROTOTYPES />
vector<string> rettaBigausNames = {"line y-intercept", "line angular coeff.", "normal mean", "normal amplitude", "sigma left", "sigma right"};
vector<Double_t> rettaBigausDefParam = {0, -0.02, 90, 1000, 1, 1};
FitPrototype rettaBigausPrototype(rettaBigausNames, rettaBigausDefParam, rettaBigaus, "normal amplitude", "line angular coeff.", fitMinX, fitMaxX);

vector<string> expBigausNames = {"exp x0", "exp amplitude", "exp k", "normal mean", "normal amplitude", "sigma left", "sigma right"};
vector<Double_t> expBigausDefParam = {36, 305, 0.02, 90, 590, 5, 4};
FitPrototype expBigausPrototype(expBigausNames, expBigausDefParam, expBigausV0, "normal amplitude", "exp amplitude", fitMinX, fitMaxX);

identCout icout;

#endif // __GLOBAL_H__