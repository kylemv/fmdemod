#ifndef MYFMMODULATOR_IMPL_BASE_H
#define MYFMMODULATOR_IMPL_BASE_H

#include <boost/thread.hpp>
#include <ossie/Resource_impl.h>
#include <ossie/ThreadedComponent.h>

#include <bulkio/bulkio.h>

class myFMModulator_base : public Resource_impl, protected ThreadedComponent
{
    public:
        myFMModulator_base(const char *uuid, const char *label);
        ~myFMModulator_base();

        void start() throw (CF::Resource::StartError, CORBA::SystemException);

        void stop() throw (CF::Resource::StopError, CORBA::SystemException);

        void releaseObject() throw (CF::LifeCycle::ReleaseError, CORBA::SystemException);

        void loadProperties();

    protected:
        // Member variables exposed as properties
        float modFactor;
        std::string modType;
        float fc;

        // Ports
        bulkio::InFloatPort *dataFloat_in;
        bulkio::OutFloatPort *dataFloat_out;

    private:
};
#endif // MYFMMODULATOR_IMPL_BASE_H
