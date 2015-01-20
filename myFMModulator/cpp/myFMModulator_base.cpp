#include "myFMModulator_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the component class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

myFMModulator_base::myFMModulator_base(const char *uuid, const char *label) :
    Resource_impl(uuid, label),
    ThreadedComponent()
{
    loadProperties();

    dataFloat_in = new bulkio::InFloatPort("dataFloat_in");
    addPort("dataFloat_in", dataFloat_in);
    dataFloat_out = new bulkio::OutFloatPort("dataFloat_out");
    addPort("dataFloat_out", dataFloat_out);
}

myFMModulator_base::~myFMModulator_base()
{
    delete dataFloat_in;
    dataFloat_in = 0;
    delete dataFloat_out;
    dataFloat_out = 0;
}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void myFMModulator_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    Resource_impl::start();
    ThreadedComponent::startThread();
}

void myFMModulator_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
{
    Resource_impl::stop();
    if (!ThreadedComponent::stopThread()) {
        throw CF::Resource::StopError(CF::CF_NOTSET, "Processing thread did not die");
    }
}

void myFMModulator_base::releaseObject() throw (CORBA::SystemException, CF::LifeCycle::ReleaseError)
{
    // This function clears the component running condition so main shuts down everything
    try {
        stop();
    } catch (CF::Resource::StopError& ex) {
        // TODO - this should probably be logged instead of ignored
    }

    Resource_impl::releaseObject();
}

void myFMModulator_base::loadProperties()
{
    addProperty(modFactor,
                .1,
                "modFactor",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(modType,
                "LIQUID_FREQMODEM_DELAYCONJ",
                "modType",
                "",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(fc,
                0,
                "fc",
                "",
                "readwrite",
                "",
                "external",
                "configure");

}


