#ifndef __ECORRHOMOHEADER__
#define __ECORRHOMOHEADER__

#include <vector>
#include <cmath>
#include "Rtypes.h"

class TProfile;
class TH1D;
class TFile;
class TGraphErrors;

class ECorr {

  	public:
		ECorr(const char*, int, int);
		~ECorr();
		void Init();
		void ReadTerms(const char *FileName = "noCbwc.root");
		void Calculate();
		void Save(const  char* OutName = "output.root");
		void Update(const  char* OutName = "output.root");

		Double_t NonNan(Double_t val) { return std::isnan(val) ? 0 : val; }

  	private:
		int _nMultBin;
		const char* ParticleType;
		int LowEventCut;
		TProfile* _V[56]; // -> _nTerms + 1
		TH1D* hEntries;
		//Double_t _q[7][7];
		//Attentation! bin center is integer multplicity value
		//zero multiplicity bin is not stored
    	//e.g. _sC[1]->GetBinContent(5) -> get C1'bin value at mult == 5
		TH1D* _sk1;
		TH1D* _sk2;
		TH1D* _sk3;
		TH1D* _sk21;
		TH1D* _sk31;
		TH1D* _sk2n;
		TH1D* _sk3n;
    	static const Int_t _nTerms = 55;
		const Char_t* Terms[_nTerms]={"q01_01q03_03", "q01_01q02_02q03_01", "q01_01_3q03_03", "q02_01q03_01", "q01_01_4q02_01", "q01_01_3", "q01_01_6", "q01_01q02_01_2", "q01_01_2q03_01", "q02_01", "q02_02q03_02", "q01_01_3q02_02", "q03_01", "q01_01_2q02_01_2", "q03_03", "q03_01q03_02", "q02_01q03_03", "q01_01_2q03_02", "q01_01_3q03_01", "q01_01q02_02q03_02", "q01_01q02_01", "q02_01q03_02", "q01_01_4", "q01_01q02_01q03_02", "q03_01_2", "q01_01_2q02_02", "q01_01_5", "q01_01q02_02_2", "q03_01q03_03", "q01_01q03_02", "q01_01q03_01", "q02_01q02_02", "q01_01_4q02_02", "q03_03_2", "q02_01_2", "q01_01q02_01q02_02", "q03_02", "q01_01q02_02q03_03", "q01_01", "q01_01_3q02_01", "q01_01q02_02", "q02_02q03_01", "q01_01q02_01q03_03", "q01_01_2q02_01q02_02", "q02_02_2", "q01_01_3q03_02", "q03_02q03_03", "q01_01_2q02_02_2", "q02_02q03_03", "q01_01_2q03_03", "q02_02", "q03_02_2", "q01_01_2", "q01_01_2q02_01", "q01_01q02_01q03_01"};

};

#endif