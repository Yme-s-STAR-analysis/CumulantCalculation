#include "NpartLoader.h"

void NpartLoader::Init() {
    std::ifstream MyFin("Npart.txt");
    std::string strt;
    int i = 0;
    while(std::getline(MyFin, strt)){
        int x = std::atoi(strt.c_str());
        edge[i] = x;
        i += 1;
        if (i > nCent){
            std::cout << "[Warning] In CentDefinition class, the target centrality edge is out of range " << i << " of " << nCent << ".\n";
            break;
        }
    }
    std::cout << "[LOG] <Npart> array: ";
    for (int i=0; i<nCent; i++) {
        std::cout << edge[i];
        if (i != nCent-1) {
            std::cout << ", ";
        } else {
            std::cout << "\n";
        }
    }
    MyFin.close();
}
