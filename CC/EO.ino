const double DAMPEN_SHORT = 4.0;  // General noise reduction
const double DAMPEN_LONG = 500.0; // Long term average
const uint8_t PIN_LIGHT_SENSOR = A0;
const uint8_t PIN_LASER = 11;
const unsigned long RESET_TIME = 1000; // ms to wait before restarting count
const double TRIGGER_POINT = 15.0; // 1/1024 volts of drop for counting to be triggered

double Average; // Long-term average reading and trigger point
double Reading; // Noise-cancelled reading
unsigned long PauseEnd = 0; //TODO: rollback safe

double Dampen(double original, double in, double rate)
{
  return (original * (rate - 1) + in) / rate;
}

void SetupEO()
{
  pinMode(PIN_LASER, OUTPUT);
  digitalWrite(PIN_LASER, HIGH);
  delay(10);
  int rawVal = analogRead(PIN_LIGHT_SENSOR);
  Reading = rawVal * 1.0;
  Average = rawVal * 1.0;
}

void LoopEO()
{
  // Interpret readings
  int rawVal = analogRead(PIN_LIGHT_SENSOR);
  double reading = Dampen(Reading, rawVal, DAMPEN_SHORT);
  Average = Dampen(Average, rawVal - TRIGGER_POINT, DAMPEN_LONG);
  if(millis() > PauseEnd && Reading <= Average && reading > Average) {
    PauseEnd = millis() + RESET_TIME;
    AllCount++;
  }
  Reading = reading;
  if (Test == TEST_EO)
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
