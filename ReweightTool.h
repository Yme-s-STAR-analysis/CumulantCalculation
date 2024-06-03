#ifndef __REW8TOOL__
#define __REW8TOOL__

class TF1;
class Rew8Tool {

    private:
        TF1* func;
        int bin_lim;

    public:
        Rew8Tool(){}
        ~Rew8Tool(){}

        void ReadPars(const char* fname, int idx);
        void SetMaxMult(int mult) { bin_lim = mult; }
        double GetWeight(int mult);

};

#endif
