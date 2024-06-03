#include "ReweightTool.h"
#include <fstream>
#include "TF1.h"
#include "TString.h"
#include <iostream>

void Rew8Tool::ReadPars(const char* fname, int idx) {
    func = new TF1(Form("func%d", idx), "[0]+[1]/([2]*x + [3]) + [4]*([2]*x + [3]) + [6]/TMath::Power([2]*x+[3] ,2) + [5]*TMath::Power([2]*x+[3] ,2)", 0, 500);
    std::ifstream fin;
    fin.open(fname);
    std::string str;
    int i = 0;
    int cnt = 0;
    while(std::getline(fin, str)){
        double tmp = std::atof(str.c_str());
        if (cnt < 7) {
            func->SetParameter(cnt, tmp);
            cnt ++;
        } else {
            SetMaxMult((int)tmp);
        }
        // std::cout << "read this: " << str << " -> " << str.c_str() << " -> " << tmp <<std::endl;
    }
    fin.close();
    // std::cout << "DEBUG: pars: ";
    // for (int i=0; i<7; i++) {
    //    std::cout << func->GetParameter(i) << " ";
    // }
    // std::cout << "\nOn mult = 20, it will be " << func->Eval(20) << std::endl;
    // std::cout << "max bin: " << bin_lim << std::endl;
}

double Rew8Tool::GetWeight(int mult) {
    return mult >= bin_lim ? 1.0 : func->Eval(mult);
}
