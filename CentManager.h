#ifndef __CENTMMGER__
#define __CENTMMGER__

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class CentManager {

    private:
        static const int nCent = 9;
        int npart[nCent];
        int mult[nCent];

    public:
        CentManager(){
            for (int i=0; i<nCent; i++) { 
                mult[i] = 0; 
                npart[i] = 0; 
            }
        }
        ~CentManager(){}

        void Init(const char* cent_edge);
        int* GetArray() { return npart; }
        int GetCent(int m);
};

#endif