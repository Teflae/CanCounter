/*
  Rules:
  Don't use underscores plz, there reserved for blanks.
  Names should be formatted vague-to-specific, eg my addres is EarthAusSA5092Leda3
  Capital case for global variables, and lower case for local variables
  Comment everywhare for future people
*/
// Settings. Keep in A-Z order or in files that are relavent, prefixed with 'const' with CamelCase.
const double DEBUG_TIMEOUT = 60000; // ms before debug mode is exited.
const String DEBUG_MESSAGE = "> CanCounter.ino | V0.1 | See manual for help";
const unsigned long DELAY_MODE_NORMAL = 100;
const unsigned long DELAY_MODE_DEBUG = 50;
const unsigned long SERIAL_BAUD_RATE = 57600;

// Configurations. Overide them in your Local.ino file
bool USE_EO = true;
bool USE_MATRIX = true;

// Global Variables
bool Debugging = false;
unsigned long DebugEnd = 0;

short DisplayBuffer[16] = { // Set to "CC\nTHS"
  0b1111111111111111,
  0b1100001110000111,
  0b1001111100111111,
  0b1011111101111111,
  0b1001111100111111,
  0b1100001110000111,
  0b1111111111111111,
  0b0000000000000000,
  0b0111101001001100,
  0b0010001001010000,
  0b0010001111001100,
  0b0010001001000010,
  0b0010001001001100,
  0b0000000000000000,
  0b0000000000000000,
  0b1111111111111111,
};

void SerialPrintError(String message)
{
  Serial.print("! ");
  Serial.println(message);
}

void setup()
{
  Local(); // * Define this function in your Local.ino file. It can be empty. That function will not be shared.
  Serial.begin(SERIAL_BAUD_RATE); // Init serial @ 56kHz
  MatrixSetup();
  if(USE_EO) SetupEO();
}

void loop()
{
  if (Debugging)
    LoopDebug(); // Debug Mode
  else
  { // Normal Mode
    // read the input on analog pin 0:
    LoopEO();
    delay(DELAY_MODE_NORMAL);
  }
}
