# Instructions for downloading the pre-compiled hex file and source code for NeuroStimDuino

The pre-compiled Hex file for NeuroStimDuino's dsPIC33F is now available for download in folder v1.0 or v2.1, depending on the version of your board 

# Accessing the source code
* Developers interested in accessing the source code of NeuroStimDuino must fill and sign the liability waiver available [here](liability_waiver.pdf) and email it to contactus@neuralaxy.com. You will then receive a link via email to download the source code. 
* The source code is in the form of a zip file containing an MPLAB-X project. We have used MPLAB-X IDE (v5.50) and its xc16 (v1.7) compiler to compile the source code and PICkit3 to program the dsPIC33FJ256GP506A microcontroller on-board NeuroStimDuino. You are free to use any other C compiler of your liking.
* You will also need to install xc16 peripheral libraries(v2.0). Although these libraries are now obsolete, these are still available for download from [Microchip](https://www.microchip.com/SWLibraryWeb/product.aspx?product=PIC24%20MCU%20dsPIC%20Peripheral%20Lib). Instead of using the legacy dsPIC peripheral libraries, Microchip suggests using their code configurator for new projects. However, last time we checked, dsPIC33FJ256GP506A was still not supported by MCC. Hence, we have continued to use the legacy dsPIC peripheral libraries.


