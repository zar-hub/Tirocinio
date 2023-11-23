#ifndef SMARTGRAPH_H
#define SMARTGRAPH_H

#include "global.h"
#include <TROOT.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPad.h>

class SmartGraph
{
    // tools and data
    string name;
    TH1D *histo;
    TF1 *fit;
    TF1 *noiseFit;
    FitPrototype *prototype;
    TVirtualPad *pad;

    // results
    Double_t rawSignal;
    Double_t noise;
    Double_t signal;

public:
    SmartGraph(const string &name, FitPrototype &prototype, TVirtualPad* pad) : name(name), prototype(&prototype), pad(pad)
    {
        histo = new TH1D(name.c_str(), name.c_str(), grBins, grMinX, grMaxX);
        histo->GetXaxis()->SetTitle("Variable: mee");
        histo->GetYaxis()->SetTitle("Counts");
        histo->SetTitle(name.c_str());

        fit = prototype.getWithName(("fit" + name).c_str());
    }

    void setParameters(Double_t p0, Double_t p1, Double_t p2 = 0, Double_t p3 = 0, Double_t p4 = 0,
                       Double_t p5 = 0, Double_t p6 = 0, Double_t p7 = 0, Double_t p8 = 0,
                       Double_t p9 = 0, Double_t p10 = 0)
    {
        fit->SetParameters(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }

    void setParameters(Double_t *parameters)
    {
        fit->SetParameters(parameters);
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

    void setStyle(Bool_t goodStyle)
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

    void Draw(const char *option = "")
    {
        pad->cd();
        histo->Draw(option);
    }

    void drawNoise(const char *option = "")
    {
        if (fit == nullptr)
            return;
        
        pad->cd();
        noiseFit = prototype->getNoise(fit);
        noiseFit->Draw(option);
    }

    void FitAndDraw(){
        // the order is important!
        // one cannot fit and then draw the noise
        // because the noise is calculated from the fit
        this->Draw();
        this->Fit();
        this->drawNoise("same");
    }

    void Fit()
    {
        if (fit != nullptr)
            histo->Fit(fit, "R");
        rawSignal = histo->Integral();
        noise = prototype->getNoise(fit)->Integral(fitMinX, fitMaxX) / histo->GetBinWidth(1);
        signal = rawSignal - noise;
    }

    void Fill(Double_t x)
    {
        histo->Fill(x);
    }

    Double_t getSignalIntegral()
    {
        return signal;
    }

    void printInfo()
    {
        if(fit == nullptr){
            cout << "No fit found" << endl;
            return;
        }
        icout << "SmartGraph: " << name << endl;
        icout.identUp();
        icout << "Entries: " << histo->GetEntries() << endl;
        icout << "Graph integral: " << rawSignal << endl;
        icout << "Fit integral: " << fit->Integral(fitMinX, fitMaxX) / histo->GetBinWidth(1) << endl;
        icout << "Noise integral: " << noise << endl;
        icout << "Extimated signal: " << signal << " %err: "<< noise/signal <<endl;
        icout.identDown();
    }

};

#endif // SMARTGRAPH_H