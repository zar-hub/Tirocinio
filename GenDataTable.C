#define analisi_cxx
// custom
#include "src/global.h"
#include "src/filters.h"
#include "src/fitFunctions.h"
#include "src/SmartGraph.h"
#include "src/BinaryFilter.h"
#include "src/json.hpp"

// root
#include <TSystem.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2.h>
#include <TSystem.h>
#include <TF1.h>

// std
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// global dummy graphic output, nothing is actually redered
TCanvas canvas;

struct TableRow
{
    // parameters
    FitPrototype *passedPrototype;
    FitPrototype *failedPrototype;
    Bool_t (*filter)() = nullptr;
    string metodoStima;
    string rilevatore;
    int nBins;

    // results
    Double_t efficiency;
    Double_t efficiencyError;
};

TableRow createTableRow(FitPrototype &passed, FitPrototype &failed,
                        Bool_t (*filter)(), string metodoStima,
                        string rilevatore, int nBins,
                        Double_t efficiency, Double_t efficiencyError)
{
    TableRow row;
    row.passedPrototype = &passed;
    row.failedPrototype = &failed;
    row.filter = filter;
    row.metodoStima = metodoStima;
    row.rilevatore = rilevatore;
    row.nBins = nBins;
    row.efficiency = efficiency;
    row.efficiencyError = efficiencyError;
    return row;
}

void printRow(TableRow row)
{
    cout << row.passedPrototype->getName() << " "
         << row.failedPrototype->getName() << " "
         << filters::parseFilter(row.filter) << " "
         << row.metodoStima << " "
         << row.rilevatore << " "
         << row.nBins << " "
         << row.efficiency << " "
         << row.efficiencyError << endl;
}

void appendRowToFile(TableRow row, ofstream &file)
{
    file << row.passedPrototype->getName() << " "
         << row.failedPrototype->getName() << " "
         << filters::parseFilter(row.filter) << " "
         << row.metodoStima << " "
         << row.rilevatore << " "
         << row.nBins << " "
         << row.efficiency << " "
         << row.efficiencyError << endl;
}

void printTable(vector<TableRow> table)
{
    // print header
    cout << "passedPrototype failedPrototype filter metodoStima rilevatore nBins efficiency efficiencyError" << endl;

    for (auto row : table)
    {
        printRow(row);
    }
}

// this template is required to run
void analisi::Loop()
{
    return;
}

// crete a new object derived from analisi to not mess with the original
class TestAnalisiWithParameters : public analisi
{
public:
    void AddRow(vector<TableRow> *table, FitPrototype &passed,
                FitPrototype &failed, Bool_t (*filter)(), int nBins)
    {
        static int i = 0;

        // load data into the filters
        filters::loadData(this);
        string name = "set" + std::to_string(i++);

        // set the GLOBAL bins
        grBins = nBins;

        // create the three structure
        FilterTree tree(name);
        tree.root->require("", filters::isTagGood);
        auto filtered = tree.root->left->require("", filter, "g");
        filtered->branch("barrel", "endcaps", filters::isBarrel, filters::isEndcaps, "g");
        filtered->left->branch("probe", filters::isProbeGood, passed, failed, "gd");
        filtered->right->branch("probe", filters::isProbeGood, passed, failed, "gd");

        // < tweak the parameters />
        // pt3 endcaps

        // global canvas
        canvas.Clear();
        filtered->setPad(&canvas);

        // health check
        if (fChain == 0)
            return;

        // variable definitions
        Long64_t nentries = fChain->GetEntriesFast();
        Long64_t nbytes = 0, nb = 0;

        for (Long64_t jentry = 0; jentry < nentries; jentry++)
        {
            Long64_t ientry = LoadTree(jentry);
            if (ientry < 0)
                break;
            nb = fChain->GetEntry(jentry);
            nbytes += nb;

            tree.root->Fill(mee);
        }

        tree.root->FitAndDraw();
        // tree.printTree(tree.root);

        // print the results in the json

        // create new rows to add to the table
        TableRow fondoBarrel = createTableRow(passed, failed, filter,
                                              "fondo", "barrel", nBins,
                                              filtered->left->getEfficiency(),
                                              filtered->left->getEfficiencyError());
        TableRow fondoEndcaps = createTableRow(passed, failed, filter,
                                               "fondo", "endcaps", nBins,
                                               filtered->right->getEfficiency(),
                                               filtered->right->getEfficiencyError());
        TableRow piccoBarrel = createTableRow(passed, failed, filter,
                                              "picco", "barrel", nBins,
                                              filtered->left->getEfficiencySignal(),
                                              filtered->left->getEfficiencyErrorSignal());
        TableRow piccoEndcaps = createTableRow(passed, failed, filter,
                                               "picco", "endcaps", nBins,
                                               filtered->right->getEfficiencySignal(),
                                               filtered->right->getEfficiencyErrorSignal());
        table->push_back(fondoBarrel);
        table->push_back(piccoBarrel);
        table->push_back(fondoEndcaps);
        table->push_back(piccoEndcaps);

        printRow(fondoBarrel);
        printRow(piccoBarrel);
        printRow(fondoEndcaps);
        printRow(piccoEndcaps);

        // save the row to file
        ofstream file;
        file.open("tableWithFunc.txt", ios::app);
        appendRowToFile(fondoBarrel, file);
        appendRowToFile(piccoBarrel, file);
        appendRowToFile(fondoEndcaps, file);
        appendRowToFile(piccoEndcaps, file);
        file.close();

        canvas.Draw();
        canvas.Update();

        // std::cout << "Press ENTER to continue...";
        //  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    };
};

void GenDataTable()
{

    // create the table
    vector<TableRow> table;

    // create the prototypes
    vector<FitPrototype> passed = {rettaBigausPrototype, expBigausPrototype};
    vector<FitPrototype> failed = {rettaBigausPrototype, expBigausPrototype};

    // create the filter array
    vector<Bool_t (*)()> filters = {
        filters::isTagGood,
        filters::isPositive,
        filters::isNegative,
        filters::isPtBin0,
        filters::isPtBin1,
        filters::isPtBin2,
        filters::isPtBin3};

    // create the bins array
    // vector<int> bins = {40, 50, 60, 70, 80, 90, 100, 150, 200};
    vector<int> bins = {60, 80, 100};

    // create the test object
    TestAnalisiWithParameters test;

    // add rows to the table
    for (auto bin : bins)
    {
        for (auto filter : filters)
        {
            for (auto passed : passed)
                for (auto failed : failed)
                    test.AddRow(&table, passed, failed, filter, bin);
        }
    }

    // printTable(table);
}