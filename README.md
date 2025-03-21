[![Platform](https://img.shields.io/badge/Platform-ARDUINO-blue.svg)](https://www.arduino.cc)
[![Language](https://img.shields.io/badge/lang-C++-blue.svg)](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers)
[![Company](https://img.shields.io/badge/-ROBOCODE-blue.svg)](https://robocode.ua)
[![CPU](https://img.shields.io/badge/CPU-ESP32_Dev_Module-blue.svg)](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/Walle-robocode/PlatformESP32)

## WALL-E ROBOCODE ESP32 Platform

<p align="center">
  <img src="https://github.com/Walle-robocode/PlatformMega/blob/master/docs/img/IMG_9801.png?raw=true" alt="WALL-E Robot" width="400"/>
</p>

### Description:
___
This project aims to improve the performance of the Wall-e robot platform from **`ROBOCODE`**. This particular part of the code is responsible for the WIFI connection and voice recognition platform. The updated code works only with the updated code for the MEGA2560 module, so both controllers need to be reprogrammed. It also relies on the updated audio recording library, which is located in a separate repository.

In general, Wall-e from **`ROBOCODE`** is a demonstration robot used to entertain students during breaks and to showcase the capabilities of small robotic platforms based on ARDUINO.

Give a ⭐ if you like the project.

### Hardware compatible
___

his code is designed exclusively for use with the Wall-e platform from **`ROBOCODE`**. The platform is based on the **ESP32-WROOM-32** controller, allowing you to customize the code for your project if necessary.

**Hardware components:**
- CPU - **ESP32-WROOM-32** module
- Storage - SD card with slot
- mic - MAX9814 microphone module

**Libraries:**

- Arduino_JSON by Arduino Version 0.2.0
- esp32 by Espressif Systems Version 3.0.7
- HTTPClient(esp32) Built-In by Markus Sattler Version 3.0.7
- ESP Async WebServer by Me-No-Dev Version 1.2.3
- AsyncTCP by Me-No-Dev Version 1.1.4

### Configuration:
___

To begin, you need to configure the settings. Rename the `config_def.h` file to `config.h` and specify the API key for connecting to the voice recognition server in the file. After that, you can proceed with building the project.

### Installations and first use
___

First, you need to have the Arduino IDE installed on your PC.

**FIRMWARE UPLOAD TO ESP32:**

To install ESP32 in the Arduino IDE, follow these steps:
1. Open the settings window in the Arduino IDE. Select the menu item "File > Preferences".
2. In the "Additional Boards Manager URLs" field, copy the address
`https://dl.espressif.com/dl/package_esp32_index.json`
1. Next, open the menu Tools | Board | Board Manager... and in the dialog box, select the option ESP32 by Espressif Systems (use the search term ESP32 for quick finding).
2. After these steps, many boards on the ESP32 platform will appear in the Arduino IDE. You should choose the specific model for your actual board. For Wall-e, this is ESP32 Dev Module.
3. To flash the ESP32 on the robot, set the `ESP / MEGA` switch to the `ESP` position.
4. To put the ESP32 into sketch upload mode:
  - With the power connected, press and hold the FLASH button
  - Press and release the RESET button
  - Release the FLASH button

Then the firmware upload proceeds in the standard way. Select the port and upload the sketch. This will take some time. If during the upload process, it stops and the word Connecting is displayed in the console, it means that you did not put the ESP32 into upload mode. Just try again.

**UPLOADING FILES TO ESP32 FLASH MEMORY**

The previously uploaded sketch uses page layout files, styles, and scripts that need to be loaded into Flash memory. To do this, you need to install the ESP32 Sketch Data Uploader utility in the Arduino IDE.

In the Tools tab, the ESP32 Sketch Data Uploader item should appear.

Before starting the upload, make sure that the sketch folder contains a data folder with all the necessary files.

Now you can put the ESP32 into upload mode and click on ESP32 Sketch Data Uploader, after which the file upload process to Flash memory will begin. Be sure to wait for the Done message in the console.

After all manipulations with the ESP32 (sketch upload, file upload), you need to restart the board using the RESET button on the ESP module.

**CONNECTING TO Wi-Fi AND OPERATING THE ROBOT**

When you first start the robot, it will immediately create an access point that you need to connect to and configure Wall-e's Wi-Fi connection parameters on the website.
Connect to the Wall-e Access Point. Password: 12345678
You can also connect using your phone.

Next, open a browser and enter the IP address displayed on Wall-e's screen.

A web interface will appear in the browser, where you can select a Wi-Fi access point. Wall-e will connect to this access point for voice recognition.

Click the Search Networks button. The ESP32 will start searching for all available access points. This may take up to 10 seconds. Once the button becomes available again, select the desired access point from the drop-down list and enter the password for the selected Wi-Fi.

Next, click the Save Settings button. Wall-e will then connect to the internet and remember the entered data to connect to this access point in the future.
The screen will notify you of a successful connection with the message: `Слухаю`, indicating that the robot has started recording speech. The recording lasts for 3 seconds.

After recording, the robot sends the data to Google servers to obtain the text content of the recorded file. At this moment, the screen will display the message `Зачекайте`.

The recognition time depends on the quality of the internet connection and can take from 2 to 10 seconds. External noise and pronunciation clarity can also affect recognition accuracy.

If the robot cannot connect to the saved Wi-Fi within 20 seconds after startup, it will create an access point again to configure a new connection.


### How to Contribute:
___

Contributing to this software is warmly welcomed.
1. Test it, and if you find any problems, then post an issue.
2. Help us solve the issues or other bugs.

___