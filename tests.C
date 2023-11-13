#include <iostream>
#include <TROOT.h>
using namespace std;

struct IsolationParameters
{
    Float_t t;
    // Float_t &combinedISO;
    // Float_t &trackerRelISO;
    // Float_t &ECALRelISO;
    // Float_t &HCALRelISO;
    
    IsolationParameters(Float_t& t):{
        &(this->t) = &t;
    }
    
};


int tests(){
Float_t a = 1.7;
IsolationParameters iso(a);
cout<< iso.t << endl;
a = 4.2;
cout<< iso.t << endl;
return 0;
}