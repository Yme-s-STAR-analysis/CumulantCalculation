#ifndef __CENTMMGER__
#define __CENTMMGER__

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class CentManager {

    public:
        static constexpr int nCent = 9;
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
        
    private:
        int npart[nCent];
        int mult[nCent];

};

#endif