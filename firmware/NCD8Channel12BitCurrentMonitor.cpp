#include "NCD8Channel12BitCurrentMonitor.h"
#include "spark_wiring_usbserial.h"

//Comment line below out to turn off Serial logging
//#define LOGGING

int address = 0x48;

byte channelCommand[] = {128, 192, 144, 208, 160, 224, 176, 240};

//Constructor
NCD8Channel12BitCurrentMonitor::NCD8Channel12BitCurrentMonitor(){
}

//Retry added
void NCD8Channel12BitCurrentMonitor::setAddress(int a0, int a1){
	int retrys = 0;
    address = 0x48;
    if(a0 == 1){
        address = address | 1;
    }
    if(a1 == 1){
        address = address | 2;
    }
    //Start I2C port
    Wire.begin();
    //Open connection to specified address
    retryAddress:
    Wire.beginTransmission(address);
    byte status = Wire.endTransmission();
    if(status != 0){
        if(retrys < 3){
            retrys++;
            goto retryAddress;
        }else{
        	Serial.println("Set Address Command failed");
            initialized = false;
            retrys = 0;
        }
        
    }else{
         Serial.println("Set Address Command Successful");
        initialized = true;
    }
}

void NCD8Channel12BitCurrentMonitor::calibrateInputs(){
	for(int i = 0; i < 8; i++){
		int calibrationPoint = 0;
		//Take 3 readings on the input
		for(int a = 0; a < 10; a++){
			Wire.beginTransmission(address);
			Wire.write(channelCommand[i]);
			Wire.endTransmission();
			Wire.requestFrom(address, 2);
			byte b1 = Wire.read();
			byte b2 = Wire.read();
			int reading = (b1*256)+b2;
			calibrationPoint = calibrationPoint + reading;
			delay(50);
		}
		//Average 3 readings and store them in the zeroPoints array
		calibrationPoint = (calibrationPoint/10);
		channelZeroPoint[i] = calibrationPoint;
		delay(20);
	}
}

float NCD8Channel12BitCurrentMonitor::readInputCurrent(int channel){
	//valid return range: 0-4095.  4096 returned in event of error
	if(channel > 8 || channel < 0){
		//Incorrect channel id.
		return 4096;
	}
	int c = (--channel);

	//Send command to controller
	Wire.beginTransmission(address);
	Wire.write(channelCommand[c]);
	byte status = Wire.endTransmission();
	if(status != 0){
		//Something went wrong.
		return 4096;
	}else{
		//Read two bytes back from controller
		Wire.requestFrom(address, 2);
		byte b1 = Wire.read();
		byte b2 = Wire.read();
		int reading = (b1*256)+b2;
		float current = convertToCurrent(reading, c);
		return current;
	}

}

float* NCD8Channel12BitCurrentMonitor::readAllInputsCurrent(float buffer[]){
	for(int i = 0; i < 8; i++){
		//Send command to controller
		Wire.beginTransmission(address);
		Wire.write(channelCommand[i]);
		byte status = Wire.endTransmission();
		if(status != 0){
			//Something went wrong.
			buffer[i] = 4096;
		}else{
			//Read two bytes back from controller
			Wire.requestFrom(address, 2);
			byte b1 = Wire.read();
			byte b2 = Wire.read();
			int reading = (b1*256)+b2;
			buffer[i] = convertToCurrent(reading, i);
		}
	}
	return buffer;
}

float NCD8Channel12BitCurrentMonitor::convertToCurrent(int val, int channel){
	double mvRef = adcVRef *1000;


	double voltage = (val/4096.0)*mvRef;
	double center = (channelZeroPoint[channel]/4096.0)*mvRef;
	double amps = ((voltage - center)/185);
	return amps;
}
