#include "spark_wiring_i2c.h"
#include "spark_wiring_usbserial.h"
#include "spark_wiring_constants.h"
#include "spark_wiring.h"

class NCD8Channel12BitCurrentMonitor{
public:
    //Constructor
	NCD8Channel12BitCurrentMonitor(void);
    //Set Address.  Indicate status of jumpers on board.  Send 0 for not installed, send 1 for installed
    void setAddress(int a0, int a1);
    //Take 10 reads on each input to find zero point.  Make sure no current is drawn on inputs prior to calling this method.
    void calibrateInputs();
    
    float readInputCurrent(int channel);

    float* readAllInputsCurrent(float buffer[]);


    //User Accessible Variables
    //Whether or not the controller is ready to accept commands
    bool initialized = false;
    float adcVRef = 5.0000;
    
private:
    float convertToCurrent(int val, int channel);

    int channelZeroPoint[8];
};
