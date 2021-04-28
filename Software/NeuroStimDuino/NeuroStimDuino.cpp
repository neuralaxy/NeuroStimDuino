/*--------------------------------------------------------------------
  NeuroStimDuino.cpp - Arduino Library for controlling NeuroStimDuino using I2C commands.
  Created by: Neuralaxy LLC
  Created on: April 1st, 2021.
  Contact: contactus[at]neuralaxy.com
  
  This library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/

#include <Arduino.h>
#include <SerialCommands.h>
#include "NeuroStimDuino.h"

volatile int NSDuino_address = default_i2c_addr; 

int I2Cwrite(int numbytes, uint8_t commd, uint8_t channel_no = -1, int data_lsb = -1, int data_msb = -1){
   int error = -1;
   NSWire.beginTransmission(NSDuino_address);
   NSWire.write(numbytes);
   switch(numbytes){
    case 1:
            NSWire.write(commd);
            break;
    case 2: 
            NSWire.write(commd);
            NSWire.write(channel_no);
            break;
    case 3: 
            NSWire.write(commd);
            if (channel_no != -1){
              NSWire.write(channel_no);              
            }            
            NSWire.write(data_lsb);
            break;
    case 4: 
            NSWire.write(commd);
            NSWire.write(channel_no);
            NSWire.write(data_lsb);
            NSWire.write(data_msb);
            break;                      
   }
   error = NSWire.endTransmission(true);
   return(error); 
}

//This is the default handler, and gets called when no other command matches. 
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}

//called for ON command
void cmd_led_on(SerialCommands* sender)
{
  digitalWrite(ArduinoLedPin, HIGH);  
  sender->GetSerial()->println("Led is on");
}

//called for OFF command
void cmd_led_off(SerialCommands* sender)
{
  digitalWrite(ArduinoLedPin, LOW);  
  sender->GetSerial()->println("Led is off");
}

void setAmplitude_Callback(SerialCommands* sender)
{
  int i2c_error = -1; 
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Amplitude is missing ");
    return;
  }
  int val = atoi(val_str);
  if (val < AMPL_LOW_LIMIT || val > AMPL_UPP_LIMIT)
  {
    sender->GetSerial()->println("ERROR! Value outside range ");
    return;
  }
  i2c_error = I2Cwrite(ThreeBytesCommds, AMPL, chan, val); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }
}

void setFrequency_Callback(SerialCommands* sender)
{
  int i2c_error = -1; 
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Frequency is missing ");
    return;
  }
  int val = atoi(val_str);
  if (val < FREQ_LOW_LIMIT || val > FREQ_UPP_LIMIT)
  {
    sender->GetSerial()->println("ERROR! Value outside range ");
    return;
  }
  i2c_error = I2Cwrite(ThreeBytesCommds, FREQ, chan, val); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }  
}

void setPulseDuration_Callback(SerialCommands* sender)
{
  int i2c_error = -1; 
  uint8_t val_lsb = 0;
  uint8_t val_msb = 0;
  
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Duration is missing ");
    return;
  }
  int val = atoi(val_str);
  if (val < DURN_LOW_LIMIT || val > DURN_UPP_LIMIT)
  {
    sender->GetSerial()->println("ERROR! Value outside range ");
    return;
  }
  //Convert int to 2 bytes with LSB byte first
  val_lsb = (val & 255);
  val_msb = ((val >> 8) & 255);
  i2c_error = I2Cwrite(FourBytesCommds, DURN, chan, val_lsb, val_msb); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }
}

void setInterPhaseDelay_Callback(SerialCommands* sender)
{
  int i2c_error = -1; 
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Inter-phase duration is missing ");
    return;
  }
  int val = atoi(val_str);
  if (val < IDLY_LOW_LIMIT || val > IDLY_UPP_LIMIT)
  {
    sender->GetSerial()->println("ERROR! Value outside range ");
    return;
  }
  i2c_error = I2Cwrite(ThreeBytesCommds, IDLY, chan, val); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }  
}

void setStartDelay_Callback(SerialCommands* sender)
{
  int i2c_error = -1; 
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Start delay is missing ");
    return;
  }
  int val = atoi(val_str);
  if (val < DELY_LOW_LIMIT || val > DELY_UPP_LIMIT)
  {
    sender->GetSerial()->println("ERROR! Value outside range ");
    return;
  }
  i2c_error = I2Cwrite(ThreeBytesCommds, DELY, chan, val); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }  
}

void setWaveformSymmetry_Callback(SerialCommands* sender)
{
  int i2c_error = -1; 
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Value is missing ");
    return;
  }
  int val = atoi(val_str);
  if (val != 0 && val != 1)
  {
    sender->GetSerial()->println("ERROR! Value outside range ");
    return;
  }
  i2c_error = I2Cwrite(ThreeBytesCommds, SYMM, chan, val); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }  
}

void changeI2Caddress_Callback(SerialCommands* sender)
{
  int i2c_error = -1;     
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Address is missing ");
    return;
  }
  int val = atoi(val_str);
  if (val < ADDR_LOW_LIMIT || val > ADDR_UPP_LIMIT)
  {
    sender->GetSerial()->println("ERROR! ADDR outside range ");
    return;
  }
  val_str = sender->Next();
  if (val_str == NULL)
  {
	  sender->GetSerial()->println("ERROR! ADDR program flag 0/1 is missing ");
	  return;
  }
  int program_addr = atoi(val_str);
  if (program_addr != 0 && program_addr != 1)
  {
	  sender->GetSerial()->println("ERROR! ADDR program flag outside range ");
	  return;
  } 
  if (program_addr == 1){
	  i2c_error = I2Cwrite(ThreeBytesCommds, ADDR, -1, val);
	  if (i2c_error != 0){
		  sender->GetSerial()->print("I2C error = ");
		  sender->GetSerial()->println(i2c_error);
		  }else{
		  NSDuino_address = val; // Update I2C address of current slave that will be communicated with
		  sender->GetSerial()->print("Current I2C slave address = ");
		  sender->GetSerial()->println(NSDuino_address, DEC);
	  }	  
  }else{
	  //Only change I2C address of current slave device. Do not program the I2C address
	  NSDuino_address = val;
	  sender->GetSerial()->print("Current I2C slave address = ");
	  sender->GetSerial()->println(NSDuino_address, DEC);
  } 
}

void resetNeuroStimDuino_Callback(SerialCommands* sender)
{
  int i2c_error = -1;     
  i2c_error = I2Cwrite(OneByteCommds, RSET); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  } 
}

void setEmergencyOFF_Callback(SerialCommands* sender)
{
  int i2c_error = -1;     
  i2c_error = I2Cwrite(OneByteCommds, EOFF); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  } 
}

void startStimulation_Callback(SerialCommands* sender)
{
  int i2c_error = -1; 
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Stim. duration is missing ");
    return;
  }
  int stimdur = atoi(val_str);
  if (stimdur < 0 || stimdur > 255)
  {
    sender->GetSerial()->println("ERROR! Stim. duration outside range ");
    return;
  }

  val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Continuous stim. flag is missing ");
    return;
  }
  int stimflag = atoi(val_str);
  if (stimflag != 0 && stimflag != 1)
  {
    sender->GetSerial()->println("ERROR! Continuous stim. flag outside range ");
    return;
  }
  
  i2c_error = I2Cwrite(FourBytesCommds, STIM, chan, stimdur, stimflag); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }  
}

void stopStimulation_Callback(SerialCommands* sender)
{
  int i2c_error = -1;     
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  i2c_error = I2Cwrite(TwoBytesCommds, STOP, chan); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  } 
}

void enableChannel_Callback(SerialCommands* sender)
{
  int i2c_error = -1; 
  char* chan_str = sender->Next();
  if (chan_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Channel # is missing ");
    return;
  }
  int chan = atoi(chan_str);
  
  char* val_str = sender->Next();
  if (val_str == NULL)
  {
    sender->GetSerial()->println("ERROR! Value is missing ");
    return;
  }
  int val = atoi(val_str);
  if (val != 0 && val != 1)
  {
    sender->GetSerial()->println("ERROR! Value outside range ");
    return;
  }
  i2c_error = I2Cwrite(ThreeBytesCommds, ENAB, chan, val); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }  
}

void startCurrentSampling_Callback(SerialCommands* sender)
{
	 int i2c_error = -1;
	 char* chan_str = sender->Next();
	 if (chan_str == NULL)
	 {
		 sender->GetSerial()->println("ERROR! Channel # is missing ");
		 return;
	 }
	 int chan = atoi(chan_str);
	 i2c_error = I2Cwrite(TwoBytesCommds, SAMP, chan);
	 if (i2c_error != 0){
		 sender->GetSerial()->print("I2C error = ");
		 sender->GetSerial()->println(i2c_error);
	 }
	
}

/*
NeuroStimDuino::NeuroStimDuino(uint8_t addr = default_i2c_addr)
{  
  _i2caddr = addr; // default address = 0x1A 
}
*/
