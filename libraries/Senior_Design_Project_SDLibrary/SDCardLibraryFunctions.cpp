/*
 SDCardLibraryFunctions.cpp
 Core implementation file for interfacing with the SD card
 Anthony Zaprzalka for Senior Design Project 2015-2016
 December 19, 2015
 
 Easier way to interface with the SD card and saving the sensor data.
 */

/*
 **** Pin Assignments based of Arduino Uno (same in .cpp file) ****
 Sensor Pins             Arduino Pins
 D3 (Chip Select)        8
 CMO (MOSI)              11
 CLK (Clock)             13
 D0 (MISO)               12
 VCC                     3.3 V
 GND                     GND
 */

#include "SDCardLibraryFunctions.h" // Include the library's header file
//#include "Arduino.h"                // Include the wiring API
//#include "SPI.h"                    // Include the Serial Paralle Interface
//#include "SD.h"                     // Include the SD library
//#include "HardwareSerial.h"         // Include the hardware serial interface


    // Initializing Card
bool SD_Functions::initializeSD(int outputSS, int chipSelect) {
    Serial.begin(9600);
        // Note that even if it's not used as the CS pin, the hardware SS pin
        // (10 on most Arduino boards, 53 on the Mega) must be left as an output
        // or the SD library functions will not work.
    pinMode(outputSS, OUTPUT);
    
        // Intialize the SD card
    if (!SD.begin(chipSelect)) {
        return false;
    }
    return true;
}

    // Print directory files
void SD_Functions::printDirectory(File dir, int numTabs) {
    while (true) {
        File entry =  dir.openNextFile();
        if (! entry) {
                // no more files
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++) {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        } else {
                // files have sizes, directories do not
            Serial.print("\t");
            Serial.print(entry.size(), DEC);
            Serial.print("\t");
            Serial.println("BYTES");
        }
        entry.close();
    }
}

    // Writing to SD card
bool SD_Functions::writeToSD(File myFile, char* writeData, char* filenameSD) {
    myFile = SD.open(filenameSD, FILE_WRITE);
    if (myFile) {
        myFile.println(writeData);
            // Close the file
        myFile.close();
        return true;
    } else {
        return false;
    }
}

    // Read from the SD card
bool SD_Functions::readFromSD(File myFile, char* filenameSD) {
    if (myFile) {
            // Read from the file until there is nothing else in it
        while(myFile.available()) {
            Serial.write(myFile.read());
        }
            // Close the file
        myFile.close();
    } else {
            // Did not open correctly
        return false;
    }
    return true;
    
}

















