/*--------------------------------------------------------------------
NeuroStimDuino_SerilaControl.ino - Example code to be used with NeuroStimDuino Library
Tested with Arduino MEGA256 board. 
Created by: Neuralaxy LLC
Created on: April 1st, 2021.
Contact: contactus[at]neuralaxy.com
Dependencies: SerialCommands library available from Pedro Pereira at https://github.com/ppedro74/Arduino-SerialCommands

This library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/
#include <Arduino.h>
#include <SerialCommands.h>
#include <Wire.h>
#include <NeuroStimDuino.h>

//Creates SerialCommands object attached to Serial
//working buffer = serial_command_buffer_
//command delimeter: Cr & Lf
//argument delimeter: SPACE
char serial_command_buffer_[50];
SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

//Note: Commands are case sensitive
SerialCommand cmd_led_on_("ON", cmd_led_on);
SerialCommand cmd_led_off_("OFF", cmd_led_off);
SerialCommand setAmplitude("AMPL", setAmplitude_Callback);                      // e.g. AMPL 1 50     // Unit: Count from 0-127
SerialCommand setFrequency("FREQ", setFrequency_Callback);                      // e.g. FREQ 1 20(Hz) // Unit: Hz
SerialCommand setPulseDuration("DURN", setPulseDuration_Callback);              // e.g. DURN 1 1550   //i.e. 1550us // Unit: uSec.
SerialCommand setInterPhaseDelay("IDLY", setInterPhaseDelay_Callback);          // e.g. IDLY 1 200    // Unit: uSec.
SerialCommand setStartDelay("DELY", setStartDelay_Callback);                    // e.g. DELY 1 5      // Unit: mSec.
SerialCommand setWaveformSymmetry("SYMM", setWaveformSymmetry_Callback);        // e.g. SYMM 1 1      //Also check the Jumper setting
SerialCommand changeI2Caddress("ADDR", changeI2Caddress_Callback);              // e.g. ADDR 26       // Reserved addresses: 0 - 7, 120 - 127
SerialCommand resetNeuroStimDuino("RSET", resetNeuroStimDuino_Callback);        // e.g. RSET
SerialCommand startStimulation("STIM", startStimulation_Callback);              // e.g. STIM 1 20 0   // Turn on Channel 1 for 20 seconds
SerialCommand stopStimulation("STOP", stopStimulation_Callback);                // e.g. STOP 2        // Stop channel 2
SerialCommand enableChannel("ENAB", enableChannel_Callback);                    // e.g. ENAB 1 1      // will enable channel 1; ENAB 1 0 will disable channel 1
SerialCommand setEmergencyOFF("EOFF", setEmergencyOFF_Callback);                // e.g. EOFF
SerialCommand startCurrentSampling("SAMP", startCurrentSampling_Callback);		  // e.g. SAMP 1        // sample current on channel 1 @20kHz
SerialCommand readRegister("READ", readRegister_Callback);          			      // e.g. READ 1 AMPL	  // Read current value of channel 1 amplitude
SerialCommand enableRamping("RAMP", enableRamping_Callback);                    // e.g. RAMP 1        // 1 - Start ramp Up & Down stimulatin intensity (i.e. pulse-duration) during start & end of stimulation
                                                                                                      // 0 - Stop ramping on both channels

/* Pending
SerialCommand setIdle("WAIT", setIdle_Callback);
SerialCommand commandsMenu("MENU", commandsMenu_Callback);
One key commands
*/

void setup() 
{
  Serial.begin(115200);
  NSWire.begin();
  delay(50); // Add delay for NeuroStimDuino to initialize 

  //Configure the LED for output and sets the intial state to off
  pinMode(ArduinoLedPin, OUTPUT);
  digitalWrite(ArduinoLedPin, LOW);

  serial_commands_.SetDefaultHandler(cmd_unrecognized);
  serial_commands_.AddCommand(&cmd_led_on_);
  serial_commands_.AddCommand(&cmd_led_off_);
  serial_commands_.AddCommand(&setAmplitude);   
  serial_commands_.AddCommand(&setFrequency);  
  serial_commands_.AddCommand(&setPulseDuration);  
  serial_commands_.AddCommand(&setInterPhaseDelay);
  serial_commands_.AddCommand(&setStartDelay);
  serial_commands_.AddCommand(&setWaveformSymmetry);
  serial_commands_.AddCommand(&changeI2Caddress);  
  serial_commands_.AddCommand(&resetNeuroStimDuino);  
  serial_commands_.AddCommand(&startStimulation);
  serial_commands_.AddCommand(&stopStimulation);
  serial_commands_.AddCommand(&enableChannel);
  serial_commands_.AddCommand(&setEmergencyOFF);
  serial_commands_.AddCommand(&startCurrentSampling);
  serial_commands_.AddCommand(&readRegister);
  serial_commands_.AddCommand(&enableRamping);

  //serial_commands_.AddCommand(&setIdle); 
  //serial_commands_.AddCommand(&commandsMenu);
    
  Serial.println("*****   Welcome to NeuroStimDuino - by Neuralaxy   *****"); 
  print_Channel_Parameters(1);
  Serial.println(" "); 
  print_Channel_Parameters(2); 
}

void loop() 
{
  serial_commands_.ReadSerial();
}
