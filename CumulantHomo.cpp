#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "ECorrHomo.h"

#include "CentManager.h"
#include "ReweightTool.h"

int main(int argc, char** argv) {
    // Read parameters from command line arguments
    if (argc != 4) {
        std::cout << "[ERROR] Should have 3 arguments!\n";
        std::cout << " - Usage: ./runCumulantHomo TASKNAME CENTRALITY REWEIGHT\n";
        std::cout << " - Example: ./runCumulantHomo default cent_edge.txt w8.txt\n";
        exit(-1);
    } else {
        std::cout << "[LOG] Note: This is homogeneous factorial cumulant version\n";
        std::cout << "[LOG] Input inforamtion: \n";
        std::cout << " - Task name: " << argv[1] << std::endl;
        std::cout << " - Centrality. edge file name: " << argv[2] << std::endl;
        std::cout << " - Reweight parameter name: " << argv[3] << std::endl;
    }

    // No CBWC
    std::cout << "[LOG] Now calculting No-CBWC results.\n";
    const int MaxMult = 1000;
    const int LowEventCut = 30; // to avoid error caused by low event number

    std::cout << "[LOG] Initializing cumulant calculating utils.\n";
    ECorr* ecp = new ECorr("Pro", MaxMult, LowEventCut); 
    ECorr* eca = new ECorr("Pbar", MaxMult, LowEventCut); 
    ecp->Init();
    ecp->ReadTerms(Form("%s.root", argv[1]));
    ecp->Calculate();
    ecp->Update(Form("cum.raw.%s.root", argv[1]));
    std::cout << "[LOG] Proton done.\n";
    eca->Init();
    eca->ReadTerms(Form("%s.root", argv[1]));
    eca->Calculate();
    eca->Update(Form("cum.raw.%s.root", argv[1]));
    std::cout << "[LOG] Antiproton done.\n";

    std::cout << "[LOG] No-CBWC results done, please check " << Form("cum.raw.%s.root", argv[1]) << ".\n";
    std::cout << "[LOG] Reading non-cbwc cumulant file...\n";
    TFile* tfin = new TFile(Form("cum.raw.%s.root", argv[1]));
    CentManager* cDef = new CentManager();
    cDef->Init(argv[2]); 
    int* nPart = cDef->GetArray();
    const int nCent = CentManager::nCent;
    Rew8Tool* rew8 = new Rew8Tool();
    rew8->ReadPars(argv[3], 0); // the index is designed for multi-reweight cases, so any integer is okay here

    std::cout << "[LOG] Initializing graphes.\n";
    // only 18 cumulants for utmost 4th order
    const int nCums = 7; // k2n and k3n
    const int nType = 2; // type for proton type
    const char* typeNames[nType] = {"Pro", "Pbar"};
    TGraphErrors* tgs[nType][nCums];
    TH1D* sCums[nType][nCums];
    const char* cumNames[nCums] = {
        // directly apply CBWC on those quantities
        "k1", "k2", "k3", "k21", "k31", "k2n", "k3n"
    };
    const char* cumTitles[nCums] = {
        "#kappa_{1}", "#kappa_{2}", "#kappa_{3}",
        "#kappa_{2}/#kappa_{1}", "#kappa_{3}/#kappa_{1}",
        "#kappa_{2}/#kappa_{1}^2", "#kappa_{3}/#kappa_{1}^3"
    };

    for (int i=0; i<nType; i++) {
        for (int j=0; j<nCums; j++) {
            tfin->GetObject(
                Form("%s%s", typeNames[i], cumNames[j]), 
                sCums[i][j]
            );
            tgs[i][j] = new TGraphErrors(nCent);
            tgs[i][j]->SetName(
                Form("%s_%s", typeNames[i], cumNames[j])
            );
            tgs[i][j]->SetTitle(cumTitles[j]);
            tgs[i][j]->SetMarkerStyle(20);
        }
    }

    const int LowMultCut = 5;
    double CentEvent[nType][nCent] = {0.0};
    double vSum[nType][nCent] = {0.0}; // value 
    double eSum[nType][nCent] = {0.0}; // error 

    // get number of events
    TH1D* hEntries[nType];
    for (int i=0; i<nType; i++) {
        tfin->GetObject(Form("%shEntries", typeNames[i]), hEntries[i]);
    }

    for (int i=0; i<nType; i++) { // i for proton types
        for (int j=0; j<nCums; j++) { // j for cumulant orders
            // loop 1, initialize the arrays to carry values and errors
            double nEvents = 0;
            for (int k=0; k<nCent; k++) { // k for centralities
                vSum[i][k] = 0;
                eSum[i][k] = 0;
                CentEvent[i][k] = 0;
            }
            
            // loop 2, sum up in different centrality bins
            for (int k=LowMultCut; k<=MaxMult; k++) { // here k for refmult3
                // Note: 0 is the first bin, +1 is a must
                nEvents = hEntries[i]->GetBinContent(k+1) * rew8->GetWeight(k+1);
                if (nEvents < LowEventCut) { continue; }

                int curCent = cDef->GetCent(k); // get current centrality
                if (curCent < 0) { continue; } // avoid invalid centrality 
                vSum[i][curCent] += (sCums[i][j]->GetBinContent(k+1) * nEvents);
                eSum[i][curCent] += pow((sCums[i][j]->GetBinError(k+1) * nEvents), 2);
                CentEvent[i][curCent] += nEvents;
            }

            // loop 3, get the averaged results (by dividing number of events)
            for (int k=0; k<nCent; k++) {
                // calculate values and errors
                vSum[i][k] /= CentEvent[i][k];
                eSum[i][k] = sqrt(eSum[i][k]) / CentEvent[i][k];
                // set points to graphes
                tgs[i][j]->SetPoint(k, nPart[k], vSum[i][k]);
                tgs[i][j]->SetPointError(k, 0.0, eSum[i][k]);
            }
        }
    }

    std::cout << "[LOG] Calculation finished, now saving.\n";
    TFile* tfout = new TFile(Form("cum.cbwc.%s.root", argv[1]), "recreate");
    tfout->cd();
    for (int i=0; i<nType; i++) {
        for (int j=0; j<nCums; j++) {
            tgs[i][j]->Write();
        }
    }
    tfin->Close();
    tfout->Close();

    std::cout << "[LOG] All done!.\n";
    return 0;
}

