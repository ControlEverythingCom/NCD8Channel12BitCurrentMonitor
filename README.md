# About

This Library is intended for use with NCD 8 channel 12 bit current monitor Particle Core/Photon compatible controllers.

The intention of this library is to make use of the NCD 8 channel 12 bit current monitor products with Particle development web IDE as simple as possible for users.
###Developer information

NCD has been designing and manufacturing computer control products since 1995.  We have specialized in hardware design and manufacturing of Relay controllers for 20 years.  We pride ourselves as being the industry leader of computer control relay products.  Our products are proven reliable and we are very excited to support Particle.  For more information on NCD please visit www.controlanything.com 

###Requirements
- NCD 8 channel Particle Core/Photon compatible current monitor board
- Particle Core/Photon module
- Knowledge base for developing and programming with Particle Core/Photon modules.

### Version
1.0.0

### How to use this library

The libary must be imported into your application.  This can be done through the Particle WEB IDE by selecting Libraries, then select the NCD8Channel12BitCurrentMonitor.  Click Include in App button.  Select the App you want to include the library in.  Finally click Add to this app.  For more information see [Particles's documentation] [sparkIncludeLibrary] 

### Example use

Once the Library is included in your applicaiton you should see an include statement at the top like this:
```cpp
//This #include statement was automatically added by the Particle IDE.
#include "NCD8Channel12BitCurrentMonitor/NCD8Channel12BitCurrentMonitor.h"
```
Now you need to instanciate an object of the library for use in your application like this:
```cpp
NCD8Channel12BitCurrentMonitor controller;
```

Here is an example use case for the class
```cpp
/* Includes ------------------------------------------------------------------*/
#include "application.h"

SYSTEM_MODE(MANUAL);
NCD8Channel12BitCurrentMonitor controller;

/* This function is called once at start up ----------------------------------*/
void setup()
{
Serial.begin(115200);
controller.setAddress(0,0);
controller.adcVRef = 4.958;
controller.calibrateInputs();
}

/* This function loops forever --------------------------------------------*/
void loop()
{
//	//Test read 1 input
//	float currentReading = controller.readInputCurrent(1);
//	Serial.print("Input ");
//	Serial.print(1);
//	Serial.print(" current reading: ");
//	Serial.println(currentReading, 4);
//	Serial.println();
//
//	delay(1000);

//Test read all inputs:
float inputReadings[8];
float* currentReadings = controller.readAllInputsCurrent(inputReadings);
for(int i = 0; i < 8; i++){
Serial.print("Input ");
Serial.print(i+1);
Serial.print(" reading: ");
Serial.println(currentReadings[i], 4);
}
delay(1000);

}
```

###Public accessible methods
```cpp
void setAddress(int a0, int a1);
```
>Must be called first before using the object.  This method should also be called any time communication with
>the controller is lost or broken to recover communication  This method accepts two int arguments.  This
>tells the Library what address to direct commands to.  a0 and a1 ints are representations of the two
>jumpers on the 8 channel relay controller which are labeled on the board A0 and A1.  If the jumper is
>installed then that int in this call should be set to 1.  If it is not installed then the int should be set to 
So if I have A0 and A1 installed I would call ```controller.setAddress(1, 1).```

```cpp
void calibrateInputs();
```
>It is highly recommended to call this method prior use of the controller.  Due to the electronics on the controller
>it is nessisary to determine the 0 point of each of the inputs.  Make sure no loads are connected to any of the channels, then call this method.  This will set the 0 current draw position of each input.  Note that you may still see some slight variations on the readings which is to be expected since the percentage of reading error is around 1.5% or about 7.5 mA.  We have seen as much as 32 mA variance.

```cpp
float readInputCurrent(int channel);
```
>This function accepts 1 int argument and returns 1 float value.  Valid int arguments are 1-8 for the respective channel to be read.  Valid float returns are 0.00 - 5.00 which is the current consumption being drawn through the respective channel.

```cpp
float* readAllInputsCurrent(float buffer[]);
```
>This function accepts 1 float array and returns 1 float array pointer.  An 8 position float array should be passed to this function as a buffer.  The array will be populated with current readings on each of the 8 on board channels representing their respective current consumption readings.

###Public accessible variables
```cpp
bool initialized;
```
>This boolean indicates the current status of the interface connection to the controller.  This variable should
>be checked often throughout your application.  If communication to the board is lost for any reason this
>boolean variable will return false.  If all is well it will return true.

```cpp
float adcVRef
```
>In order to assure readings as accurate as possible it is important to set the voltage reference for the on board ADC chip.  The user should take a reading on the VCC line of the on board ADC IC and use that reading to set this float variable.  This will ensure readings as accurate as possible.


License
----

GNU
[sparkIncludeLibrary]:https://docs.particle.io/guide/getting-started/build/photon/