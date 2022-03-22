```

Optional Installs:
Tortois GIT - Explorer Extension Windows


:Note:  the use ``` at top & bottom of text files make it 'MARKDOWN LAUGUAGE'
"# ArduinoProjects" 


GetHub NOTES: 
===============================================================

GetHub project location: ride4sun/ArduidoProjects
================================================
ERROR HELP:
compile error:  avrdud: stk500v1_getsync(): timeout commuication with programmer
- It's the wrong board. It should be: 
Select Board: Ardunio Nano (Ardunio AVR Board)
Processor: ATmega328P (Old BootLoader)



=================================================

from VSCODE interface: 

[Cntrl] [shft] [p] = opens a command line

-install git (git-scm.com) (github code version manager)(SCM = source code manager)
-install github desktop
-install C/C++ (if not already automatically installed).


git config --global user.name "Groove M"
git config --global user.email "groove@burningman.org"

Arduino - change board type (select your board)
Arduino - board configuration - select Processor (Nano - ATmega328P (Old Bootloader))

arduino - select baud rate
arduino - select serial port
arduino - upload 
arduidno - Open Serial Monitor 

: setup the intellesense in VSCODE
https://cuneyt.aliustaoglu.biz/en/enabling-arduino-intellisense-with-visual-studio-code/

|File| ->Preferences -> Settings 
[Extensions] / [Ardunio Setting] [edit in settings.json]
"arduino.path": "C:\\Program Files (x86)\\Arduino",
"arduino.additionalUrls": "", 
"arduino.logLevel": "info", 
"arduino.enableUSBDetection": true, "C_Cpp.intelliSenseEngine": "Tag Parser"

==================================================
I found a website about using a different  '_cpp_properties.json' file: this fixed my intelesence issues: (C:\Users\Owner\Documents\VSCODE\ArduinoProjects\.vscode):

{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/Program Files (x86)/Arduion/hardware/arduino/avr/cores/arduino",
                "C:/Program Files (x86)/Arduino/hardware/arduino/avr/libraries/EEPROM/src",
                "C:/Program Files (x86)/Arduino/hardware/arduino/avr/libraries/HID/src",
                "C:/Program Files (x86)/Arduino/hardware/arduino/avr/libraries/SoftwareSerial/src",
                "C:/Program Files (x86)/Arduino/hardware/arduino/avr/libraries/SPI/src",
                "C:/Program Files (x86)/Arduino/hardware/arduino/avr/libraries/Wire/src",
                "C:/Program Files (x86)/Arduino/hardware/arduino/avr/libraries/SPI/src"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE",
                "F_CPU=16000000L",
                "ARDUINO=10805",
                "ARDUINO_AVR_UNO",
                "ARDUINO_ARCH_AVR"
            ],
            "compilerPath": "C:/Program Files (x86)/Arduino/hardware/tools/avr/bin/avr-gcc.exe",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "intelliSenseMode": "clang-x64"
        }
    ],
    "version": 4
}


=============================================
: commit is for local versions branch
: push is to copy all versions to the server..
: crunch - is to compress to all versions.

=============================================


```
