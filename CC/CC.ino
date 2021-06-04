/*
  Rules:
  Don't use underscores plz, there reserved for blanks.
  Names should be formatted vague-to-specific, eg my addres is EarthAusSA5092Leda3
  Capital case for global variables, and lower case for local variables
  Comment everywhare for future people
*/
#include <EEPROM.h>

// Settings. Keep in A-Z order or in files that are relavent, prefixed with 'const' with CamelCase.
const double DEBUG_TIMEOUT = 60000; // ms before debug mode is exited.
const String DEBUG_MESSAGE = "> CanCounter.ino | V0.3 | See manual for help";
const unsigned long DELAY_MODE_NORMAL = 10;
const unsigned long DELAY_MODE_DEBUG = 50;
const unsigned long SERIAL_BAUD_RATE = 57600;

// Configurations. Overide them in your Local.ino file
bool USE_EEPROM = true; // For simulators, set to false to save your EEPROM from permanent damage. EEPROM is limited to 100,000 cycles before breaking.
bool USE_EO = true; // Whether the Electro-optical system, the laser and photoresistor, are attached.
bool USE_MATRIX = true; // Whether a display is physicly attached

// Global Variables
bool Debugging = false; // Determines whether Normal mode || Debug (or console) mode is running.
unsigned long DebugEnd = 0; // imeout for debug mode
int Test = 0; // Test flag
int AllCount = 0; // All-time total counts. Saved to EEPROM
const int AllCountAddress = 0; // Position to save All-time count on EEPROM. If EEPROM does not work change this to a new location.

short DisplayBuffer[16] = { // Set to "CC\nTHS", acts as splash screen
  0b1111111111111111,
  0b1100001110000111,
  0b1001111100111111,
  0b1011111101111111,
  0b1001111100111111,
  0b1100001110000111,
  0b1111111111111111,
  0b0000000000000000,
  0b1111101001001100,
  0b0010001001010000,
  0b0010001111001100,
  0b0010001001000010,
  0b0010001001001100,
  0b0000000000000000,
  0b0000000000000000,
  0b1111111111111111,
};

void setup()
{
 Local(); // * Define this function in your Local.ino file. It can be empty. That function will not be shared.
  // Memory
  EEPROM.get(AllCountAddress, AllCount);

  // Serial
  Serial.begin(SERIAL_BAUD_RATE); // Init serial @ 56kHz
  Serial.println(DEBUG_MESSAGE);

  // Outsorced
  MatrixSetup();
  if(USE_EO) SetupEO();
}

void loop()
{
  if (Debugging)
    LoopDebug(); // Debug Mode
  else
  { // Normal Mode
    LoopEO();
    if (Serial.available() > 0) // Debug mode triggers when messages are recived through Serial (USB) cable
    {
      Debugging = true;
      DebugEnd = millis() + DEBUG_TIMEOUT;
      Serial.println(DEBUG_MESSAGE);
    }
    delay(DELAY_MODE_NORMAL); // Regulate speed
  }
}
