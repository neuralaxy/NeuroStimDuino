# NeuroStimDuino
An open source neurostimulator for students, researchers, and hobbyists interested in neuroscience

## Update for 2025 - NeuroStimDuino v3.0  launched!! :boom: :boom: 💫
Hello NeuroStim Enthusiats!! Its been a while....over the last year, we have been busy revising the design of NeuroStimDuino. The main reason for the modifying NSDuino2.1 was that one of its crucial components, the high voltage DC-DC converter NMT0572SC from Murata has now became obsolete and out of circulation. Sadly, there was no alternative component that could take its place and we spend months trying to find a replacement. 

Finally we had a breakthrough, and we found another DC-DC converter IH0524 from XP Power. We had to redesign the board slightly to incorporate this Converter. But the *downside* is that the new converter only allows a compliance voltage of 35V instead of 60V available in the previous versions. How will this effect your experiments? Well, it means that now you will have to take extra effort in minimizing the skin impedance to deliver higher currents. But we have tried stimulating our arm muscles using this newer version, and it is just as effective as before. And we highly recommend using hydrogel electrodes, as we have suggested before in the [Zucchini](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/tips-on-electrode-placement) experiment. 

On the *upside*, while we were redesigning the board, we replaced the manual High Voltage slide switch with an electronic switch (see below). So now, you don't have to worry about turning ON the high voltage switch before activating the stimulator. Whenever you give the STIM command, the microcontroller will automatically turn ON the high voltage supply and once the stimulation is turned off, the high voltage supply is automatically turned off. Another benefit of adding the electronic switch is that it will minimize power consumption, since the high-voltage supply doesn't need to be always ON. Now that's smart😎.

<img width="3182" height="1609" alt="Replaced_Slide_Switch_with_SSR" src="https://github.com/user-attachments/assets/0fea120a-9c40-4458-85fc-b0134da14ceb" />

All the other features from NSDuino2.1, like ramping stimulation intensity, touchproof conenctors, etc. have been carried over to NSDuino3.0. Late in September 2025, we launched NeuroStimDuino v3.0 abd shipped out the first batch to our distributor Mouser. A big shout out for 📢 Mouser for consistently supporting us over the years. Some of you might have already received the newer version. Try it and let us know how it goes.  

## Important update for 2023
In January 2023 we have launched NeuroStimDuino v2.1, which comes with additional features and improved functionality than its predecessor NeuroStimDuino v1.0. 

![NeuroStimDuino_v2 1](https://user-images.githubusercontent.com/80208904/212980615-347159de-76d2-47a9-8e5e-a4e41399fbee.jpg)


Here is a list of important upgrades for NeuroStimDuino v2.1 (NSDuino2.1): 
* Choosing between symmetrical and asymmetrical waveform shapes is now done entirely in software. In NeuroStimDuino v1.0, to switch from symmetrical to asymmetrical waveform, you had to change the jumper (J5, J6) as well as call the `SYMM` command. In NSDuino2.1, its handled entirely by software and the jumpers have been removed. In the default asymmetrical output mode, the anodic phase's amplitude is half of cathodic phase's amplitude and the duration of anodic phase is double that of the cathodic phase (to ensure charge balance). This ratio can be changed by modifying the firmware.

* NSDuino2.1's I<sup>2</sup>C address is now stored on EEPROM. So, whenever you assign a new I<sup>2</sup>C address to NSDuino2.1 using the `ADDR` command, it will get stored permanently and will be recalled at power-on reset. This is especially helpful when communicating with multiple NeuroStimDuinos. In NeuroStimDuino v1.0, the address would reset to the default value (0x1A) everytime the board was turned on and you would have to painstakingly re-assign the new addresses each time. With NSDuino2.1 the new address only needs to be set once and it will store it on-board its EEPROM.

* Many of NeuroStimDuino v1.0 users (who are acutally scientists!!) have asked whether it is possible to ramp up and down the stimulation intensity? Ramping is used in neurostimulation research to avoid painful sensations allow smoother transitions between movements when applying electrical stimulation. With NSDuino2.1, we have also expanded our software (Arduino) library and now provide the option to Ramp Up and Down the stimulation intentsity. When you enable ramping by using the command `RAMP 1`, the stimulation intensity will gradually increase from 0 to the desired level over a 1 second interval. At the end of stimulation, the stimulation intensity will gradually decrease to zero over a 1 second interval. There are some caveats to using ramping: 
  * In the scientific literature, ramping is usually done by increasing/decreasing the current amplitude. While this is doable, by changing the counts of our on-board digital potentiometer, it would require multiple SPI calls between the MCU and potentiometer which will introduce delays. Instead, we are performing ramping by gradually increasing the stimulation pulse-width. This is handled entirely by the MCU and hence it is more accurate. Additionally, it has the same effect as changing the stimulation current, so it won't affect the outcomes of your experiments. Refer to the below gif to understand how ramping essentially works:
  
  ![NSDuino_Ramping_gif](https://user-images.githubusercontent.com/80208904/210726288-6426e95b-322c-423b-a19b-45e27edf90cd.gif)
  
  * Currently, ramping only works for finite stimulation intervals. This means the `Stim Indefinitely` flag of the `STIM` command must be set to `FALSE`. An example code snippet is as follows:
  
  ```
  RAMP 1          // Turn ON ramping on channels 1 & 2
  STIM 1 5 0      // Stimulate on channel 1 for 5 sec, stim indefinitely flag = 0
  RAMP 0          // Turn OFF ramping on channels 1 & 2
  ```
  * Ramping can also be deployed in NeuroStimDuino v1.0. This would require a firmware update. We will post an upgraded firmware for NeuroStimDuino v1.0 on our repository in a few weeks time. Feel free to reach out to us for any queries regarding ramping

* Additional minor modifications:
  * We removed the bi-color(red/green) LEDs to indicate when channels 1 and 2 are stimulating, with regular monocolor LEDs. These still function in the same way
  
  ![LEDs_changed](https://user-images.githubusercontent.com/80208904/210715605-54008530-ebbe-4a52-90f9-221777023e54.png)

  * We changed the onboard cable connectors for Channels 1 & 2 with new ones that are compatible with our stimulation cables and you no longer need to remove the cable shroud or sheath cover. This will hopefully help reduce the hassle during setup and the touchproof connectors will offer additional safety during use.
  
![connector_with_shroud](https://user-images.githubusercontent.com/80208904/212980710-d8f2f829-21ef-4c3a-a5dd-4c0e876f8dc5.png)


# Getting Started Tutorial for NeuroStimDuino v2.1
**_Important:_** For instructions on getting started with NeuroStimDuino v1.0, please click [here](https://github.com/neuralaxy/NeuroStimDuino/blob/main/NeuroStimDuino%20v1.0%20Getting%20Started.md#getting-started-tutorial-for-neurostimduino-v10)

* To get started, first and foremost familiarize yourself with NeuroStimDuino pinout and board layout below:

![NeuroStimDuino_v21_Pinout_Diagram png](https://user-images.githubusercontent.com/80208904/213846343-dc6b36f8-4060-457c-b830-5ba1f036120e.jpg)

![NeuroStimDuino_v21_Components png](https://user-images.githubusercontent.com/80208904/213846347-981a3a14-a25d-4eea-af21-166ae5f73f79.jpg)

* If you have ordered the NeuroStimDuino board (without the accessory pack), then you will have received the following components.

![package_contents_v2 1](https://user-images.githubusercontent.com/80208904/213716002-0fc87029-cfec-48da-aa18-9cea9afd9868.png)

* You also need to order a pair of hydrogel electrodes (from any retail vendor) if you plan to stimulate a fake muscle (such as a [Zucchini](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/tips-on-electrode-placement)) or for your research.

* Finally, before you begin, please remember to review the **safety guidelines** listed at the [end](https://github.com/neuralaxy/NeuroStimDuino/blob/main/README.md#important-safety-guidelines) of this tutorial and follow the recommendations mentioned therein.  

## Hardware Setup
* The battery holder included requires two 18650 Li-ion batteries (not included) to be connected in series. The battery holder allows the batteries to be oriented in series or parallel connection. Unfortunately, the battery holders we shipped only have Chinese labels to indicate the series or parallel oritentation. Make sure you connect the batteries in series (as shown below) and not in parallel. Alternately, you can power NeuroStimDuino using your own battery source between 6-20V. We do not recommend powering NeuroStimDuino using an AC-DC adapter. However, if you must use an adapter, then you need an adapter with atleast 2 Amps output rating

![battery_holder](https://user-images.githubusercontent.com/80208904/139629701-303a772e-c762-4384-8296-19b42c08dd85.jpg)

* The High-Voltage Power Switch S3 turns ON/OFF the +/-72V DC-DC converter (NMT0572SC). When using NeuroStimDuino for the first time we recommend starting with the switch in OFF position. Afterwards, once you are ready to stimulate then flip the switch ON. That way even if you accidentally turn ON the stimulation, it will not have any effect because the high-voltage source will be OFF. Plus it will also help extend battery life. 

![power_switch](https://user-images.githubusercontent.com/80208904/139684900-5c1c3573-6c94-4e15-9f8a-692e6af69ae4.png)

* To control NeuroStimDuino using an Arduino Due or Mega, stack NeuroStimDuino on top of the Arduino board by aligning the hearder pins, as shown below.

![20210426_080326](https://user-images.githubusercontent.com/80208904/139623995-dda8e7de-36c7-4d57-898c-be94603645a9.jpg)

* Once the hardware setup is complete, the amber or yellow Emergency-OFF LED will remain continuously ON. Also TEST_LED1 and TEST_LED2 will start blinking at a rate of 5 blinks per sec.

![20210927_151823](https://user-images.githubusercontent.com/80208904/139687809-0c590ef5-987f-4c82-96e6-ba40544af32c.jpg)

## Software Setup
* Next, you will need to download the latest version of our [Arduino library](https://github.com/neuralaxy/NeuroStimDuino/tree/main/Software/NeuroStimDuino). Read the full update [here](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/arduino-library-released) and remember to install the [SerialCommands library] (https://github.com/ppedro74/Arduino-SerialCommands) from Pedro Pereira. Next, upload the example code NeuroStimDuino_SerialControl.ino onto the Arduino.  

* To write instructions to control NeuroStimDuino, you will use the Serial terminal that is in-built Arduino IDE (Tools > Serial Monitor). Once you open the Serial Monitor, set the baudrate to 115200 bps and add "CR+LF" at the end of each command. When you power either the Arduino or NeuroStimDuino board via the DC plug, you will see test LEDs 1 & 2 flashing on the NeuroStimDuino board. These LEDs are flashing with the default stimulation frequency of 10Hz. Now you can enter some example commands into the Serial Monitor, one command at a time and then press enter.

* After uploading the example code NeuroStimDuino_SerialControl.ino and making the connections as above, when you first open the Serial terminal you will see the following welcome message. This mean that a successful connection between Arduino and NeuroStimDuino has been established. It will also give a readout of the current stimulation parameters for each channel. 

![Welcome_message](https://user-images.githubusercontent.com/80208904/142420135-adf1695d-dc7a-4a32-b45f-c45c8de938a6.png)

* You can type in some example commands into Serial Monitor and change the settings for the stimulator (type one statement at a time). Also, the commands are case-sensitive, so type it in all caps. 
```
FREQ 1 20        // This will set Channel 1's frequency at 20Hz. You will see the TEST LED1 blink at this new frequency
FREQ 2 50        // This will set Channel 2's frequency at 50Hz. You will see the TEST LED1 blink at this new frequency
AMPL 1 15        // Set Channel 1 intensity 15mA
DURN 1 500       // Set Channel 1 pulse duration at 500 uSec
ENAB 1 1         // This will enable Channel 1 of the stimulation. By default, both channels are enabled at reset
STIM 1 20 0      // This command will turn on stimulation on Channel 1 for 20 seconds. Ensure that the High Voltage Power Switch is ON. 
                 // You will see a Red-Green LED (labelled Ch. 1) blink  very briefly (for 500us), every time the stimulation is generated (i.e, 20  times/sec)
```

## Moving forward

* This completes this introductory tutorial. From here on, if you would like to connect cables and electrodes and actuatlly stimulate a fake Zucchini muscle, then follow the setup instructions shown in our [April Update](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/tips-on-electrode-placement) published on CrowdSupply. 

* If you want connect multiple NeuroStimDuinos together and control them using a single Arduino, then look up the steps and instructions posted [earlier](https://www.crowdsupply.com/neuralaxy/neurostimduino/updates/controlling-multiple-neurostimduinos-on-a-single-i2c-bus). Note that with NeuroStimDuino_v2.1, when you change the board's I<sup>2</sup>C using the `ADDR` command, the new address is saved on-baord its EEPROM and will be recalled even after reset. It is the User's responsiblity to reset the default I<sup>2</sup>C to `0x1A` if they wish to. 
 
* <br />**One important change** required in hardware when stacking 2 or more NeuroStimDuinos is to disconnect pins 5 & 6 on JP3. Another way to identify these pins is that they normally connect with PWM pins 9 and 10 on Arduino Due or Mega. The reason for this is that these pins can also be used to drive TEST_LED1 and TEST_LED2 of NeuroStimDuino. Therefore, if you connect them together on 2 NeuroStimDuinos, the LEDs will not function properly and can be confusing. Hence, to avoid any cross-talk, its best to fold these pins out of the way as shown below.

![20211023_150523](https://user-images.githubusercontent.com/80208904/139626983-ed9ccd4f-f83d-4f08-85e3-67781891e534.jpg)


* We hope you enjoy tinkering with NeuroStimDuino and use it for all your neuroscience experiments. If you face any issue with the device or are stuck anytime, feel free to write to us on contactus@neuralaxy.com or submit an issue here. 

## Important safety guidelines:
* NeuroStimDuino is not an FDA approved product for clinical use and it is intended for **RESEARCH PURPOSES ONLY**. 
* NeuroStimDuino is not intended for human use or animal use, without receiving prior approval from a local ethics committee such as Institutional Review Boards and Institutional Animal Care and Use Committees  
* Under no circumstances, should the stimulation electrodes be placed across the chest or close to the heart
* NeuroStimDuino requires an external DC input voltage from 6-20V to operate. We **strongly** recommend to use an external battery for this and avoid the use of an AC adapter or anything that's plugged into the AC mains
--->
