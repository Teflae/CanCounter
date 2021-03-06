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
const String DEBUG_MESSAGE = "> CanCounter.ino | V0.4 | See manual for help";
const unsigned long DELAY_MODE_NORMAL = 25; // Ensures that code runs every given milliseconds. Not really a delay.
const unsigned long DELAY_MODE_DEBUG = 50;
const unsigned long SERIAL_BAUD_RATE = 57600;

// Configurations. Overide them in your Local.ino file
bool USE_EEPROM = true; // For simulators, set to false to save your EEPROM from permanent damage. EEPROM is limited to 100,000 cycles before breaking.
bool USE_EO = true; // Whether the Electro-optical system, the laser and photoresistor, are attached.
bool USE_MATRIX = true; // Whether a display is physicly attached

const byte TEST_FLAGS_LENGTH = 3;
const String TEST_FLAGS[TEST_FLAGS_LENGTH] = {
  "null",
  "eo",
  "timing"
};
const int TEST_EO = 1;
const int TEST_TIMING = 2;

// Global Variables
int AllCount = 0; // All-time total counts. Saved to EEPROM
const int AllCountAddress = 0; // Position to save All-time count on EEPROM. If EEPROM does not work change this to a new location.
bool Debugging = false; // Determines whether Normal mode || Debug (or console) mode is running.
unsigned long DebugEnd = 0; // Timeout for debug mode
unsigned long LoopStart = 0; // Timestamp at which the main loop() starts running
unsigned long LoopEnd = 0;
int Test = 0; // Test flag
bool Warning = false; // Resets every loop. Set to true to flash built-in LED to indicate error.

short DisplayBuffer[16] = { // Set to "CC\nTHS", acts as splash screen
  0b1111111111111111,
  0b1110000111000011,
  0b1100111110011111,
  0b1101111110111111,
  0b1100111110011111,
  0b1110000111000011,
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

// Data
// Record last 32 possible counts for debugging
typedef struct {
  bool Counted;
  unsigned long Time;
  unsigned int Duration;
  float Reading;
  float Average;
} EOBreak;

byte BreaksIndex = 0;
EOBreak Breaks[32];

// Declarations
void Local(); // * Define this function in your Local.ino file. It can be empty. That function will not be shared.
void SetupMatrix();
void SetupEO();
void LoopMatrix();
void LoopEO();

// Prints a message in standardised format, and terminates it if it is last.
void SerialPrintParam (unsigned long message, bool last = false) {
  Serial.print(message);
  if (last) Serial.println();
  else Serial.print('|');
}

void setup()
{
  Local(); // * Define this function in your Local.ino file. It can be empty. That function will not be shared.
  pinMode(LED_BUILTIN, OUTPUT);
  // Memory
  EEPROM.get(AllCountAddress, AllCount);

  // Serial
  Serial.begin(SERIAL_BAUD_RATE); // Init serial @ 56kHz
  Serial.println(DEBUG_MESSAGE);

  // Outsorced
  if (USE_MATRIX) SetupMatrix();
  if (USE_EO) SetupEO();
}

void loop()
{
  Warning = false;
  LoopStart = micros();
  matrixLoop();
  if (Debugging)
    LoopDebug(); // Debug Mode
  else
  { // Normal Mode
    if (Test == TEST_TIMING) {
      SerialPrintParam(LoopEnd, true);
    }

    // Code
    LoopEO();

    // Check for serial
    if (Serial.available() > 0) // Debug mode triggers when messages are recived through Serial (USB) cable
    {
      Debugging = true;
      DebugEnd = millis() + DEBUG_TIMEOUT;
      Serial.println(DEBUG_MESSAGE);
    }

    // Timing
    unsigned long loopFinish = micros() - LoopStart;
    if (Test == TEST_TIMING) {
      SerialPrintParam(loopFinish);
      SerialPrintParam(AllCount);
    }

    long delayTime = (DELAY_MODE_NORMAL * 1000) - (micros() - LoopStart); // Calculate remaining time to next cycle.
    if (delayTime > 0) { // Is there enough time?
      delayMicroseconds(delayTime);
    }
    else Warning = true;

    // Warning indicator
    if (Warning) {
      digitalWrite(LED_BUILTIN, (millis() % 200 < 100) ? HIGH : LOW); // Else flash built-in LED
    } else {
      digitalWrite(LED_BUILTIN, LOW); // Indicate that timing is ok.
    }
    LoopEnd = micros() - LoopStart;
  }
}
