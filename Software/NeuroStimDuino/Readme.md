## I2C Library for NeuroStimDuino
NeuroStimDuino can be controlled by an external microcontroller such as Arduino Due or Mega using the I2C interface, with NeuroStimDuino acting as I2C slave device (default address = 0x1A). Pull-up resistors for I2C communication are already placed onboard NeuroStimDuino and hence additional pull-up resistors are not required. A list of Opcode-style commands can be used to configure different parameters of stimulation waveform such as frequency, amplitude, etc. Each I2C message should have following structure:

- Byte 0 indicates the number of bytes that will be transferred
- Byte 1 is the Opcode command from the instruction set below for the stimulation parameter that will be modified
- Byte 2 is the channel number (1 or 2) that will written to or read from
- Byte 3 is the LSB byte of the value written
- Byte 4 is the MSB byte of the value written

## Instruction Set 
| **Instruction** | **Address** | **Description** | **Acceptable Range** | 
|----------------|--------------|-----------------|------------------------|
|  AMPL |  0x01 |   Change stimulation amplitude | 0 - 25, Unit: milliAmps |
|  FREQ  |  0x02 |   Change stimulation frequency | 1 - 100, Unit:  Hz |
|  DURN |  0x03 |  Change pulse-duration of the cathodic pulse. | 0 - 2000, Unit: uSec. |
|  IDLY | 0x04 | Change inter-phase delay | 0 - 255, Unit: uSec. |
|  DELY | 0x05 | Delay to the start of the stimulation pulse in each cycle | 0 - 1/(stimulation frequency), Unit: mSec.|
|  SYMM | 0x06 | Generate symmetrical or asymmetrical stim waveform <br /> (*Note: Make sure to change jumpers J5 and J6 settings on the board*) | 1 = Symm.; 0 = Asymm. |
|  ADDR  | 0x07 | Change 7-bit I2C address | Allowed addresses = 0x08 - 0x77; <br /> program/switch_only flag = 1/0|
|  SAMP  | 0x08 | Sample current on channel 1 or 2 | - |
|  READ  | 0x09 | Read value of register for specified channel <br /> Usage: READ Channel# Address <br /> (e.g. To read amplitude of Channel 1, use "READ 1 AMPL")   | - |
|  RSET  | 0x0A | Reset NeuroStimDuino to default register values | - |
|  EOFF | 0x0F | Generate Emergency Off condition | - |
|  STIM | 0x10 | Start stimulation on channel 1 or 2 for finite duration or stimulate indefinitely <br /> Usage: STIM Channel# Seconds# Stim Indefinitely <br /> (e.g. To stimulate on Channel 1 for 5 seconds, use "STIM 1 5 0") | 0 - 255 Sec; <br /> Stim indefinitely flag = 1/0| 
|  STOP | 0x12 | Stop stimulation on channel 1 or 2 | - | 
|  ENAB | 0x14 | Enable or disable a stim. channel | 1 = Enable; 0 = Disable | 
|  WAIT  | 0x20 | Idle | - |

## Arduino Example
An example sketch is included: NeuroStimDuino_SerialControl.ino to send I2C commands to NeuroStimDuino over the serial port. Set the baudrate at 115200 bps and add "CR+LF" at the end of each command. For using this library, you will need to install SerialCommands library available from Pedro Pereira at https://github.com/ppedro74/Arduino-SerialCommands. The library will automatically switch between SDA, SCL pins (Wire) for Arduino MEGA256 and SDA1,SCL1 pins (Wire1) for Arduino Due. 

**Some examples of Commands sent over COM port** <br />
* AMPL 1 15     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; // Set Channel 1 intensity at 15mA
* FREQ 1 20     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; // Set Channel 1 frequency at 20Hz
* DURN 1 500    &nbsp;&nbsp;&nbsp;&nbsp; // Set Channel 1 pulse duration at 500 uSec
* STIM 1 20 0   &nbsp;&nbsp;&nbsp;&nbsp; // Turn on Channel 1 for 20 seconds
* STIM 1 20 1   &nbsp;&nbsp;&nbsp;&nbsp; // Turn on Channel 1 indefinitely

## Measure Stimulation Output
Using an on-baord current sense amplifier you can measure the stimulation output as shown below - 

![Symmetrical_Asymm_Outputs_500us](https://user-images.githubusercontent.com/80208904/115607178-a7525000-a2b2-11eb-8d5b-1cbbaa320363.png)
Signal traces were obtained above by averaging 10 trials for each amplitude setting, in order to minimize background noise. 
