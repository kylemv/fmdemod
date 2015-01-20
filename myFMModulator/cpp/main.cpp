#include <iostream>
#include "ossie/ossieSupport.h"

#include "myFMModulator.h"
int main(int argc, char* argv[])
{
    myFMModulator_i* myFMModulator_servant;
    Resource_impl::start_component(myFMModulator_servant, argc, argv);
    return 0;
}

