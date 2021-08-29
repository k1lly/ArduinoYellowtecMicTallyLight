// Copyright 2021 Alexey Bugaev (https://alexeybugaev.com/)

// Library used to control driver shield - Adafruit Motor Shield library - Version: 1.0.1
#include <AFMotor.h>

/*
  Instead of constantly calculating required light's intensity at any given point
  a pre-generated lookup table is used.

  The lowest intensity used is 10/255 because at lower levels
  Mika mic tally light starts flickering unpleasantly.
  My experiments show that 10 is the lowest usable value.
  Changing the lowest value to 15, 20 and 25 still gives good results
  while providing less intensity contrast. If you want to do just that --
  use `Lookup table calculation\MicLookupTable.xlsx`.
  
  To avoid "data section exceeds available space in board" (not enough memory) error message
  store computed intensity curve's 500 points as array in Arduino's flash memory instead of SRAM.
  To do that use PROGMEM
  See https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

*/
const static int tallyTable[] PROGMEM = {10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
                                         14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18,
                                         18, 18, 19, 19, 19, 20, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 23, 24, 24, 25, 25, 25, 25, 26, 26, 27, 27,
                                         27, 28, 28, 29, 29, 30, 30, 30, 31, 32, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 38, 38, 39, 39, 40, 41, 41, 42, 43,
                                         43, 44, 45, 45, 46, 46, 47, 48, 49, 50, 50, 51, 52, 53, 53, 54, 55, 56, 57, 58, 58, 60, 60, 61, 62, 63, 64, 65, 66, 67,
                                         68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 87, 88, 89, 91, 92, 93, 94, 96, 97, 98, 99, 101,
                                         102, 103, 104, 106, 107, 109, 110, 111, 113, 114, 115, 117, 118, 119, 121, 122, 124, 125, 126, 128, 129, 131, 132, 134,
                                         135, 136, 138, 139, 141, 142, 144, 145, 147, 148, 150, 151, 153, 154, 156, 157, 159, 160, 162, 163, 165, 166, 167, 169,
                                         171, 172, 174, 175, 176, 178, 179, 181, 182, 184, 185, 187, 188, 189, 191, 192, 194, 195, 196, 198, 199, 200, 202, 203,
                                         205, 206, 207, 209, 210, 211, 212, 214, 215, 216, 217, 219, 220, 221, 222, 223, 224, 225, 226, 227, 229, 230, 230, 232,
                                         232, 233, 234, 235, 236, 237, 238, 239, 240, 240, 241, 242, 243, 243, 244, 245, 246, 246, 247, 248, 248, 249, 249, 250,
                                         250, 251, 251, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                         255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 253, 253, 253, 253, 253, 252, 252, 252, 252, 252, 251,
                                         251, 250, 250, 250, 250, 249, 249, 248, 248, 248, 247, 247, 247, 246, 246, 245, 245, 244, 244, 243, 243, 242, 242, 241,
                                         241, 240, 240, 239, 238, 238, 237, 237, 236, 236, 235, 234, 234, 233, 232, 232, 231, 230, 230, 229, 229, 228, 227, 226,
                                         225, 225, 224, 224, 223, 222, 221, 220, 220, 219, 218, 217, 216, 215, 215, 214, 213, 212, 212, 210, 210, 209, 208, 207,
                                         206, 205, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185,
                                         184, 183, 182, 181, 181, 179, 178, 177, 176, 176, 174, 174, 172, 172, 171, 169, 169, 167, 167, 166, 165, 164, 162, 162,
                                         161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138,
                                         137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 128, 126, 126, 124, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115,
                                         114, 113, 113, 112, 111, 110, 109, 108, 107, 106, 106, 104, 104, 103, 102, 101, 100, 99, 99, 98, 97, 96, 95, 94, 94,
                                         93, 92, 91, 90, 89, 89, 88, 87, 86, 86, 85, 84, 83, 83, 82, 81, 80, 80, 79, 78, 78, 77, 76, 75, 75, 74, 73, 73, 72,
                                         71, 70, 70, 69, 68, 68, 67, 66, 66, 65, 65, 64, 63, 63, 62, 61, 61, 60, 60, 59, 58, 58, 57, 57, 56, 56, 55, 55, 54,
                                         53, 53, 52, 52, 51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 45, 44, 44, 43, 43, 42, 42, 41, 41, 41, 40,
                                         40, 39, 39, 38, 38, 38, 37, 37, 36, 36, 36, 36, 35, 35, 35, 34, 34, 33, 33, 33, 32, 32, 32, 32, 31, 31, 31, 30, 30,
                                         30, 29, 29, 29, 28, 28, 28, 28, 27, 27, 27, 27, 27, 26, 26, 26, 25, 25, 25, 25, 25, 24, 24, 24, 23, 23, 23, 23, 23,
                                         23, 22, 22, 22, 22, 22, 21, 21, 21, 21, 21, 20, 20, 20, 20, 20, 20, 20, 19, 19, 19, 19, 18, 18, 18, 18, 18, 18, 18,
                                         18, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 16, 16, 16, 16, 16, 16, 16, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
                                         15, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
                                         13, 13, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
                                         12, 12, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
                                         11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                                         10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                                         10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                                         10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                                         10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                                         10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

/*
  Get the array's stored in PROGMEM size properly (500)
  See Notes and Warnings section at:
  https://www.arduino.cc/reference/en/language/variables/utilities/sizeof/
*/
int sizeOftallyTable = sizeof(tallyTable) / sizeof(tallyTable[0]);

// Select the motor number matching the shield's terminal number and set its frequency.
AF_DCMotor motor(2, MOTOR12_64KHZ);

// tallyTable pointer
int *myPointer;

// Initial state -- OFF, waiting for commands via Serial port.
const byte numChars = 32;
int i;
char receivedChars[numChars]; // an array to store the received data
boolean newData = false;
String currentMode = "off";

void setup()
{
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
}

void loop()
{
  recvWithEndMarker();
  showNewData();
  setLightMode();
}

// Getting mode message from Serial port
void recvWithEndMarker()
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false)
  {
    rc = Serial.read();
    if (rc != endMarker)
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    else
    {
      receivedChars[ndx] = '\0';
      ndx = 0;
      currentMode = receivedChars;
      Serial.println(currentMode);
      newData = true;
    }
  }
}

// Confirming received message
void showNewData()
{
  if (newData == true)
  {
    Serial.print("Setting mode: ");
    Serial.println(currentMode);
    newData = false;
  }
}

// Setting Mic Tally Light mode
void setLightMode()
{

  if (currentMode == "off")
  {
    // Turning the light off
    motor.run(RELEASE);
  }

  if (currentMode == "idle")
  {
    // White light
    motor.run(FORWARD);
    for (i = 0; i < sizeOftallyTable; i++)
    {
      // Getting lookup table value address
      myPointer = &tallyTable[i];
      // Setting light intensity according to the lookup table value
      int lightIntensity = pgm_read_word(tallyTable + i);
      motor.setSpeed(lightIntensity);
      delay(5);
    }
  }

  if (currentMode == "hot")
  {
    // Red light
    motor.run(BACKWARD);
    for (i = 0; i < sizeOftallyTable; i++)
    {
      // Getting lookup table value address
      myPointer = &tallyTable[i];
      // Setting light intensity according to the lookup table value
      int lightIntensity = pgm_read_word(tallyTable + i);
      motor.setSpeed(lightIntensity);
      delay(5);
    }
  }
}