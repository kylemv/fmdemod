#ifndef MYFMDEMODULATOR_IMPL_H
#define MYFMDEMODULATOR_IMPL_H

#include "myFMDemodulator_base.h"
#include <stdio.h>
#include <liquid/liquid.h>
#include <complex>
#include <math.h>

class myFMDemodulator_i : public myFMDemodulator_base
{
    ENABLE_LOGGING
    public:
        myFMDemodulator_i(const char *uuid, const char *label);
        ~myFMDemodulator_i();
        int serviceFunction();
    private:
        //Property change listeners
        void modFactorChanged(const float *oldVal, const float *newVal);
        void bandwidthChanged(const float *oldval, const float *newVal);
        void modTypeChanged(const std::string *oldVal, const std::string *newVal);
        void fcChanged(const float *oldVal, const float *newVal);
        //Helper Functions
        void createDemod(void);
        void sizeVectors(void);
        //Local Variables and Properties
        freqmodem m_demod;
        float m_factor;
        liquid_freqmodem_type m_type;
        std::vector<float> m_output;


        //SRI Data
        double m_delta;
        unsigned int m_size;
        unsigned int m_lastSize;
        BULKIO::StreamSRI m_sriOut;

        boost::mutex propertyLock_;
};

#endif // MYFMDEMODULATOR_IMPL_H
