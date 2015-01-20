#ifndef MYFMMODULATOR_IMPL_H
#define MYFMMODULATOR_IMPL_H

#include "myFMModulator_base.h"
#include <liquid/liquid.h>
class myFMModulator_i : public myFMModulator_base
{
    ENABLE_LOGGING
    public:
        myFMModulator_i(const char *uuid, const char *label);
        ~myFMModulator_i();
        int serviceFunction();
    private:
		//Property change listeners
		void modFactorChanged(const float *oldVal, const float *newVal);
		void modTypeChanged(const std::string *oldVal, const std::string *newVal);
		void fcChanged(const float *oldVal, const float *newVal);
		//Helper Functions
		void createModem(void);
		void sizeVectors(void);
		//Local Variables and Properties
		freqmodem m_modem;
		liquid_freqmodem_type m_type;
		std::vector< std::complex<float> > output;


		//SRI Data
		double m_delta;
		unsigned int m_size;
		unsigned int m_lastSize;
		BULKIO::StreamSRI m_sriOut;

		boost::mutex propertyLock_;
};

#endif // MYFMMODULATOR_IMPL_H
