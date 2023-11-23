#ifndef SMARTGRAPH_H
#define SMARTGRAPH_H

#include "global.h"
#include <TROOT.h>
#include <TH1D.h>
#include <TF1.h>

class SmartGraph
{
    string name;
    TH1D *histo;
    TF1 *fit;
    Bool_t goodStyle = false;

public:
    SmartGraph(const string &name, Bool_t goodStyle) : name(name), goodStyle(goodStyle)
    {
        histo = new TH1D(name.c_str(), name.c_str(), grBins, grMinX, grMaxX);
        histo->GetXaxis()->SetTitle("Variable: mee");
        histo->GetYaxis()->SetTitle("Counts");

        fit = nullptr;
    }

    void setFit(Double_t (*fitFunction)(Double_t *, Double_t *), int nPar)
    {
        fit = new TF1(("fit" + capitalize(name)).c_str(), fitFunction, fitMinX, fitMaxX, nPar);
    }

    void setFitPar(vector<string> names, vector<Double_t> values){
        if(fit == nullptr )
            return;
        for( int i = 0; i < values.size() ; i++){
           fit->SetParName(i, names[i].c_str());
           fit->SetParameter(i, values[i]);
        }
    }

    void setColor(int primary, int secondary)
    {
        histo->SetLineColor(primary);
        histo->SetFillColor(secondary);
    }

    void setFitStyle(int lineColor, int lineStyle = 9)
    {
        if (fit == nullptr)
            return;
        fit->SetLineColor(lineColor);
        fit->SetLineStyle(lineStyle);
    }

    void setStyle()
    {
        if (goodStyle)
        {
            this->setColor(kGreen + 1, kTeal);
            this->setFitStyle(kGreen + 1);
        }
        else
        {
            this->setColor(kRed, 40);
            this->setFitStyle(kRed - 2);
        }
    }

    void Draw()
    {
        histo->Draw();
        if (fit != nullptr)
            histo->Fit(fit, "R");
    }

    void Fill(Double_t x)
    {
        histo->Fill(x);
    }
};

#endif // SMARTGRAPH_H