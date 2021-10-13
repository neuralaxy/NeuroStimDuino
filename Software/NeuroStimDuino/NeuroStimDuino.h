/*--------------------------------------------------------------------
  NeuroStimDuino.h - Arduino Library for controlling NeuroStimDuino using I2C commands.
  Created by: Neuralaxy LLC
  Created on: April 1st, 2021.
  Contact: contactus[at]neuralaxy.com
  
  This library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/

#ifndef NeuroStimDuino_h
#define NeuroStimDuino_h

#include <Arduino.h>
#include <SerialCommands.h>
#include <Wire.h>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  //For Arduino Mega  
  #define NSWire Wire
    
#elif defined(__SAM3X8E__)
  //For Ardunio Due
  #define NSWire Wire1

#else
  //For anyone else
  #define NSWire Wire

#endif 

#define default_i2c_addr 0x1A
#define FourBytesCommds 4
#define ThreeBytesCommds 3 
#define TwoBytesCommds 2
#define OneByteCommds 1
#define AMPL_UPP_LIMIT  127
#define AMPL_LOW_LIMIT  0
#define FREQ_UPP_LIMIT  100
#define FREQ_LOW_LIMIT  3
#define DURN_UPP_LIMIT  2000
#define DURN_LOW_LIMIT  0
#define IDLY_UPP_LIMIT  255
#define IDLY_LOW_LIMIT  0
#define DELY_UPP_LIMIT  165  //50% of max. possible @3Hz stim frequency, 
#define DELY_LOW_LIMIT  0
#define ADDR_UPP_LIMIT  119
#define ADDR_LOW_LIMIT  8


const int ArduinoLedPin = 13;
const int Data_samples_size = 100;
extern volatile int NSDuino_address; 

enum I2Ccommand {
	ERR  = 0x00,    
    AMPL = 0x01,
    FREQ = 0x02,
    DURN = 0x03,
    IDLY = 0x04,
    DELY = 0x05,
    SYMM = 0x06,
    ADDR = 0x07,
    SAMP = 0x08,
    READ = 0x09,
    RSET = 0x0A,
    EOFF = 0x0F,
    STIM = 0x10,
    STOP = 0x12,
    ENAB = 0x14,
    WAIT = 0x20
};

void cmd_led_on(SerialCommands* );
void cmd_led_off(SerialCommands* );
void setAmplitude_Callback(SerialCommands* );
void setFrequency_Callback(SerialCommands* );
void setPulseDuration_Callback(SerialCommands* );
void setInterPhaseDelay_Callback(SerialCommands* );
void setStartDelay_Callback(SerialCommands* );
void setWaveformSymmetry_Callback(SerialCommands* );
void changeI2Caddress_Callback(SerialCommands* );
void resetNeuroStimDuino_Callback(SerialCommands* );
void startStimulation_Callback(SerialCommands* );
void stopStimulation_Callback(SerialCommands* );
void enableChannel_Callback(SerialCommands* );
void setEmergencyOFF_Callback(SerialCommands* );
void startCurrentSampling_Callback(SerialCommands* );
void readRegister_Callback(SerialCommands* );
void cmd_unrecognized(SerialCommands* , const char* );
void print_Channel_Parameters(int );
int I2Cwrite(int , uint8_t , uint8_t , uint8_t , uint8_t );
int I2Cread(int ); 
char* I2Cread_byte_array(int );

enum I2Ccommand str2enum (const char* );

/*
class NeuroStimDuino
{
  public:
    NeuroStimDuino(uint8_t addr);
    //int I2Cwrite(int numbytes, I2Ccommand comm, uint8_t channel_no, int data1, int data2);    
    int I2Cread();
                
  private:
    uint8_t _i2caddr;
};
*/
#endif
