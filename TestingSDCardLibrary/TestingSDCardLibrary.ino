// Libraries needed
#include <SDCardLibraryFunctions.h>
#include <stdlib.h>

// STRINGS ARE BAD...DO NOT USE STRINGS....

// Variables
SD_Functions sdCardFunctions;         // Variable linked to class definition from the library
bool clarity;                         // Return value check
File root;                            // Directory path
File myFile;                          // Filename path
int sensorDataInt = 0;                // Hold the string data from the sensor
char sensorDataChar[12];              // Holds the converted sensor data from the integer
int timeDataInt;
char timeDataTemp[5];
char directoryPath[21] = "/sendata/"; // Hold the directory pathname when passed through the library functions
char fullInput[32];

int analogPin = 0;

int sensorCounter = 0;

int ledPin = 7;

void setup() {

  pinMode(ledPin, OUTPUT);
  
  // Initialize the SD card
  clarity = sdCardFunctions.initializeSD(10,8);
  // Check for return value
  if (clarity) {
   Serial.println(F("Correctly initialized through SDCardFunctionsLibrary"));
  } else {
    Serial.println(F("Incorrectly initialized through SDCardFunctionsLibrary"));
    digitalWrite(ledPin,HIGH);
    while (true) {}
  }
  
  getMemoryNumber();

  delayThree(); // Short Delay

  // Set the time
  setTime(10,30,00,1,1,15); // hr, min, sec, day, month, yr

  Serial.println(F("\nEnd of setup()"));
  Serial.println(F("\n\n"));
} // End of setup()


void loop() {
  switch(sensorCounter) {
    case 0:
      Serial.println(F("On case 0..."));
      strcat(directoryPath, "analog0.txt");
      analogPin = 0;
      sensorCounter++;
      break;
    case 1:
      Serial.println(F("On case 1..."));
      strcat(directoryPath, "analog1.txt");
      analogPin = 1;
      sensorCounter++;
      break;
    case 2:
      Serial.println(F("On case 2..."));
      strcat(directoryPath, "analog2.txt");
      analogPin = 2;
      sensorCounter++;
      break;
    case 3:
      Serial.println(F("On case 3..."));
      strcat(directoryPath, "analog3.txt");
      analogPin = 3;
      sensorCounter++;
      break;
    case 4:
      Serial.println(F("On case 4..."));
      strcat(directoryPath, "analog4.txt");
      analogPin = 4;
      sensorCounter = 0;
      break;
  }

  
  getMemoryNumber();

  
  // Get the current time
  timeDataInt = month();
  itoa(timeDataInt, timeDataTemp, 10);
  strcat(fullInput, timeDataTemp);
  strcat(fullInput, "/");

  timeDataInt = day();
  itoa(timeDataInt, timeDataTemp, 10);
  strcat(fullInput, timeDataTemp);
  strcat(fullInput, "/");

  timeDataInt = year();
  itoa(timeDataInt, timeDataTemp, 10);
  strcat(fullInput, timeDataTemp);

  strcat(fullInput, " ");

  timeDataInt = hour();
  itoa(timeDataInt, timeDataTemp, 10);
  strcat(fullInput, timeDataTemp);
  strcat(fullInput, ":");

  timeDataInt = minute();
  itoa(timeDataInt, timeDataTemp, 10);
  strcat(fullInput, timeDataTemp);
  strcat(fullInput, ".");

  timeDataInt = second();
  itoa(timeDataInt, timeDataTemp, 10);
  strcat(fullInput, timeDataTemp);
  strcat(fullInput, " ");


  // Get the currently selected sensor value
  sensorDataInt = analogRead(analogPin);
  // Convert the read in data to a string to be saved to the SD card
  itoa(sensorDataInt, sensorDataChar, 10);
  Serial.print("The sensor data that was read was from analog pin... ");
  Serial.print(analogPin);
  Serial.print(" the value is... ");
  Serial.println(sensorDataChar);


  // Select the correct file path name. Should use a switch statement here based on for loop number for selecting the
  // correct file pathname
  Serial.print("Attempt to write to ");
  Serial.print(directoryPath);
  Serial.print("...");


  strcat(fullInput, sensorDataChar);

  
  // Prepare for writing to the SD card
  myFile = SD.open(directoryPath, FILE_WRITE);
  clarity = sdCardFunctions.writeToSD(myFile, fullInput, directoryPath);
  if (clarity) {
    Serial.println("The file was opened correctly and the data was saved to it.");
    getMemoryNumber();
  } else {
    Serial.println("Something happened. Either the file did not open correctly or not saved correctly");
    getMemoryNumber();
    digitalWrite(ledPin,HIGH);
    while (true) {}
  }

  
  // Reading from the SD card. Don't need this later, only for testing purposes and writing checking
  myFile = SD.open(directoryPath);
  clarity = sdCardFunctions.readFromSD(myFile, directoryPath);
  if (clarity) {
    Serial.print("Done reading from ");
    Serial.println(directoryPath);
    getMemoryNumber();
  } else {
    Serial.print("Could not read from ");
    Serial.println(directoryPath);
    getMemoryNumber();
    digitalWrite(ledPin,HIGH);
    while (true) {}
  }
  myFile.close();

  
  // Revert all the variables for the next read
  sensorDataInt = 0;
  memset(sensorDataChar, 0, sizeof(sensorDataChar));
  memset(timeDataTemp, 0, sizeof(timeDataTemp));
  memset(fullInput, 0, sizeof(fullInput));
  memset(directoryPath, 0, sizeof(directoryPath));


  strcat(directoryPath, "/sendata/");


  Serial.println("End of loop. Delay 5 seconds.");
  getMemoryNumber();
  delay(5000);
  Serial.println("-----------------------------------------------------------------------\n");
} // End of loop()

void delayThree() {
  Serial.print(F("\nDelay 3..."));
  delay(1000);
  Serial.print(F("2..."));
  delay(1000);
  Serial.print(F("1..."));
  delay(1000);
  Serial.println(F("Done\n"));
}

void getMemoryNumber() {
  Serial.print(F("Amount of free memory = "));
  Serial.println(freeMemory());
}












