#ifndef MYFMDEMODULATOR_IMPL_BASE_H
#define MYFMDEMODULATOR_IMPL_BASE_H

#include <boost/thread.hpp>
#include <ossie/Resource_impl.h>
#include <ossie/ThreadedComponent.h>

#include <bulkio/bulkio.h>

class myFMDemodulator_base : public Resource_impl, protected ThreadedComponent
{
    public:
        myFMDemodulator_base(const char *uuid, const char *label);
        ~myFMDemodulator_base();

        void start() throw (CF::Resource::StartError, CORBA::SystemException);

        void stop() throw (CF::Resource::StopError, CORBA::SystemException);

        void releaseObject() throw (CF::LifeCycle::ReleaseError, CORBA::SystemException);

        void loadProperties();

    protected:
        // Member variables exposed as properties
        float modFactor;
        std::string modType;
        float bandwidth;
        float fc;
        float sample_rate;

        // Ports
        bulkio::InFloatPort *dataFloat_in;
        bulkio::OutFloatPort *dataFloat_out;

    private:
};
#endif // MYFMDEMODULATOR_IMPL_BASE_H
