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
    TF1 *fit = nullptr;
    TF1 *noiseFit = nullptr;
    FitPrototype *prototype = nullptr;
    TVirtualPad *pad = nullptr;

    // results
    Double_t rawSignal;
    Double_t noise;
    Double_t signal;

    // helpers
    void initHistogram()
    {
        histo = new TH1D(name.c_str(), name.c_str(), grBins, grMinX, grMaxX);
        histo->GetXaxis()->SetTitle("Variable: mee");
        histo->GetYaxis()->SetTitle("Counts");
        histo->SetTitle(name.c_str());
    }

public:
    SmartGraph(const string &name) : name(name)
    {
        initHistogram();
    }
    SmartGraph(const string &name, FitPrototype &prototype) : name(name), prototype(&prototype)
    {
        initHistogram();
        fit = prototype.getWithName(("fit" + name).c_str());
    }
    SmartGraph(const string &name, FitPrototype &prototype, TVirtualPad *pad) : name(name), prototype(&prototype), pad(pad)
    {
        initHistogram();
        fit = prototype.getWithName(("fit" + name).c_str());
    }

    // < Setters />
    void setPad(TVirtualPad *pad)
    {
        this->pad = pad;
    }

    void setFit(FitPrototype &prototype)
    {
        this->prototype = &prototype;
        fit = prototype.getWithName(("fit" + name).c_str());
    }

    void setParameters(Double_t p0, Double_t p1, Double_t p2 = 0, Double_t p3 = 0, Double_t p4 = 0,
                       Double_t p5 = 0, Double_t p6 = 0, Double_t p7 = 0, Double_t p8 = 0,
                       Double_t p9 = 0, Double_t p10 = 0)
    {
        if (fit != nullptr)
        {
            fit->SetParameters(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
        }
    }

    void setParameters(Double_t *parameters)
    {
        if (fit != nullptr)
        {
            fit->SetParameters(parameters);
        }
    }

    void setColor(int primary, int secondary)
    {
        if (histo != nullptr)
        {
            histo->SetLineColor(primary);
            histo->SetFillColor(secondary);
        }
    }

    void setFitStyle(int lineColor, int lineStyle = 9)
    {
        if (fit != nullptr)
        {
            fit->SetLineColor(lineColor);
            fit->SetLineStyle(lineStyle);
        }
    }

    void setStyle(const string &style = "good")
    {
        if (histo != nullptr)
        {
            if (style == "good")
            {
                this->setColor(kGreen + 1, kTeal);
            }
            else if (style == "bad")
            {
                this->setColor(kRed, 40);
            }
            else if (style == "ugly")
            {
                this->setColor(kBlue, 38);
            }
            else if (style == "noise")
            {
                this->setColor(kBlack, 1);
            }
            else
            {
                this->setColor(kBlack, 1);
            }
        }
        if (fit != nullptr)
        {
            if (style == "good")
            {
                this->setFitStyle(kGreen + 1);
            }
            else if (style == "bad")
            {
                this->setFitStyle(kRed - 2);
            }
            else if (style == "ugly")
            {
                this->setFitStyle(kBlue);
            }
            else if (style == "noise")
            {
                this->setFitStyle(kBlack);
            }
            else
            {
                this->setFitStyle(kBlack);
            }
        }
    }

    // < Getters />

    Double_t getSignalIntegral()
    {
        return signal;
    }

    Double_t getNoiseIntegral()
    {
        return noise;
    }

    Double_t getRawSignalIntegral()
    {
        return rawSignal;
    }

    Double_t getEntries()
    {
        if (histo != nullptr)
        {
            return histo->GetEntries();
        }
        return 0;
    }

    const string &getName()
    {
        return name;
    }

    void Fill(Double_t x)
    {
        if (histo != nullptr)
        {
            histo->Fill(x);
        }
    }

    // <  Fit related members/>

    void Fit()
    {
        if (histo != nullptr && fit != nullptr)
        {
            histo->Fit(fit, "R");
            rawSignal = histo->Integral();
            noise = prototype->getNoise(fit)->Integral(fitMinX, fitMaxX) / histo->GetBinWidth(1);
            signal = rawSignal - noise;
        }
    }

    // < Draw related members />
    void Draw(const char *option = "")
    {
        if (pad != nullptr && histo != nullptr)
        {
            pad->cd();
            histo->Draw(option);
        }
        else
        {
            if (pad == nullptr)
            {
                throw std::runtime_error("Pad is nullptr");
            }
            if (histo == nullptr)
            {
                throw std::runtime_error("Histo is nullptr");
            }
        }
    }

    void drawNoise(const char *option = "")
    {
        if (pad != nullptr && fit != nullptr)
        {
            pad->cd();
            noiseFit = prototype->getNoise(fit);
            noiseFit->Draw(option);
        }
    }

    void FitAndDraw()
    {
        if (fit == nullptr || pad == nullptr)
        {
            return;
        }

        // the order is important!
        // one cannot fit and then draw the noise
        // because the noise is calculated from the fit
        this->Draw();
        this->Fit();
        this->drawNoise("same");
    }

    void printInfo(const std::string &prefix = "")
    {
        cout << prefix << "Entries: " << histo->GetEntries() << endl;

        if (fit == nullptr)
        {
            cout << prefix << "No fit found" << endl;
            return;
        }
        else
        {
            cout << prefix << "Graph integral: " << rawSignal << endl;
            cout << prefix << "Fit integral: " << fit->Integral(fitMinX, fitMaxX) / histo->GetBinWidth(1) << endl;
            cout << prefix << "Noise integral: " << noise << endl;
            cout << prefix << "Estimated signal: " << signal << " %err: " << noise / signal << endl;
        }
    }
};

#endif // SMARTGRAPH_H