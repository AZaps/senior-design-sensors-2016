// Libraries needed
#include <SDCardLibraryFunctions.h>
#include <stdlib.h>
#include <LiquidCrystal.h>

// STRINGS ARE BAD...DO NOT USE STRINGS....

// Variables
SD_Functions sdCardFunctions;         // Variable linked to class definition from the library
bool clarity;                         // Return value check
File root;                            // Directory path
File myFile;                          // Filename path
int sensorDataInt = 0;                // Hold the string data from the sensor
char sensorDataChar[12];              // Holds the converted sensor data from the integer
int timeDataInt;                      // Holds integer value of time
char timeDataTemp[5];                 // Temporary holder for integer to char conversion
char directoryPath[21] = "/sendata/"; // Hold the directory pathname when passed through the library functions
char fullInput[32];                   // Holds the full inputted save file

int analogPin = 0;                    // Initializing a variable that will be used to designate the correct analog pin

int sensorCounter = 0;                // Sensor counter variable

bool isSDCardFunctional;              // Boolean to test if there is an SD card being used
bool wasSDCardInitialized;            // Boolean to check if SD card was already initialized
                                      // If it wasn't the program needs to be reset to check SD card for correct filename
                                      
// Initialize the library with the numbers of the interface pins
// LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7) 
LiquidCrystal lcd(22, 23, 24, 25, 26, 27, 28, 29, 30, 31);



void setup() {

  // Set the LCD's number of columns and rows
  lcd.begin(16, 2);

  // Initialize the SD card
  clarity = sdCardFunctions.initializeSD(53, 49);
  // Check for return value
  if (clarity) {
    lcd.print("SD card found");
    isSDCardFunctional = true;
  } else {
    lcd.print("SD card");
    lcd.setCursor(0, 1);
    lcd.print("not found");
    isSDCardFunctional = false;
  }

  // Check for directory
  // If it doesn't exist create it

  // Check for all sensor files
  // If it doesn't exist create it

  // ?? Maybe put these to library instead ??

  
  delay(3000);

  
  // Set the time
  setTime(10, 30, 00, 5, 1, 15); // hr, min, sec, day, month, yr
  
} // End of setup()


void loop() {
  // Clear LCD display
  clearLCD();

  switch (sensorCounter) {
    case 0:
      strcat(directoryPath, "analog0.txt");
      analogPin = 0;
      sensorCounter++;
      break;
    case 1:
      strcat(directoryPath, "analog1.txt");
      analogPin = 1;
      sensorCounter++;
      break;
    case 2:
      strcat(directoryPath, "analog2.txt");
      analogPin = 2;
      sensorCounter++;
      break;
    case 3:
      strcat(directoryPath, "analog3.txt");
      analogPin = 3;
      sensorCounter++;
      break;
    case 4:
      strcat(directoryPath, "analog4.txt");
      analogPin = 4;
      sensorCounter = 0;
      break;
  }

  getMemoryNumber();

  // Get the current time
  // Initial input text format is xx(month)/xx(day)/xxxx(year) xx(hour):xx(minute).xx(second)
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


  printSensorDataLCD(analogPin, sensorDataInt);
  printTime();


  // Select the correct file path name.
  Serial.print("Attempt to write to ");
  Serial.print(directoryPath);
  Serial.print("...");

  // Attaches full input text format is xx/xx/xxxx xx:xx.xx xxx
  strcat(fullInput, sensorDataChar);


  // Prepare for writing to the SD card
  myFile = SD.open(directoryPath, FILE_WRITE);
  clarity = sdCardFunctions.writeToSD(myFile, fullInput, directoryPath);
  if (clarity) {
    Serial.println("The file was opened correctly and the data was saved to it.");
    getMemoryNumber();
  } else {
    Serial.println("Something happened. Either the file did not open correctly or not saved correctly");
    isSDCardFunctional = false;
    getMemoryNumber();
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
    isSDCardFunctional = false;
    getMemoryNumber();
  }
  myFile.close();


  // Revert all the variables for the next read
  sensorDataInt = 0;
  memset(sensorDataChar, 0, sizeof(sensorDataChar));
  memset(timeDataTemp, 0, sizeof(timeDataTemp));
  memset(fullInput, 0, sizeof(fullInput));
  memset(directoryPath, 0, sizeof(directoryPath));

  strcat(directoryPath, "/sendata/");


  if (isSDCardFunctional == false) {
    delay(5000);
    clearLCD();
    lcd.print("Data not saved");
    lcd.setCursor(0, 1);
    lcd.print("Insert SD Card");
    delay(3000);
    clearLCD();

    // Check if SD card was inserted
    clarity = sdCardFunctions.initializeSD(10, 8);
    // Check for return value
    if (clarity) {
      lcd.print("SD card found");
      isSDCardFunctional = true;
    }
  }

  Serial.println("End of loop. Delay 2 seconds.");
  getMemoryNumber();
  delay(2000);
  Serial.println("-----------------------------------------------------------------------\n");

} // End of loop()

void getMemoryNumber() {
  Serial.print(F("Amount of free memory = "));
  Serial.println(freeMemory());
}

void clearLCD() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
}

void printTime() {
  lcd.setCursor(0, 1);
  lcd.print(hour());
  lcd.print(":");
  lcd.print(minute());
}

void printSensorDataLCD(int sensorNumber, int sensorValue) {
  lcd.setCursor(0, 0);
  lcd.print("Sensor ");
  lcd.print(sensorNumber);
  lcd.print(" ");
  lcd.print(sensorValue);
}











