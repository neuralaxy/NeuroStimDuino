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
volatile bool Pulse_Channel_1 = false, Pulse_Channel_2 = false; 

int I2Cwrite(int numbytes, uint8_t commd, uint8_t channel_no = -1, uint8_t data_lsb = -1, uint8_t data_msb = -1){
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

int I2Cread(int num_bytes){
	int error = -1;
	int reg_value = 0;
	int count = 1;
	NSWire.requestFrom(NSDuino_address, num_bytes); //Request num_bytes from NSDuino
	while(NSWire.available()){
		if (count == 1){
			reg_value = NSWire.read();
			count++;
		}
		else if(count == 2)
		{
			reg_value = (NSWire.read()<<8) + reg_value; 
		}		
	}	
	return (reg_value);
}

char* I2Cread_byte_array(int num_bytes){
	int error = -1;
	static char data_array[20];	//Size must be equal to num_bytes 
	int count = 0;
	int samp_val; 
		 
	NSWire.requestFrom(NSDuino_address, num_bytes); //Request num_bytes from NSDuino	
	while(NSWire.available()){
		data_array[count] = NSWire.read();		
		count++;		
		if(count >= num_bytes){
			//terminate the loop using break statement
			break;
		}		
	}	
	return data_array;
}

enum I2Ccommand str2enum (const char* reg)
{
	if(!strcmp(reg, "AMPL"))
		return AMPL;
	else if(!strcmp(reg, "FREQ"))
		return FREQ;
	else if(!strcmp(reg, "DURN"))
		return DURN;
	else if(!strcmp(reg, "IDLY"))
		return IDLY;
	else if(!strcmp(reg, "DELY"))
		return DELY;
	else if(!strcmp(reg, "SYMM"))
		return SYMM;
	else if(!strcmp(reg, "ENAB"))
		return ENAB;
	else if(!strcmp(reg, "RAMP"))
		return RAMP;
	else
		return ERR; // reg not found
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
  float valf = atof(val_str); //int val = atoi(val_str);
  if (valf < AMPL_LOW_LIMIT || valf > AMPL_UPP_LIMIT)
  {
    sender->GetSerial()->println("ERROR! Value outside range ");
    return;
  }
  //Convert stim amplitude from milliAmps to digital potentiometer counts (0-127)
  int val = round(valf/current_conversion_ratio);
  i2c_error = I2Cwrite(ThreeBytesCommds, AMPL, chan, val); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);   
  }
  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  val = I2Cread(1);	
  //Convert stim amplitude from digital potentiometer count to milliAmps
  valf = val*current_conversion_ratio; //val = ceil(val*current_conversion_ratio);
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  sender->GetSerial()->print(", amplitude is set to ");
  sender->GetSerial()->print(valf,3);					
  sender->GetSerial()->println(" mA");
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

  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  val = I2Cread(1);	
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  sender->GetSerial()->print(", frequency is set to ");
  sender->GetSerial()->print(val);	
  sender->GetSerial()->println(" Hz");  
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
  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  val = I2Cread(2);	
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  sender->GetSerial()->print(", pulse duration is set to ");
  sender->GetSerial()->print(val);					  
  sender->GetSerial()->println(" uSec");
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
  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  val = I2Cread(1);	
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  sender->GetSerial()->print(", inter-phase duration is set to ");
  sender->GetSerial()->print(val);	
  sender->GetSerial()->println(" uSec");
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

  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  val = I2Cread(1);	
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  sender->GetSerial()->print(", start delay is set to ");
  sender->GetSerial()->print(val);	
  sender->GetSerial()->println(" mSec");
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

  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  val = I2Cread(1);	
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  sender->GetSerial()->print(", pulse shape is set to ");
  if (val == 1){
	sender->GetSerial()->println("symmetrical");  
  }else{
	sender->GetSerial()->println("asymmetrical");  
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
		  sender->GetSerial()->print("Updated I2C slave address = ");
		  sender->GetSerial()->println(NSDuino_address, DEC);
	  }	  
  }else{
	  //Only change I2C address of current slave device. Do not program the I2C address
	  NSDuino_address = val;
	  sender->GetSerial()->print("Now connected to I2C slave address = ");
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
  sender->GetSerial()->println(" ");
  sender->GetSerial()->println(" ");
  sender->GetSerial()->println("NeuroSimDuino reset to default setting via Software");
  sender->GetSerial()->println(" ");
  delay(50);
  print_Channel_Parameters(1);  
  sender->GetSerial()->println(" "); 
  print_Channel_Parameters(2); 
}

void setEmergencyOFF_Callback(SerialCommands* sender)
{
  int i2c_error = -1;     
  i2c_error = I2Cwrite(OneByteCommds, EOFF); 
  if (i2c_error != 0){
    sender->GetSerial()->print("I2C error = ");
    sender->GetSerial()->println(i2c_error);    
  } 
  sender->GetSerial()->println(" ");
  sender->GetSerial()->println(" ");
  sender->GetSerial()->println("Emergency OFF called via Software");
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
  
  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  int val = I2Cread(2);	
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  sender->GetSerial()->print(" will generate ");
  sender->GetSerial()->print(val);	
  sender->GetSerial()->println(" stim. pulses");
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

  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  int val = I2Cread(1);	
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  if (val == 0){
	  sender->GetSerial()->println(" has stopped");
  }else if (val == 255){
	  sender->GetSerial()->println(" has not stopped");
  }else{
	  sender->GetSerial()->println(" malfunctioned, check!!");
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
  
  delay(3); // Required for dsPIC to finish processing I2C write command
  // Now read the register value
  val = I2Cread(1);	
  sender->GetSerial()->print("Channel ");
  sender->GetSerial()->print(chan);
  if (val == 1){
	sender->GetSerial()->println(" is enabled");  
  }else{
	sender->GetSerial()->println(" is disabled");  
  }  
}

void startCurrentSampling_Callback(SerialCommands* sender)
{
	 int i2c_error = -1;
	 char *samples_array; 
	 int i, samp_val = 0;
	 char buff2[10];
	 
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
	 delay(1000); // wait for the samples to captured into array and be ready for transmission
	 for(i=1;i<=10;i++){
		 // Read 10 integers x 10 times to read 100 samples from NSDuino 
		 // Reason: I2C requestFrom on works for reading upto 32 bytes
		 samples_array = I2Cread_byte_array(20); // i.e. 10 integer samples
		 for (int n = 0; n < 20; n=n+2){
			 samp_val = ((samples_array[n+1]<<8) | samples_array[n]);
			 if ((samp_val & 0x8000) == 0){
				 //positive number
				 sender->GetSerial()->print(samp_val);
			 }else{	
				 //negative number
				 sender->GetSerial()->print(samp_val-65536);
			 }
			 
			 sender->GetSerial()->print(", ");
			 //sender->GetSerial()->print(buff2);
		 }		 
	 }
	 sender->GetSerial()->println("");	 
}

void readRegister_Callback(SerialCommands* sender)
{
	int i2c_error = -1;
	int reg_val = 0;
	float reg_valf = 0;
	char* chan_str = sender->Next();
	if (chan_str == NULL)
	{
		sender->GetSerial()->println("ERROR! Channel # is missing");
		return;
	}
	int chan = atoi(chan_str);
	
	char* cmd_str = sender->Next();
	if (cmd_str == NULL)
	{
		sender->GetSerial()->println("ERROR! Register name is missing");
		return;
	}
	// Convert string to I2C Register
	enum I2Ccommand reg_addr = str2enum(cmd_str);
	if (reg_addr == 0){
		sender->GetSerial()->println("ERROR! Register not found");		
		return;
	}
	i2c_error = I2Cwrite(ThreeBytesCommds, READ, chan, reg_addr);
	if (i2c_error != 0){
		sender->GetSerial()->print("I2C error = ");
		sender->GetSerial()->println(i2c_error);
	}
	delay(3); // Required for dsPIC to finish processing I2C write command
	// Now read the register value
	if (reg_addr == DURN){
		reg_val = I2Cread(2);	
	}else{
		reg_val = I2Cread(1);
		if (reg_addr == AMPL){
			//Convert stim amplitude from digital potentiometer count to milliAmps
			reg_valf = reg_val*current_conversion_ratio;
		}
	}	
	sender->GetSerial()->print("Channel ");
	sender->GetSerial()->print(chan);
	sender->GetSerial()->print(", ");
	sender->GetSerial()->print(cmd_str);
	sender->GetSerial()->print(" = ");
	if (reg_addr == AMPL){
		sender->GetSerial()->println(reg_valf,3);					
	}else {
		sender->GetSerial()->println(reg_val);					
	}
}

void enableRamping_Callback(SerialCommands* sender)
{
	 int i2c_error = -1; 
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
	 i2c_error = I2Cwrite(ThreeBytesCommds, RAMP, -1, val); 
	 if (i2c_error != 0){
		sender->GetSerial()->print("I2C error = ");
		sender->GetSerial()->println(i2c_error);   
	 }
	 
	 delay(3); // Required for dsPIC to finish processing I2C write command
	 // Now read the register value
	 val = I2Cread(1);	
	 sender->GetSerial()->print("Ramping");	 
	 if (val == 0){
		 sender->GetSerial()->println(" is disabled on channels 1 & 2");
	 }else if (val == 1){
		 sender->GetSerial()->println(" is enabled on channels 1 & 2");
	 }else{
		 sender->GetSerial()->println(" has malfunctioned, check!!");		 
	 }   
}

void print_Channel_Parameters(int chan_no)
{
    uint8_t reg_addr;
    int reg_val = 0, i2c_err = 0;
	float reg_valf = 0;
    char buff[50];

    sprintf(buff,"Channel %d Settings are: ", chan_no);
    Serial.println(buff); 
    
    for (reg_addr = AMPL; reg_addr <= SYMM; reg_addr++)
    {
     i2c_err = I2Cwrite(ThreeBytesCommds, READ, chan_no, reg_addr);
     if (i2c_err != 0){
        Serial.print("I2C error = ");
        Serial.println(i2c_err);
        }
	 delay(3); // Required for dsPIC to finish processing I2C write command
     // Now read the register value
     if (reg_addr == DURN){
       reg_val = I2Cread(2); 
     }else{
       reg_val = I2Cread(1); 
     }
      switch(reg_addr){
       case AMPL:				  				  
				  //Convert stim amplitude from digital potentiometer count to milliAmps
			      reg_valf = reg_val*current_conversion_ratio;
                  sprintf(buff,"Amplitude (mA): .............................%.2f", reg_valf);                          
                  Serial.println(buff); 
                  break;                                           
       case FREQ:
                  sprintf(buff,"Frequency (Hz): .............................%d", reg_val);                          
                  Serial.println(buff); 
                  break;                                                      
       case IDLY:
                  sprintf(buff,"Inter-phase Delay (uSec): ...................%d", reg_val);                          
                  Serial.println(buff); 
                  break;                                                      
       case DURN:
                  sprintf(buff,"Pulse-Duration (uSec): ......................%d", reg_val);                          
                  Serial.println(buff); 
                  break;                                                      
       case DELY:
                  sprintf(buff,"Start delay (mSec): .........................%d", reg_val);                          
                  Serial.println(buff);  
                  break;                                                      
       case SYMM:
                  if (reg_val == 1){
                    Serial.println("Pulse shape: Symmetrical");                     
                  }else{
                    Serial.println("Pulse shape: Asymmetrical");                     
                  }                  
                  break;                                           
       default:
                  break;           
      }
	  delay(3); // Required for dsPIC to finish processing I2C write command
    }
    // Check if Channel is enabled/disabled    
    i2c_err = I2Cwrite(ThreeBytesCommds, READ, chan_no, ENAB);
	delay(3); // Required for dsPIC to finish processing I2C write command
    reg_val = I2Cread(1); 
    if (reg_val == 1){
      Serial.println("Status: Enabled");                     
    }else{
      Serial.println("Status: Disabled");                     
    }         
}

void startPulse_Callback(SerialCommands* sender)
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
  switch(chan){
	  case 1:
			 if (val == 1){
				Pulse_Channel_1 = true;
				sender->GetSerial()->println("Pulse stim is ON on Channel 1");
			  }else{
				Pulse_Channel_1 = false;
				sender->GetSerial()->println("Pulse stim is OFF on Channel 1");
			  }
			  break;
	  case 2:
			if (val == 1){
				Pulse_Channel_2 = true;
				sender->GetSerial()->println("Pulse stim is ON on Channel 2");
			  }else{
				Pulse_Channel_2 = false;
				sender->GetSerial()->println("Pulse stim is OFF on Channel 2");
			  }
			  break;
  }   
}

void startStimulationPulse(int chan_no)
{
  int i2c_error = -1;
  i2c_error = I2Cwrite(TwoBytesCommds, SAMP, chan_no);
  if (i2c_error != 0){
    Serial.print("I2C error = ");
    Serial.println(i2c_error);
  }
}

/*
NeuroStimDuino::NeuroStimDuino(uint8_t addr = default_i2c_addr)
{  
  _i2caddr = addr; // default address = 0x1A 
}
*/
