/*
  Rules:
  Don't use underscores plz, there reserved for blanks.
  Names should be formatted vague-to-specific, eg my addres is EarthAusSA5092Leda3
  Capital case for global variables, and lower case for local variables
  Comment everywhare for future people
*/
// Settings. Keep in A-Z order or in files that are relavent, prefixed with 'const' with CamelCase.
const double Dampen = 20.0;
const double DampenLong = 2000.0;
const double DebugTimeut = 60000; // ms before debug mode is exited.
const String DebugMessage = "> CanCounter.ino | V0.1 | See manual for help";
const unsigned long DelayModeNormal = 1000;
const unsigned long DelayModeDebug = 50;
const unsigned long SerialBaudRate = 57600;
const uint8_t PinLightSensor = A0;

// Global Variables
double avg = 1;
double damp = 1;
bool Debugging = false;
unsigned long DebugEnd = 0;

void SerialPrintError(String message)
{
  Serial.print("! ");
  Serial.println(message);
}

void setup()
{
  Serial.begin(SerialBaudRate); // Init serial @ 56kHz
}

void loop()
{
  if (Debugging)
    LoopDebug(); // Debug Mode
  else
  { // Normal Mode
    // read the input on analog pin 0:
    int sensorValue = analogRead(PinLightSensor);
    damp = (damp * (Dampen - 1) + sensorValue) / Dampen;
    avg = (avg * (DampenLong - 1) + sensorValue) / DampenLong;
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
      DebugEnd = millis() + DebugTimeut;
      Serial.println(DebugMessage);
    }
    delay(DelayModeNormal);
  }
}
