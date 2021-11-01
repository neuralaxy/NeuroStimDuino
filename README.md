# NeuroStimDuino
An open source neurostimulator for students, researchers, and hobbyists interested in neuroscience

# Getting Started Tutorial
* To get started, first and foremost familiarize yourself with NeuroStimDuino features, [board layout and pin diagram](https://www.crowdsupply.com/neuralaxy/neurostimduino)

* If you just ordered the NeuroStimDuino board (without the accessory pack), then you will have received the following components.  
![CS_NEUROSTIM_01](https://user-images.githubusercontent.com/80208904/139623705-254dc788-df37-42cd-ad99-0f92750ce4e5.png)

* If you have ordered the accessory pack as well, then you will also receive hydrogel electrodes and extra cables (seen below). The hydrogel electrodes are required if you plan to stimulate a fake muscle (such as a [Zucchini](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/tips-on-electrode-placement)) or for your research.
![CS_NEUROSTIM_02](https://user-images.githubusercontent.com/80208904/139623724-8c97063e-d2cb-42e4-9db0-988889267173.png)

* Finally, before you begin, please remember to review the **safety guidelines** listed at the [end](https://github.com/neuralaxy/NeuroStimDuino/blob/main/README.md#important-safety-guidelines) of this tutorial and follow the recommendations mentioned therein.  

## Hardware Setup
* The battery holder included requires two 18650 Li-ion batteries (not included) to be connected in series. The battery holder allows the batteries to be oriented in series or parallel connection. Unfortunately, the battery holders we shipped only have Chinese labels to indicate the series or parallel oritentation. Make sure you connect the batteries in series (as shown below) and not in parallel. Alternately, you can power NeuroStimDuino using your own battery source between 6-20V. We do not recommend powering NeuroStimDuino using an AC-DC adapter. However, if you must use an adapter, then you need an adapter with atleast 2 Amps output rating

![battery_holder](https://user-images.githubusercontent.com/80208904/139629701-303a772e-c762-4384-8296-19b42c08dd85.jpg)

* For connecting the stimulation cables, you will have to remove the cable shroud or sheath cover as shown below and then plug it into the 0.7mm DC Jack.

![20200817_080616](https://user-images.githubusercontent.com/80208904/139632181-44e05390-6c77-4859-948c-c67302b4d6e7.jpg)

* To control NeuroStimDuino using an Arduino Due or Mega, stack NeuroStimDuino on top of the Arduino board by aligning the hearder pins, as shown below.

![20210426_080326](https://user-images.githubusercontent.com/80208904/139623995-dda8e7de-36c7-4d57-898c-be94603645a9.jpg)

## Software Setup
* Next, you will need to download the latest version of our [Arduino library](https://github.com/neuralaxy/NeuroStimDuino/tree/main/Software/NeuroStimDuino). Read the full update [here](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/arduino-library-released) and remember to install the [SerialCommands library] (https://github.com/ppedro74/Arduino-SerialCommands) from Pedro Pereira. Next, upload the example code NeuroStimDuino_SerialControl.ino onto the Arduino.  

* To write instructions to control NeuroStimDuino, you will use the Serial terminal that is in-built Arduino IDE (Tools > Serial Monitor). Once you open the Serial Monitor, set the baudrate to 115200 bps and add "CR+LF" at the end of each command. When you power either the Arduino or NeuroStimDuino board via the DC plug, you will see test LEDs 1 & 2 flashing on the NeuroStimDuino board. These LEDs are flashing with the default stimulation frequency of 10Hz. Now you can enter some example commands into the Serial Monitor, one command at a time and then press enter.

* After uploading the example code NeuroStimDuino_SerialControl.ino and making the connections as above, when you first open the Serial terminal you will see the following welcome message. This mean that a successful connection between Arduino and NeuroStimDuino has been established. It will also give a readout of the current stimulation parameters for each channel. 

![Welcome_message](https://user-images.githubusercontent.com/80208904/139626723-b4dfde3e-464e-4d69-8721-ae6790aae649.png)

* You can type in some example commands into Serial Monitor and change the settings for the stimulator (type one statement at a time). Also, the commands are case-sensitive, so type it in all caps. 
```
FREQ 1 20        // This will set Channel 1's frequency at 20Hz. You will see the TEST LED1 blink at this new frequency
FREQ 2 50        // This will set Channel 2's frequency at 50Hz. You will see the TEST LED1 blink at this new frequency
AMPL 1 15        // Set Channel 1 intensity 15mA
DURN 1 500       // Set Channel 1 pulse duration at 500 uSec
ENAB 1 1         // This will enable Channel 1 of the stimulation. By default, both channels are enabled at reset
STIM 1 20 0      // This command will turn on stimulation on Channel 1 for 20 seconds. You will see a Red-Green LED (labelled Ch. 1) blink  very briefly (for 500us), every time the stimulation is generated (i.e, 20  times/sec)
```

## Moving forward

* This completes this introductory tutorial. From here on, if you would like to connect cables and electrodes and actuatlly stimulate a fake Zucchini muscle, then follow the setup instructions shown in our [April Update](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/tips-on-electrode-placement) published on CrowdSupply. 

* If you want connect multiple NeuroStimDuinos together and control them using a single Arduino, then look up the steps and instructions posted [earlier](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/controlling-multiple-neurostimduinos-on-a-single-i2c-bus). 
<br />**One important change** required in hardware when stacking 2 or more NeuroStimDuinos is to disconnect pins 5 & 6 on JP3. Another way to identify these pins is that they normally connect with PWM pins 9 and 10 on Arduino Due or Mega. The reason for this is that these pins can also be used to drive TEST_LED1 and TEST_LED2 of NeuroStimDuino. Therefore, if you connect them together on 2 NeuroStimDuinos, the LEDs will not function properly and can be confusing. Hence, to avoid any cross-talk, its best to fold these pins out of the way as shown below.

![20211023_150523](https://user-images.githubusercontent.com/80208904/139626983-ed9ccd4f-f83d-4f08-85e3-67781891e534.jpg)


* We hope you enjoy tinkering with NeuroStimDuino and use it for all your neuroscience experiments. If you face any issue with the device or are stuck anytime, feel free to write to us on contactus@neuralaxy.com or submit an issue here. 

## Important safety guidelines:
* NeuroStimDuino is not an FDA approved product for clinical use and it is intended for **RESEARCH PURPOSES ONLY**. 
* NeuroStimDuino is not intended for human use or animal use, without receiving prior approval from a local ethics committee such as Institutional Review Boards and Institutional Animal Care and Use Committees  
* Under no circumstances, should the stimulation electrodes be placed across the chest or close to the heart
* NeuroStimDuino requires an external DC input voltage from 6-20V to operate. We **strongly** recommend to use an external battery for this and avoid the use of an AC adapter or anything that's plugged into the AC mains
