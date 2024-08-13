# Cover-Crop-Micro-Environment-Sensor-Bar

Cover Crop Micro Environment Sensor Bar is a fully integrated embedded system tool to gather Crop field Micro environment data with ease for Cover crops. Just carry it to your plot, put in the plot number, and press the button to capture Above canopy PAR light, ground PAR light, soil moisture properties and get them all stored in a CSV file.

**![image](https://github.com/Farmers-Data-Lab/Cover-Crop-Micro-Environment-Sensor-Bar/assets/82876080/669425a2-d16d-46f3-b19b-863964cf9a30)**




## SECTION-1: How to Use –

**(To build the device see Section 2 onwards)**

This sensor bar captures the following data and stores it in a .CSV file(Microsoft excel file) in the SD card put in it –

1) Above canopy PAR light (light above the crops)
2) ground PAR light (light below the crop cover near the ground)
3) Soil moisture Properties and soil temperature
4) A three-digit number to indicate plot number (to get an idea of the location of the reading point on the farm)
5) Date and time of the instant when a particular reading is taken.

To take a reading-

1) Turn on the sensor Bar by pressing the Blue switch at the bottom of the box. The screen should light up, all values except the current plot number will be displayed as zeroes. This is the initial screen which will show up whenever the device is turned on. Rotate the Knobs on the right side of the box to change the plot number on the screen. The “Rep” knob will change the hundreds place of the plot number and represents the number of the current replication while “plot ” knob will change the tens and ones place of the plot number and indicates the plot number inside the replication. 
2) Turn the knobs to get a feel of their sensitivity, make sure the plot number is not flickering when you are not rotating the knobs, if it is flickering its probably just at the boundary of the two values so just turn the knob slightly more/less to make the number stable. After you feel confident with the knobs, put the replication and plot number of the location of the first reading you wish to take.
3) Take the sensor bar to the farm, point the soil sensor nails towards the ground and use the foot pedal to gently push the sensor into the soil. Make sure that the light sensors are completely level and horizontal using the bubble leveler attached on the bar before pushing the soil sensor in.
4) After the bar is placed properly, you are ready to take a reading. Press the green button on the side of the box to take a reading. When you press it, the buzzer will beep once, and the screen will show some of the values you just captured.
5) Look at the reading taken and if the values look alright(non zero), move to another spot.
6) You can take as many as you can in the same plot number, the number of readings taken for a particular plot in the current session will be displayed on the screen.
7) If you wish to delete a particular reading for some reason, press the red button on the bottom of the box once, you will hear 3 beeps and number of readings taken for that plot will be decremented by one, in the CSV file you will see this deletion marked by the word “deleted” in all values for that point.
8) You can delete multiple previous readings but pressing the red button multiple times.
9) After you are done with a plot, turn the knobs, change the plot number and go to the other plot, repeat above steps for all plots.
10) When done taking readings, switch off the device by turning off the blue switch at the bottom. Every time the device is turned off/on, a new .CSV will be created and stored in the SD card without loosing the previous files. The name of the file that is being written in the current session is also displayed on the screen for convenience later. In a particular session all the data will be written into one file, the file name displayed on the screen.

To get the data-

1) Take out the SD card from the data logger by pulling it out gently, and put the SD card in your laptop SD slot(if your device has that).
2) It will show up as a regular hard disk, open it and you will see the .CSV files stored in it. 
3) Open the CSV files in MS excel or any other software to view the data.

## SECTION-2:  How to build at home –

Do the following-

1) Check out the Part list in section 3 and order them in the same configuration as described for smooth integration of things. I have added some helpful documentation and website links that contain the datasheet and how to use tutorials of each component in detail. These can be used improve upon the functionality/operation of the sensor bar if there is scope. (the documentation and links are added in a separate folder above named sensor_documentation)
2) After all the parts are gathered, assemble the physical structure of the sensor bar. We have used aluminum bars and mounts for the structure and a Plastic box for keeping the embedded system in, the CAD design of the structure can be seen below in section 4. The user can build it with other materials like wood/plastic, etc. The important thing to follow is that the upper light sensor should be protruding above the crops while the lower light sensor bar should be inside the canopy and both should be completely horizontal and level (this can be ensured by the Bubble levelers attached on both sensors). The connectors and jacks used in our design are fixed to the box by drilling holes into it using an electrical drill. All the parts used by us are also linked in Section 4.  
3) After the Physical structure is assembled, Connect the embedded system by using the Circuit diagram in Section 5. The Pin assignments to the different components can also be verified using the comments in the Code of the Arduino MEGA attached above. While making the circuit make sure- a) Put a coin cell battery in the Data Logger RTC slot to make sure the system clock keeps running even after the device is powered off. b) Put an SD card inside the data logger to store the files.
4) After connecting everything according to the circuit diagram upload the code given above (file name – code\_v1.ino) inside the Arduino Mega using a USB cable, The code would need to be compiled and uploaded using the Arduino IDE which can be downloaded for free from the Arduino Website. Once you download the IDE, you need to install a few Arduino libraries for this code to work, which can be done by going to the library manager on the left side vertical panel and searching and installing the libraries mentioned in Section 6. After installing all these libraries you should be able to compile and upload the code onto the Arduino. the screen should light up displaying all values as zero. This is the initial screen and will display the correct values after the first recording is taken. You can also test the functionality of the code by establishing a serial connection with Arduino at baud rate 9600. This can be done directly using the Serial monitor in the Arduino IDE API by going to Tools>Serial Monitor in the GUI or by simply pressing Ctrl + Shift + M. You can also do a serial connection via other interfacing softwares like Putty or MobaXterm.
5) After coding the Arduino take out the usb cable, your device is ready to use!

## SECTION-3:  Parts used in the project –
Here is the list of parts that we have used - 
1) 1x Adafruit 2.8" TFT touch shield with Capacitive touch - https://www.adafruit.com/product/1947
2) 1x Adafruit data logger shield - https://learn.adafruit.com/adafruit-data-logger-shield/overview
3) 1x Arduino Mega
4) 1x STEVENS HydraProbe Soil moisture Sensor **RS-485 version not SD-12** - https://stevenswater.com/products/hydraprobe/
5) 1x Lightscout 6 Sensor Quantum Bar for measuring ground light - https://www.specmeters.com/weather-monitoring/sensors-and-accessories/sensors/light-sensors/lightscout-3-or-6-sensor-quantum-bar-3668i3parent/
6) 1x LightScout Quantum PAR Light Sensor for top light sensing - https://www.specmeters.com/lightscout-quantum-par-light-sensor/
7) 2x 10K Potentiometer (10 Kilo Ohms resistance) - https://www.amazon.com/TWTADE-Adjustable-Potentiometer-Resistance-XH2-54-3P/dp/B082FCRQS2/ref=sr_1_4?dib=eyJ2IjoiMSJ9.WjLsnGSG3zIbMeAZ-PQaxwPRSKUefHCapaOEzIhUK80HnnZLyMarXMtaVgEyeFW1IIL7mI_Op3TJQKUvjt3LKKbi-KjBDkqelpsyab4UqkefTRvVGnBcJNgl4KQsXOMqLNblltum22MJScvHbM0RXLDHlPnLwTk03sOOj3cjCWKDxjA9zRGhxdk_u1FCoM4OAzKtpxlePqt4qplqmpbFkUziB2LY4Oay0YLioNS-JQM.okxEJqYtSlQwhScDl4bVCxFiTDD0b3zSJ1WYJXXvxlo&dib_tag=se&keywords=10k+potentiometer&qid=1718747906&sr=8-4
8) 1x Max485 - RS485 to TTL(Serial) converter - https://www.amazon.com/gp/product/B0B9XT9HM8/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1
9) 2x Push buttons with LEDs, one GREEN and One RED.
10) 1x ON/OFF tilt button - BLUE
11) 1x Rechargable Lithium ion Battery 14v
12) 3x Adjustable Buck converters
13) Connecting wires
14) 1x Buzzer
15) Additional items like connectors, screw shields, power buses to make connections strong and reliable (links to buy them available above)
    
## SECTION-4:  Physical structure of the Sensor Bar –

![1718748892413-1a74704d-81fa-4f16-b077-eea326ce2445_1](https://github.com/Farmers-Data-Lab/Cover-Crop-Micro-Environment-Sensor-Bar/assets/82876080/8314e057-e6e9-4d9f-a885-58fd8c49681a)


## SECTION-5:  Circuit Diagram –
![image](https://github.com/Farmers-Data-Lab/Cover-Crop-Micro-Environment-Sensor-Bar/assets/82876080/73c5d211-8fd2-4823-8b92-77bbe895f497)



### STEP 1:
Use the instructions on the Adafruit website to prepare the data logger shield-
link- https://learn.adafruit.com/adafruit-data-logger-shield/installing-the-headers

![image](https://github.com/Farmers-Data-Lab/Cover-Crop-Micro-Environment-Sensor-Bar/assets/82876080/e7a398fe-9abf-49cf-9e57-6cdf2a57a271)

![image](https://github.com/Farmers-Data-Lab/Cover-Crop-Micro-Environment-Sensor-Bar/assets/82876080/3a989567-73ac-423d-b26a-0d109c1665d4)






