#ifndef __NPARTLOADER__
#define __NPARTLOADER__

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class NpartLoader{

    private:
        static const int nCent = 9;
        int edge[nCent];

    public:
        NpartLoader(){
            for (int i=0; i<nCent; i++) { edge[i] = 0; }
        }
        ~NpartLoader(){}

        void Init();
        int* GetArray() { return edge; }
};

#endif
