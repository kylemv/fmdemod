/**************************************************************************

    This is the component code. This file contains the child class where
    custom functionality can be added to the component. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "myFMDemodulator.h"

PREPARE_LOGGING(myFMDemodulator_i)

myFMDemodulator_i::myFMDemodulator_i(const char *uuid, const char *label) :
    myFMDemodulator_base(uuid, label)
{
	addPropertyChangeListener("modFactor", this, &myFMDemodulator_i::modFactorChanged);
	addPropertyChangeListener("bandwidth", this, &myFMDemodulator_i::bandwidthChanged);
	addPropertyChangeListener("modType", this, &myFMDemodulator_i::modTypeChanged);
	addPropertyChangeListener("fc", this, &myFMDemodulator_i::fcChanged);
	m_demod = NULL;
	m_type = LIQUID_FREQMODEM_DELAYCONJ;
	m_delta = 0.0;		//input xdelta
	m_size = 0;			//input Packet size
	m_lastSize = 0;		//previous Packet Size

	m_sriOut = bulkio::sri::create("myFMDemodulator_OUT");
}

myFMDemodulator_i::~myFMDemodulator_i()
{
	if(m_demod){
		freqmodem_destroy(m_demod);
		m_demod = NULL;
	}
}
/************************* Property Change Listeners *******************************************/
void myFMDemodulator_i::modFactorChanged(const float *oldVal, const float *newVal)
{
	boost::mutex::scoped_lock lock(propertyLock_);

	//Validate proper modulation factor
	if(m_delta != 0){
		if(*newVal <= 0 || *newVal > 2*M_PI){
			modFactor = *oldVal;
			bandwidth = sample_rate/2;
			std::cerr << "WARNING! -- 'Modulation Factor' Must Be Greater Than Zero and Less Than Or Equal To One"<<std::endl;
			std::cerr << "-- Keeping 'Modulation Factor' as "<<*oldVal<<std::endl;
		}
		else{
			modFactor = *newVal;
		}
		bandwidth = modFactor / m_delta;
		createDemod();
	}
}
void myFMDemodulator_i::bandwidthChanged(const float *oldVal, const float *newVal)
{
	boost::mutex::scoped_lock lock(propertyLock_);

	//Validate bandwidth value
	if(m_delta != 0){
		if(*newVal > (sample_rate/2) || *newVal <= 0){
			bandwidth = *oldVal;
			std::cerr << "WARNING! -- 'Bandwidth' Must Be Equal to or Less Than 0.5*'Sample Rate' and greater than 0!" << std::endl;
			std::cerr << "-- Keeping 'Bandwidth' as " << *oldVal << std::endl;
		}
		else{
			bandwidth = *newVal;
		}
		//modFactor = m_delta * bandwidth;
		if(modFactor <= 0 || modFactor > 2*M_PI){
			std::cerr<<"WARNING! -- The New Bandwidth Value caused the Modulation Index to go out of the range 0 to 2pi"<<std::endl;
			std::cerr<<"-- Reseting the index to 0.  This may cause incorrect output"<<std::endl;
		}

		createDemod();
	}
}
void myFMDemodulator_i::modTypeChanged(const std::string *oldVal, const std::string *newVal)
{
	boost::mutex::scoped_lock lock(propertyLock_);

	if(*newVal == "LIQUID_FREQMODEM_DELAYCONJ"){
		m_type = LIQUID_FREQMODEM_DELAYCONJ;
	}else if(*newVal == "LIQUID_FREQMODEM_PLL"){
		m_type = LIQUID_FREQMODEM_PLL;
	}else{
		std::cerr << "No Valid 'Modulation Type'." << std::endl;
		std::cerr << "-- Setting 'Modulation Type' as 'LIQUID_FREQMODEM_DELAYCONJ'." << std::endl;
		m_type = LIQUID_FREQMODEM_DELAYCONJ;
	}
	//Create Demod if SRI has been received
	if (m_delta != 0){
		createDemod();
	}
}
void myFMDemodulator_i::fcChanged(const float *oldVal, const float *newVal)
{
	boost::mutex::scoped_lock lock(propertyLock_);

	if(*newVal <= -0.5 || *newVal >= 0.5){
		std::cerr << "Invalid 'Carrier Frequency, fc'.  fc must be greater than -0.5 and less than 0.5"<<std::endl;
		std::cerr << "-- Keeping 'fc' as"<<*oldVal<<std::endl;
		fc = *oldVal;
	}
	else {
		fc = *newVal;
	}
	if(m_delta != 0)
		createDemod();
}
/***********************************************************************************************

    Basic functionality:

        The service function is called by the serviceThread object (of type ProcessThread).
        This call happens immediately after the previous call if the return value for
        the previous call was NORMAL.
        If the return value for the previous call was NOOP, then the serviceThread waits
        an amount of time defined in the serviceThread's constructor.
        
    SRI:
        To create a StreamSRI object, use the following code:
                std::string stream_id = "testStream";
                BULKIO::StreamSRI sri = bulkio::sri::create(stream_id);

	Time:
	    To create a PrecisionUTCTime object, use the following code:
                BULKIO::PrecisionUTCTime tstamp = bulkio::time::utils::now();

        
    Ports:

        Data is passed to the serviceFunction through the getPacket call (BULKIO only).
        The dataTransfer class is a port-specific class, so each port implementing the
        BULKIO interface will have its own type-specific dataTransfer.

        The argument to the getPacket function is a floating point number that specifies
        the time to wait in seconds. A zero value is non-blocking. A negative value
        is blocking.  Constants have been defined for these values, bulkio::Const::BLOCKING and
        bulkio::Const::NON_BLOCKING.

        Each received dataTransfer is owned by serviceFunction and *MUST* be
        explicitly deallocated.

        To send data using a BULKIO interface, a convenience interface has been added 
        that takes a std::vector as the data input

        NOTE: If you have a BULKIO dataSDDS or dataVITA49  port, you must manually call 
              "port->updateStats()" to update the port statistics when appropriate.

        Example:
            // this example assumes that the component has two ports:
            //  A provides (input) port of type bulkio::InShortPort called short_in
            //  A uses (output) port of type bulkio::OutFloatPort called float_out
            // The mapping between the port and the class is found
            // in the component base class header file

            bulkio::InShortPort::dataTransfer *tmp = short_in->getPacket(bulkio::Const::BLOCKING);
            if (not tmp) { // No data is available
                return NOOP;
            }

            std::vector<float> outputData;
            outputData.resize(tmp->dataBuffer.size());
            for (unsigned int i=0; i<tmp->dataBuffer.size(); i++) {
                outputData[i] = (float)tmp->dataBuffer[i];
            }

            // NOTE: You must make at least one valid pushSRI call
            if (tmp->sriChanged) {
                float_out->pushSRI(tmp->SRI);
            }
            float_out->pushPacket(outputData, tmp->T, tmp->EOS, tmp->streamID);

            delete tmp; // IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCK
            return NORMAL;

        If working with complex data (i.e., the "mode" on the SRI is set to
        true), the std::vector passed from/to BulkIO can be typecast to/from
        std::vector< std::complex<dataType> >.  For example, for short data:

            bulkio::InShortPort::dataTransfer *tmp = myInput->getPacket(bulkio::Const::BLOCKING);
            std::vector<std::complex<short> >* intermediate = (std::vector<std::complex<short> >*) &(tmp->dataBuffer);
            // do work here
            std::vector<short>* output = (std::vector<short>*) intermediate;
            myOutput->pushPacket(*output, tmp->T, tmp->EOS, tmp->streamID);

        Interactions with non-BULKIO ports are left up to the component developer's discretion

    Properties:
        
        Properties are accessed directly as member variables. For example, if the
        property name is "baudRate", it may be accessed within member functions as
        "baudRate". Unnamed properties are given the property id as its name.
        Property types are mapped to the nearest C++ type, (e.g. "string" becomes
        "std::string"). All generated properties are declared in the base class
        (myFMDemodulator_base).
    
        Simple sequence properties are mapped to "std::vector" of the simple type.
        Struct properties, if used, are mapped to C++ structs defined in the
        generated file "struct_props.h". Field names are taken from the name in
        the properties file; if no name is given, a generated name of the form
        "field_n" is used, where "n" is the ordinal number of the field.
        
        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            //  - A boolean called scaleInput
              
            if (scaleInput) {
                dataOut[i] = dataIn[i] * scaleValue;
            } else {
		fc = 0;
                dataOut[i] = dataIn[i];
            }
            
        Callback methods can be associated with a property so that the methods are
        called each time the property value changes.  This is done by calling 
        addPropertyChangeListener(<property name>, this, &myFMDemodulator_i::<callback method>)
        in the constructor.

        Callback methods should take two arguments, both const pointers to the value
        type (e.g., "const float *"), and return void.

        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            
        //Add to myFMDemodulator.cpp
        myFMDemodulator_i::myFMDemodulator_i(const char *uuid, const char *label) :
            myFMDemodulator_base(uuid, label)
        {
            addPropertyChangeListener("scaleValue", this, &myFMDemodulator_i::scaleChanged);
        }

        void myFMDemodulator_i::scaleChanged(const float *oldValue, const float *newValue)
        {
            std::cout << "scaleValue changed from" << *oldValue << " to " << *newValue
                      << std::endl;
        }
            
        //Add to myFMDemodulator.h
        void scaleChanged(const float* oldValue, const float* newValue);
        

************************************************************************************************/
int myFMDemodulator_i::serviceFunction()
{
    //LOG_DEBUG(myFMDemodulator_i, "serviceFunction() example log message");
    
    bulkio::InFloatPort::dataTransfer *input = dataFloat_in->getPacket(bulkio::Const::BLOCKING);
    if(not input)
     	return NOOP;
    std::vector< std::complex<float> >* preDemod = (std::vector<std::complex<float> >*) &(input->dataBuffer);
    m_size = preDemod->size();
    {
    	//boost::mutex::scoped_lock lock(propertyLock_);
    	//remake demodulation object with new properties
    	if(input->sriChanged){
    		m_delta = input->SRI.xdelta;
    		//sample_rate = 1.0 / m_delta;
    		//modFactor = m_delta * bandwidth;

    		createDemod();
    		sizeVectors();

    		//push new SRI data
    		m_sriOut = input->SRI;
    		m_sriOut.mode = 0;
    		dataFloat_out->pushSRI(m_sriOut);
    	}
    	if(m_lastSize != m_size) {
    		sizeVectors();
    	}

    	for(unsigned int i = 0; i < m_size; i++){
    		freqmodem_demodulate(m_demod, *((liquid_float_complex*) &preDemod->at(i)), &m_output[i]);
    	}
    }

    dataFloat_out->pushPacket(m_output, input->T,input->EOS,input->streamID);
    delete input;
    return NORMAL;
}

void myFMDemodulator_i::createDemod(void)
{
	//boost::mutex::scoped_lock lock(propertyLock_);
	if(m_demod)
		freqmodem_destroy(m_demod);
	std::cout<<"Modulation Factor is "<<modFactor<<std::endl;
	m_demod = freqmodem_create(modFactor,fc,m_type);
}
void myFMDemodulator_i::sizeVectors(void)
{
	m_output.resize(m_size);
	m_lastSize = m_size;
}
