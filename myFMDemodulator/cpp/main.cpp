#include <iostream>
#include "ossie/ossieSupport.h"

#include "myFMDemodulator.h"
int main(int argc, char* argv[])
{
    myFMDemodulator_i* myFMDemodulator_servant;
    Resource_impl::start_component(myFMDemodulator_servant, argc, argv);
    return 0;
}

