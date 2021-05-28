/*
  Rules:
  Don't use underscores plz, there reserved for blanks.
  Names should be formatted vague-to-specific, eg my addres is EarthAusSA5092Leda3
  Capital case for global variables, and lower case for local variables
  Comment everywhare for future people
*/
// Settings. Keep in A-Z order or in files that are relavent, prefixed with 'const' with CamelCase.
const double DAMPEN = 20.0;
const double DAMPEN_LONG = 2000.0;
const double DEBUG_TIMEOUT = 60000; // ms before debug mode is exited.
const String DEBUG_MESSAGE = "> CanCounter.ino | V0.1 | See manual for help";
const unsigned long DELAY_MODE_NORMAL = 100;
const unsigned long DELAY_MODE_DEBUG = 50;
const unsigned long SERIAL_BAUD_RATE = 57600;
const uint8_t PIN_LIGHT_SENSOR = A0;
const uint8_t PIN_LASER = 11;

// Configurations. Overide them in your Local.ino file
bool USE_EO = true;

// Global Variables
double avg = 1;
double damp = 1;
bool Debugging = false;
unsigned long DebugEnd = 0;
short DisplayBuffer[16];

void SerialPrintError(String message)
{
  Serial.print("! ");
  Serial.println(message);
}

void setup()
{
  Local(); // * Define this function in your Local.ino file. It can be empty. That function will not be shared.
  Serial.begin(SERIAL_BAUD_RATE); // Init serial @ 56kHz
  pinMode(PIN_LASER, OUTPUT);
  if(USE_EO) digitalWrite(PIN_LASER, HIGH);
}

void loop()
{
  if (Debugging)
    LoopDebug(); // Debug Mode
  else
  { // Normal Mode
    // read the input on analog pin 0:
    int sensorValue = analogRead(PIN_LIGHT_SENSOR);
    damp = (damp * (DAMPEN - 1) + sensorValue) / DAMPEN;
    avg = (avg * (DAMPEN_LONG - 1) + sensorValue) / DAMPEN_LONG;
    // print out the value you read:
    Serial.print(sensorValue);
    Serial.print('|');
    Serial.print(damp);
    Serial.print('|');
    Serial.print(avg);
    Serial.println();
    if (Serial.available() > 0)
    {
      Debugging = true;
      DebugEnd = millis() + DEBUG_TIMEOUT;
      Serial.println(DEBUG_MESSAGE);
    }
    delay(DELAY_MODE_NORMAL);
  }
}
