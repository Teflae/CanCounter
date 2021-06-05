const double DAMPEN_SHORT = 4.0;  // General noise reduction
const double DAMPEN_LONG = 500.0; // Long term average
const uint8_t PIN_LIGHT_SENSOR = A0;
const uint8_t PIN_LASER = 11;
const unsigned long TIME_RESET = 1000; // ms to wait before restarting count
const unsigned long TIME_IGNORE = 5000; // ms after break that a connection won't be counted
const double TRIGGER_POINT = 15.0; // 1/1024 volts of drop for counting to be triggered

double Average; // Long-term average reading and trigger point
double Reading; // Noise-cancelled reading
unsigned long PauseEnd = 0; //TODO: rollback safe // Pause counting after a count to prevent duplicate counting
unsigned long BreakEnd = 0; // The maximum time an item could block an object, else the light has turned off.

double Dampen(double original, double in, double rate) // Like an ease-in animation or fade, akin to the behavior of an analogue geiger counter
{
  return (original * (rate - 1) + in) / rate;
}

void SetupEO()
{
  if (USE_EO) {
    pinMode(PIN_LASER, OUTPUT);
    digitalWrite(PIN_LASER, HIGH);
    delay(25); // SO that laser and photoresistor can overcome capacitance and 'warm up'
  }
  int rawVal = analogRead(PIN_LIGHT_SENSOR);
  Reading = rawVal * 1.0; // Autocalibrate. (' * 1.0' ensures int converts to float)
  Average = (rawVal - TRIGGER_POINT) * 1.0;
}

void LoopEO()
{
  // Interpret readings
  int rawVal = analogRead(PIN_LIGHT_SENSOR); // Read photoresistor
  double reading = Dampen(Reading, rawVal, DAMPEN_SHORT); // Dampen global variable Reading, then keep in a local value to allow for comparison
  Average = Dampen(Average, rawVal - TRIGGER_POINT, DAMPEN_LONG); // Dampen Average (at a different rate to Reading)
  if (Reading >= Average && reading < Average) BreakEnd = millis() + TIME_IGNORE;
  if (Reading <= Average && reading > Average) { // If not paused && Reading was abnormal && reading became normal
    Breaks[BreaksIndex].Counted = millis() > PauseEnd && millis() < BreakEnd;
    Breaks[BreaksIndex].Time = millis();
    Breaks[BreaksIndex].Duration = millis() - BreakEnd + TIME_IGNORE;
    Breaks[BreaksIndex].Average = Average;
    Breaks[BreaksIndex].Reading = Reading;
    if (Breaks[BreaksIndex].Counted) {
      PauseEnd = millis() + TIME_RESET; // Pause to prevent duplicate, or multiple counting of the same item
      AllCount++;
      if (USE_EEPROM && USE_EO) EEPROM.put(AllCountAddress, AllCount);
    }
    BreaksIndex++;
    if(BreaksIndex >= 32) BreaksIndex = 0;
    //TODO: Weekly count
  }
  Reading = reading; // Now, set global variable Reading to it's local variable
  if (Test == TEST_EO) // If test flag requests EO to be tested, or console "test EO", print all parameters. I recommend https://github.com/hyOzd/serialplot
  {
    Serial.print(rawVal);
    Serial.print('|');
    Serial.print(Reading);
    Serial.print('|');
    Serial.print(Average);
    Serial.print('|');
    Serial.print(AllCount);
    Serial.println();
  }
}
