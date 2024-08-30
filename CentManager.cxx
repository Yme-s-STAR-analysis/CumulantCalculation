#include "CentManager.h"

void CentManager::Init(const char* cent_edge) {
    // read Npart array
    std::ifstream fInNpart("Npart.txt");
    std::string strt;
    int i = 0;
    while(std::getline(fInNpart, strt)){
        int x = std::atoi(strt.c_str());
        npart[i] = x;
        i += 1;
        if (i > nCent){
            std::cout << "[Warning] - CentManager: the target Npart array length is out of range (" << i << " of " << nCent << ").\n";
            break;
        }
    }
    std::cout << "[LOG] <Npart> array: ";
    for (int i=0; i<nCent; i++) {
        std::cout << npart[i];
        if (i != nCent-1) {
            std::cout << ", ";
        } else {
            std::cout << "\n";
        }
    }
    fInNpart.close();
    // read multiplicity edge
    std::ifstream fInEdge(cent_edge);
    i = 0;
    while(std::getline(fInEdge, strt)){
        int x = std::atoi(strt.c_str());
        mult[i] = x;
        i += 1;
        if (i > nCent){
            std::cout << "[Warning] - CentManager: the target centrality edge index is out of range (" << i << " of " << nCent << ").\n";
            break;
        }
    }
    std::cout << "[LOG] Centrality bin edge: ";
    for (int i=0; i<nCent; i++) {
        std::cout << mult[i];
        if (i != nCent-1) {
            std::cout << ", ";
        } else {
            std::cout << "\n";
        }
    }
    fInNpart.close();
}

int GetCent(int m){
    for (int i=0; i<nCent; i++){
        if (m >= mult[i]){ return i; }
    }
    return -1;
}