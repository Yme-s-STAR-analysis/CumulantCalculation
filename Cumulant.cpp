#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "ECorr.h"

#include "CentManager.h"
#include "ReweightTool.h"

int main(int argc, char** argv){
    // Read parameters from command line arguments
    if (argc != 4) {
        std::cout << "[ERROR] Should have 3 arguments!\n";
        std::cout << " - Usage: ./runCumulant TASKNAME CENTRALITY REWEIGHT\n";
        std::cout << " - Example: ./runCumulant default cent_edge.txt w8.txt\n";
        exit(-1);
    } else {
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
    ECorr* ecn = new ECorr("Netp", MaxMult, LowEventCut); 
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
    ecn->Init();
    ecn->ReadTerms(Form("%s.root", argv[1]));
    ecn->Calculate();
    ecn->Update(Form("cum.raw.%s.root", argv[1]));
    std::cout << "[LOG] Net-proton done.\n";

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
    // 22 + 4: ppb, C2/ppb, C3/ppb, C3/C1
    const int nCums = 26; // cums for cumulant and correlation function (orders) 
    const int nType = 3; // type for proton type
    const char* typeNames[nType] = {"Pro", "Pbar", "Netp"};
    TGraphErrors* tgs[3][nCums];
    TH1D* sCums[3][nCums];
    const char* cumNames[nCums] = {
        "C1", "ppb", "C2", "C3", "C4", "C5", "C6", // 7
        "R21", "R2s", "R31", "R32", "R3s", "R42", "R51", "R62", // 15
        "k1", "k2", "k3", "k4", "k5", "k6", // 21
        "k21", "k31", "k41", "k51", "k61" // 26
    };
    const char* cumTitles[nCums] = {
        "C_{1}", "<p+#bar{p}>", "C_{2}", "C_{3}", "C_{4}", "C_{5}", "C_{6}",
        "C_{2}/C_{1}", "C_{2}/<p+#bar{p}>", "C_{3}/C_{1}", "C_{3}/C_{2}", "C_{3}/<p+#bar{p}>", "C_{4}/C_{2}", "C_{5}/C_{1}", "C_{6}/C_{2}",
        "#kappa_{1}", "#kappa_{2}", "#kappa_{3}", "#kappa_{4}", "#kappa_{5}", "#kappa_{6}",
        "#kappa_{2}/#kappa_{1}", "#kappa_{3}/#kappa_{1}", "#kappa_{4}/#kappa_{1}", "#kappa_{5}/#kappa_{1}", "#kappa_{6}/#kappa_{1}"
    };

    for (int i=0; i<nType; i++){
        for (int j=0; j<nCums; j++){
            if (i == 2) { // skip kappa for net proton
                if (j >= 15) { continue; } 
            } else { // skip ppb for proton and antiproton
                if (j == 1 || j == 8 || j == 11) { continue; }
            }
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
    for (int i=0; i<nType; i++){
        tfin->GetObject(Form("%shEntries", typeNames[i]), hEntries[i]);
    }

    for (int i=0; i<nType; i++){ // i for proton types
        for (int j=0; j<nCums; j++){ // j for cumulant orders
            if (i == 2) { // skip kappa for net proton
                if (j >= 15) { continue; } 
            } else { // skip ppb for proton and antiproton
                if (j == 1 || j == 8 || j == 11) { continue; }
            }

            // loop 1, initialize the arrays to carry values and errors
            double nEvents = 0;
            for (int k=0; k<nCent; k++){ // k for centralities
                vSum[i][k] = 0;
                eSum[i][k] = 0;
                CentEvent[i][k] = 0;
            }
            
            // loop 2, sum up in different centrality bins
            for (int k=LowMultCut; k<=MaxMult; k++){ // here k for refmult3
                // Note: 0 is the first bin, +1 is a must
                nEvents = hEntries[i]->GetBinContent(k+1) * rew8->GetWeight(k+1);
                if (nEvents < LowEventCut){ continue; }

                int curCent = cDef->GetCent(k); // get current centrality
                if (curCent < 0){ continue; } // avoid invalid centrality 
                if (
                    (j < 7) || // C1 ~ C6 + ppb
                    (j >= 15 && j <= 20) // k1 ~ k6
                ){ // for ratios, the value is just a dividing results, no need to do cbwc
                    vSum[i][curCent] += (sCums[i][j]->GetBinContent(k+1) * nEvents);
                }
                // but the errors should be calculated
                eSum[i][curCent] += pow((sCums[i][j]->GetBinError(k+1) * nEvents), 2);
                CentEvent[i][curCent] += nEvents;
            }

            // loop 3, get the averaged results (by dividing number of events)
            for (int k=0; k<nCent; k++){
                // calculate values
                if (i == 2) { // skip kappa for net proton
                    if (j >= 15){ continue; } 
                } else { // skip ppb for proton and antiproton
                    if (j == 1 || j == 8 || j == 11) { continue; }
                }
                if (
                    (j < 7) || 
                    (j >= 15 && j <= 20)
                ){
                    vSum[i][k] /= CentEvent[i][k];
                } else if (j == 7) {
                    vSum[i][k] = *(tgs[i][2]->GetY()+k) / *(tgs[i][0]->GetY()+k); // C2 / C1
                } else if (j == 8) {
                    vSum[i][k] = *(tgs[i][2]->GetY()+k) / *(tgs[i][1]->GetY()+k); // C2 / ppb
                } else if (j == 9) {
                    vSum[i][k] = *(tgs[i][3]->GetY()+k) / *(tgs[i][0]->GetY()+k); // C3 / C1
                } else if (j == 10) {
                    vSum[i][k] = *(tgs[i][3]->GetY()+k) / *(tgs[i][2]->GetY()+k); // C3 / C2
                } else if (j == 11) {
                    vSum[i][k] = *(tgs[i][3]->GetY()+k) / *(tgs[i][1]->GetY()+k); // C3 / ppb
                } else if (j == 12) {
                    vSum[i][k] = *(tgs[i][4]->GetY()+k) / *(tgs[i][2]->GetY()+k); // C4 / C2
                } else if (j == 13) {
                    vSum[i][k] = *(tgs[i][5]->GetY()+k) / *(tgs[i][0]->GetY()+k); // C5 / C1
                } else if (j == 14) {
                    vSum[i][k] = *(tgs[i][6]->GetY()+k) / *(tgs[i][2]->GetY()+k); // C6 / C2
                } else if (j >= 21) {
                    vSum[i][k] = *(tgs[i][j-5]->GetY()+k) / *(tgs[i][15]->GetY()+k); // k(2~6) / k1
                }
                // calculate errors
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
    for (int i=0; i<nType; i++){
        for (int j=0; j<nCums; j++){
            if (i == 2) { // skip kappa for net proton
                if (j >= 15){ continue; } 
            } else { // skip ppb for proton and antiproton
                if (j == 1 || j == 8 || j == 11) { continue; }
            }
            tgs[i][j]->Write();
        }
    }
    tfin->Close();
    tfout->Close();

    std::cout << "[LOG] All done!.\n";
    return 0;
}

